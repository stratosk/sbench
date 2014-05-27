/*
 * Simple Benchmark
 *
 * sbench.c
 *
 *  Copyright (C) 2014 Stratos Karafotis <stratosk@semaphore.gr>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#define _GNU_SOURCE
#include <pthread.h>
#include <stdio.h>
#include <sys/time.h>
#include <inttypes.h>
#include <unistd.h>
#include <sched.h>
#include <errno.h>

#define NSEC		(1000000000)
#define MSEC2NSEC	(1000000)
#define CAL_LOOP	(100000000)
#define DEF_CNT		(10000000)
#define DEF_ITER	(10)
#define SLEEP		(200000000)

char *progname;
unsigned long cnt, nsecs, iter, sleep_time;
struct timespec ts_start, ts_end, ts_total_start, ts_total_end;

unsigned long do_bench()
{
	unsigned long i;

	clock_gettime(CLOCK_MONOTONIC, &ts_start);

	for (i = 0; i < cnt; i++);

	clock_gettime(CLOCK_MONOTONIC, &ts_end);

	nsecs = (ts_end.tv_sec - ts_start.tv_sec) * NSEC +
                (ts_end.tv_nsec - ts_start.tv_nsec);

	return nsecs / 1000;
}

void inline n_sleep(unsigned long t)
{
	struct timespec sl;
	int ret;

	sl.tv_sec = 0;
	sl.tv_nsec = t;

	while (1) {
		ret = nanosleep(&sl, &sl);
		if (!ret)
			return;
		else if (ret == EINTR)
			continue;
	}
}

void benchmark()
{
	unsigned long i;
	unsigned long total = 0;
	unsigned long sec, msec, bt;

	clock_gettime(CLOCK_MONOTONIC, &ts_total_start);	

	for (i = 0; i < iter; i++) {
		printf("Iteration %u, ", i);

		bt = do_bench();
		total += bt;

		printf("Elapsed time: %u usecs\n", bt);	
		n_sleep(sleep_time);
	}
	total /= iter;

	clock_gettime(CLOCK_MONOTONIC, &ts_total_end);
	sec = ts_total_end.tv_sec - ts_total_start.tv_sec;
	msec = (ts_total_end.tv_nsec - ts_total_start.tv_nsec) / MSEC2NSEC;

	printf("Average time: %u\n", total);
	printf("Total elapsed time: %u.%u\n", sec, msec);
}

void usage()
{
	errx(1, "%s: [-c counter][-i iterations][-s sleep time]\n", progname);
}

void cmdline(int argc, char **argv)
{
	int opt;

	progname = argv[0];

	while ((opt = getopt(argc, argv, "c:i:s:")) != -1) {
		switch (opt) {
		case 'c':
			cnt = atoi(optarg);
			break;
		case 'i':
			iter = atoi(optarg);
			break;
		case 's':
			sleep_time = atoi(optarg) * MSEC2NSEC;
			break;
		default:	
			usage();
		}
	}
}

int main(int argc, char **argv)
{
	cnt = DEF_CNT;
	iter = DEF_ITER;
	sleep_time = SLEEP;

	cmdline(argc, argv);

	cpu_set_t  mask;
	CPU_ZERO(&mask);
	CPU_SET(2, &mask);
	sched_setaffinity(0, sizeof(mask), &mask);

	printf("Starting benchmark\n");
	printf("cnt = %u, iter = %u, sleep = %u\n\n", cnt, iter, sleep_time);

	benchmark();	

	return 0;
}
