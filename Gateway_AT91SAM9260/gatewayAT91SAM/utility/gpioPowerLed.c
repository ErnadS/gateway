/*
 * gpioPowerLed.c
 *
 *  Created on: Mar 28, 2012
 *      Author: ernad
 */


/*
 * MyGpio.c
 *
 *  Created on: Mar 3, 2012
 *      Author: ernad
 */
/*
 * GpioButton.cpp
 *
 *  Created on: Mar 3, 2012
 *      Author: ernad
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include <sys/shm.h>

#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <unistd.h>
#include <fcntl.h>
#include <strings.h>
#include <string.h>
#include <time.h>

#include "AT91SAM9260.h"
#include "gpioPowerLed.h"
#include "MyGpio.h"


void blinkPowerLed(void) {
	setPowerLedOutput(0);
	usleep(300000);
	setPowerLedOutput(1);
	usleep(300000);
	setPowerLedOutput(0);
	usleep(300000);
	setPowerLedOutput(1);
	usleep(300000);
	setPowerLedOutput(0);
	usleep(300000);
	setPowerLedOutput(1);
	usleep(300000);
	setPowerLedOutput(0);
	usleep(300000);
	setPowerLedOutput(1);
	usleep(300000);
	setPowerLedOutput(0);
	usleep(300000);
	setPowerLedOutput(1);
	usleep(300000);
	setPowerLedOutput(0);
	usleep(300000);
	setPowerLedOutput(1);
		usleep(300000);
		setPowerLedOutput(0);
		usleep(300000);
		setPowerLedOutput(1);
		usleep(300000);
		setPowerLedOutput(0);
		usleep(300000);
		setPowerLedOutput(1);
		usleep(300000);
		setPowerLedOutput(0);
		usleep(300000);
		setPowerLedOutput(1);
}







