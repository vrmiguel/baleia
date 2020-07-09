/*
 * RPIData
 * https://github.com/vrmiguel/rpidata
 *
 * Copyright (c) 1717 Vinícius R. Miguel <vinicius.miguel at unifesp.br>
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

#include <iostream>
#include <ctime>
#include "includes/cli_inputs.h"
#include "includes/librpidata.h"

using std::cout;

int main(int argc, char ** argv)
{
    config_t cfg = parse_cli_input(argc, argv);

    time_t rawtime;
    struct tm * timeinfo;
    char buffer [80];
    time (&rawtime);
    timeinfo = localtime (&rawtime);
    strftime (buffer,80,"baleia-log-%B-%d-%y-%Hh%Mm%Ss",timeinfo);

    FILE * out_file = fopen(buffer, "w");

    if(cfg.toml_format)
    {
        if(cfg.save_version)
        {
            fprintf(out_file, "[file-info]\nbaleia-ver=\"%s\"\n", __RPIDATA_VER);
            fprintf(out_file, "filename=\"%s\"\n", buffer);
        }
        if(cfg.save_user_info)
        {
            fprintf(out_file, "[user-info]\n");
            fprintf(out_file, "username=\"%s\"\n", RPIData::get_username().c_str());
            fprintf(out_file, "hostname=\"%s\"\n", RPIData::get_hostname().c_str());
            fprintf(out_file, "distro=\"%s\"\n",   RPIData::get_distro().c_str());
            fprintf(out_file, "kernel=\"%s\"\n",   RPIData::get_kernel().c_str());
            fprintf(out_file, "uptime=\"%s\"\n",   RPIData::get_uptime().c_str());
        }
        if(cfg.save_cpu_info)
        {
            fprintf(out_file, "[cpu-info]\n");
            fprintf(out_file, "cur_freq=\"%s\"\n", RPIData::get_freq(0).c_str());
            fprintf(out_file, "min_freq=\"%s\"\n", RPIData::get_freq(1).c_str());
            fprintf(out_file, "max_freq=\"%s\"\n", RPIData::get_freq(2).c_str());
            fprintf(out_file, "temperature=\"%s\"\n",  RPIData::get_temp().c_str());
            fprintf(out_file, "cpu_gov=\"%s\"\n",      RPIData::get_gov().c_str());
        }
    }
    else
    {   // Save in a simple, human-friendly format
        fprintf(out_file, "%-17s\t%s\n", "Version: ",          __RPIDATA_VER);
        fprintf(out_file, "%-17s\t%s\n",   "Filename: ",         buffer);
        fprintf(out_file, "%-17s\t%s\n", "Username: ",         RPIData::get_username().c_str());
        fprintf(out_file, "%-17s\t%s\n", "Hostname: ",         RPIData::get_hostname().c_str());
        fprintf(out_file, "%-17s\t%s\n",   "Distro: ",           RPIData::get_distro().c_str());
        fprintf(out_file, "%-17s\t%s\n",   "Kernel: ",           RPIData::get_kernel().c_str());
        fprintf(out_file, "%-17s\t%s\n", "Uptime: ",           RPIData::get_uptime().c_str());
        fprintf(out_file, "%-17s\t%s\n", "Temperature: ",      RPIData::get_temp().c_str());
        fprintf(out_file, "%-17s\t%s\n", "Cur. CPU freq: ",    RPIData::get_freq(0).c_str());
        fprintf(out_file, "%-17s\t%s\n", "Min. CPU freq: ",    RPIData::get_freq(1).c_str());
        fprintf(out_file, "%-17s\t%s\n", "Max. CPU freq: ",    RPIData::get_freq(2).c_str());
        fprintf(out_file, "%-17s\t%s\n", "CPU scaling gov.: ", RPIData::get_gov().c_str());
    }

    fclose(out_file);

    FILE * in_file = fopen(buffer, "r");
    char buf[1024];
    size_t nread;
    while ((nread = fread(buf, 1, sizeof buf, in_file)) > 0)
    {
        fwrite(buf, 1, nread, stdout);
    }
    if (ferror(in_file))
    {
        //
    }
    fclose(in_file);

    return 0;
}
