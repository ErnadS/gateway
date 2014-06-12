/*
 * watchDogInterface.c
 *
 *  Created on: 30. aug. 2010
 *      Author: Ernad
 */

#include "watchDogInterface.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/watchdog.h>

int watchDog_fd;

char watchDogInterface_start(void) {
	watchDog_fd = open("/dev/watchdog", O_RDWR);

	if (watchDog_fd == -1) {
		printf("!!! ERROR: Watchdog device not enabled\n");
		return 1;
	}
	else
		printf("!!! Watchdog device enabled\n");

	return 0;
}


void watchDogInterface_keepAlive(void) {
	unsigned long dummy=0;

	long ret;
	ret = ioctl(watchDog_fd, WDIOC_KEEPALIVE, dummy);

	if (ret != 0)
		printf("!!! watchdog keepAlive ERROR\n");
}
