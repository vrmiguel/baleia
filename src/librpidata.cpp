/*
 * librpidata
 * https://github.com/vrmiguel/rpidata
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
#include <fstream>
#include <cstdio>

using std::string;
using std::ifstream;

std::string RPIData::get_temp()
{
    ifstream in;
    in.open("/sys/class/thermal/thermal_zone0/temp");
    if (!in)
    {
        fprintf(stderr, "In librpidata::get_temp: failed to open 'thermal_zone0/temp'");
        exit(1);
    }
    float heat;
    in >> heat;
    char buff[20];
    snprintf(buff, sizeof(buff), "%.2f °C", (float) heat/1000);
    return std::string(buff);
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
    }

    if (!in)
    {
        fprintf(stderr, "In librpidata::get_freq: failed to open file. (type = %u)\n", type);
        exit(1);
    }
    unsigned int freq;
    in >> freq;
    freq = freq/1000;
    return std::to_string(freq) + " MHz";
}
