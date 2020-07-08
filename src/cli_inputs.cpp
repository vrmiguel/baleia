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

#define __RPIM_USAGE "Usage: ./rpimonitor [-h, --help] [-f, --freq] [-t, --temp] [-g, --gov]\n"


config_t parse_cli_input(int argc, char ** argv)
{
    if (argc == 1)
    {
        return { true, true, true };    //! Save all info. by default
    }
    config_t cfg;

    for (u8 i = 1; i < argc; i++)
    {
        if (!strcmp(argv[i], "-f") || !strcmp(argv[i], "--freq"))
        {
            cfg.save_freq = true;
        }
        else if (!strcmp(argv[i], "-t") || !strcmp(argv[i], "--temp"))
        {
            cfg.save_temp = true;
        }
        else if (!strcmp(argv[i], "-g") || !strcmp(argv[i], "--gov"))
        {
            cfg.save_gov = true;
        }
        else {
            fprintf(stderr, "rpimonitor: unknown option %s.\n", argv[i]);
            fprintf(stderr, __RPIM_USAGE);
            exit(0);
        }
    }
    return cfg;
}
