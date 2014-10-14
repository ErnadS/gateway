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

#include "MyGpio.h"
/* ********************** LOCAL DEFINES ****************************** */

/* We will use signals in header SV3 for this demo
 *
 * Reference: SV3 pin 1  = +3.3V
 *            SV3 pin 9  = GND
 *            SV3 pin 10 = GND
 *
 * Use PC14 (SV3 pin 2) as output pin
 * Use PC5  (SV3 pin 4) as input pin
 *
 * see AT91SAM9260 datasheet for more information
 */
#define	OUTPUT_PIN	AT91C_PIO_PA9
#define	INPUT_PIN	AT91C_PIO_PA6 // AT91C_PIO_PC5


static int						memMapFile;
static AT91PS_PIOMAP			at91PioCtrlr;


/*
 * Set the output pin
 */
/*
static void SetOutput(unsigned high)
{
	if (high)
	{
		// set high
		at91PioCtrlr->PIOC_SODR = (OUTPUT_PIN);
	}
	else
	{
		// set low aka "clear"
		at91PioCtrlr->PIOC_CODR = (OUTPUT_PIN);
	}
	// do not worry about sync
}*/


/*
 * Read the input pin
 */
unsigned GetInput(void)
{
	if ((at91PioCtrlr->PIOA_PDSR) & (INPUT_PIN))
	{
		return (1);
	}

	return (0);
}


void setPowerLedOutput(unsigned high)
{
	if (high)
	{
		// set high
		at91PioCtrlr->PIOA_SODR = (OUTPUT_PIN);
	}
	else
	{
		// set low aka "clear"
		at91PioCtrlr->PIOA_CODR = (OUTPUT_PIN);
	}
	// do not worry about sync
}

/* *************** HARDWARE SUPPORT FUNCTIONS ************************ */


/*
 * Access the hardware
 */
int OpenSystemController(void)
{
	if ((memMapFile = open("/dev/mem", O_RDWR | O_SYNC)) < 0)
	{
		printf("ERROR: Unable to open /dev/mem\n");
		return (errno);
	}

	at91PioCtrlr = mmap(NULL, 4096, PROT_READ | PROT_WRITE,
		MAP_SHARED, memMapFile, (unsigned)AT91C_BASE_AIC);

	if (at91PioCtrlr == MAP_FAILED)
	{
		printf("ERROR: Unable to mmap the system controller\n");
		close (memMapFile);
		memMapFile = -1;
		return (errno);
	}

	/* set digital input ports */
	at91PioCtrlr->PIOA_ODR		= INPUT_PIN;
	at91PioCtrlr->PIOA_IFER		= INPUT_PIN;
	at91PioCtrlr->PIOA_PPUDR	= INPUT_PIN;
	at91PioCtrlr->PIOA_PER		= INPUT_PIN;


	/* set digital output port */
	at91PioCtrlr->PIOA_CODR		= OUTPUT_PIN;
	at91PioCtrlr->PIOA_PPUDR	= OUTPUT_PIN;
	at91PioCtrlr->PIOA_PER		= OUTPUT_PIN;
	at91PioCtrlr->PIOA_OER		= OUTPUT_PIN;

	/* sync memfile */

	setPowerLedOutput(1);

	return (0);
}




