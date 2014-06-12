/*
 * canInterface.h
 *
 *  Created on: 28. jan. 2010
 *      Author: Ernad
 */
#include "../unitTG/model/tg_data.h"
#include "../unitIL/model/il_data.h"
#include "../unitSW/model/swData.h"
#include "../unitAP/ap_data.h"
#include "../unitGW/model/device.h"
#include "../config.h"

#ifndef CANINTERFACE_H_
#define CANINTERFACE_H_

#ifndef ERROR_CAN
#define ERROR_CAN  0xffffffff
#endif

typedef enum {
	CAN_OK = 0,
	NOT_RECEIVED,
	CAN_BUSY,
	CAN_ERROR
} RECEIVING_STATE;

int timeToStartDevicesRefresh;

int canInterface_init(void);
void * canInterface_canThreadTask(void *ptr);
int canInterface_searchDevices(void);
int canInterface_refreshDevices(void);
unsigned long canInterface_getValue(DEV* device, int nMsgId);

void onSlaveBootup(unsigned char slaveID);

unsigned char canInterface_getString(char* uint8Result, int nCanAddress, unsigned int nIndex, unsigned char nSubindex);
unsigned char canInterface_getInt32(long* longResult, int nCanAddress, unsigned int nIndex, unsigned char nSubindex);
unsigned char canInterface_getUint8(unsigned char* uint8Result, int nCanAddress, unsigned int nIndex, unsigned char nSubindex);
unsigned char canInterface_getUint16(unsigned int* uint16Result, int nCanAddress, unsigned int nIndex, unsigned char nSubindex);

unsigned char canInterface_writeString(char* strValue, unsigned char nCanAddress,
		unsigned int index, unsigned char subIndex);
unsigned char canInterface_writeInt32(long nValue, unsigned char nCanAddress,
		unsigned int index, unsigned char subIndex);
unsigned char canInterface_writeUint8 (char nValue, unsigned char nCanAddress, unsigned int index, unsigned char subIndex);
unsigned char canInterface_writeUint16(unsigned int nValue, unsigned char nCanAddress, unsigned int index, unsigned char subIndex);

char canInterface_getTgData(tgSetup* tgSetupData, int nCanAddress);
char canInterface_setTgData(tgSetup* tgSetupData, int nCanAddress);
char canInterface_getTgMesData(tgMes* tgMesData, int nCanAddress);

char canInterface_getIlData(ilSetup* ilSetupData, int nCanAddress);
char canInterface_setIlData(ilSetup* ilSetupData, int nCanAddress);
char canInterface_getIlMesData(ilMes* ilMesData, int nCanAddress);

char canInterface_getSwData(swSetup* swSetupData, int nCanAddress);
char canInterface_setSwData(swSetup* swSetupData, int nCanAddress);
char canInterface_getSwMesData(swMes* swMesData, int nCanAddress);

char canInterface_getApData(apSetup* apSetupData, int nCanAddress);
char canInterface_setApData(apSetup* apSetupData, int nCanAddress);

void canInterface_onReceivedAlarm(unsigned char nodeID, unsigned int errCode, unsigned char errReg,unsigned int index,unsigned char subindex);
//char canInterface_isAlarm();
void canInterface_readAndSendAlarm();
char canInterface_checkOldAlarm(int nCanAddress);

//char canInterface_isAlarmCleared();
//void canInterface_sendAlarmCleared();

char * canInterface_getHeardBeatPointer();

char canHeardBeat[126];

#endif /* CANINTERFACE_H_ */

