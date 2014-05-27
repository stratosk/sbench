/*
 * Simple Benchmark
 *
 * calibrate.c
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

#define NSEC 		(1000000000)
#define MSEC2NSEC	(1000000)
#define CAL_LOOP	(100000000)

void calibrate()
{
	uint64_t i, cnt, nsecs;
	struct timespec ts_start, ts_end;

	printf("Calibrating...\n");
	printf("NSEC=%u\n", NSEC);
	printf("CAL_LOOP=%u\n", CAL_LOOP);

	sleep(5);

	clock_gettime(CLOCK_MONOTONIC, &ts_start);

	for (i = 0; i < CAL_LOOP; i++);

	clock_gettime(CLOCK_MONOTONIC, &ts_end);

	nsecs = (ts_end.tv_sec - ts_start.tv_sec) * NSEC +
		(ts_end.tv_nsec - ts_start.tv_nsec);
	printf("nsec = %u\n", nsecs);

	/* find the counter for 10ms */
	cnt = i * (10 * MSEC2NSEC) / nsecs;

	printf("cnt: %u\n", cnt);
}

int main(void)
{
	calibrate();

	return 0;
}
