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

package cliargs

import (
	"fmt"
	"os"
	"strings"
)

const usage = "Usage: ./baleia [-c, --cpu] [-f, --file-info] [-u, --user] [-a, --all] [-d, --discard] [-t, --toml] [--fmt <format-string>]\n\n"
const timefmt = "baleia-log-Jan-02-06-15h04m05s"

// CLIArgs stores the passed command-line options
type CLIArgs struct {
	SaveUserInfo     bool
	SaveFileInfo     bool
	SaveCPUInfo      bool
	SaveOutput       bool
	SaveInTomlFormat bool
	TimeFormat       string
}

func printHelp() {
	fmt.Printf(usage)
	fmt.Printf("%-16s\tShow this message and exit.\n", "-h, --help")
	fmt.Printf("%-16s\tSave CPU frequency, temperature and scaling governor.\n", "-c, --cpu")
	fmt.Printf("%-16s\tSave user and OS data.\n", "-u, --user")
	fmt.Printf("%-16s\tSave filename and Baleia version.\n", "-f, --file-info")
	fmt.Printf("%-16s\tSave all available data.\n", "-a, --all")
	fmt.Printf("%-16s\tPrint to stdout without saving to a file.\n", "-d, --discard")
	fmt.Printf("%-16s\tSaves in a TOML-friendly key-value format.\n", "-t, --toml")
	fmt.Printf("%-16s\tSaves output according to the given string, following Go's time.Format specification.\n", "--fmt <fmt-str>")
}

func parseSingleHyphen(arg string, cfg *CLIArgs) {
	for _, ch := range arg[1:] {
		switch ch {
		case 'h':
			printHelp()
			os.Exit(0)
		case 'd':
			cfg.SaveOutput = false
		case 'c':
			cfg.SaveCPUInfo = true
		case 'a':
			cfg.SaveCPUInfo = true
			cfg.SaveFileInfo = true
			cfg.SaveUserInfo = true
		case 'u':
			cfg.SaveUserInfo = true
		case 'f':
			cfg.SaveFileInfo = true
		case 't':
			cfg.SaveInTomlFormat = true
		default:
			fmt.Fprintf(os.Stderr, "error: unknown option -%c\n", ch)
			os.Exit(0)
		}
	}
}

// ParseCLIArgs reads through the given CLI arg. list and builds a CliArgs
func ParseCLIArgs(args []string) CLIArgs {
	if len(args) == 1 {
		// SaveUserInfo, SaveFileInfo, SaveCPUInfo, SaveOutput, SaveInTomlFormat, TimeFormat
		return CLIArgs{true, true, true, true, false, timefmt}
	}
	// SaveUserInfo, SaveFileInfo, SaveCPUInfo, SaveOutput, SaveInTomlFormat, TimeFormat
	var cfg = CLIArgs{false, false, false, true, false, timefmt}
	for i := 1; i < len(args); i++ {
		arg := args[i]
		if strings.Contains(arg, "--") {
			if arg == "--help" {
				printHelp()
				os.Exit(0)
			} else if arg == "--discard" {
				cfg.SaveOutput = false
			} else if arg == "--cpu" {
				cfg.SaveCPUInfo = true
			} else if arg == "--all" {
				cfg.SaveCPUInfo = true
				cfg.SaveFileInfo = true
				cfg.SaveUserInfo = true
			} else if arg == "--user" {
				cfg.SaveUserInfo = true
			} else if arg == "--toml" {
				cfg.SaveInTomlFormat = true
			} else if arg == "--file-info" {
				cfg.SaveFileInfo = true
			} else if arg == "--fmt" {
				if i+1 >= len(args) {
					fmt.Println("missing value to --fmt")
					os.Exit(1)
				}
				i++
				cfg.TimeFormat = args[i]
			} else {
				fmt.Fprintf(os.Stderr, "error: unknown option %s\n", arg)
				fmt.Println(usage)
				os.Exit(1)
			}
		} else {
			if arg[0] == '-' {
				parseSingleHyphen(arg, &cfg)
			} else {
				fmt.Fprintf(os.Stderr, "error: '%s' is not a valid option.\n", arg)
				os.Exit(0)
			}
		}
	}
	return cfg
}
