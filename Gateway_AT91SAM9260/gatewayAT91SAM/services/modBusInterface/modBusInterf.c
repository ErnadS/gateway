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

#include "LinkedListAlrMsg.h"

modbus_t* mb;
pthread_t modBusThread;
pthread_t modBusRegistersThread;

modbus_mapping_t *mb_mapping; // place to save modBus values
char modBusRunThread;

void * modBusInterface_modBusThreadTask(void *ptr);
void * modBusInterface_ThreadUpdateRegisters(void *ptr);

//void modBusInterf_writeAlarmReg(int registerNo, char* msg);

void modBusInterf_writeInsulationResistance(uint16_t newValue);
void modBusInterf_writeTransformTemperature(uint16_t newValue);
void modBusInterf_writeTransformLoad(uint16_t newValue);
void modBusInterf_writePrefferedSuppVolt(uint16_t newValue);
void modBusInterf_writeSecondary_1_SuppVot(uint16_t newValue);
void modBusInterf_writeSecondary_2_SuppVot(uint16_t newValue);
void modBusInterf_writeIT_transformOutpVolt(uint16_t newValue);
void modBusInterf_writeExternAlarmInput_1(uint16_t newValue);
void modBusInterf_writeExternAlarmInput_2(uint16_t newValue);

// moved to .h file
/*
void modBusInterf_writeLocation(char* location);
void modBusInterf_writeMAC(char* mac);
void modBusInterf_writeSwVersion(char* ver);
*/
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
	return (void*) 0;
}
// 								MOD BUS REG					CAN Index	Subindex	Unit type
//------------------------------------------------------------------------------------------------------
// Insulation resistance     		30011						0x3000 		1		 TG
// Transformer temperature   		30012						0x3002		1		 TG
// Transformer load 	     		30013	 					0x3001 		1		 TG 	(Transformer current)

// Preferred supply voltage	 		30014
// Secondary 1 supply voltage	 	30015						0x3000 		1		 SW 	(Supply 1 voltage)
// Secondary 2 supply voltage	 	30016						0x3001 		1		 SW 	(Supply 2 voltage)
// IT transformer output voltage    30017
// Extern alarm input 1 state		30018
// Extern alarm input 2 state		30019

// GateWay alarm registers:
//------------------------------------------------------------------------------------------------------
// Alarm register 1					30020
// Alarm register 2					30020
// Alarm register 3					30020
// Alarm register 4					30020
// Alarm register 5					30020

// GateWay info:
//------------------------------------------------------------------------------------------------------
// Location/ID						30080
// Ethernet Mac address/S.no.		30092
// SW version						30095

#include "../../unitGW/model/linkedList/deviceLinkedList.h"
#include "../../unitGW/model/device.h"
#include "../../can/canInterface.h"
#include <unistd.h>

////////////////////////////////////////////////////
// Uncomment this line for testing of CAN bus
////////////////////////////////////////////////////
// #define CAN_STRES_TEST

void * modBusInterface_ThreadUpdateRegisters(void *ptr) {
	unsigned char status;
	uint16_t result_16;

	DEV* device;

	sleep(5);
	int i;

	while (modBusRunThread) {
#ifdef CAN_STRES_TEST
		for (i = 0; i < 1000; i++) {
#endif
			device = deviceLinkedList_getDeviceByType(TG);

			if (device != NULL) {
				status = canInterface_getUint16((unsigned int*) (&result_16),
						device->adr, 0x3000, 1);
				if (status == CAN_OK) {
					modBusInterf_writeInsulationResistance(result_16);
				}

				status = canInterface_getUint16((unsigned int*) (&result_16),
						device->adr, 0x3002, 1);
				if (status == CAN_OK) {
					modBusInterf_writeTransformTemperature(result_16);
				}

				status = canInterface_getUint16((unsigned int*) (&result_16),
						device->adr, 0x3001, 1);
				if (status == CAN_OK) {
					modBusInterf_writeTransformLoad(result_16);
				}
			}

			device = deviceLinkedList_getDeviceByType(SW);
			if (device != NULL) {
				status = canInterface_getUint16((unsigned int*) (&result_16),
						device->adr, 0x3000, 1);
				if (status == CAN_OK) {
					modBusInterf_writePrefferedSuppVolt(result_16);
				}
				status = canInterface_getUint16((unsigned int*) (&result_16),
						device->adr, 0x3001, 1);
				if (status == CAN_OK) {
					modBusInterf_writeSecondary_1_SuppVot(result_16);
				}

				status = canInterface_getUint16((unsigned int*) (&result_16),
						device->adr, 0x3002, 1);
				if (status == CAN_OK) {
					modBusInterf_writeSecondary_2_SuppVot(result_16);
				}
			}
#ifdef CAN_STRES_TEST
		}
		sleep(1);
#else
		sleep(30);// 30 sec
#endif
	} // while

	return 0;
}

