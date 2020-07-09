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

#include <iostream>
#include "includes/cli_inputs.h"
#include "includes/librpidata.h"
#include <iostream>

#include <memory>

using std::cout;

int main(int argc, char ** argv)
{
    config_t cfg = parse_cli_input(argc, argv);
    if (cfg.save_temp)
    {
        cout << RPIData::get_temp() << '\n';
    }

    if (cfg.save_freq)
    {
        cout << RPIData::get_freq(0) << '\n'; //! Current frequency
        cout << RPIData::get_freq(1) << '\n'; //! Minimum frequency
        cout << RPIData::get_freq(2) << '\n'; //! Maximum frequency
    }

    cout << RPIData::get_gov() << '\n';
    cout << RPIData::get_distro();
    cout << RPIData::get_username() << '\n';
    cout << RPIData::get_kernel();
    cout << RPIData::get_uptime() << '\n';
    return 0;
}
