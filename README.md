# cpu-load-meter – a tool for metering the CPU utilization in real time


This tools read the `/proc/stat` and use the first line of this file and can only
used in Linux (Unix based) system.

In order to caculate the CPU utilization, this tools read the 2nd to 8st colume
of the first line in file `/proc/stat`.

the seven columes of the first line  are the following:

2 user time

3 nice time

4 system time

5 idle time

6 iowait time

7 irq time

8 softirq time

## Detail
For the Detail of the `/proc/stat`, Please read the man document:

```Shell
man proc
```


## Compile and Run

```Shell
gcc -o cpu_load_meter cpu.c

./cpu_load_meter
```


## License

The tool is released under the GNU General Public License

Copyright (C) 2013 Datawolf

