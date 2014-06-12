/*#include <stdio.h>
 #include <stdlib.h>
 #include <sys/types.h>
 #include <sys/stat.h>
 #include <fcntl.h>
 #include <linux/ioctl.h>
 #include <string.h>
 //#include <linux/gpio_dev.h>
 #define GPIO_GET        _IO(IOC_GPIODEV_MAGIC, 10)
 #define GPIO_SET        _IO(IOC_GPIODEV_MAGIC, 11)
 #define GPIO_CLEAR      _IO(IOC_GPIODEV_MAGIC, 12)
 #define GPIO_DIR_IN     _IO(IOC_GPIODEV_MAGIC, 13)
 #define GPIO_DIR_OUT    _IO(IOC_GPIODEV_MAGIC, 14)


 void
 print_usage()
 {
 printf("gpioctl dirin|dirout|get|set|clear gpio\n");
 exit(0);
 }

 int
 main(int argc, char **argv)
 {
 int gpio_pin;
 int fd;
 int result = 0;

 if (argc != 3)
 {
 print_usage();
 }

 if ((fd = open("/dev/testDriver", O_RDWR)) < 0)
 {
 printf("Error whilst opening /dev/testDriver\n");
 return -1;
 }

 gpio_pin = atoi(argv[2]);

 printf("using gpio pin %d\n", gpio_pin);

 if (!strcmp(argv[1], "dirin"))
 {
 ioctl(fd, GPIO_DIR_IN, gpio_pin);
 } else if (!strcmp(argv[1], "dirout"))
 {
 ioctl(fd, GPIO_DIR_OUT, gpio_pin);
 } else if (!strcmp(argv[1], "get"))
 {
 result = ioctl(fd, GPIO_GET, gpio_pin);
 printf("Pin %d is %s\n", gpio_pin, (result ? "HIGH" : "LOW"));
 } else if (!strcmp(argv[1], "set"))
 {
 ioctl(fd, GPIO_SET, gpio_pin);
 } else if (!strcmp(argv[1], "clear"))
 {
 ioctl(fd, GPIO_CLEAR, gpio_pin);
 } else print_usage();

 return result;
 }

 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <string.h>
#include "nothing.h"

struct canControlMsg controlMsgStruct;

#define RTR 1<<4

char* postRequest;

void readMessage();
void sendMessage(int nAddress, char* buffer, char msgLength);
void writeRegister(char address, char data);
char readAddress(char address);
void disableInterrupt(void);
void enableInterrupt(void);

int fd;

int main() {

	int i;

	printf("Test driver starting\n");

	if ((fd = open("/dev/testDriver", O_RDWR)) < 0) {
		printf("ERROR: can't open driver\n");
		return -1;
	}
	sleep(2);
	char buufer[100];

	printf("Test driver open\n");

	//// TEST /////
	/*for (i = 0; i < 30; i++) {
	 char res = readAddress(i);
	 printf("addr: %d result: %d\n", i, res);
	 }*/

	controlMsgStruct.address = 15;
	controlMsgStruct.newValue = 0x00;
	ioctl(fd, IOCTL_GET, &controlMsgStruct);

	//////////////////////////////////////////////
	// SET "RESET MODE"
	//////////////////////////////////////////////
	controlMsgStruct.address = 0;

	for (i = 0; i < 100; i++) {
		controlMsgStruct.newValue = 0x23;
		ioctl(fd, IOCTL_SET, &controlMsgStruct); // SET "RESET" MODE

		ioctl(fd, IOCTL_GET, &controlMsgStruct);
		if ((controlMsgStruct.newValue & 0x23) > 0) {
			printf("controlMsgStruct.newValue: %d\n", controlMsgStruct.newValue);
			break;
		}
	}

	if (i >= 100) {
		printf("CANNOT SET RESET MODE\n");
		return 1;
	}

	//sleep(1);
	controlMsgStruct.newValue = 0;
	controlMsgStruct.address = 4;
	ioctl(fd, IOCTL_GET, &controlMsgStruct);

	//////////////////////////////////////////////
	// SET BUS TIMING (BAUT RATE = 250kb/s)
	//////////////////////////////////////////////
	controlMsgStruct.address = 6; // BUS_TIMING_0_REG
	controlMsgStruct.newValue = 64;//7; // BRP3
	ioctl(fd, IOCTL_SET, &controlMsgStruct);

	controlMsgStruct.address = 7; // BUS_TIMING_1_REG
	controlMsgStruct.newValue = 28;//0xA5;
	ioctl(fd, IOCTL_SET, &controlMsgStruct);

	//////////////////////////////////////////////
	// SET ACCEPTANCE
	//////////////////////////////////////////////
	controlMsgStruct.address = ACCEPTANCE_CODE_REG;
	controlMsgStruct.newValue = 0x00;
	ioctl(fd, IOCTL_SET, &controlMsgStruct);
	//sleep(1);

	controlMsgStruct.address = ACCEPTANCE_MASK_REG;
	controlMsgStruct.newValue = 0xFF; //Accept all ID
	ioctl(fd, IOCTL_SET, &controlMsgStruct);

	//////////////////////////////////////////////
	// SET OUTPUT MODE (side 52)
	//////////////////////////////////////////////
	controlMsgStruct.address = 8;
	controlMsgStruct.newValue = 0xDA;//0x4A; //0xDA;
	ioctl(fd, IOCTL_SET, &controlMsgStruct);

	//// TEST /////
	for (i = 0; i < 30; i++) {
		char res = readAddress(i);
		printf("addr: %d result: %d\n", i, res);
	}

	//////////////////////////////////////////////
	// SET NORMAL MODE
	//////////////////////////////////////////////
	controlMsgStruct.address = 0;

	for (i = 0; i < 100; i++) {
		ioctl(fd, IOCTL_GET, &controlMsgStruct);
		if (controlMsgStruct.newValue == 0x3A)
			break;
		controlMsgStruct.newValue = 0x3A;
		ioctl(fd, IOCTL_SET, &controlMsgStruct); // SET "NORMAL" MODE
	}

	if (i > 100) {
		printf("CAN NOT SET NORMAL MODE\n");
		return 1;
	}

	printf("Initialising OK\n");
	/*
	 for (i = 0; i < 9; i++) {
	 readAddress(i);
	 usleep(50000);
	 }
	 */
	sleep(1);
	/*	while (1) {

	 }*/

	writeRegister(1, 0x0C); //clear overrun interrupt and release rec buffer
	char buff[] = { 1, 2, 3, 4 };
	char test;
	while (1) {
		/*writeRegister(1, 0x04); //clear release rec buffer
		 test = readAddress(22);
		 printf("DATA1: %d\n", test);
		 test = readAddress(23);
		 printf("DATA2: %d\n", test);*/
		sleep(5);
		test = readAddress(CONTROL_REG);
		printf("CONTROL REG: %d\n", test);
		readMessage();
		/*
		 if (readAddress(2) & 4) { // transmit released
		 sendMessage(201, buff, 4);
		 printf("Sent msg\n");
		 } else {
		 printf("transmit not released\n");
		 }*/
	}

	return 1;
}

