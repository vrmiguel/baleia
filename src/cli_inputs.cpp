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

#define __RPIM_USAGE "Usage: ./rpimonitor [-c, --cpu] [-u, --user] [-a, --all] [-d, --discard] [--cfg] [-f, --fmt <format-string>]\n"

// TODO: remove -n, --no

static inline void print_help()
{
    printf(__RPIM_USAGE);
    printf("%-17s\tShow this message and exit.\n", "-h, --help");
    printf("%-17s\tSave CPU frequency, temperature and scaling governor.\n", "-c, --cpu");
    printf("%-17s\tSave user and OS data.\n", "-u, --user");
    printf("%-17s\tSave all possible data. Set by default.\n", "-a, --all");
    printf("%-17s\tPrint to stdout without saving to a file.\n", "-d, --discard");
    printf("%-17s\tSaves in a TOML-friendly key-value format.\n", "--cfg");
    printf("%-17s\tSaves output according to the given string, following `strftime` format specification.\n", "-f, --fmt <fmt-str>");
}

config_t parse_cli_input(int argc, char ** argv)
{
    if (argc == 1)
    {
        return { true, true, false, true, true, "baleia-log-%B-%d-%y-%Hh%Mm%Ss" };    //! Save all info. by default
    }
    config_t cfg {false, false, false, true, true, "baleia-log-%B-%d-%y-%Hh%Mm%Ss"};

    for (u8 i = 1; i < argc; i++)
    {
        if (!strcmp(argv[i], "-a") || !strcmp(argv[i], "--all"))
        {
            cfg.save_cpu_info  = true;
            cfg.save_user_info = true;
            cfg.save_version   = true;
        }
        else if (!strcmp(argv[i], "-c") || !strcmp(argv[i], "--cpu"))
        {
            cfg.save_cpu_info = true;
        }
        else if (!strcmp(argv[i], "-d") || !strcmp(argv[i], "--discard"))
        {
            cfg.save_output = false;
        }
        else if (!strcmp(argv[i], "-f") || !strcmp(argv[i], "--fmt"))
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
            cfg.format_string = argv[++i];
        }
        else if (!strcmp(argv[i], "-u") || !strcmp(argv[i], "--user"))
        {
            cfg.save_user_info = true;
        }
        else if (!strcmp(argv[i], "--cfg"))
        {
            cfg.toml_format = true;
        }
        else if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help"))
        {
            print_help();
            exit(0);
        }
        else {
            fprintf(stderr, "rpimonitor: unknown option %s.\n", argv[i]);
            fprintf(stderr, __RPIM_USAGE);
            exit(0);
        }
    }
    return cfg;
}
