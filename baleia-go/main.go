/*
 * baleia-go
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

package main

import (
	"bufio"
	"fmt"
	"io/ioutil"
	"os"
	"time"

	"baleia-go/cliargs"
	"baleia-go/rpidata"
)

const version = "baleia-go v0.1-beta"

func main() {
	cfg := cliargs.ParseCLIArgs(os.Args)
	var filename string
	var f *os.File
	var err error
	var buf *bufio.Writer
	if cfg.SaveOutput {
		curTime := time.Now()
		filename = curTime.Format(cfg.TimeFormat)
		f, err = os.Create(filename)
		rpidata.ExitIfErrored(err)
		buf = bufio.NewWriter(f)
	} else {
		buf = bufio.NewWriter(os.Stdout)
	}

	if cfg.SaveInTomlFormat {
		if cfg.SaveFileInfo {
			fmt.Fprintf(buf, "[file-info]\n")
			fmt.Fprintf(buf, "version=\"%s\"\n", version)
			if cfg.SaveOutput {
				fmt.Fprintf(buf, "filename=\"%s\"\n", filename)
			}
		}
		if cfg.SaveUserInfo {
			fmt.Fprintf(buf, "[user-info]\n")
			fmt.Fprintf(buf, "username=\"%s\"\n", rpidata.GetUsername())
			fmt.Fprintf(buf, "hostname=\"%s\"\n", rpidata.GetHostname())
			fmt.Fprintf(buf, "distro=\"%s\"\n", rpidata.GetDistro())
			fmt.Fprintf(buf, "kernel=\"%s\"\n", rpidata.GetKernel())
			fmt.Fprintf(buf, "uptime=\"%s\"\n", rpidata.GetUptime())
		}
		if cfg.SaveCPUInfo {
			fmt.Fprintf(buf, "[cpu-info]\n")
			curFreq, _ := rpidata.GetFrequency(0)
			minFreq, _ := rpidata.GetFrequency(1)
			maxFreq, _ := rpidata.GetFrequency(2)
			fmt.Fprintf(buf, "cur_freq=\"%s\"\n", curFreq)
			fmt.Fprintf(buf, "min_freq=\"%s\"\n", minFreq)
			fmt.Fprintf(buf, "max_freq=\"%s\"\n", maxFreq)
			fmt.Fprintf(buf, "temperature=\"%s\"\n", rpidata.GetTemperature())
			fmt.Fprintf(buf, "cpu_gov=\"%s\"\n", rpidata.GetGovernor())
			fmt.Fprintf(buf, "cpu_mem=\"%s\"\n", rpidata.GetAvailableMemory(true))
			fmt.Fprintf(buf, "gpu_mem=\"%s\"\n", rpidata.GetAvailableMemory(false))
			fmt.Fprintf(buf, "core_volt=\"%s\"\n", rpidata.GetCoreVoltage())
		}
	} else { // Save in human-friendly format
		if cfg.SaveFileInfo {
			fmt.Fprintf(buf, "%-18s\t%s\n", "Version: ", version)
			if cfg.SaveOutput {
				fmt.Fprintf(buf, "%-18s\t%s\n", "Filename: ", filename)
			}
		}
		if cfg.SaveUserInfo {
			fmt.Fprintf(buf, "%-18s\t%s\n", "Username: ", rpidata.GetUsername())
			fmt.Fprintf(buf, "%-18s\t%s\n", "Hostname: ", rpidata.GetHostname())
			fmt.Fprintf(buf, "%-18s\t%s\n", "Distro: ", rpidata.GetDistro())
			fmt.Fprintf(buf, "%-18s\t%s\n", "Kernel: ", rpidata.GetKernel())
			fmt.Fprintf(buf, "%-18s\t%s\n", "Uptime: ", rpidata.GetUptime())
		}
		if cfg.SaveCPUInfo {
			curFreq, _ := rpidata.GetFrequency(0)
			minFreq, _ := rpidata.GetFrequency(1)
			maxFreq, _ := rpidata.GetFrequency(2)
			fmt.Fprintf(buf, "%-18s\t%s\n", "Temperature: ", rpidata.GetTemperature())
			fmt.Fprintf(buf, "%-18s\t%s\n", "Cur. CPU freq: ", curFreq)
			fmt.Fprintf(buf, "%-18s\t%s\n", "Min. CPU freq: ", minFreq)
			fmt.Fprintf(buf, "%-18s\t%s\n", "Max. CPU freq: ", maxFreq)
			fmt.Fprintf(buf, "%-18s\t%s\n", "CPU scaling gov.: ", rpidata.GetGovernor())
			fmt.Fprintf(buf, "%-18s\t%s\n", "CPU memory: ", rpidata.GetAvailableMemory(true))
			fmt.Fprintf(buf, "%-18s\t%s\n", "GPU memory: ", rpidata.GetAvailableMemory(false))
			fmt.Fprintf(buf, "%-18s\t%s\n", "Core voltage: ", rpidata.GetCoreVoltage())
		}
	}

	// rootUsage := rpidata.GetDiskUsage("/")
	// fmt.Printf("root - all: %s\n", rootUsage.All)
	// fmt.Printf("root - available: %s\n", rootUsage.Available)
	// fmt.Printf("root - free: %s\n", rootUsage.Free)
	// fmt.Printf("root - used: %s\n", rootUsage.Used)

	// homeUsage := rpidata.GetDiskUsage("/bin")
	// fmt.Printf("/home - all: %s\n", homeUsage.All)
	// fmt.Printf("/home - available: %s\n", homeUsage.Available)
	// fmt.Printf("/home - free: %s\n", homeUsage.Free)
	// fmt.Printf("/home - used: %s\n", homeUsage.Used)

	buf.Flush()
	if cfg.SaveOutput {
		savedLog, err := ioutil.ReadFile(filename)
		rpidata.ExitIfErrored(err)
		fmt.Print(string(savedLog))
	}
}