void readMessage() {
	int i;
	controlMsgStruct.address = 30; // ikke bra. Address 30 brukes for å si at vi henter
	// mottatte CAN message

	do {
		ioctl(fd, IOCTL_GET, &controlMsgStruct);

		char temp[200];
		sprintf(temp, "Received: ");
		int nPos = 10;
		for (i = 0; i < controlMsgStruct.resultLength; i++) {
			sprintf(&temp[nPos], "%d ", controlMsgStruct.result[i]);
			if (controlMsgStruct.result[i] > 99)
				nPos += 4;
			else if (controlMsgStruct.result[i] > 9)
				nPos += 3;
			else
				nPos += 2;
		}
		temp[nPos] = '\n';
		printf(temp);
	} while (controlMsgStruct.id > 0);
}

void sendMessage(int nAddress, char* buffer, char msgLength) {
	char address_H = (char) (nAddress >> 3);
	char address_L = (char) ((nAddress & 0x03) << 5);
	char i;

	if (address_L > 0) {
		address_L &= (~RTR);
		address_L |= msgLength;
		char test;
		for (i = 0; i < msgLength; i++) {
			writeRegister(12 + i, buffer[i]);
		}
	} else {
		address_L |= RTR;
	}
	writeRegister(10, address_H);
	writeRegister(11, address_L);

	writeRegister(COMMAND_REG, 1);
}

void disableInterrupt(void) {
	controlMsgStruct.address = 8;
	controlMsgStruct.newValue = 0xDA;//0x4A; //0xDA;
	ioctl(fd, IOCTL_SET, &controlMsgStruct);
}

void enableInterrupt(void) {
	controlMsgStruct.address = 8;
	controlMsgStruct.newValue = 0xDA;//0x4A; //0xDA;
	ioctl(fd, IOCTL_SET, &controlMsgStruct);
}

void writeRegister(char address, char data) {
	controlMsgStruct.address = address;
	controlMsgStruct.newValue = data;
	ioctl(fd, IOCTL_SET, &controlMsgStruct);
}

char readAddress(char address) {
	controlMsgStruct.address = address;
	ioctl(fd, IOCTL_GET, &controlMsgStruct);
	return controlMsgStruct.newValue;
}
