/*
 * Alarm.c
 *
 *  Created on: Mar 6, 2014
 *      Author: ernad
 */

#include "Alarm.h"
#include <string.h>
#include <stdio.h>
#include <unistd.h> // sleep
#include <pthread.h> // mutex
#include "can/canInterface.h"

#include "unitGW/model/device.h"
#include "unitGW/model/linkedList/deviceLinkedList.h"
//#include "../unitGW/dynamic/treeNodes.h"
#include "unitGW/dynamic/gwSetupDeviceMonitor.h"
#include "unitGW/utility/gwAlarmLogUtility.h"
#include "services/modBusInterface/modBusInterf.h"
#include "config.h"


#define MAX_ALARMS  10
DetectedAlarms alarms[MAX_ALARMS]; // new alarms, waiting to get alarm info and to be sent on e-mail

/*
#define MAX_WAIT_ON_CLEAN   5
int nNextUnclearedAlrPos = 0;
DetectedAlarms uncleredAlarms[MAX_WAIT_ON_CLEAN];  // waiting on clear msg
*/

pthread_mutex_t alarm_mutex;

int nextAlarmId = 0;

void alarm_readAndSendMail(void);
int getAlarmArrayIndex(unsigned char alarmNodeID, unsigned int cleaningAlarmID) ;

//SJETI SE DA VRATIS NORSKE BOKSTAVER     myString_convertCANtoUTF8

void * runAlarmHandler(void *ptr) {
	int i;

	printf("!!!!!!!!!!! INIT  ALARMS ARRAY !!!!!!!!!!!!!!!!\n");
	for (i = 0; i < MAX_ALARMS; i++)
		alarms[i].alarmNodeID = 0;

	sleep(3);

	while(1) {
		alarm_readAndSendMail();

		sleep(1);
	}
	return NULL;
}

int getAlarmArrayIndex(unsigned char alarmNodeID, unsigned int cleaningAlarmID) {
	int i;

	for (i = 0; i < MAX_ALARMS; i++) {
		if ((alarms[i].alarmNodeID == alarmNodeID) && (alarms[i].alarm_errCode == cleaningAlarmID)) {
			printf("yes\n");
			return i;
		}
	}
	printf("no\n");
	return -1; // not found
}

