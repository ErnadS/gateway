/*
 * modBusInterf.c
 *
 *  Created on: 13. okt. 2010
 *      Author: Ernad
 */

#include "modBusInterf.h"
#include "modbus.h"
#include <stdio.h>
#include <syslog.h>
#include "modbus-private.h"
modbus_t* mb;
pthread_t modBusThread;
modbus_mapping_t *mb_mapping; // place to save modBus values
char modBusRunThread;

void * modBusInterface_modBusThreadTask(void *ptr);

// "/dev/ttyS1", 19200, 'N', 8, 1
char modBusInterf_start(const char* serialName, unsigned char baudRate,
		char parity, unsigned char bytes, unsigned char stopBit, int slaveId) {
	int rc;
	unsigned int br;
	char par;

	switch(baudRate) {
	case 0:
		br = 1200;
	break;
	case 1:
		br = 2400;
	break;
	case 2:
		br = 4800;
	break;
	case 3:
		br = 9600;
	break;
	//case 0:
		//br = 14400;
	//break;
	case 4:
		br = 19200;
	break;
	case 5:
		br = 28800;
	break;
	case 6:
		br = 38400;
	break;
	//case 0:
		//br = 56000;
	//break;
	case 7:
		br = 57600;
	break;
	case 8:
		br = 115200;
	break;
	default:
		syslog(LOG_ERR,  "Error modBusInterf_start, wrong baudRate index: %u\n", baudRate);
		return -1;
	break;
	}

	if (parity == 0)
		par = 'N';
	else if (parity == 1)
		par = 'O';
	else if (parity == 2)
		par = 'E';
	else {
		syslog(LOG_ERR,  "Error modBusInterf_start, wrong parity index: %u\n", parity);
		return -1;
	}

	if (bytes < 0 || bytes > 3 || stopBit < 0 || stopBit > 1) {
		syslog(LOG_ERR,  "Error modBusInterf_start, bits index: %u, stopBit index: %u\n", bytes, stopBit);
		return -1;
	}

//////////////////////////////////////////////////////////////////////////////////////////////////////
	if (modBusRunThread == 1) {
		modBusInterf_stop();
	}

	mb = modbus_new_rtu(serialName, br, par, bytes + 5, stopBit + 1, slaveId); // last argument is "start as slave"

	mb->debug = 1;
	rc = modbus_connect(mb);
	if (rc < 0) {
		syslog(LOG_ERR,  "Error modbus_connect\n");
		return -1;
	}
	syslog(LOG_ERR,  "ModBus start. Address: %u, Baud Rate: %u, parity: %c, bytes: %u, stop bytes: %u\n", slaveId, br, par, (bytes + 5), (stopBit + 1));
	modBusRunThread = 1;
	mb_mapping = modbus_mapping_new(1, 0, 0, 0);
	mb_mapping->tab_bits[0] = 0; // init value: not alarm

	return pthread_create(&modBusThread, NULL, modBusInterface_modBusThreadTask,
				(void*) NULL);
}

char modBusInterf_stop() {
	modBusRunThread = 0;
	modbus_close(mb);
	return 0;
}

void modBusInterface_notifyAlarm() {
	if (modBusRunThread == 1)
		mb_mapping->tab_bits[0] = 1;
}

void * modBusInterface_modBusThreadTask(void *ptr) {
	unsigned char hhh[100];
	int modBusResult;

	while (modBusRunThread == 1) {
		modBusResult = modbus_receive(mb, -1, hhh);
		if (modBusResult > 0)
			modbus_reply(mb, hhh, modBusResult, mb_mapping);
		else {
			//printf("Modbus timeout?\n");
		}
	}
	syslog(LOG_ERR, "ModBus closed\n");
	modbus_free(mb);
	return (void*)0;
}