// "/dev/ttyS1", 19200, 'N', 8, 1
char modBusInterf_start(const char* serialName, unsigned char baudRate,
		char parity, unsigned char bytes, unsigned char stopBit, int slaveId) {
	int rc;
	unsigned int br;
	char par;

	switch (baudRate) {
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
		syslog(LOG_ERR, "Error modBusInterf_start, wrong baudRate index: %u\n",
				baudRate);
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
		syslog(LOG_ERR, "Error modBusInterf_start, wrong parity index: %u\n",
				parity);
		return -1;
	}

	if (bytes < 0 || bytes > 3 || stopBit < 0 || stopBit > 1) {
		syslog(LOG_ERR,
				"Error modBusInterf_start, bits index: %u, stopBit index: %u\n",
				bytes, stopBit);
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
		syslog(LOG_ERR, "Error modbus_connect\n");
		return -1;
	}
	syslog(LOG_ERR,
			"ModBus start. Address: %u, Baud Rate: %u, parity: %c, bytes: %u, stop bytes: %u\n",
			slaveId, br, par, (bytes + 5), (stopBit + 1));
	modBusRunThread = 1;
	mb_mapping = modbus_mapping_new(1, 0, 100, 0); // we have total 86 registers * 2 bytes (but starts from reg 10)
	mb_mapping->tab_bits[0] = 0; // init value: not alarm

	pthread_create(&modBusThread, NULL, modBusInterface_modBusThreadTask,
			(void*) NULL);

	return pthread_create(&modBusRegistersThread, NULL,
			modBusInterface_ThreadUpdateRegisters, (void*) NULL);
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

// Register address 30011 (2 bytes)
void modBusInterf_writeInsulationResistance(uint16_t newValue) {
	mb_mapping->tab_registers[INSULATION_RESISTANCE_ADDR] = newValue; // tab_register is used when received command "FC_READ_HOLDING_REGISTERS"
}

// Register address 30012 (2 bytes)
void modBusInterf_writeTransformTemperature(uint16_t newValue) {
	mb_mapping->tab_registers[TRANSFORM_TEMPER_ADDR] = newValue;
}

// Register address 30013 (2 bytes)
void modBusInterf_writeTransformLoad(uint16_t newValue) {
	mb_mapping->tab_registers[TRANSFORM_LOAD_ADDR] = newValue;
}

// Register address 30014 (2 bytes)
void modBusInterf_writePrefferedSuppVolt(uint16_t newValue) {
	mb_mapping->tab_registers[PREFERRED_SUPP_VOLT_ADDR] = newValue;
}

// Register address 30015 (2 bytes)
void modBusInterf_writeSecondary_1_SuppVot(uint16_t newValue) {
	mb_mapping->tab_registers[SECOND_1_SUPPL_VOLT_ADDR] = newValue;
}

// Register address 30016 (2 bytes)
void modBusInterf_writeSecondary_2_SuppVot(uint16_t newValue) {
	mb_mapping->tab_registers[SECOND_2_SUPPL_VOLT_ADDR] = newValue;
}

// Register address 30017 (2 bytes)
void modBusInterf_writeIT_transformOutpVolt(uint16_t newValue) {
	mb_mapping->tab_registers[IT_TRANSFORM_OUTP_VOLT_ADDR] = newValue;
}

// Register address 30018 (2 bytes but used only 1 bit)
void modBusInterf_writeExternAlarmInput_1(uint16_t newValue) {
	mb_mapping->tab_registers[EXTERN_ALARM_INPUT_1_ADDR] = newValue;
}

// Register address 30019 (2 bytes but used only 1 bit)
void modBusInterf_writeExternAlarmInput_2(uint16_t newValue) {
	mb_mapping->tab_registers[EXTERN_ALARM_INPUT_2_ADDR] = newValue;
}

void modBusInterf_writeLocation(char* location) {
	memcpy(&(mb_mapping->tab_registers[GW_LOCATION_ID_ADDR]), location, 12);
}

// Argument is without ":"
// First 6 chars are "Organisationally Unique Identifier". We are not using tham
// Last 6 are "Network Interface Contoller Specific" and we are using the same as Serial Number
void modBusInterf_writeMAC(char* mac) {
	char netwInteraceMac[6]; //last 6 chars;

	if (strlen(mac) == 12) {
		netwInteraceMac[0] = mac[6];
		netwInteraceMac[1] = mac[7];

		netwInteraceMac[2] = mac[8];
		netwInteraceMac[3] = mac[9];

		netwInteraceMac[4] = mac[10];
		netwInteraceMac[5] = mac[11];

		memcpy(&(mb_mapping->tab_registers[GW_ETHERN_MAC_ADDR]), netwInteraceMac, 6);
	}
}

// argument is e.g. "R.3.02"
// Remove "R" and first point
void modBusInterf_writeSwVersion(char* ver) {
	char numbVer[4];
	if (strlen(ver) == 6) {
		numbVer[0] = ver[2];
		numbVer[1] = ver[3];
		numbVer[2] = ver[4];
		numbVer[3] = ver[5];
		memcpy(&(mb_mapping->tab_registers[GW_SW_VER_ADDR]), numbVer, 4);
	}
}



void modBusInerf_addNewAlarmMsg(char* msg) {
	int nLength = strlen(msg);
	if (nLength > 23)
		msg[23] = 0;

	alr_LinkedList_addElement(msg);

	alr_LinkedList_copyAll((char*)(&(mb_mapping->tab_registers[ALARM_REG_1_ADDR])));
}

void modBusInerf_clearAlarmMsg(char* msg) {

	alr_LinkedList_removeElement(msg);
	alr_LinkedList_copyAll((char*)(&(mb_mapping->tab_registers[ALARM_REG_1_ADDR])));
}

