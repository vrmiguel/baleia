/*
 * librpidata
 * https://github.com/vrmiguel/baleia
 *
 * Copyright (c) 2020 Vinícius R. Miguel <vinicius.miguel at unifesp.br>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "includes/librpidata.h"
#include <fstream>   // ifstream
#include <memory>    // unique_ptr
#include <unistd.h>  // gethostname
#include <pwd.h>     // struct passwd, getpwuid_r

using std::string;
using std::ifstream;

#include <iostream> // used for debug, remember to remove

static void string_replace( string &s, const string &search, const string &replace ) {
    for( size_t pos = 0; ; pos += replace.length() ) {
        pos = s.find(search, pos);
        if(pos == string::npos) break;
        s.erase(pos, search.length());
        s.insert(pos, replace);
    }
}

static std::string shell_cmd(const char * cmd) {
    char buffer[128];
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe)
    {
        return "RPIData::shell_cmd failed.";
    }
    while (fgets(buffer, 128, pipe.get()) != nullptr)
    {
        result += buffer;
    }
    return result;
}

std::string RPIData::get_temp()
{
    ifstream in;
    in.open("/sys/class/thermal/thermal_zone0/temp");
    if (!in)
    {
        fprintf(stderr, "In librpidata::get_temp: failed to open 'thermal_zone0/temp'");
        return "get_temp failed";
    }
    float heat;
    in >> heat;
    char buff[20];
    snprintf(buff, sizeof(buff), "%.2f °C", heat/1000);
    return buff;
}

std::string RPIData::get_freq(unsigned char type)
{
    ifstream in;
    if (type == 0)       //! Type 0 --- Current frequency
        in.open("/sys/devices/system/cpu/cpu0/cpufreq/scaling_cur_freq");
    else if (type == 1)  //! Type 1 --- Minimum frequency
        in.open("/sys/devices/system/cpu/cpu0/cpufreq/scaling_min_freq");
    else if (type == 2)  //! Type 2 --- Maximum frequency
        in.open("/sys/devices/system/cpu/cpu0/cpufreq/scaling_max_freq");
    else
    {
        fprintf(stderr, "In librpidata::get_freq: type given (\"%u\") is invalid.", type);
        return "get_freq failed";
    }

    if (!in)
    {
        fprintf(stderr, "In librpidata::get_freq: failed to open file. (type = %u)\n", type);
        return "get_freq failed";
    }
    unsigned int freq;
    in >> freq;
    freq = freq/1000;
    return std::to_string(freq) + " MHz";
}

std::string RPIData::get_gov()
{
    ifstream in;
    in.open("/sys/devices/system/cpu/cpu0/cpufreq/scaling_governor");
    if (!in)
    {
        fprintf(stderr, "In librpidata::get_gov: failed to open cpufreq/scaling_governor.");
        return "get_gov failed";
    }
    std::string res;
    in >> res;
    return res;
}

std::string RPIData::get_distro()
{
    std::string res = shell_cmd("cat /etc/os-release | grep PRETTY_NAME=");
    //res = std::regex_replace(res, std::regex("PRETTY_NAME="), "");
    //res = std::regex_replace(res, std::regex("\""), "");
    string_replace(res, "PRETTY_NAME=", "");
    string_replace(res, "\"", "");
    string_replace(res, "\n", "");
    return res;
}

std::string RPIData::get_kernel()
{
    std::string res = shell_cmd("uname -mrs");
    string_replace(res, "\n", "");
    return res;
}

std::string RPIData::get_username()
{
    // Gets the effective ID of the user
    uid_t uid = geteuid();
    struct passwd pwent;
    struct passwd *pwent_ptr;
    char buffer[1024];

    // Looks for the UID on the password databank and saves the result on pwent
    getpwuid_r(uid, &pwent, buffer, sizeof buffer, &pwent_ptr);
    return pwent.pw_name;   // Saves username
}

std::string RPIData::get_hostname()
{
    char buffer[64];
    gethostname(buffer, 64);
    return buffer;
}

std::string RPIData::get_uptime()
{
    ifstream in;
    in.open("/proc/uptime");
    unsigned long int proc_uptime;
    in >> proc_uptime;
    int years   =  proc_uptime / 60 / 60 / 24   / 365;
    int days    = (proc_uptime / 60 / 60 / 24)  % 365;
    int hours   = (proc_uptime / 3600) % 24;
    int minutes = (proc_uptime / 60) % 60;
    int seconds = proc_uptime  % 60;

    std::string result;
    if (years)
    {
        result += std::to_string(years);
        result += years > 1 ? " years " : " year ";
    }

    if (days)
    {
        result += std::to_string(days);
        result += days > 1 ? " days " : " day ";
    }

    if (hours)
    {
        result += std::to_string(hours);
        result += hours > 1 ? " hours " : " hour ";
    }

    if (minutes)
    {
        result += std::to_string(minutes);
        result += minutes > 1 ? " minutes " : " minute ";
    }

    if (seconds)
    {
        result += std::to_string(seconds);
        result += seconds > 1 ? " seconds" : " second";
    }

    return result;
}

// true: get memory available for the CPU; false: get memory available to the GPU
std::string RPIData::get_mem(bool choice)
{
    std::string res;
    if(choice)
    {
        res = shell_cmd("/opt/vc/bin/vcgencmd get_mem arm");
        if (res.empty()) return res = "vcgencmd unavailable";
        string_replace(res, "arm=", "");
        string_replace(res, "\n", "");
    }
    else
    {
        res = shell_cmd("/opt/vc/bin/vcgencmd get_mem gpu");
        if (res.empty()) return res = "vcgencmd unavailable";
        string_replace(res, "gpu=", "");
        string_replace(res, "\n", "");
    }
    return res += "B";
}

std::string RPIData::get_core_voltage()
{
    std::string res = shell_cmd("/opt/vc/bin/vcgencmd measure_volts core");
    if (res.empty()) return res = "vcgencmd unavailable";
    string_replace(res, "volt=", "");
    string_replace(res, "\n", "");
    return res;
}