////////////////////////////////////////////////////////////////////////////////
// Alarm received from "emcy.c" (void "_post_emcy")
////////////////////////////////////////////////////////////////////////////////
void alarm_addNew(unsigned int alarmIndex, unsigned char alarmSubindex, unsigned char alarmNodeID, unsigned int alarm_errCode) {
	unsigned char temp;
	int alarmArrayIndex;

	printf("addNew: ind: %d, sub: %d, ID: %d, cod: %d \n", alarmIndex, alarmSubindex, alarmNodeID, alarm_errCode);
	pthread_mutex_lock(&alarm_mutex);

	// USER CLEAN (from AlarmPanel)
	if (alarm_errCode == 0 && alarmIndex == 0xfe) {  //  0x00fe => User confirm alarm. We don't nead to clear alarm. Just send e-mail: "User clean ..."
		alarms[nextAlarmId].alarmNodeID = alarmNodeID; // CAN address
		alarms[nextAlarmId].alarm_errCode = 0;  // this 2 is telling that we have received "User clean". Don't need to update other parameters.
		alarms[nextAlarmId].alarmIndex = 0xfe;

		alarms[nextAlarmId].trialCount = 0;
		alarms[nextAlarmId].alarmUnitName[0] = 0;
		alarms[nextAlarmId].alarmText[0] = 0;

		alarms[nextAlarmId].cleared = 1;
		alarms[nextAlarmId].sent = 1;

		alarms[nextAlarmId].timeDownCounter = 1; // do not wait

		nextAlarmId++;
		if (nextAlarmId >= MAX_ALARMS)
			nextAlarmId = 0;
		pthread_mutex_unlock(&alarm_mutex);
		printf("Cleared by user\n");
		return;
	}

	// ALRAM CLEAN (from node which has a alarm)
	if (alarm_errCode == 0) { // ALARM CLEAR RECEIVED
		//alarms[nextAlarmId].alarmType = 0;
		alarmArrayIndex = getAlarmArrayIndex(alarmNodeID, alarmIndex); // If "clean" received, "alarmIndex" is ID of the alarm ("alarmIndex" from alarm we are cleaning now)
		if (alarmArrayIndex >= 0) {  // alarm found and now is cleared (just send from thread)
			alarms[alarmArrayIndex].cleared = 1;
			alarms[alarmArrayIndex].timeDownCounter = 1; // don't wait with sending, we have all info we need for e-mail
			alarms[alarmArrayIndex].trialCount = 0;
			pthread_mutex_unlock(&alarm_mutex);
			printf("found 1 and cleared\n");
			return;
		} else { // not found. Probably removed. Add to list as new alarm and send later without "alarm text" information.*/
			alarms[nextAlarmId].cleared = 1;
			alarms[nextAlarmId].alarmIndex = 0;  // clear msg don't have this information. In "alarmIndex" is original "errorCode" from the alarm
			alarms[nextAlarmId].alarmSubindex = 0;
			alarms[nextAlarmId].alarmNodeID = alarmNodeID; // CAN address
			alarms[nextAlarmId].alarm_errCode = alarmIndex; // !!! Clear msg. has errCode in "alarmIndex" (additional info)
			alarms[nextAlarmId].timeDownCounter = 5; // wait 5 sec before getting node name
			alarms[nextAlarmId].trialCount = 0;
			alarms[nextAlarmId].alarmUnitName[0] = 0;
			alarms[nextAlarmId].alarmText[0] = 0;
			alarms[nextAlarmId].sent = 1;  // tell to code that we will not send "received and cleared" (if not "sent", we are sending this text).
			printf("Clear, not found. make a new Alarm-clean msg\n");
		}
	} else { // NEW ALARM RECEIVED
		printf("add new alarm: %u\n", alarmNodeID);
		alarms[nextAlarmId].cleared = 0;
		alarms[nextAlarmId].alarmIndex = alarmIndex;
		alarms[nextAlarmId].alarmSubindex = alarmSubindex;
		alarms[nextAlarmId].alarmNodeID = alarmNodeID; // CAN address
		alarms[nextAlarmId].alarm_errCode = alarm_errCode;
		alarms[nextAlarmId].trialCount = 0;
		alarms[nextAlarmId].alarmUnitName[0] = 0;
		alarms[nextAlarmId].alarmText[0] = 0;
		alarms[nextAlarmId].sent = 0;

		temp = (alarms[nextAlarmId].alarm_errCode & 0xFF); //ES: 2014.02 !!! It was "alarm_errCode && 0xFF". Wrong?

		if (temp >= 0 && temp <= 0x7F) { // READ ALARM TEXT after 5 sec.
			alarms[nextAlarmId].alarmType = 1;
			alarms[nextAlarmId].timeDownCounter = 5;
			printf("should get al.text\n");
		} else { // DO NOT READ ALARM TEXT
			alarms[nextAlarmId].alarmType = 2;
			sprintf(alarms[nextAlarmId].alarmText, "Error Code: 0x%04x", alarms[nextAlarmId].alarm_errCode);
			alarms[nextAlarmId].timeDownCounter = 1; // don't wait with sending
			printf("don't get alarm text\n");
		}
		printf("*********ID is: %u\n", alarms[nextAlarmId].alarmNodeID);
	}

	nextAlarmId++;
	if (nextAlarmId >= MAX_ALARMS)
		nextAlarmId = 0;

	pthread_mutex_unlock(&alarm_mutex);
}


