#ifndef SYSTIME_H_
#define SYSTIME_H_

#include "../unitGW/model/gwDevParData.h"

void sysTime_set(int nYear, int nMonth, int nDay, int nHour, int nMin,
		int nSec);

void sysTime_get(GW_TIME *timeStruct);

int sysTime_toString(char* currDate, char* currTime);
int sysTime_toString2(char* currDate, char* currTime);
int sysTime_toString3(char* currDate, char* currTime);
#endif /*PIPEHANDLER_H_*/

