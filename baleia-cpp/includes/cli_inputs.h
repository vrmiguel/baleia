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

#ifndef __CLI_INPUTS_H
#define __CLI_INPUTS_H

typedef unsigned char u8;
#define __BALEIA_DEFAULT_FMT "baleia-log-%B-%d-%y-%Hh%Mm%Ss"

class  Config {
    void parse_single_hyphen(const char * arg);
public:
    bool save_cpu_info;      //! Saves the SoC's temperature as measured by the on-board temperature sensor. (Celsius)
    bool save_user_info;     //! Saves the CPU's current frequency. (MHz)
    bool toml_format;        //! Saves data in a TOML-friendly format.
    bool save_file_info;     //! Saves filename and baleia version.
    bool save_output;        //! Save a file following the spec. given by format_string.
    const char * format_string;
    Config() {}
    Config(int) : save_cpu_info(true), save_user_info(true), toml_format(false), save_file_info(true), save_output(true), format_string(__BALEIA_DEFAULT_FMT) {}
    Config(int argc, char ** argv);
};

#endif // CLI_INPUTS_H
