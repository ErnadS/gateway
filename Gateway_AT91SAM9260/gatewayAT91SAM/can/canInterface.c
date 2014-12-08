/*
 * canInterface.c
 *
 *  Created on: 28. jan. 2010
 *      Author: Ernad
 */
#include <string.h>
#include "canInterface.h"
#include "../unitGW/model/device.h"
#include "../unitGW/model/linkedList/deviceLinkedList.h"
//#include "../unitGW/dynamic/treeNodes.h"
#include "../unitGW/dynamic/gwSetupDeviceMonitor.h"
#include "../unitGW/utility/gwAlarmLogUtility.h"
#include "../services/modBusInterface/modBusInterf.h"
#include "../config.h"

#include <unistd.h> // sleep
#include <pthread.h> // mutex
// CAN BUS
#include "can_AVR.h"
#include "ncanbus.h"
#include "sdo.h"
#include "data.h"
#include "ObjDict.h"
#include <sys/time.h>
#include "../utility/myString.h"
#include <string.h>
#include "lifegrd.h"

#include "../Alarm.h"

#define fNaN (0.0f/0.0f)
#define n32NaN -2147483647

#define MAX_STRING_LENGTH  32

char bAlarmFlag = 0;
char alarmUnitName[25];
char alarmText[30];
unsigned int alarmIndex;
unsigned char alarmSubindex;
unsigned char alarmNodeID;
unsigned char alarmType = 0;
unsigned int alarm_errCode;

// ALARM CLEARED
char alarmClearedText[50];
unsigned char alarmClearedNodeID;

char bAlarmClearedFlag = 0;

#define CAN_TIMEOUT  320000   // 320000*10us = 3.2ms



char* canInterface_getHeardBeatPointer(){
	return canHeardBeat;
}


// VARIABLES SAVING RESULT:
char strResult[MAX_STRING_LENGTH + 1];
long nInt32Result;
unsigned char nUint8Result;
unsigned int nUint16Result;
RECEIVING_STATE receivingState;
char bWriteFinished;

pthread_mutex_t can_mutex;


// USED BY logging
unsigned int logIndex[] = { /*SW*/0x3000, 0x3001, 0x3002, 0x3003, /*TG*/0x3002,
		0x3001, 0x3000, /*IL*/0x3000, 0x3001, 0x3002, 0x3003, 0x3004, 0x3005,
		0x3006, 0x3007, 0x3008, 0x3009, 0x300A, 0x300B, 0x300C, 0x300D, 0x300E,
		0x300F };
