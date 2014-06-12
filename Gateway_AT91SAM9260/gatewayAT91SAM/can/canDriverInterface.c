/*
 * canInterface.c
 *
 *  Created on: 15. juni 2010
 *      Author: Ernad
 */
#include "canDriverInterface.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <string.h>
#include <sys/time.h>
#define RTR 1<<4

struct canControlMsg controlMsgStruct;
char* postRequest;
int fd;

char canDriverInterface_sendInitCommands(void);
/*void readMessage();
void sendMessage(int nAddress, char* buffer, char msgLength);*/
void writeRegister(char address, char data);
char readAddress(char address);
void disableInterrupt(void);
void enableInterrupt(void);

char canDriverInterface_init(void) {
	printf("CAN driver starting\n");

	if ((fd = open("/dev/SJA1000driver", O_RDWR)) < 0) {
		printf("ERROR: can't open CanBus driver\n");
		return 0;
	}

	printf("SJA1000driver driver open\n");

	return canDriverInterface_sendInitCommands();
}

char canDriverInterface_sendInitCommands(void) {
	int i;
	//////////////////////////////////////////////
	// SET "RESET MODE"
	//////////////////////////////////////////////
	controlMsgStruct.address = CONTROL_REG;

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
		return 0;
	}

	//sleep(1);
	/*controlMsgStruct.address = ACCEPTANCE_CODE_REG;
	 controlMsgStruct.newValue = 0;
	 ioctl(fd, IOCTL_GET, &controlMsgStruct);*/

	//////////////////////////////////////////////
	// SET BUS TIMING (BAUT RATE = 250kb/s)
	//////////////////////////////////////////////
	controlMsgStruct.address = BUS_TIMING_0_REG; // BUS_TIMING_0_REG
	controlMsgStruct.newValue = 64;//7; // BRP3
	ioctl(fd, IOCTL_SET, &controlMsgStruct);

	controlMsgStruct.address = BUS_TIMING_1_REG; // BUS_TIMING_1_REG
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
	controlMsgStruct.address = OUTPUT_CONTR_REG; // 8
	controlMsgStruct.newValue = 0xDA;// side 52. Out on TX0 and TX1. Both push and pull transistor connected
	ioctl(fd, IOCTL_SET, &controlMsgStruct);
	/*
	 //// TEST /////
	 for (i = 0; i < 30; i++) {
	 char res = readAddress(i);
	 printf("addr: %d result: %d\n", i, res);
	 }
	 */
	//////////////////////////////////////////////
	// SET NORMAL MODE
	//////////////////////////////////////////////
	controlMsgStruct.address = CONTROL_REG;

	for (i = 0; i < 100; i++) {
		ioctl(fd, IOCTL_GET, &controlMsgStruct);
		if (controlMsgStruct.newValue == 0x3A)
			break;
		controlMsgStruct.newValue = 0x3A;
		ioctl(fd, IOCTL_SET, &controlMsgStruct); // SET "NORMAL" MODE
	}

	if (i > 100) {
		printf("CAN NOT SET NORMAL MODE\n");
		return 0;
	}

	writeRegister(COMMAND_REG, 0x0C); //clear overrun interrupt and release rec buffer

	printf("Initializing OK\n");

	return 1;
}

unsigned char canDriverInterface_readMessage(Message *m) {
	int nResult;
	controlMsgStruct.address = 30; // ikke bra. Address 30 brukes for � si at vi henter
	// mottatte CAN message

	nResult = ioctl(fd, IOCTL_GET, &controlMsgStruct);
	if (nResult == -20) {//EPIPE returned (broken pipe)
		// sleep(50); //
		sleep(5); // Changed 2014.02.14
		canDriverInterface_sendInitCommands();
		return 0;
	}

	if (controlMsgStruct.id < 0)
		return 0;
	else {
	//	printf("rec, id= %d len: %d, msg: %u %u %u %u %u\n", controlMsgStruct.id, controlMsgStruct.resultLength, controlMsgStruct.result[0]
		                            //, controlMsgStruct.result[1], controlMsgStruct.result[2], controlMsgStruct.result[3], controlMsgStruct.result[4]);
		memcpy(m->data, controlMsgStruct.result, controlMsgStruct.resultLength);
		m->len = controlMsgStruct.resultLength;
		if (controlMsgStruct.resultLength > 0)
			m->rtr = 0;
		else
			m->rtr = 1;
		m->cob_id = controlMsgStruct.id;
	}

	return 1;
}

unsigned char canDriverInterface_sendMessage(int nAddress, char* buffer,
		char msgLength) {

	char address_H = (char) (nAddress >> 3);
	char address_L = (char) ((nAddress & 0x07) << 5);
	char i;

	if (msgLength > 0) {
		address_L &= (~RTR);
		address_L |= msgLength;

		for (i = 0; i < msgLength; i++) {
			writeRegister(TRANSMIT_BYTE_1_REG + i, buffer[(int) i]);
		}
	} else {
		address_L |= RTR;
	}
	writeRegister(10, address_H);
	writeRegister(11, address_L);

	writeRegister(COMMAND_REG, 1);

	//printf("sent CAN to ID: % d, length: %d, msg:%u,%u,%u\n", nAddress, msgLength, buffer[0], buffer[1], buffer[2]);
	return 1;
}


void disableInterrupt(void) {
}

void enableInterrupt(void) {
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

