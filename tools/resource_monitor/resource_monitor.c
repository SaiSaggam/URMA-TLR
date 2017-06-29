/*****************************************************************************
 * Copyright (c) 2015 Digi International Inc., All Rights Reserved
 *
 * This software contains proprietary and confidential information of Digi
 * International Inc.  By accepting transfer of this copy, Recipient agrees
 * to retain this software in confidence, to prevent disclosure to others,
 * and to make no use of this software other than that for which it was
 * delivered.  This is an unpublished copyrighted work of Digi International
 * Inc.  Except as permitted by federal law, 17 USC 117, copying is strictly
 * prohibited.
 *
 * Restricted Rights Legend
 *
 * Use, duplication, or disclosure by the Government is subject to
 * restrictions set forth in sub-paragraph (c)(1)(ii) of The Rights in
 * Technical Data and Computer Software clause at DFARS 252.227-7031 or
 * subparagraphs (c)(1) and (2) of the Commercial Computer Software -
 * Restricted Rights at 48 CFR 52.227-19, as applicable.
 *
 * Digi International Inc. 11001 Bren Road East, Minnetonka, MN 55343
 *
 *****************************************************************************/
#include "resource_monitor.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

// Maximum number of CPU's supported
#define MAX_CPU 16

// Check CPU usage once a second
#define SAMPLING_RATE 1

// Output the results to this file
#define OUTFILE "/tmp/cpu_usage"

static int num_cpus;

// Values from /proc/stat
static struct jiffy_counts_t
{
	unsigned long long user;
	unsigned long long nice;
	unsigned long long system;
	unsigned long long idle;
	unsigned long long iowait;
	unsigned long long irq;
	unsigned long long softirq;
	unsigned long long steal;
	unsigned long long guest;
	unsigned long long guest_nice;
} jiffy_counts;

static struct cpu_status_t {
	double current;
	double min;
	double max;
	double average;

	unsigned long long total;
	unsigned long long busy;
	unsigned long long prev_total;
	unsigned long long prev_busy;
} cpu_status[MAX_CPU];


static int read_cpu_jiffies(FILE *fp)
{
	char *line = NULL;
	size_t len=0;
	ssize_t read;
	int ret=-1, i;
	char buf[16];

	rewind(fp);
	fflush(fp);

	for (i=0; i < num_cpus; i++)
	{
		if ((read = getline(&line, &len, fp)) != -1) {
			sscanf(line, "%s %llu %llu %llu %llu %llu %llu %llu %llu %llu %llu",
				buf, &jiffy_counts.user, &jiffy_counts.nice,
				&jiffy_counts.system, &jiffy_counts.idle,
				&jiffy_counts.iowait, &jiffy_counts.irq,
				&jiffy_counts.softirq, &jiffy_counts.steal,
				&jiffy_counts.guest, &jiffy_counts.guest_nice);
		} else {
			goto read_jiffies_error;
		}

		// Save the previous counts
		cpu_status[i].prev_total = cpu_status[i].total;
		cpu_status[i].prev_busy  = cpu_status[i].busy;

		cpu_status[i].total = jiffy_counts.user + jiffy_counts.nice +
					jiffy_counts.system + + jiffy_counts.idle +
					jiffy_counts.iowait + jiffy_counts.irq +
					jiffy_counts.softirq + jiffy_counts.steal;

		cpu_status[i].busy  = cpu_status[i].total - jiffy_counts.idle - jiffy_counts.iowait;

	}

	ret = 0;

read_jiffies_error:
	if (line != NULL) {
		free(line);
	}

	return ret;
}


