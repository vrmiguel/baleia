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

#ifndef __LIBRPIDATA_H
#define __LIBRPIDATA_H

#include <string>

#define __RPIDATA_VER "baleia v.0.1-alpha"

namespace RPIData
{
    //!
    //! \brief get_temp
    //! \return
    //!
    std::string get_temp();

    //!
    //! \brief get_freq
    //! \return
    //!
    std::string get_freq(unsigned char);

    //!
    //! \brief get_gov
    //! \return
    //!
    std::string get_gov();

    std::string get_distro();
    std::string get_username();
    std::string get_hostname();
    std::string get_kernel();
    std::string get_uptime();
    std::string get_mem(bool);  // true: get memory available for the CPU; false: get memory available to the GPU
    std::string get_core_voltage();
}

#endif // LIBRPIDATA_H