/////////////////////////////////////////////////////////////////////////
// Read this function periodically.
// Check all alarms in array: is it any new alarm (but older then 5 sec.
// saw we can get alarm-name and send mail to user
/////////////////////////////////////////////////////////////////////////
void alarm_readAndSendMail(void) {
	unsigned char result;
	int i;

	char alarmCompletMsgTxt[120];
	unsigned char nodeId;
	char nodeName[50];

	// Check array with alarms

	for (i = 0; i < MAX_ALARMS; i++) {
		pthread_mutex_lock(&alarm_mutex);  // prevent changing of "alarms[i]" when this thread use them.

		alarmCompletMsgTxt[0] = 0;
		if (alarms[i].alarmNodeID < 1 || alarms[i].alarmNodeID > 127) { // empty array element
			pthread_mutex_unlock(&alarm_mutex);
			continue;
		}

		if (alarms[i].sent && alarms[i].cleared != 1) { // we are still waiting on clear for this
			pthread_mutex_unlock(&alarm_mutex);
			continue;
		}

		alarms[i].timeDownCounter --;

		if (alarms[i].timeDownCounter > 0) {
			//printf("not yet, ID: %d, tick: %d\n", alarms[i].alarmNodeID, alarms[i].timeDownCounter);
			pthread_mutex_unlock(&alarm_mutex);
			continue; // not yet, wait 5-6 seconds before getting alarm name
		}


		DEV* dev = deviceLinkedList_getDevice(alarms[i].alarmNodeID);

		if (dev == NULL) {
			//bAlarmFlag = 0;
			printf("** !!! DEVICE NOT EXIST but received alarm. NodeID: %u\n",
					alarms[i].alarmNodeID);
			alarms[i].alarmNodeID = 0;  // Clear this alarm
			alarms[i].alarmUnitName[0] = 0;
			alarms[i].alarmText[0] = 0;
			pthread_mutex_unlock(&alarm_mutex);
			continue;
		}

		///////////////////////////////////////////////////////////////////////////////////////
		// Get Unit name if empty:
		///////////////////////////////////////////////////////////////////////////////////////
		if (alarms[i].alarmUnitName[0] == 0) {
			result = canInterface_getString(alarms[i].alarmUnitName, alarms[i].alarmNodeID, 0x2002, 0);
			if (result != CAN_OK) {
				if (alarms[i].trialCount < 2) {
					alarms[i].trialCount++;
					pthread_mutex_unlock(&alarm_mutex);
					printf("Error get name\n");
					continue; // try again after one second
				} else {
					printf("alarm, error reading unit name. CAN addr.: %d\n", alarms[i].alarmNodeID);
					strcpy(alarms[i].alarmUnitName, "error getName");
				}
			} else
				printf("read alarm, unit name: %s\n", alarms[i].alarmUnitName);
		}

		///////////////////////////////////////////////////////////////////////////////////////
		// If "User clean":
		///////////////////////////////////////////////////////////////////////////////////////
		if (alarms[i].alarm_errCode == 0  && alarms[i].alarmIndex == 0xfe) {  // cleared by user
			sprintf(alarmCompletMsgTxt, "CLEARED: by user,  0xfe");
		}
		///////////////////////////////////////////////////////////////////////////////////////
		// If NOT CLEARED YET. Do not remove alarm after sending. We needs some info when "clear" is coming.
		///////////////////////////////////////////////////////////////////////////////////////
		else if (alarms[i].cleared == 0) {  // "sent" is "0" (not sent). It's a new alarm
			printf("not cleared\n");
			if (alarms[i].alarmType == 1) {
				result = canInterface_getString(alarms[i].alarmText, alarms[i].alarmNodeID, alarms[i].alarmIndex, alarms[i].alarmSubindex); // get alarmText
				if (result == CAN_OK) {
					sprintf(alarmCompletMsgTxt, "%s, error: 0x%04x", alarms[i].alarmText, alarms[i].alarm_errCode);
				}
				else {
					if (alarms[i].trialCount < 2) {  // return and try next time
						alarms[i].trialCount++;
						printf("err get alarm text\n");
						pthread_mutex_unlock(&alarm_mutex);
						continue; // try again after one second
					}
					else { // GIV UP
						printf("111\n");
						sprintf(alarmCompletMsgTxt, "error getAlarmText, error: 0x%04x", alarms[i].alarm_errCode);
					}
				}
			} else { // type = 2 =>  We have error code inside alarmText. Do not try to get real "alarmText" for this kind of alarm
				printf("3333 %s\n", alarmCompletMsgTxt);
				sprintf(alarmCompletMsgTxt, "%s", alarms[i].alarmText);
			}
		}
		///////////////////////////////////////////////////////////////////////////////////////
		//  Alarm is cleared. If alarmText is not get, now is to late (device doen't have this information)
		///////////////////////////////////////////////////////////////////////////////////////
		else {
			if (alarms[i].sent == 1) { // alarm is sent before and cleared now. Now send "CLEARED" mail.
				if (alarms[i].alarmText[0] != 0) { // we have a alarm info (found when alarm occurred)
					printf("444\n");
					sprintf(alarmCompletMsgTxt, "CLEARED: %s, error: 0x%04x", alarms[i].alarmText, alarms[i].alarm_errCode);
				} else {  // we don't have a alarm text
					printf("555\n");
					sprintf(alarmCompletMsgTxt, "CLEARED unknown alarm, error: 0x%04x", alarms[i].alarm_errCode);
				}
			} else {  // Clear received before sending of the alarm:
				if (alarms[i].alarmText[0] != 0) {
					printf("666\n");
					sprintf(alarmCompletMsgTxt, "%s received and CLEARED, error: 0x%04x", alarms[i].alarmText, alarms[i].alarm_errCode);
				}
				else {
					printf("777\n");
					sprintf(alarmCompletMsgTxt, "Alarm received and CLEARED, error: 0x%04x", alarms[i].alarm_errCode);
				}
			}
		}

		nodeId = alarms[i].alarmNodeID;  // copy variables to not be overwritten by other Thread
		strcpy(nodeName, alarms[i].alarmUnitName);

		if (alarms[i].cleared) {
			alarms[i].alarmNodeID = 0; // Clear this alarm
			alarms[i].alarmUnitName[0] = 0;
			alarms[i].alarmText[0] = 0;
			alarms[i].cleared = 0;
			alarms[i].sent = 0;
		} else {
			alarms[i].sent = 1;
		}

		pthread_mutex_unlock(&alarm_mutex); // and release mutex


		gwAlarmLogUtility_saveAndSendAlarmMsg(nodeId, nodeName, alarmCompletMsgTxt);
		printf("**Saved ALARM: %s\n", alarmCompletMsgTxt);
	}
}

