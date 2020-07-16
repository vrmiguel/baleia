/*
 * RPIData
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

#include "includes/cli_inputs.h"    // config_t
#include <cstdio>                   // fprintf, stderr
#include <cstdlib>                  // exit
#include <cstring>                  // strcmp

#define __BALEIA_USAGE "Usage: ./baleia [-c, --cpu] [-f, --file-info] [-u, --user] [-a, --all] [-d, --discard] [-C, --cfg] [--fmt <format-string>]\n\n"


static inline void print_help()
{
    printf(__BALEIA_USAGE);
    printf("%-16s\tShow this message and exit.\n", "-h, --help");
    printf("%-16s\tSave CPU frequency, temperature and scaling governor.\n", "-c, --cpu");
    printf("%-16s\tSave user and OS data.\n", "-u, --user");
    printf("%-16s\tSave filename and Baleia version.\n", "-F, --file-info");
    printf("%-16s\tSave all available data.\n", "-a, --all");
    printf("%-16s\tPrint to stdout without saving to a file.\n", "-d, --discard");
    printf("%-16s\tSaves the output in TOML format.\n", "-t, --toml");
    printf("%-16s\tDictate output's filename following the `strftime` format specification.\n", "--fmt <fmt-str>");
}

void Config::parse_single_hyphen(const char *arg)
{
    for(u8 i = 1; arg[i] != '\0'; i++)
    {
        switch (arg[i])
        {
        case 'h':
            print_help();
            exit(0);
            break;
        case 'd':
            this->save_output = false;
            break;
        case 'c':
            this->save_cpu_info = true;
            break;
        case 'a':
            this->save_cpu_info  = true;
            this->save_user_info = true;
            this->save_file_info = true;
            break;
        case 'u':
            this->save_user_info = true;
            break;
        case 't':
            this->toml_format = true;
            break;
        case 'f':
            this->save_file_info = true;
            break;
        default:
            fprintf(stderr, "error: unknown option -%c\n", arg[i]);
            exit(0);
        }
    }
}

Config::Config(int argc, char ** argv)
{
    this->format_string = __BALEIA_DEFAULT_FMT;
    this->save_cpu_info = false;
    this->save_file_info = false;
    this->save_user_info = false;
    this->save_output = true;
    this->toml_format = false;

    for (u8 i = 1; i < argc; i++)
    {
        if (strstr(argv[i], "--"))
        {
            if (!strcmp(argv[i], "--all"))
            {
                this->save_cpu_info  = true;
                this->save_user_info = true;
                this->save_file_info = true;
            }
            else if (!strcmp(argv[i], "--cpu"))
            {
                this->save_cpu_info = true;
            }
            else if (!strcmp(argv[i], "--file-info"))
            {
                this->save_file_info = true;
            }
            else if (!strcmp(argv[i], "--discard"))
            {
                this->save_output = false;
            }
            else if (!strcmp(argv[i], "--fmt"))
            {
                if (i + 1 >= argc)
                {
                    fprintf(stderr, "error: missing value to --fmt");
                    exit(0);
                }
                if (strlen(argv[i+1]) > 70)
                {
                    fprintf(stderr, "error: the resulting format string must not exceed 80 characters.\n");
                    exit(0);
                }
                this->format_string = argv[++i];
            }
            else if (!strcmp(argv[i], "--user"))
            {
                this->save_user_info = true;
            }
            else if (!strcmp(argv[i], "--toml"))
            {
                this->toml_format = true;
            }
            else if (!strcmp(argv[i], "--help"))
            {
                print_help();
                exit(0);
            }
            else {
                fprintf(stderr, "error: unknown option %s.\n", argv[i]);
                fprintf(stderr, __BALEIA_USAGE);
                exit(0);
            }
        } else {
            if (argv[i][0] == '-')
            {
                parse_single_hyphen(argv[i]);
            } else {
                fprintf(stderr, "error: '%s' is not a valid option.\n", argv[i]);
                exit(0);
            }
        }
    }
}
