# baleia

Baleia is a pure C++11 command-line tool to retrieve data about your Raspberry Pi. Although made to target the RPi, it should run (although lacking `vcgencmd`-originated data) on any system running GNU/Linux.

### Usage

```
Usage: ./rpimonitor [-c, --cpu] [-u, --user] [-a, --all] [-d, --discard] [--cfg] [-f, --fmt <format-string>]
-h, --help       	Show this message and exit.
-c, --cpu        	Save CPU frequency, temperature and scaling governor.
-u, --user       	Save user and OS data.
-a, --all        	Save all possible data. Set by when no options are supplied.
-d, --discard    	Print to stdout without saving to a file.
--cfg            	Saves in a TOML-friendly key-value format.
-f, --fmt <fmt-str>	Saves output according to the given string, following `strftime` format specification.
```

#### Output formats

###### .ini-style format

Easily parsable format, ready for use with [TOML](https://github.com/toml-lang/toml) or [rxi/ini](https://github.com/rxi/ini). This format is used when the `--cfg` option is used.

An example follows:
```
./baleia -a --cfg
[file-info]
baleia-ver="baleia v.0.1-alpha"
filename="baleia-log-July-10-20-01h50m39s"
[user-info]
username="pi"
hostname="raspberrypi"
distro="Raspbian GNU/Linux 10 (buster)"
kernel="Linux 4.19.118-v7+ armv7l"
uptime="55 minutes 32 seconds"
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

###### Simple, readability-first, output

An example in this format follows:

```
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

Build with `qmake && make` or download a binary from the [releases](https://github.com/vrmiguel/baleia/releases/) page.


