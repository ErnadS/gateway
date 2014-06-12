/*
 * Alarm.h
 *
 *  Created on: Mar 6, 2014
 *      Author: ernad
 */

#ifndef ALARM_H_
#define ALARM_H_

typedef struct {
	char alarmUnitName[50];
	char alarmText[60];
	unsigned int alarmIndex;
	unsigned char alarmSubindex;
	unsigned char alarmNodeID;  // CAN address
	unsigned char alarmType;
	unsigned int alarm_errCode;
	signed char timeDownCounter;  // down counter from 5 to 0 (5 sec before getting alarm text)
	char cleared;
	char sent;
	char trialCount; // if not answering on get alarm text or get node name, count up (give up after second trial)
} DetectedAlarms;

void * runAlarmHandler(void *ptr);
void alarm_addNew(unsigned int alarmIndex, unsigned char alarmSubindex, unsigned char alarmNodeID, unsigned int alarm_errCode);

#endif /* ALARM_H_ */