static void calculate_cpu_usage(void)
{
	double percent_usage;
	int count;
	static unsigned long long n=0;

	for (count=0; count < num_cpus; count++)
	{
		unsigned long long delta_total, delta_busy;

		delta_total = cpu_status[count].total - cpu_status[count].prev_total;
		if (delta_total == 0)
			delta_total = 1;

		delta_busy = cpu_status[count].busy - cpu_status[count].prev_busy;

		percent_usage = (delta_busy / (double) delta_total) * 100;

		cpu_status[count].current = percent_usage;

		cpu_status[count].max = (cpu_status[count].max < percent_usage) ? percent_usage : cpu_status[count].max;
		cpu_status[count].min = (cpu_status[count].min > percent_usage) ? percent_usage : cpu_status[count].min;

		// We don't keep a running sum to avoid overflow

		cpu_status[count].average = ((cpu_status[count].average * (10-1)) + (percent_usage))/ 10;

		//printf("percent_usage = %f\r\n", percent_usage);
		cpu_status[count].average += (percent_usage - cpu_status[count].average) / ++n;


	}
}

static int output_usage(void)
{
	int count;
	int ret=-1;
	FILE * fp;

	if ((fp = fopen(OUTFILE, "w+")) == NULL)
	{
		perror("fopen");
		goto output_error;
	}

	for (count=0; count < num_cpus; count++)
	{
		if (count == 0)
		{
			fprintf(fp, "Total CPU Usage: %3.2lf%%\r\n", cpu_status[count].current);
			fprintf(fp, "Total CPU Min:   %3.2lf%%\r\n", cpu_status[count].min);
			fprintf(fp, "Total CPU Max:   %3.2lf%%\r\n", cpu_status[count].max);
			fprintf(fp, "Total CPU Avg:   %3.2lf%%\r\n", cpu_status[count].average);
		}
		else
		{
			fprintf(fp, "\tCPU%d Usage: %3.2lf%%\r\n", count, cpu_status[count].current);
			fprintf(fp, "\tCPU%d Min:   %3.2lf%%\r\n", count, cpu_status[count].min);
			fprintf(fp, "\tCPU%d Max:   %3.2lf%%\r\n", count, cpu_status[count].max);
			fprintf(fp, "\tCPU%d Avg:   %3.2lf%%\r\n\r\n", count, cpu_status[count].average);
		}
		//printf("%3.2lf%%\r\n", cpu_status[count].current);
	}
	fflush(fp);

	ret = 0;
output_error:

	if (fp != NULL)
		fclose(fp);

	return ret;
}


static int get_num_cpus(void)
{
    int n;
#if defined(_CUNIT_)
    n = 4;
#else
    FILE *output=NULL;
	char buf[64];

    n = 0;
    output = popen("cat /proc/cpuinfo | grep processor | wc -l", "r");
	if (output != NULL)
	{
		if (fgets(buf, sizeof buf, output) != NULL)
		{
			n = atoi(buf);
		}

    	pclose(output);
	}
#endif
    return n;
}

// the num_iterations field is used for unit testing, a value of -1 indicates an
// infinite loop.
int res_main(int num_iterations)
{
	int ret=EXIT_FAILURE, i, num_loops=num_iterations;
	FILE *fp_proc = NULL;

	// For unit testing this code we use a different file
	// so we call a routine to get the name of the file.
	extern char *get_proc_stat_file_name(void);

	if((fp_proc = fopen(get_proc_stat_file_name(), "r")) == NULL)
	{
		perror("fopen");
		goto error;
	}

	num_cpus = get_num_cpus() + 1; // Add one for total CPU usage entry

	for (i = 0; i < num_cpus; i++)
	{
		// Initialize the minimum cpu usage to 100%
		cpu_status[i].min = 100;
	}

	// To calculate CPU usage we need at least two sets of readings, so we take
	// the first set, then wait the sampling period and take the second set.
	if (num_iterations == -1)
	{
		if (read_cpu_jiffies(fp_proc) < 0)
		{
			goto error;
		}
	}

	while (1)
	{
		sleep(SAMPLING_RATE);
		if (read_cpu_jiffies(fp_proc) < 0)
		{
			goto error;
		}

		calculate_cpu_usage();

		if (output_usage() < 0)
		{
			goto error;
		}

		if (num_iterations != -1)
		{
			if (--num_loops == 0)
				break;
		}
	}
	ret = EXIT_SUCCESS;

error:
	if (fp_proc != NULL)
	{
		fclose(fp_proc);
	}
	return ret;
}