unsigned int logSubindex[] = { /*SW*/1, 1, 1, 1, /*TG*/1, 1, 1, /*IL*/1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
unsigned int logVariableType[] = { /*SW*/1, 1, 1, 1, /*TG*/1, 1, 1, /*IL*/1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }; //uint8= 0, uint16=1, uint32=2, string=3, float=4


void canInterface_OnSdoReadUint8(CO_Data* d, unsigned char nodeid);
void canInterface_OnSdoReadInt16(CO_Data* d, unsigned char nodeid);
void canInterface_OnSdoReadInt32(CO_Data* d, unsigned char nodeid);
/*void canInterface_OnSdoReadFloat(CO_Data* d, unsigned char nodeid);
void canInterface_OnSdoReadChar(CO_Data* d, unsigned char nodeid);*/
void canInterface_OnSdoReadString(CO_Data* d, unsigned char nodeid);

void checkWriteSDO(CO_Data* d, unsigned char nodeid);

int canInterface_init(void) {
	unsigned char nodeID;
	nodeID = 127;
	timeToStartDevicesRefresh = -1; // do not start new search
	setNodeId(&ObjDict_Data, nodeID);
	setState(&ObjDict_Data, Operational);

	return 0;
}

unsigned long canInterface_getValue(DEV* device, int nMsgId) {
	unsigned long temp = 0;

	if (device->type == TG) {
		temp = 4; // first is SW and it has 4 elements
	} else if (device->type == IL) {
		temp = 7; // first is SW and TG. Thay has 4 + 3 elements
	} else if (device->type == SW) {
		temp = 0;
	} else {
		fprintf(stderr, "** ERR: canInterface_getValue, id: %u, wrong dev type: %u",
				device->adr, device->type);
		return ERROR_CAN;
	}

	unsigned char failed = NOT_RECEIVED;
	unsigned long result;

	if (logVariableType[temp] == 0) { // uint8
		failed = canInterface_getUint8((unsigned char*) (&result), device->adr,
				logIndex[temp + nMsgId], logSubindex[temp + nMsgId]);
	} else if (logVariableType[temp] == 1) { // uint16
		failed = canInterface_getUint16((unsigned int*) (&result), device->adr,
				logIndex[temp + nMsgId], logSubindex[temp + nMsgId]);
	} else if (logVariableType[temp] == 2) { // uint32
		failed = canInterface_getInt32((long*) (&result), device->adr,
				logIndex[temp + nMsgId], logSubindex[temp + nMsgId]);
	}

	if (failed != CAN_OK) {
		return ERROR_CAN;
	}

	return (unsigned long) result;
}

void * canInterface_canThreadTask(void *ptr) {
	static Message m = Message_Initializer;
#ifdef DEBUG_CAN_TIME
	unsigned long test = 0;
	struct timeval currentTime;
#endif
	while (1) {
#ifdef DEBUG_CAN_TIME
		test++;
		 if (test % 100000 == 0) {
		 gettimeofday(&currentTime, NULL);
		 printf("CAN 100000\n"); // rec: %lu\n", (currentTime.tv_sec * 1000000 + currentTime.tv_usec) / 8);
		 }
#endif
		if (canReceive(&m)) // a message received
		{
#ifdef DEBUG_CAN_TIME2
			gettimeofday(&currentTime, NULL);
			 printf("before: %lu\n", (currentTime.tv_sec * 1000000
			 + currentTime.tv_usec) / 8);
#endif

			 canDispatch(&ObjDict_Data, &m); // process it


#ifdef DEBUG_CAN_TIME2
			gettimeofday(&currentTime, NULL);
			 printf("after:  %lu\n", (currentTime.tv_sec * 1000000
			 + currentTime.tv_usec) / 8);
#endif
		} else {
			sleep(0);
		}
	}
	return 0;
}

//////////////////////////////////////////////
// callback from lifegrd after slave restart
//////////////////////////////////////////////
void onSlaveBootup(unsigned char slaveID) {
	printf("**** detected device ID: %d\n", slaveID);
	DEV* foundDevice;
	DEV dev;

	if (timeToStartDevicesRefresh < 5)
		timeToStartDevicesRefresh = 5; // start devices refresh about 10 sec.

	// NEW DEV
	if ((foundDevice = deviceLinkedList_getDevice(slaveID)) == NULL) {
		dev.enab = 1;
		dev.found = 1;
		dev.adr = slaveID;
		dev.type = -1;
		deviceLinkedList_addElement(&dev); // dev will be copied to list
	} else {
		foundDevice->found = 1;
	}
}

int canInterface_searchDevices(void) {
	printf("SENDER RESET\n");
	// Send BROADCAST RESET COMMANDO:
	masterSendNMTstateChange(&ObjDict_Data, 0, NMT_Reset_Comunication);

	sleep(3);

	return canInterface_refreshDevices();
}

int canInterface_refreshDevices(void) {
	int nSize = deviceLinkedList_getSize();
	printf("Found %u devices in list\n", nSize);

	int i;
	DEV * dev;
	long devType;
	ObjDict_highestSubIndex_obj1016 = 0;

	for (i = 0; i < nSize; i++) {
		dev = deviceLinkedList_getElementAt(i);
		//if ((dev->type == -1)) { // unknown
		unsigned char result = canInterface_getInt32((long*) (&devType),
				dev->adr, 0x1000, 0);
		if (result != 0) {
			fprintf(stderr, "CAN error reading dev type for Id = %u\n", dev->adr);
		} else {
			dev->type = ((devType) & 0xff000000) >> 24;
			printf("Dev Id = %u has type: %u\n", dev->adr, dev->type);
		}
		// Update heartbit timeout
		ObjDict_highestSubIndex_obj1016++;
		ObjDict_obj1016[i] = (0x4E20 | (dev->adr << 16));
	}

	lifegrd_clearErrors();

	for(i = 0; i < 126; i++) {
		canHeardBeat[i] = 0;
	}
	return 0;
}



unsigned char canInterface_getUint8(unsigned char* uint8Result,
		int nCanAddress, unsigned int nIndex, unsigned char nSubindex) {
	if(getState(&ObjDict_Data)==Stopped)
	{
#ifdef DEBUG_CAN
		fprintf(stderr, "Error reading Uint8, can stopped by master\n");
#endif
		return CAN_BUSY;
	}
	pthread_mutex_lock(&can_mutex);
	receivingState = NOT_RECEIVED;
	nUint8Result = 0;

	unsigned char result;
	result = readNetworkDictCallbackCC(&ObjDict_Data, nCanAddress, nIndex, nSubindex,
			uint8, canInterface_OnSdoReadUint8);

	if (result != 0) {
		pthread_mutex_unlock(&can_mutex);
#ifdef DEBUG_CAN
		printf(
				"fprintf(stderr, canInterface_getUint8. Ret: %d, Adr: %u, ind: %u, sub: %u\n",
				result, nCanAddress, nIndex, nSubindex);
#endif
		return 3;
	}
	int i;
	for (i = 0; i < CAN_TIMEOUT; i++) {
		if (receivingState == CAN_OK) {
			*uint8Result = nUint8Result;
#ifdef DEBUG_CAN
			printf("uint8Result: %u, nUint8Result: %u\n", *uint8Result,
					nUint8Result);
#endif
			pthread_mutex_unlock(&can_mutex);
			return 0; // SUCCESS
			//break;
		} else if (receivingState == CAN_ERROR) {
			fprintf(stderr, "Error getting Uint8, id:%u, ind:%u, sub:%u\n", nCanAddress, nIndex, nSubindex);
			pthread_mutex_unlock(&can_mutex);
			return 2; // received ERROR flag
		}
		usleep(10);
		//sleep(0);
	}
	closeSDOtransfer(&ObjDict_Data, nCanAddress, SDO_CLIENT);
	pthread_mutex_unlock(&can_mutex);
	return 1; //ERROR, not received answer
}

unsigned char canInterface_getUint16(unsigned int* uint16Result,
		int nCanAddress, unsigned int nIndex, unsigned char nSubindex) {
	if(getState(&ObjDict_Data)==Stopped)
	{
#ifdef DEBUG_CAN
		fprintf(stderr, "Error reading Uint16, can stopped by master\n");
#endif
		return CAN_BUSY;
	}

	pthread_mutex_lock(&can_mutex);
	receivingState = NOT_RECEIVED;
	nUint16Result = 0;

	unsigned char result;
	result = readNetworkDictCallbackCC(&ObjDict_Data, nCanAddress, nIndex, nSubindex,
			uint16, canInterface_OnSdoReadInt16);

	if (result != 0) {
		pthread_mutex_unlock(&can_mutex);
		fprintf(stderr, "ERROR, canInterface_getUint16. Ret: %d, Adr: %u, ind: %u, sub: %u\n",
				result, nCanAddress, nIndex, nSubindex);
		return 3;
	}

	int i;
	for (i = 0; i < CAN_TIMEOUT; i++) {
		if (receivingState == CAN_OK) {
			*uint16Result = nUint16Result;
			pthread_mutex_unlock(&can_mutex);
			return 0; // SUCCESS
			//break;
		} else if (receivingState == CAN_ERROR) {
			// printf("!!!!!!!!!!!!!!!! EXIT, CALLBACK WITH ERROR\n");
			pthread_mutex_unlock(&can_mutex);
			return 2; // received ERROR flag
		}
		usleep(10);
		//sleep(0);
	}
	closeSDOtransfer(&ObjDict_Data, nCanAddress, SDO_CLIENT);
	// printf("!!!!!!!!!!!!!!!! EXIT, CLOSE SDO CAUSED BY ERROR\n");
	pthread_mutex_unlock(&can_mutex);
	return 1; //ERROR, not received answer
}

unsigned char canInterface_getInt32(long *uint32Result, int nCanAddress,
		unsigned int nIndex, unsigned char nSubindex) {
	if(getState(&ObjDict_Data)==Stopped)
	{
#ifdef DEBUG_CAN
		fprintf(stderr, "Error reading Uint32, can stopped by master\n");
#endif
		return CAN_BUSY;
	}
	pthread_mutex_lock(&can_mutex);
	receivingState = NOT_RECEIVED;
	nInt32Result = 0;

	int i;
	unsigned char result;
	result = readNetworkDictCallbackCC(&ObjDict_Data, nCanAddress, nIndex, nSubindex,
			int32, canInterface_OnSdoReadInt32);

	if (result != 0) {
		pthread_mutex_unlock(&can_mutex);
		fprintf(stderr, "ERROR, canInterface_getInt32. Ret: %d, Adr: %u, ind: %u, sub: %u\n",
				result, nCanAddress, nIndex, nSubindex);
		return 3;
	}

	for (i = 0; i < CAN_TIMEOUT; i++) {
		if (receivingState == CAN_OK) {
			*uint32Result = nInt32Result;
			pthread_mutex_unlock(&can_mutex);
			return 0; // SUCCESS
		} else if (receivingState == CAN_ERROR) {
			pthread_mutex_unlock(&can_mutex);
			return CAN_ERROR; // Received error flag
		}
		usleep(10);
		//sleep(0);
	}
	closeSDOtransfer(&ObjDict_Data, nCanAddress, SDO_CLIENT);
	fprintf(stderr, "ERROR: getInt32 timeout, nCanAddress = %u, index = %u, sub = %u\n",
			nCanAddress, nIndex, nSubindex);
	pthread_mutex_unlock(&can_mutex);
	return CAN_ERROR; // Not received answer
}

unsigned char canInterface_getString(char* stringResult, int nCanAddress,
		unsigned int nIndex, unsigned char nSubindex) {
	if(getState(&ObjDict_Data)==Stopped)
	{
#ifdef DEBUG_CAN
		fprintf(stderr, "Error reading String, can stopped by master\n");
#endif
		return CAN_BUSY;
	}

	pthread_mutex_lock(&can_mutex);
	receivingState = NOT_RECEIVED;
	strResult[0] = 0;

	int i;
	unsigned char result;
	result = readNetworkDictCallbackCC(&ObjDict_Data, nCanAddress, nIndex,
			nSubindex, visible_string, canInterface_OnSdoReadString);

	if (result != 0) {
		pthread_mutex_unlock(&can_mutex);
		fprintf(stderr, "ERROR, canInterface_getString. Ret: %d, Adr: %u, ind: %u, sub: %u\n",
				result, nCanAddress, nIndex, nSubindex);
		return 3;
	}
	for (i = 0; i < CAN_TIMEOUT; i++) {
		if (receivingState == CAN_OK) {
#ifdef DEBUG_CAN
			printf("Received, before convert: 0x%02x 0x%02x 0x%02x 0x%02x\n", strResult[0], strResult[1],strResult[2],strResult[3]);
#endif
			myString_convertCANtoUTF8(strResult);
			//myString_validateString(strResult);
			strcpy(stringResult, strResult);
#ifdef DEBUG_CAN
			printf("Received from id=%d, str= %s, len: %u\n", nIndex, stringResult, strlen(stringResult));
#endif
			pthread_mutex_unlock(&can_mutex);
			return CAN_OK;
		} else if (receivingState == CAN_ERROR) {
			pthread_mutex_unlock(&can_mutex);
			return 2; // detected error on CAN bus
		}
		usleep(10);
		//sleep(0);
	}
	closeSDOtransfer(&ObjDict_Data, nCanAddress, SDO_CLIENT);
	fprintf(stderr, "ERROR: getString timeout\n");
	pthread_mutex_unlock(&can_mutex);
	return CAN_ERROR; // Error, not received answer
}


unsigned char canInterface_writeUint8(char nValue, unsigned char nCanAddress,
		unsigned int index, unsigned char subIndex) {

	if(getState(&ObjDict_Data)==Stopped)
	{
		return CAN_BUSY;
	}
	pthread_mutex_lock(&can_mutex);
	bWriteFinished = 0;
	int i;

	writeNetworkDictCallBack(&ObjDict_Data, nCanAddress, index, subIndex, 1,
			uint8, (void *) &nValue, checkWriteSDO);
	for (i = 0; i < CAN_TIMEOUT; i++) {
		if (bWriteFinished > 0) {
			pthread_mutex_unlock(&can_mutex);
			return (bWriteFinished - 1); // SUCCESS or Error
		}
		usleep(10);
		//sleep(0);
	}
	closeSDOtransfer(&ObjDict_Data, nCanAddress, SDO_CLIENT);
	pthread_mutex_unlock(&can_mutex);

	return 1; // TODO: ??? Close transfer (callback missing) // closeSDOtransfer(&ObjDict_Data, nodeid, SDO_CLIENT);
}

unsigned char canInterface_writeUint16(unsigned int nValue,
		unsigned char nCanAddress, unsigned int index, unsigned char subIndex) {

	if(getState(&ObjDict_Data)==Stopped)
	{
		return CAN_BUSY;
	}
	pthread_mutex_lock(&can_mutex);
	bWriteFinished = 0;
	int i;

	writeNetworkDictCallBack(&ObjDict_Data, nCanAddress, index, subIndex, 2,
			uint16, (void *) &nValue, checkWriteSDO);

	for (i = 0; i < CAN_TIMEOUT; i++) {
		if (bWriteFinished > 0) {
			pthread_mutex_unlock(&can_mutex);
			return (bWriteFinished - 1); // SUCCESS or Error
		}
		usleep(10);
		//sleep(0);
	}
	closeSDOtransfer(&ObjDict_Data, nCanAddress, SDO_CLIENT);
	pthread_mutex_unlock(&can_mutex);

	return 1; // TODO: ??? Close transfer (callback missing) // closeSDOtransfer(&ObjDict_Data, nodeid, SDO_CLIENT);
}

unsigned char canInterface_writeInt32(long nValue, unsigned char nCanAddress,
		unsigned int index, unsigned char subIndex) {

	if(getState(&ObjDict_Data)==Stopped)
	{
		return CAN_BUSY;
	}
	pthread_mutex_lock(&can_mutex);
	bWriteFinished = 0;
	int i;

	writeNetworkDictCallBack(&ObjDict_Data, nCanAddress, index, subIndex, 4,
			uint32, (void *) &nValue, checkWriteSDO);

	for (i = 0; i < CAN_TIMEOUT; i++) {
		if (bWriteFinished > 0) {
			pthread_mutex_unlock(&can_mutex);
			return (bWriteFinished - 1); // SUCCESS or Error
		}
		usleep(10);
		//sleep(0);
	}
	closeSDOtransfer(&ObjDict_Data, nCanAddress, SDO_CLIENT);
	pthread_mutex_unlock(&can_mutex);

	return 1; // TODO: ??? Close transfer (callback missing) // closeSDOtransfer(&ObjDict_Data, nodeid, SDO_CLIENT);
}

unsigned char canInterface_writeString(char* strValue,
		unsigned char nCanAddress, unsigned int index, unsigned char subIndex) {

	if(getState(&ObjDict_Data)==Stopped)
	{
		return CAN_BUSY;
	}

	pthread_mutex_lock(&can_mutex);
	char* newString = (char*)malloc(strlen(strValue) * 2);  // *2 is new in version 2.11 (13.04.2013)

	myString_convertUTF8toCAN(strValue, newString, 23);

	bWriteFinished = 0;
	int i;
	unsigned char res;

	if ((res = writeNetworkDictCallBack(&ObjDict_Data, nCanAddress, index,
			subIndex, strlen(newString), visible_string, (void *) newString,
			checkWriteSDO)) != 0) {
		fprintf(stderr, "Error. writeNetworkDictCallBack return 1. nCanAddress: %u, index: %u, subindex: %u\n",
				nCanAddress, index, subIndex);
		free(newString);
		pthread_mutex_unlock(&can_mutex); // NEW (var ikke i version

		return 1;
	}

	for (i = 0; i < CAN_TIMEOUT; i++) {
		if (bWriteFinished > 0) {
			free(newString);
			pthread_mutex_unlock(&can_mutex);
			return (bWriteFinished - 1); // SUCCESS or Error
		}
		usleep(10);
		//sleep(0);
	}

	closeSDOtransfer(&ObjDict_Data, nCanAddress, SDO_CLIENT);
	free(newString);
	pthread_mutex_unlock(&can_mutex);

	fprintf(stderr, "Error, writeString, TIMEOUT. nCanAddress: %u, index: %u, subindex: %u\n", nCanAddress, index, subIndex);


	return 1;
}

void checkWriteSDO(CO_Data* d, unsigned char nodeid) {
	unsigned long abortcode;

	if (getWriteResultNetworkDict(&ObjDict_Data, nodeid, &abortcode)
			!= SDO_FINISHED) {
		fprintf(stderr, "ERROR. nodeid: %u, getWriteResultNetworkDict != SDO_FINISHED, abortcode: %lu\n",
				nodeid, abortcode);
		closeSDOtransfer(&ObjDict_Data, nodeid, SDO_CLIENT);
		bWriteFinished = 2;
		return;
	}

	/* Finalize last SDO transfer with this node */
	closeSDOtransfer(&ObjDict_Data, nodeid, SDO_CLIENT);
	bWriteFinished = 1;
}


void canInterface_OnSdoReadString(CO_Data* d, unsigned char nodeid) {
	//unsigned char data[MAX_STRING_LENGTH + 1];
	unsigned long size;
	unsigned long abortcode;

	if (getReadResultNetworkDict(&ObjDict_Data, nodeid, &strResult, &size,
			&abortcode) != SDO_FINISHED) {
		memset(strResult, 0, MAX_STRING_LENGTH + 1);
		strResult[0] = '\0';

		fprintf(stderr, "Fail onSdoReadString. nodeId= %u, abortcode=%lu\n", nodeid,
				abortcode);

		receivingState = CAN_ERROR;
	} else {
		strResult[size] = 0;
		receivingState = CAN_OK;
	}

	/* Finalize last SDO transfer with this node */
	closeSDOtransfer(&ObjDict_Data, nodeid, SDO_CLIENT);
}


void canInterface_OnSdoReadUint8(CO_Data* d, unsigned char nodeid) {
	//unsigned char data[16];//TEST
	unsigned long size;//TEST
	unsigned long abortcode;//TEST

	if (getReadResultNetworkDict(&ObjDict_Data, nodeid, &nUint8Result, &size,
			&abortcode) != SDO_FINISHED) {
		fprintf(stderr, "Fail canInterface_OnSdoReadInt8. Ret: %lu\n", abortcode);
		receivingState = CAN_ERROR;
	} else {
		//nUint8Result = data[0];
#ifdef DEBUG_CAN
		printf("Received from id=%d, int8= %u\n", nodeid, nUint8Result);
#endif
		receivingState = CAN_OK;
	}

	closeSDOtransfer(&ObjDict_Data, nodeid, SDO_CLIENT);
}

void canInterface_OnSdoReadInt16(CO_Data* d, unsigned char nodeid) {
	unsigned long size;//TEST
	unsigned long abortcode;//TEST

	if (getReadResultNetworkDict(&ObjDict_Data, nodeid, &nUint16Result, &size,
			&abortcode) != SDO_FINISHED) {
		fprintf(stderr, "Fail canInterface_OnSdoReadInt16. Ret: %lu\n", abortcode);
		receivingState = CAN_ERROR;
	} else {
		//nUint16Result = data[1]*256 + data[0];
#ifdef DEBUG_CAN
		printf("Received from id=%d, int16= %u\n", nodeid, nUint16Result);
#endif
		receivingState = CAN_OK;
	}

    closeSDOtransfer(&ObjDict_Data, nodeid, SDO_CLIENT);
}

void canInterface_OnSdoReadInt32(CO_Data* d, unsigned char nodeid) {
	unsigned long size;//TEST
	unsigned long abortcode;//TEST

	if (getReadResultNetworkDict(&ObjDict_Data, nodeid, &nInt32Result, &size,
			&abortcode) != SDO_FINISHED) {
		fprintf(stderr, "Fail canInterface_OnSdoReadInt32. Ret: %lu\n", abortcode);
		receivingState = CAN_ERROR;
		nInt32Result = n32NaN;
	} else {
#ifdef DEBUG_CAN
		printf("Received from id=%d, int32= %ld (as uint32: %lu)\n", nodeid,
				nInt32Result, nInt32Result);
#endif
		receivingState = CAN_OK;
	}

	closeSDOtransfer(&ObjDict_Data, nodeid, SDO_CLIENT);
}

char canInterface_getTgData(tgSetup* tgSetupData, int nCanAddress) {

	unsigned char result;
	unsigned long temp;
	unsigned char tempChar;
#ifdef DEBUG_CAN
	printf("Get TG data for node ID: %u\n", nCanAddress);
#endif
	result = canInterface_getString(tgSetupData->location, nCanAddress, 0x2002, 0);
	if (result != CAN_OK) {
		return result;
	}

	// get password as int32
	long passInt32;

	result = canInterface_getInt32(&passInt32, nCanAddress, 0x2003, 0);
	if (result != CAN_OK) { return result; }

	memcpy(tgSetupData->password, (char*) &passInt32, 4);
	tgSetupData->password[4] = 0;
	//myString_convertUTF8toCAN(tgSetupData->password, 4);


	result = canInterface_getUint8((unsigned char*) &tgSetupData->language, nCanAddress,
			0x2004, 0);
	if (result != CAN_OK) { return result; }
	tgSetupData->canBusAddr = nCanAddress;

	result = canInterface_getInt32((long*) &temp, nCanAddress, 0x2005, 0);
	if (result != CAN_OK) { return result; }

	tgSetupData->displayContrast = (temp & 0xf0000) >> 16;
	tgSetupData->displayTimeout = (temp & 0xffff);
	/////////////////
	result = canInterface_getString(tgSetupData->type, nCanAddress, 0x2006, 1);
	if (result != CAN_OK) { return result; }

	result = canInterface_getUint16((unsigned int*) &tgSetupData->power, nCanAddress, 0x2006, 2);
	if (result != CAN_OK) { return result; }

	result = canInterface_getUint16((unsigned int*) &tgSetupData->input, nCanAddress, 0x2006, 3);
	if (result != CAN_OK) { return result; }

	result = canInterface_getUint16((unsigned int*) &tgSetupData->output, nCanAddress, 0x2006, 4);
	if (result != CAN_OK) { return result; }

	result = canInterface_getUint16((unsigned int*) &tgSetupData->outputCurrent, nCanAddress, 0x2006, 5);
	if (result != CAN_OK) { return result; }

	result = canInterface_getString(tgSetupData->tempClass, nCanAddress, 0x2006, 6);
	if (result != CAN_OK) { return result; }

	//////////////
	result = canInterface_getUint8(
			(unsigned char*) &(tgSetupData->alLevels.insul.alarm), nCanAddress, 0x3000, 4);
	if (result != CAN_OK) { return result; }

	result = canInterface_getUint8(
			(unsigned char*) &tgSetupData->alLevels.insul.prealarm, nCanAddress, 0x3000, 3);
	if (result != CAN_OK) { return result; }

	result = canInterface_getUint8((unsigned char*) &tgSetupData->alLevels.load.alarm, nCanAddress, 0x3001, 4);
	if (result != CAN_OK) { return result; }

	result = canInterface_getUint8(
			(unsigned char*) &tgSetupData->alLevels.load.prealarm, nCanAddress, 0x3001, 3);
	if (result != CAN_OK) { return result; }

	result = canInterface_getUint8((unsigned char*) &tgSetupData->alLevels.temper.alarm,
			nCanAddress, 0x3002, 4);
	if (result != CAN_OK) { return result; }

	result = canInterface_getUint8(
			(unsigned char*) &tgSetupData->alLevels.temper.prealarm, nCanAddress, 0x3002, 3);
	if (result != CAN_OK) { return result; }

	result = canInterface_getString(tgSetupData->alMessages.insul.alarmMsg, nCanAddress, 0x3000, 6);
	if (result != CAN_OK) { return result; }

	result = canInterface_getString(tgSetupData->alMessages.insul.prealarmMsg, nCanAddress, 0x3000, 5);
	if (result != CAN_OK) { return result; }

	result = canInterface_getString(tgSetupData->alMessages.load.alarmMsg, nCanAddress, 0x3001, 6);
	if (result != CAN_OK) { return result; }

	result = canInterface_getString(tgSetupData->alMessages.load.prealarmMsg, nCanAddress, 0x3001, 5);
	if (result != CAN_OK) { return result; }

	result = canInterface_getString(tgSetupData->alMessages.temper.alarmMsg, nCanAddress, 0x3002, 6);
	if (result != CAN_OK) { return result; }

	result = canInterface_getString(tgSetupData->alMessages.temper.prealarmMsg, nCanAddress, 0x3002, 5);
	if (result != CAN_OK) { return result; }

	/////////////////////////////////////////////////////////////////////
	// EXTERNAL ALARM INPUT 1
	result = canInterface_getUint8((unsigned char*) &tempChar, nCanAddress, 0x3003, 3);
	if (result != CAN_OK) { return result; }

	if (tempChar & 0x01) {
		tgSetupData->eai1.enabled = 1;
	} else if (tempChar & 0x02) {
			tgSetupData->eai1.enabled = 2;
	} else {
		tgSetupData->eai1.enabled = 0;
	}

	if ((tempChar & 0x04) > 0)
		tgSetupData->eai1.pullups = 1;
	else
		tgSetupData->eai1.pullups = 0;


	result = canInterface_getString(tgSetupData->eai1.message, nCanAddress, 0x3003, 4);
	if (result != CAN_OK) { return result; }

	///////////////////////////////////////////////////////////////////////////
	// EXTERNAL ALARM INPUT 2
	result = canInterface_getUint8((unsigned char*) &tempChar, nCanAddress, 0x3004, 3);
	if (result != CAN_OK) { return result; }

	if (tempChar & 0x01) {
			tgSetupData->eai2.enabled = 1;
	} else if (tempChar & 0x02) {
		tgSetupData->eai2.enabled = 2;
	} else {
		tgSetupData->eai2.enabled = 0;
	}

	if ((tempChar & 0x04) > 0)
		tgSetupData->eai2.pullups = 1;
	else
		tgSetupData->eai2.pullups = 0;

	result = canInterface_getString(tgSetupData->eai2.message, nCanAddress, 0x3004, 4);
	if (result != CAN_OK) { return result; }
	///////////////////////////////////////////////////////////////////////
	return CAN_OK;
}

char canInterface_setTgData(tgSetup* tgSetupData, int nCanAddress) {
	unsigned char result;
	result = canInterface_writeString(tgSetupData->location, nCanAddress,
			0x2002, 0);
	if (result != CAN_OK) { return result; }

	// send password as int32
	long passInt32;
	memcpy((char*) &passInt32, tgSetupData->password, 4);
	result = canInterface_writeInt32(passInt32, nCanAddress, 0x2003, 0);
	if (result != CAN_OK) { return result; }

	result = canInterface_writeUint8((unsigned char) (tgSetupData->language),
			nCanAddress, 0x2004, 0);
	if (result != CAN_OK) { return result; }

	unsigned long temp;
	temp = tgSetupData->displayContrast << 16;
	temp |= tgSetupData->displayTimeout;

	result = canInterface_writeInt32((long) temp, nCanAddress, 0x2005, 0);
	if (result != CAN_OK) { return result; }

	result = canInterface_writeString(tgSetupData->type, nCanAddress, 0x2006, 1);
	if (result != CAN_OK) { return result; }

	result = canInterface_writeUint16((unsigned int) tgSetupData->power, nCanAddress,
			0x2006, 2);
	if (result != CAN_OK) { return result; }

	result = canInterface_writeUint16((unsigned int) tgSetupData->input, nCanAddress,
			0x2006, 3);
	if (result != CAN_OK) { return result; }

	result = canInterface_writeUint16(
			(unsigned int) tgSetupData->output, nCanAddress, 0x2006, 4);
	if (result != CAN_OK) { return result; }

	result = canInterface_writeUint16((unsigned int) tgSetupData->outputCurrent,
			nCanAddress, 0x2006, 5);
	if (result != CAN_OK) { return result; }

	result = canInterface_writeString(tgSetupData->tempClass, nCanAddress, 0x2006, 6);
	if (result != CAN_OK) { return result; }

	result = canInterface_writeUint8((unsigned char) tgSetupData->alLevels.insul.alarm,
			nCanAddress, 0x3000, 4);
	if (result != CAN_OK) { return result; }

	result = canInterface_writeUint8(
			(unsigned char) tgSetupData->alLevels.insul.prealarm, nCanAddress, 0x3000, 3);
	if (result != CAN_OK) { return result; }

	result = canInterface_writeUint8((unsigned char) tgSetupData->alLevels.load.alarm,
			nCanAddress, 0x3001, 4);
	if (result != CAN_OK) { return result; }

	result = canInterface_writeUint8(
			(unsigned char) tgSetupData->alLevels.load.prealarm, nCanAddress, 0x3001, 3);
	if (result != CAN_OK) { return result; }

	result = canInterface_writeUint8((unsigned char) tgSetupData->alLevels.temper.alarm, nCanAddress, 0x3002, 4);
	if (result != CAN_OK) { return result; }

	result = canInterface_writeUint8(
			(unsigned char) tgSetupData->alLevels.temper.prealarm, nCanAddress, 0x3002, 3);
	if (result != CAN_OK) { return result; }

	result = canInterface_writeString(tgSetupData->alMessages.insul.alarmMsg,
			nCanAddress, 0x3000, 6);
	if (result != CAN_OK) { return result; }

	result = canInterface_writeString(tgSetupData->alMessages.insul.prealarmMsg,
			nCanAddress, 0x3000, 5);
	if (result != CAN_OK) { return result; }

	result = canInterface_writeString(tgSetupData->alMessages.load.alarmMsg,
			nCanAddress, 0x3001, 6);
	if (result != CAN_OK) { return result; }

	result = canInterface_writeString(tgSetupData->alMessages.load.prealarmMsg,
			nCanAddress, 0x3001, 5);
	if (result != CAN_OK) { return result; }

	result = canInterface_writeString(tgSetupData->alMessages.temper.alarmMsg,
			nCanAddress, 0x3002, 6);
	if (result != CAN_OK) { return result; }

	result = canInterface_writeString(tgSetupData->alMessages.temper.prealarmMsg,
			nCanAddress, 0x3002, 5);
	if (result != CAN_OK) { return result; }

	/////////////////////////////////////////////////////////////////////
	// EXTERNAL ALARM INPUT 1

	if (tgSetupData->eai1.enabled == 0) {
		temp = 0x00;
	} else if (tgSetupData->eai1.enabled == 1) {
		temp = 0x01;
	} else {
		temp = 0x02;
	}

	if (tgSetupData->eai1.pullups == 1)
		temp |= 0x04;

	result = canInterface_writeUint8((unsigned char) temp, nCanAddress, 0x3003, 3);
	if (result != CAN_OK) { return result; }

	result = canInterface_writeString(tgSetupData->eai1.message, nCanAddress, 0x3003, 4);
	if (result != CAN_OK) { return result; }

	///////////////////////////////////////////////////////////////////////////
	// EXTERNAL ALARM INPUT 2
	if (tgSetupData->eai2.enabled == 0) {
		temp = 0x00;
	} else if (tgSetupData->eai2.enabled == 1) {
		temp = 0x01;
	} else {
		temp = 0x02;
	}

	if (tgSetupData->eai2.pullups == 1)
		temp |= 0x04;

	result = canInterface_writeUint8((unsigned char) temp, nCanAddress, 0x3004, 3);
	if (result != CAN_OK) { return result; }

	result = canInterface_writeString(tgSetupData->eai2.message, nCanAddress, 0x3004, 4);
	if (result != CAN_OK) { return result; }

	///////////////////////////////////////////////////////////////////////
	if (nCanAddress != tgSetupData->canBusAddr) {//NEW CAN ADDRESS
		result = canInterface_writeUint8(
				(unsigned char) (tgSetupData->canBusAddr), nCanAddress, 0x2000,
				0);
		if (result != CAN_OK) { return result; }

		fprintf(stderr, "Changed CAN ADDRESS. Old: %u, new: %u\n", nCanAddress,
				tgSetupData->canBusAddr);
	}
	return CAN_OK;
}

char canInterface_getTgMesData(tgMes* tgMesData, int nCanAddress) {
	unsigned char result;

	result = canInterface_getUint16((unsigned int*) &tgMesData->ir, nCanAddress, 0x3000,
			1);
	if (result != 0) {
		if (result != CAN_OK) { return result; }
	}
	result = canInterface_getUint16((unsigned int*) &tgMesData->tc, nCanAddress, 0x3001, 1);
	if (result != CAN_OK) { return result; }

	result = canInterface_getUint16((unsigned int*) &tgMesData->tt, nCanAddress, 0x3002, 1);
	if (result != CAN_OK) { return result; }

	return CAN_OK;
}

char canInterface_getIlData(ilSetup* ilSetupData, int nCanAddress) {
	unsigned char result;
	int i;
#ifdef DEBUG_CAN
	printf("Get IL data for node ID: %u\n", nCanAddress);
#endif
	result = canInterface_getString(ilSetupData->location, nCanAddress, 0x2002,
			0);
	if (result != CAN_OK) { return result; }

	// get password as int32
	long passInt32;

	result = canInterface_getInt32(&passInt32, nCanAddress, 0x2003, 0);
	if (result != CAN_OK) { return result; }

	memcpy(ilSetupData->password, (char*) &passInt32, 4);
	ilSetupData->password[4] = 0;
	//myString_convertUTF8toCAN(ilSetupData->password, 4);

	result = canInterface_getUint8((unsigned char*) &ilSetupData->language, nCanAddress,
			0x2004, 0);
	if (result != CAN_OK) { return result; }

	ilSetupData->canBusAddr = nCanAddress;

	unsigned long temp;
	result = canInterface_getInt32((long*) &temp, nCanAddress, 0x2005, 0);
	if (result != CAN_OK) { return result; }

	ilSetupData->displayContrast = (temp & 0xf0000) >> 16;
	ilSetupData->displayTimeout = (temp & 0xffff);

	//////////////////////////////////////////////////////////////////////
	//	Current
	//////////////////////////////////////////////////////////////////////
	for (i = 0; i < 16; i++) {
		result = canInterface_getUint8((unsigned char*) &(ilSetupData->En[i]),
				nCanAddress, 0x3000 + i, 3); // alarm level
		if (result != CAN_OK) { return result; }

		result = canInterface_getUint16((unsigned int*) &(ilSetupData->AL[i]),
				nCanAddress, 0x3000 + i, 4); // alarm level
		if (result != CAN_OK) { return result; }

		result = canInterface_getString(ilSetupData->AT[i], nCanAddress, 0x3000 + i, 5); // alarm text
		if (result != CAN_OK) { return result; }
	}
	return 0;
}

char canInterface_setIlData(ilSetup* ilSetupData, int nCanAddress) {
	unsigned char result;
	int i;
	result = canInterface_writeString(ilSetupData->location, nCanAddress,
			0x2002, 0);
	if (result != CAN_OK) { return result; }

	// send password as int32
	long passInt32;
	memcpy((char*) &passInt32, ilSetupData->password, 4);
	result = canInterface_writeInt32(passInt32, nCanAddress, 0x2003, 0);
	if (result != CAN_OK) { return result; }

	result = canInterface_writeUint8((unsigned char) (ilSetupData->language),
			nCanAddress, 0x2004, 0);
	if (result != CAN_OK) { return result; }

	unsigned long temp;
	temp = ilSetupData->displayContrast << 16;
	temp |= ilSetupData->displayTimeout;

	result = canInterface_writeInt32((long) temp, nCanAddress, 0x2005, 0);
	if (result != CAN_OK) { return result; }

	for (i = 0; i < 16; i++) {
		result = canInterface_writeUint8((unsigned char) (ilSetupData->En[i]),
				nCanAddress, 0x3000 + i, 3); // alarm level
		if (result != CAN_OK) { return result; }

		result = canInterface_writeUint16((unsigned int) (ilSetupData->AL[i]),
				nCanAddress, 0x3000 + i, 4); // alarm level
		if (result != CAN_OK) { return result; }

		result = canInterface_writeString((ilSetupData->AT[i]), nCanAddress, 0x3000 + i,
				5); // alarm text
		if (result != CAN_OK) { return result; }
	}

	///////////////////////////////////////////////////////////////////////
	if (nCanAddress != ilSetupData->canBusAddr) {//NEW CAN ADDRESS
		result = canInterface_writeUint8(
				(unsigned char) (ilSetupData->canBusAddr), nCanAddress, 0x2000, 0);
		if (result != CAN_OK) { return result; }

		printf("Changed CAN ADDRESS. Old: %u, new: %u\n", nCanAddress,
				ilSetupData->canBusAddr);
	}

	return CAN_OK;
}

char canInterface_getIlMesData(ilMes* ilMesData, int nCanAddress) {
	unsigned char result;
	result = canInterface_getUint16((unsigned int*) &ilMesData->ch1, nCanAddress,
			0x3000, 1);

	if (result != CAN_OK) { return result; }

	result = canInterface_getUint16((unsigned int*) &ilMesData->ch2, nCanAddress,
			0x3001, 1);
	if (result != CAN_OK) { return result; }

	result = canInterface_getUint16((unsigned int*) &ilMesData->ch3, nCanAddress,
			0x3002, 1);
	if (result != CAN_OK) { return result; }

	result = canInterface_getUint16((unsigned int*) &ilMesData->ch4, nCanAddress,
			0x3003, 1);
	if (result != CAN_OK) { return result; }

	result = canInterface_getUint16((unsigned int*) &ilMesData->ch5, nCanAddress,
			0x3004, 1);
	if (result != CAN_OK) { return result; }

	result = canInterface_getUint16((unsigned int*) &ilMesData->ch6, nCanAddress,
			0x3005, 1);
	if (result != CAN_OK) { return result; }

	result = canInterface_getUint16((unsigned int*) &ilMesData->ch7, nCanAddress,
			0x3006, 1);
	if (result != CAN_OK) { return result; }

	result = canInterface_getUint16((unsigned int*) &ilMesData->ch8, nCanAddress,
			0x3007, 1);
	if (result != CAN_OK) { return result; }

	result = canInterface_getUint16((unsigned int*) &ilMesData->ch9, nCanAddress,
			0x3008, 1);
	if (result != CAN_OK) { return result; }

	result = canInterface_getUint16((unsigned int*) &ilMesData->ch10, nCanAddress,
			0x3009, 1);
	if (result != CAN_OK) { return result; }

	result = canInterface_getUint16((unsigned int*) &ilMesData->ch11, nCanAddress,
			0x300A, 1);
	if (result != CAN_OK) { return result; }

	result = canInterface_getUint16((unsigned int*) &ilMesData->ch12, nCanAddress,
			0x300B, 1);
	if (result != CAN_OK) { return result; }

	result = canInterface_getUint16((unsigned int*) &ilMesData->ch13, nCanAddress,
			0x300C, 1);
	if (result != CAN_OK) { return result; }

	result = canInterface_getUint16((unsigned int*) &ilMesData->ch14, nCanAddress,
			0x300D, 1);
	if (result != CAN_OK) { return result; }

	result = canInterface_getUint16((unsigned int*) &ilMesData->ch15, nCanAddress,
			0x300E, 1);
	if (result != CAN_OK) { return result; }

	result = canInterface_getUint16((unsigned int*) &ilMesData->ch16, nCanAddress,
			0x300F, 1);
	if (result != CAN_OK) { return result; }


	return CAN_OK;
}

char canInterface_getSwData(swSetup* swSetupData, int nCanAddress) {
	unsigned char result;
#ifdef DEBUG_CAN
	printf("Get SW data for node ID: %u\n", nCanAddress);
#endif
	result = canInterface_getString(swSetupData->location, nCanAddress, 0x2002,
			0);
	if (result != CAN_OK) { return result; }

	// get password as int32
	long passInt32;

	result = canInterface_getInt32(&passInt32, nCanAddress, 0x2003, 0);
	if (result != CAN_OK) { return result; }

	memcpy(swSetupData->password, (char*) &passInt32, 4);
	swSetupData->password[4] = 0;
	//myString_convertUTF8toCAN(swSetupData->password, 4);

	result = canInterface_getUint8((unsigned char*) &swSetupData->language, nCanAddress,
			0x2004, 0);
	if (result != CAN_OK) { return result; }

	swSetupData->canBusAddr = nCanAddress;

	unsigned long temp;
	result = canInterface_getInt32((long*) &temp, nCanAddress, 0x2005, 0);
	if (result != CAN_OK) { return result; }

	swSetupData->displayContrast = (temp & 0xf0000) >> 16;
	swSetupData->displayTimeout = (temp & 0xffff);

	result = canInterface_getUint8((unsigned char*) &(swSetupData->uv), nCanAddress,
			0x3004, 1);
	if (result != CAN_OK) { return result; }

	result = canInterface_getUint8((unsigned char*) &(swSetupData->ov), nCanAddress,
			0x3004, 2);
	if (result != CAN_OK) { return result; }
	//////////////////////////////////////////////////////////////////////
	//	Contractor 1 Setup
	//////////////////////////////////////////////////////////////////////
	result = canInterface_getInt32((long*) &temp, nCanAddress, 0x3000, 3); // k1OnSetup
	if (result != CAN_OK) { return result; }

	swSetupData->k1On = (temp & 0x00030000) >> 16; // bit 16-17 //
	swSetupData->pOn1 = temp & 0xff; // bit 0-15
	swSetupData->ocOn1 = (temp & 0x00040000) >> 18; // bit 18

	canInterface_getInt32((long*) &temp, nCanAddress, 0x3000, 4); // k1OfSetup
	if (result != CAN_OK) { return result; }

	swSetupData->k1Of = (temp & 0x00030000) >> 16; // bit 16-17 //
	swSetupData->pOf1 = temp & 0xff; // bit 0-15
	swSetupData->ocOf1 = (temp & 0x00040000) >> 18; // bit 18

	//////////////////////////////////////////////////////////////////////
	//	Contractor 2 Setup
	//////////////////////////////////////////////////////////////////////
	result = canInterface_getInt32((long*) &temp, nCanAddress, 0x3001, 3); // k2OnSetup
	if (result != CAN_OK) { return result; }

	swSetupData->k2On = (temp & 0x00030000) >> 16; // bit 16-17 //
	swSetupData->pOn2 = temp & 0xff; // bit 0-15
	swSetupData->ocOn2 = (temp & 0x00040000) >> 18; // bit 18

	result = canInterface_getInt32((long*) &temp, nCanAddress, 0x3001, 4); // k2OfSetup
	if (result != CAN_OK) { return result; }

	swSetupData->k2Of = (temp & 0x00030000) >> 16; // bit 16-17 //
	swSetupData->pOf2 = (temp & 0xff); // bit 0-15
	swSetupData->ocOf2 = (temp & 0x00040000) >> 18; // bit 18

	//////////////////////////////////////////////////////////////////////
	//	Contractor 3 Setup
	//////////////////////////////////////////////////////////////////////
	result = canInterface_getInt32((long*) &temp, nCanAddress, 0x3002, 3); // k3OnSetup
	if (result != CAN_OK) { return result; }

	swSetupData->k3On = (temp & 0x00030000) >> 16; // bit 16-17 //
	swSetupData->pOn3 = temp & 0xff; // bit 0-15
	swSetupData->ocOn3 = (temp & 0x00040000) >> 18; // bit 18

	result = canInterface_getInt32((long*) &temp, nCanAddress, 0x3002, 4); // k3OfSetup
	if (result != CAN_OK) { return result; }

	swSetupData->k3Of = (temp & 0x00030000) >> 16; // bit 16-17 //
	swSetupData->pOf3 = temp & 0xff; // bit 0-15
	swSetupData->ocOf3 = (temp & 0x00040000) >> 18; // bit 18

	//////////////////////////////////////////////////////////////////////
	//	Switchover timing
	//////////////////////////////////////////////////////////////////////
	result = canInterface_getUint16((unsigned int*) &(swSetupData->rd), nCanAddress,
			0x3005, 1);
	if (result != CAN_OK) { return result; }
	result = canInterface_getUint16((unsigned int*) &(swSetupData->ofP), nCanAddress,
			0x3005, 2);
	if (result != CAN_OK) { return result; }
	result = canInterface_getUint16((unsigned int*) &(swSetupData->rt), nCanAddress,
			0x3005, 3);
	if (result != CAN_OK) { return result; }
	//////////////////////////////////////////////////////////////////////
	//	Alarm messages
	//////////////////////////////////////////////////////////////////////
	// failure
	result = canInterface_getString(swSetupData->f1, nCanAddress, 0x3000, 5);
	if (result != CAN_OK) { return result; }
	result = canInterface_getString(swSetupData->f2, nCanAddress, 0x3001, 5);
	if (result != CAN_OK) { return result; }
	result = canInterface_getString(swSetupData->f3, nCanAddress, 0x3002, 5);
	if (result != CAN_OK) { return result; }

	result = canInterface_getString(swSetupData->fd, nCanAddress, 0x3003, 3); // failure download stream
	if (result != CAN_OK) { return result; }

	result = canInterface_getString(swSetupData->f1On, nCanAddress, 0x3000, 6);
	if (result != CAN_OK) { return result; }
	result = canInterface_getString(swSetupData->f1Of, nCanAddress, 0x3000, 7);
	if (result != CAN_OK) { return result; }
	result = canInterface_getString(swSetupData->f2On, nCanAddress, 0x3001, 6);
	if (result != CAN_OK) { return result; }
	result = canInterface_getString(swSetupData->f2Of, nCanAddress, 0x3001, 7);
	if (result != CAN_OK) { return result; }
	result = canInterface_getString(swSetupData->f3On, nCanAddress, 0x3002, 6);
	if (result != CAN_OK) { return result; }
	result = canInterface_getString(swSetupData->f3Of, nCanAddress, 0x3002, 7);
	if (result != CAN_OK) { return result; }

	return 0;
}

char canInterface_setSwData(swSetup* swSetupData, int nCanAddress) {
	unsigned char result;
	result = canInterface_writeString(swSetupData->location, nCanAddress,
			0x2002, 0);
	if (result != CAN_OK) { return result; }

	// send password as int32
	long passInt32;
	memcpy((char*) &passInt32, swSetupData->password, 4);
	result = canInterface_writeInt32(passInt32, nCanAddress, 0x2003, 0);
	if (result != CAN_OK) { return result; }

	result = canInterface_writeUint8((unsigned char) (swSetupData->language),
			nCanAddress, 0x2004, 0);
	if (result != CAN_OK) { return result; }

	unsigned long temp;
	temp = swSetupData->displayContrast << 16;
	temp |= swSetupData->displayTimeout;

	result = canInterface_writeInt32((long) temp, nCanAddress, 0x2005, 0);
	if (result != CAN_OK) { return result; }

	result = canInterface_writeUint8((unsigned char) (swSetupData->uv), nCanAddress, 0x3004, 1);
	if (result != CAN_OK) { return result; }

	result = canInterface_writeUint8((unsigned char) (swSetupData->ov), nCanAddress, 0x3004, 2);
	if (result != CAN_OK) { return result; }

	//////////////////////////////////////////////////////////////////////
	//	Contractor 1 Setup
	//////////////////////////////////////////////////////////////////////
	temp = (swSetupData->k1On << 16);
	temp |= (swSetupData->pOn1);
	temp |= (swSetupData->ocOn1) << 18;
	result = canInterface_writeInt32((long) temp, nCanAddress, 0x3000, 3); // k1OnSetup
	if (result != CAN_OK) { return result; }

	temp = (swSetupData->k1Of << 16);
	temp |= (swSetupData->pOf1);
	temp |= (swSetupData->ocOf1) << 18;
	result = canInterface_writeInt32((long) temp, nCanAddress, 0x3000, 4); // k1OfSetup
	if (result != CAN_OK) { return result; }

	//////////////////////////////////////////////////////////////////////
	//	Contractor 2 Setup
	//////////////////////////////////////////////////////////////////////

	temp = (swSetupData->k2On << 16);
	temp |= (swSetupData->pOn2);
	temp |= (swSetupData->ocOn2) << 18;
	result = canInterface_writeInt32((long) temp, nCanAddress, 0x3001, 3); // k2OnSetup
	if (result != CAN_OK) { return result; }

	temp = (swSetupData->k2Of << 16);
	temp |= (swSetupData->pOf2);
	temp |= (swSetupData->ocOf2) << 18;
	result = canInterface_writeInt32((long) temp, nCanAddress, 0x3001, 4); // k2OfSetup
	if (result != CAN_OK) { return result; }

	//////////////////////////////////////////////////////////////////////
	//	Contractor 3 Setup
	//////////////////////////////////////////////////////////////////////
	temp = (swSetupData->k3On << 16);
	temp |= (swSetupData->pOn3);
	temp |= (swSetupData->ocOn3) << 18;
	result = canInterface_writeInt32((long) temp, nCanAddress, 0x3002, 3); // k3OnSetup
	if (result != CAN_OK) { return result; }


	temp = (swSetupData->k3Of << 16);
	temp |= (swSetupData->pOf3);
	temp |= (swSetupData->ocOf3) << 18;
	result = canInterface_writeInt32((long) temp, nCanAddress, 0x3002, 4); // k3OfSetup
	if (result != CAN_OK) { return result; }
	//////////////////////////////////////////////////////////////////////
	//	Switchover timing
	//////////////////////////////////////////////////////////////////////
	result = canInterface_writeUint16((unsigned int) (swSetupData->rd), nCanAddress, 0x3005, 1);
	if (result != CAN_OK) { return result; }

	result = canInterface_writeUint16((unsigned int) (swSetupData->ofP), nCanAddress, 0x3005, 2);
	if (result != CAN_OK) { return result; }

	//////////////////////////////////////////////////////////////////////
	//	Alarm messages
	//////////////////////////////////////////////////////////////////////
	// failure
	result = canInterface_writeString(swSetupData->f1, nCanAddress, 0x3000, 5);
	if (result != CAN_OK) { return result; }

	result = canInterface_writeString(swSetupData->f2, nCanAddress, 0x3001, 5);
	if (result != CAN_OK) { return result; }

	result = canInterface_writeString(swSetupData->f3, nCanAddress, 0x3002, 5);
	if (result != CAN_OK) { return result; }

	result = canInterface_writeString(swSetupData->fd, nCanAddress, 0x3003, 3); // failure download stream
	if (result != CAN_OK) { return result; }

	result = canInterface_writeString(swSetupData->f1On, nCanAddress, 0x3000, 6);
	if (result != CAN_OK) { return result; }
	result = canInterface_writeString(swSetupData->f1Of, nCanAddress, 0x3000, 7);
	if (result != CAN_OK) { return result; }
	result = canInterface_writeString(swSetupData->f2On, nCanAddress, 0x3001, 6);
	if (result != CAN_OK) { return result; }
	result = canInterface_writeString(swSetupData->f2Of, nCanAddress, 0x3001, 7);
	if (result != CAN_OK) { return result; }
	result = canInterface_writeString(swSetupData->f3On, nCanAddress, 0x3002, 6);
	if (result != CAN_OK) { return result; }
	result = canInterface_writeString(swSetupData->f3Of, nCanAddress, 0x3002, 7);
	if (result != CAN_OK) { return result; }

	if (nCanAddress != swSetupData->canBusAddr) {//NEW CAN ADDRESS
		result = canInterface_writeUint8(
				(unsigned char) (swSetupData->canBusAddr), nCanAddress, 0x2000,
				0);
		if (result != CAN_OK) { return result; }

		printf("Changed CAN ADDRESS. Old: %u, new: %u\n", nCanAddress,
				swSetupData->canBusAddr);
	}
	return CAN_OK;
}

char canInterface_getSwMesData(swMes* swMesData, int nCanAddress) {
	unsigned char result;
	result = canInterface_getUint16((unsigned int*) &swMesData->su1, nCanAddress,
			0x3000, 1);
	if (result != CAN_OK) { return result; }

	result = canInterface_getUint16((unsigned int*) &swMesData->su2, nCanAddress,
			0x3001, 1);
	if (result != CAN_OK) { return result; }
	result = canInterface_getUint16((unsigned int*) &swMesData->su3, nCanAddress,
			0x3002, 1);
	if (result != CAN_OK) { return result; }
	result = canInterface_getUint16((unsigned int*) &swMesData->dv, nCanAddress, 0x3003,
			1);
	if (result != CAN_OK) { return result; }
	return CAN_OK;
}

char canInterface_getApData(apSetup* apSetupData, int nCanAddress) {
	unsigned char result;
	printf("getAP data for id: %u\n", nCanAddress);
	result = canInterface_getString(apSetupData->location, nCanAddress, 0x2002,
			0);

	if (result != CAN_OK) { return result; }

	result = canInterface_getUint8((unsigned char*) &(apSetupData->language),
			nCanAddress, 0x2004, 0);
	if (result != CAN_OK) { return result; }

	if (apSetupData->language > 4) //TODO: !!!! BARE FOR DEMO
		apSetupData->language = 0;
	apSetupData->canBusAddr = nCanAddress;

	unsigned long temp;
	result = canInterface_getInt32((long*) &temp, nCanAddress, 0x2005, 0);
	if (result != CAN_OK) { return result; }

	apSetupData->displayContrast = (temp & 0xf0000) >> 16;
	apSetupData->displayTimeout = (temp & 0xffff);
	return CAN_OK;
}

char canInterface_setApData(apSetup* apSetupData, int nCanAddress) {
	unsigned char result;
	result = canInterface_writeString(apSetupData->location, nCanAddress,
			0x2002, 0);
	if (result != CAN_OK) { return result; }

	result = canInterface_writeUint8((unsigned char) (apSetupData->language),
			nCanAddress, 0x2004, 0);
	if (result != CAN_OK) { return result; }

	unsigned long temp;
	temp = apSetupData->displayContrast << 16;
	temp |= apSetupData->displayTimeout;

	result = canInterface_writeInt32((long) temp, nCanAddress, 0x2005, 0);
	if (result != CAN_OK) { return result; }

	///////////////////////////////////////////////////////////////////////
	if (nCanAddress != apSetupData->canBusAddr) {//NEW CAN ADDRESS
		result = canInterface_writeUint8(
				(unsigned char) (apSetupData->canBusAddr), nCanAddress, 0x2000,
				0);
		if (result != CAN_OK) {
			printf("Change CAN address, return Error: %d\n", result);
			return result;
		}

		printf("Changed CAN ADDRESS. Old: %u, new: %u\n", nCanAddress,
				apSetupData->canBusAddr);
	}
	return 0;
}

char canInterface_checkOldAlarm(int nCanAddress) {
	unsigned char result;
	unsigned char oldAlarm;
    long oldAlarmU32;
	unsigned int oldAlarmCode;
	char alarmCompletMsgTxt[60];
	unsigned int oldIndex;
	unsigned char oldSubindex;
	char unitName[50];
	char alarmTxt[50];
#ifdef DEBUG_CAN
	printf("check old alarm for id: %u\n", nCanAddress);
#endif
	result = canInterface_getUint8(&oldAlarm, nCanAddress, 0x1001, 0);

	if (result != 0) {
		fprintf(stderr, "error canInterface_checkOldAlarm for id: %u\n",nCanAddress);
		return 1;
	}

	if (oldAlarm > 0) {
		result = canInterface_getInt32(&oldAlarmU32, nCanAddress, 0x1003, 1);
		if (result != 0) {
			fprintf(stderr, "error canInterface_checkOldAlarm Code for id: %u\n",nCanAddress);
			return 1;
		}

		oldAlarmCode = (oldAlarmU32 & 0x0000ffff);
		oldIndex = 0x3000 + ((oldAlarmU32 & 0xff000000) >> 24);
		oldSubindex = (oldAlarmU32 & 0x00ff0000) >> 16;

		result = canInterface_getString(unitName, nCanAddress, 0x2002, 0);
		if (result != 0)
			strcpy(unitName, "???");

		if(oldIndex > 0) {
			result = canInterface_getString(alarmTxt, nCanAddress, oldIndex, oldSubindex);
			if (result == 0)
				sprintf(alarmCompletMsgTxt, "%s, error: 0x%04x", alarmTxt, oldAlarmCode);
			else
				sprintf(alarmCompletMsgTxt, "Unknown, add: %u, error: 0x%04x", nCanAddress, oldAlarmCode);
		} else {
			sprintf(alarmCompletMsgTxt, "Error Code: 0x%04x", oldAlarmCode);
		}
		gwAlarmLogUtility_saveAndSendAlarmMsg(nCanAddress, unitName, alarmCompletMsgTxt);
		fprintf(stderr, "Saved OLD ALARM: %s\n", alarmCompletMsgTxt);
	}
	return 0;
}


// Alarm received from "emcy.c" (void "_post_emcy")
void canInterface_onReceivedAlarm(unsigned char nodeID, unsigned int errCode,
		unsigned char errReg, unsigned int index, unsigned char subindex) {
	//unsigned char temp;

	fprintf(stderr, "** onReceivedAlarm, nodeID: %u, error code 0x%04x, ind:%u, sub:%u\n", nodeID, errCode, index, subindex);

	alarm_addNew(index, subindex, nodeID, errCode);
	modBusInterface_notifyAlarm();

	/*	pthread_mutex_lock(&alarm_mutex);

	if (errCode == 0) { // clearing of alarm
		alarmClearedNodeID = nodeID;

		sprintf(alarmClearedText, "Cleared error: 0x%04x", index);

		pthread_mutex_unlock(&alarm_mutex);
		bAlarmClearedFlag = 1;
		return;
	}

	alarmIndex = index;
	alarmSubindex = subindex;
	alarmNodeID = nodeID;
	alarm_errCode = errCode;
	alarmText[0] = 0;

	//if (errCode != 0) {
		temp = (errCode && 0xFF);
		if (temp >= 0 && temp <= 0x7F) { // read alarm text after one sec.
			alarmType = 1;
		} else {
			alarmType = 2;
			sprintf(alarmText, "Error Code: 0x%04x", errCode);
		}
		pthread_mutex_unlock(&alarm_mutex);
		modBusInterface_notifyAlarm();
	//}

	bAlarmFlag = 1;*/
}



