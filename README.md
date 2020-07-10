# baleia [![CodeFactor](https://www.codefactor.io/repository/github/vrmiguel/baleia/badge)](https://www.codefactor.io/repository/github/vrmiguel/baleia) [![Codacy Badge](https://app.codacy.com/project/badge/Grade/091b600655654901a95a9d8f7769ff86)](https://www.codacy.com/manual/lemao.vrm07/baleia?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=vrmiguel/baleia&amp;utm_campaign=Badge_Grade) [![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

*⚠️ Early work in progress.*

Baleia is a pure C++11 command-line tool to retrieve data about your Raspberry Pi. Although made to target the RPi, it should run (although lacking `vcgencmd`-originated data) on any system running GNU/Linux.

## Usage

```shell
Usage: ./baleia [-c, --cpu] [-u, --user] [-a, --all] [-d, --discard] [--cfg] [-f, --fmt <format-string>]
-h, --help       	Show this message and exit.
-c, --cpu        	Save CPU frequency, temperature and scaling governor.
-u, --user       	Save user and OS data.
-a, --all        	Save all possible data. Set when no options are supplied.
-d, --discard    	Print to stdout without saving to a file.
--cfg            	Saves in a TOML-friendly config. file-like format.
-f, --fmt <fmt-str>	Saves output according to the given string, following `strftime` format specification.
```

The `-f, --fmt` option requires a string that conforms to the [strftime](https://man7.org/linux/man-pages/man3/strftime.3.html) specification. If this flag is not 
used, the default `""baleia-log-%B-%d-%y-%Hh%Mm%Ss"` will be used, which yields filenames such as `baleia-log-July-10-20-12h31m56s`.

### Output formats

#### .ini-style format

Easily parsable format, ready for use with [TOML](https://github.com/toml-lang/toml) or [rxi/ini](https://github.com/rxi/ini). This format is used when the `--cfg` flag is set.

An example follows:

```shell
./baleia -a --cfg
[file-info]
baleia-ver="baleia v.0.1-alpha"
filename="baleia-log-July-10-20-13h36m15s"
[user-info]
username="pi"
hostname="raspberrypi"
distro="Raspbian GNU/Linux 10 (buster)"
kernel="Linux 4.19.118-v7+ armv7l"
uptime="8 hours 18 minutes 52 seconds"
[cpu-info]
cur_freq="600 MHz"
min_freq="600 MHz"
max_freq="1200 MHz"
temperature="39.70 °C"
cpu_gov="ondemand"
cpu_mem="940MB"
gpu_mem="84MB"
core_volt="1.2000V"

```

#### Simple, readability-first, output

An example in this format follows:

```shell
Version:         	baleia v.0.1-alpha
Filename:        	baleia-log-July-10-20-01h46m53s
Username:        	pi
Hostname:        	raspberrypi
Distro:          	Raspbian GNU/Linux 10 (buster)
Kernel:          	Linux 4.19.118-v7+ armv7l
Uptime:          	51 minutes 46 seconds
Temperature:     	40.24 °C
Cur. CPU freq:   	600 MHz
Min. CPU freq:   	600 MHz
Max. CPU freq:   	1200 MHz
CPU scaling gov.: 	ondemand
CPU memory:      	940MB
GPU memory:      	84MB
Core voltage:    	1.2000V
```

### Build

Build with `qmake && make` inside the project's root folder or download a binary from the [releases](https://github.com/vrmiguel/baleia/releases/) page.

### Internals

If you wish to use this programmatically, you can use the `librpidata.cpp/.h` files, which provide a RPIData namespace with the methods you'd want to use.
