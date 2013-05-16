/***************************************************************************** 
    Linux CPU Load Meter

    Copyright (C) 2013   Datawolf 

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*****************************************************************************/

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <errno.h>

#define BUFSIZE 256
#define PROC_STAT "/proc/stat"
//#define DEBUG

typedef     unsigned long long ul_t;

typedef     struct cpu_t {
    /* as represented in /proc/stat (the first line of 1st to 7th colume) */
    ul_t    user, nice, system, idle, iowait, irq, softirq;
    ul_t    user_save, nice_save, system_save, idle_save, iowait_save, irq_save, softirq_save;
} cpu_t;


cpu_t   *cpu_refresh(cpu_t *cpus)
{
    static  FILE    *fp = NULL;
    int             i;
    int             num;
    char            buf[BUFSIZE];

    if (!fp) {
        if (!(fp = fopen(PROC_STAT, "r")))
            fprintf(stderr, "Failed to open /proc/stat: %s", strerror(errno));
    }

    rewind(fp);
    fflush(fp);

    /* first value the last slot with the cpu summary line */
    if (!(fgets(buf, sizeof(buf), fp)))
        fprintf(stderr,"Failed to read /proc/stat: %s", strerror(errno));

    num = sscanf(buf, "cpu %Lu %Lu %Lu %Lu %Lu %Lu %Lu",
            &cpus->user,
            &cpus->nice,
            &cpus->system,
            &cpus->idle,
            &cpus->iowait,
            &cpus->irq,
            &cpus->softirq
            );


    if ( num < 4 )
        fprintf(stderr,"Failed to read /proc/stat: %s", strerror(errno));


    return cpus;
}


static void format_output(cpu_t *cpu, const char *prefix)
{
    long long   user_frame;
    long long   nice_frame;
    long long   system_frame;
    long long   idle_frame;
    long long   iowait_frame;
    long long   irq_frame;
    long long   softirq_frame;
    long long   total_frame;
    float       scale;

    user_frame = cpu->user - cpu->user_save;
    nice_frame = cpu->nice - cpu->nice_save;
    system_frame = cpu->system - cpu->system_save;
    idle_frame = cpu->idle - cpu->idle_save;
    iowait_frame = cpu->iowait - cpu->iowait_save;
    irq_frame = cpu->irq - cpu->irq_save;
    softirq_frame = cpu->softirq - cpu->softirq_save;

    total_frame = user_frame + nice_frame
        + system_frame + idle_frame
        + iowait_frame + irq_frame
        + softirq_frame;

    scale = 100.0 / (float)total_frame;


    fprintf(stdout, "%s: %.2f%%\n", prefix, 100.0 - (float)idle_frame*scale);

    cpu->user_save = cpu->user;
    cpu->nice_save = cpu->nice;
    cpu->system_save = cpu->system;
    cpu->idle_save = cpu->idle;
    cpu->iowait_save = cpu->iowait;
    cpu->irq_save = cpu->irq;
    cpu->softirq_save = cpu->softirq;

}

int main(int argc, char ** argv)
{
    cpu_t   *cpu;

    cpu = (cpu_t*)malloc(sizeof(cpu_t));
    memset(cpu, 0, sizeof(cpu_t));

    cpu_refresh(cpu);
    sleep(1);

    while(1)
    {
        cpu_refresh(cpu);
#ifndef  DEBUG
        system("clear");
#endif
        format_output(cpu, "CPU usage");
        sleep(1);
    }

    return 0;
}
