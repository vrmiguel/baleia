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

package rpidata

import (
	"bytes"
	"errors"
	"fmt"
	"log"
	"math"
	"os"
	"os/exec"
	"os/user"
	"strconv"
	"strings"

	syscall "golang.org/x/sys/unix"
)

// ExitIfErrored logs the error and exit if err is not nil.
func ExitIfErrored(err error) {
	if err != nil {
		log.Fatalln(err)
	}
}

// GetTemperature returns the devices temperature as measured by the first thermal zone.
func GetTemperature() string {
	file, err := os.Open("/sys/class/thermal/thermal_zone0/temp")
	if err != nil {
		log.Fatal(err)
	}
	var rawTemp int
	_, err = fmt.Fscanf(file, "%d\n", &rawTemp)
	if err != nil {
		log.Fatal(err)
	}
	return fmt.Sprintf("%.2f °C", float32(rawTemp)/1000.0)
}

// GetFrequency returns the current CPU0 frequency if choice == 0, the minimum frequency if choice == 1
// or the maximum frequency if choice == 2
func GetFrequency(choice int8) (string, error) {
	var file *os.File
	var err error
	if choice == 0 {
		file, err = os.Open("/sys/devices/system/cpu/cpu0/cpufreq/scaling_cur_freq")
	} else if choice == 1 {
		file, err = os.Open("/sys/devices/system/cpu/cpu0/cpufreq/scaling_min_freq")
	} else if choice == 2 {
		file, err = os.Open("/sys/devices/system/cpu/cpu0/cpufreq/scaling_max_freq")
	} else {
		return "", errors.New("invalid argument to GetFrequency")
	}
	if err != nil {
		log.Fatal(err)
	}
	var rawFreq int
	_, err = fmt.Fscanf(file, "%d\n", &rawFreq)
	ExitIfErrored(err)
	return fmt.Sprintf("%d MHz", rawFreq/1000), nil
}

// GetGovernor returns the current scaling governor ruling the CPU
func GetGovernor() string {
	file, err := os.Open("/sys/devices/system/cpu/cpu0/cpufreq/scaling_governor")
	ExitIfErrored(err)
	var governor string
	_, err = fmt.Fscanf(file, "%s\n", &governor)
	ExitIfErrored(err)
	return governor
}

// DiskUsageData keeps the total, available, free and used memory of adisk
type DiskUsageData struct {
	All       string
	Available string
	Free      string
	Used      string
}

func round(val float64, roundOn float64, places int) (newVal float64) {
	var round float64
	pow := math.Pow(10, float64(places))
	digit := pow * val
	_, div := math.Modf(digit)
	if div >= roundOn {
		round = math.Ceil(digit)
	} else {
		round = math.Floor(digit)
	}
	newVal = round / pow
	return
}

// Formats a quantity of bytes into a human-friendly string
func formatSize(sizeInBytes float64) string {
	suffixes := [4]string{"B", "KB", "MB", "GB"}
	base := math.Log(sizeInBytes) / math.Log(1024)
	size := round(math.Pow(1024, base-math.Floor(base)), .5, 2)
	suffix := suffixes[int(math.Floor(base))]
	return strconv.FormatFloat(size, 'f', -1, 64) + " " + string(suffix)
}

// GetDiskUsage returns a struct with relevant disk usage data
func GetDiskUsage(path string) (dud DiskUsageData) {
	fs := syscall.Statfs_t{}
	err := syscall.Statfs(path, &fs)
	if err != nil {
		ExitIfErrored(err)
	}

	diskAll := fs.Blocks * uint64(fs.Bsize)
	diskAvail := fs.Bavail * uint64(fs.Bsize)
	diskFree := fs.Bfree * uint64(fs.Bsize)
	diskUsed := diskAll - diskFree

	dud.All = formatSize(float64(diskAll))
	dud.Available = formatSize(float64(diskAvail))
	dud.Free = formatSize(float64(diskFree))
	dud.Used = formatSize(float64(diskUsed))
	return
}

// GetDistro returns a the distro currently being used
//! TODO: use some actual Golang regex here
func GetDistro() string {
	grep := exec.Command("grep", "PRETTY_NAME=")
	cat := exec.Command("cat", "/etc/os-release")
	pipe, _ := cat.StdoutPipe()
	defer pipe.Close()
	grep.Stdin = pipe
	cat.Start()
	res, _ := grep.Output()
	sres := string(res)
	sres = strings.Replace(sres, "PRETTY_NAME=", "", 1)
	sres = strings.Replace(sres, "\"", "", 2)
	return sres[:len(sres)-1]
}

// GetKernel returns the system's architecture, kernel release and kernel name
func GetKernel() string {
	uname := exec.Command("uname", "-mrs")
	res, err := uname.Output()
	ExitIfErrored(err)
	sres := string(res)
	return sres[:len(sres)-1]
}

// GetUsername returns current user's Unix username
func GetUsername() string {
	curUser, err := user.Current()
	ExitIfErrored(err)
	return curUser.Username
}

// GetHostname returns current user's hostname
func GetHostname() string {
	curHost, err := os.Hostname()
	ExitIfErrored(err)
	return curHost
}

// GetAvailableMemory returns the memory available to the CPU if choice is true, to the GPU otherwise.
func GetAvailableMemory(choice bool) string {
	var vcgencmd *exec.Cmd
	var arg string

	if choice {
		arg = "arm"
	} else {
		arg = "gpu"
	}

	vcgencmd = exec.Command("/opt/vc/bin/vcgencmd", "get_mem", arg)
	res, err := vcgencmd.Output()
	if err != nil {
		return "vcgencmd unavailable"
	}
	sres := string(res)
	sres = strings.Replace(sres, arg+"=", "", 2)
	return sres[:len(sres)-1] + "B"
}

func pluralize(wordCount int64, word string) string {
	res := strconv.FormatInt(wordCount, 10) + " " + word
	if wordCount > 1 {
		return res + "s"
	}
	return res
}

// GetUptime returns the system's current uptime in a pretty, human-readable form
func GetUptime() string {
	file, err := os.Open("/proc/uptime")
	ExitIfErrored(err)
	var rawUptime int64
	_, err = fmt.Fscanf(file, "%d", &rawUptime)
	ExitIfErrored(err)
	years := rawUptime / 60 / 60 / 24 / 365
	days := (rawUptime / 60 / 60 / 24) % 365
	hours := (rawUptime / 3600) % 24
	minutes := (rawUptime / 60) % 60
	seconds := rawUptime % 60
	var buffer bytes.Buffer
	if years > 0 {
		buffer.WriteString(pluralize(years, "year"))
		buffer.WriteByte(' ')
	}
	if days > 0 {
		buffer.WriteString(pluralize(days, "day"))
		buffer.WriteByte(' ')
	}
	if hours > 0 {
		buffer.WriteString(pluralize(hours, "hour"))
		buffer.WriteByte(' ')
	}
	if minutes > 0 {
		buffer.WriteString(pluralize(minutes, "minute"))
		buffer.WriteByte(' ')
	}
	if seconds > 0 {
		buffer.WriteString(pluralize(seconds, "second"))
	}
	return buffer.String()
}

// GetCoreVoltage gets the Pi's core voltage as reported by vcgencmd
func GetCoreVoltage() string {
	vcgencmd := exec.Command("/opt/vc/bin/vcgencmd", "measure_volts", "core")
	res, err := vcgencmd.Output()
	if err != nil {
		return "vcgencmd unavailable"
	}
	sres := string(res)
	sres = strings.Replace(sres, "volt=", "", 1)
	return sres[:len(sres)-1]
}
