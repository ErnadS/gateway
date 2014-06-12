/*
 * serial.c
 *
 *  Created on: 26. sep. 2010
 *      Author: Ernad
 */
#include <stdio.h>   /* Standard input/output definitions */
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */

#include <stdio.h>   /* Standard input/output definitions */
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */

/*
fd = open("/dev/ttyS0", O_RDWR | O_NOCTTY | O_NDELAY);
	if (fd == -1) {
		perror("open_port: Unable to open /dev/ttyS0 - ");
		return 1;
	} else {
		fcntl(fd, F_SETFL, 0);
	}


	prije citanja:
	fcntl(fd, F_SETFL, FNDELAY); // don't block serial read


	close(fd);

*/


int initport(int fd, unsigned int bautRate) {
	struct termios options;

	int nBautRate_int = 15;
	/*
	 temios.h
		#define  B9600	0000015
		#define  B19200	0000016
		#define  B38400	0000017
		#define  B57600   0010001
		#define  B115200  0010002
	 */

	switch(bautRate) {
	case 9600:
		nBautRate_int = 15;
		break;
	case 19200:
		nBautRate_int = 16;
			break;
	case 38400:
		nBautRate_int = 17;
			break;
	case 57600:
		nBautRate_int = 18;
			break;
	case 115200:
		nBautRate_int = 0010002; //19;
			break;
	}
	// Get the current options for the port
	tcgetattr(fd, &options);
	// Change the input baud rates
	cfsetispeed(&options, nBautRate_int);
	// Change the output baud rates
	cfsetospeed(&options, nBautRate_int);
	// Enable the receiver and set local mode...
	options.c_cflag |= (CLOCAL | CREAD);

	options.c_cflag &= ~PARENB;
	options.c_cflag &= ~CSTOPB;
	options.c_cflag &= ~CSIZE;
	options.c_cflag |= CS8;

	// Set the new options for the port...
	tcsetattr(fd, TCSANOW, &options);
	return 1;
}



int writeport(int fd, char *chars) {
	int n = write(fd, chars, strlen(chars));
	if (n < 0) {
		printf("write failed!\n");
		return 0;
	}
	printf("writen %d\n", n);
	return 1;
}

int readport(int fd, char *result) {
	int iIn = read(fd, result, 254);
	result[iIn-1] = 0x00;
	if (iIn < 0) {
		if (errno == EAGAIN) {
			printf("SERIAL EAGAIN ERROR\n");
			return 0;
		} else {
			printf("SERIAL read error %d %s\n", errno, strerror(errno));
			return 0;
		}
	}
	return 1;
}

int getbaud(int fd) {
	struct termios termAttr;
	int inputSpeed = -1;
	speed_t baudRate;
	tcgetattr(fd, &termAttr);
	/* Get the input speed.                              */
	baudRate = cfgetispeed(&termAttr);
	switch (baudRate) {
		case B0:      inputSpeed = 0; break;
		case B50:     inputSpeed = 50; break;
		case B110:    inputSpeed = 110; break;
		case B134:    inputSpeed = 134; break;
		case B150:    inputSpeed = 150; break;
		case B200:    inputSpeed = 200; break;
		case B300:    inputSpeed = 300; break;
		case B600:    inputSpeed = 600; break;
		case B1200:   inputSpeed = 1200; break;
		case B1800:   inputSpeed = 1800; break;
		case B2400:   inputSpeed = 2400; break;
		case B4800:   inputSpeed = 4800; break;
		case B9600:   inputSpeed = 9600; break;
		case B19200:  inputSpeed = 19200; break;
		case B38400:  inputSpeed = 38400; break;
	}
	return inputSpeed;
}
