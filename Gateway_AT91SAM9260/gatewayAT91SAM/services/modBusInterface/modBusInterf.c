/*
 * modBusInterf.c
 *
 *  Created on: 13. okt. 2010
 *      Author: Ernad
 */

#include "modBusInterf.h"
#include "modbus.h"
#include <stdio.h>
#include <string.h>
#include <syslog.h>
#include "modbus-private.h"

void modBusInterf_writeAlarmReg(int registerNo, char* msg);

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
	mb_mapping = modbus_mapping_new(1, 0, 0, 86);  // 86 registers * 2 bytes
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
// 2 bytes (unsigned short)
#define INSULATION_RESISTANCE_ADDR   	10  // 30001 + 10 = 30011
#define TRANSFORM_TEMPER_ADDR   		11
#define TRANSFORM_LOAD_ADDR   			12
#define PREFERRED_SUPP_VOLT_ADDR   		13
#define SECOND_1_SUPPL_VOLT_ADDR   		14
#define SECOND_2_SUPPL_VOLT_ADDR   		15
#define IT_TRANSFORM_OUTP_VOLT_ADDR   	16
#define EXTERN_ALARM_INPUT_1_ADDR   	17
#define EXTERN_ALARM_INPUT_2_ADDR   	18

// Text
#define ALARM_REG_1_ADDR   				19  // 30001 + 19 = 30020
#define ALARM_REG_2_ADDR   				31
#define ALARM_REG_3_ADDR   				44
#define ALARM_REG_4_ADDR   				56
#define ALARM_REG_5_ADDR   				68

#define GW_LOCATION_ID_ADDR				79
#define GW_ETHERN_MAC_ADDR				91
#define GW_SW_VER_ADDR					94


void modBusInterf_writeInsulationResistance(uint16_t newValue) {
	mb_mapping->tab_registers[INSULATION_RESISTANCE_ADDR] = newValue;  // tab_register is used when received command "FC_READ_HOLDING_REGISTERS"
}

void modBusInterf_writeTransformTemperature(uint16_t newValue) {
	mb_mapping->tab_registers[TRANSFORM_TEMPER_ADDR] = newValue;
}

void modBusInterf_writeTransformLoad(uint16_t newValue) {
	mb_mapping->tab_registers[TRANSFORM_LOAD_ADDR] = newValue;
}

void modBusInterf_writePrefferedSuppVolt(uint16_t newValue) {
	mb_mapping->tab_registers[PREFERRED_SUPP_VOLT_ADDR] = newValue;
}

void modBusInterf_writeSecondary_1_SuppVot(uint16_t newValue) {
	mb_mapping->tab_registers[SECOND_1_SUPPL_VOLT_ADDR] = newValue;
}

void modBusInterf_writeSecondary_2_SuppVot(uint16_t newValue) {
	mb_mapping->tab_registers[SECOND_2_SUPPL_VOLT_ADDR] = newValue;
}

void modBusInterf_writeIT_transformOutpVolt(uint16_t newValue) {
	mb_mapping->tab_registers[IT_TRANSFORM_OUTP_VOLT_ADDR] = newValue;
}

void modBusInterf_writeExternAlarmInput_1(uint16_t newValue) {
	mb_mapping->tab_registers[EXTERN_ALARM_INPUT_1_ADDR] = newValue;
}

void modBusInterf_writeExternAlarmInput_2(uint16_t newValue) {
	mb_mapping->tab_registers[EXTERN_ALARM_INPUT_2_ADDR] = newValue;
}

int nCurrentModBusAlarmIndex = 0;
void modBusInerf_addNewAlarmMsg(char* msg) {
	switch (nCurrentModBusAlarmIndex) {
	case 0:
		modBusInterf_writeAlarmReg(ALARM_REG_1_ADDR, msg);
		break;
	case 1:
		modBusInterf_writeAlarmReg(ALARM_REG_2_ADDR, msg);
		break;
	case 2:
		modBusInterf_writeAlarmReg(ALARM_REG_3_ADDR, msg);
		break;
	case 3:
		modBusInterf_writeAlarmReg(ALARM_REG_4_ADDR, msg);
		break;
	case 4:
		modBusInterf_writeAlarmReg(ALARM_REG_5_ADDR, msg);
		break;
	}

	nCurrentModBusAlarmIndex ++;

	if (nCurrentModBusAlarmIndex > 4)
		nCurrentModBusAlarmIndex = 0;
}

/////////////// TEXT //////////////////////
void modBusInterf_writeAlarmReg(int registerNo, char* msg) {
	int nLength = strlen(msg);
	if (nLength > 24)
		nLength = 24;

	int nRest = nLength % 2;
	nLength -= nRest;
	int i;
	uint16_t temp;

	for (i = 0; i < nLength/2; i++) {
		temp = msg[2*i] << 8 | msg[2*i+1];
		mb_mapping->tab_registers[registerNo + i] = temp;
	}

	// if odd, add last char
	if (nRest > 0) {
		mb_mapping->tab_registers[registerNo + i] = msg[nLength -1];
		i++;
	}

	// fill rest with zero
	for (; i<12; i++) {
		mb_mapping->tab_registers[registerNo + i] = 0;
	}
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

