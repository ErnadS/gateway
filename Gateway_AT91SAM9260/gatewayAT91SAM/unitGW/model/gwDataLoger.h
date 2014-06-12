/*
 * gwDataLoger.h
 *
 *  Created on: 23. apr. 2010
 *      Author: Ernad
 */

#ifndef GWDATALOGER_H_
#define GWDATALOGER_H_

#define MAX_LOG_LINES  600 //200
#define MAX_LOG_TASKS  10


typedef struct {
	char measTime[20];  // 9 + 11 + 1 -1  // 12:22:44 31.12.2009
	char msgId;
	unsigned long measValue;
} meassuringStruct;

typedef struct {
	int canAddress;
	meassuringStruct measStruct[MAX_LOG_LINES];
	int currentIndex;
	int structSize;
} DEV_MEASSUR_LIST;

typedef struct {
	unsigned char adr;
	unsigned char msid;
	unsigned char chan;
	unsigned char per;
	int timer;
	DEV_MEASSUR_LIST meassuringsList; // measuring results
} DEV_MEASSUR_TASK;


#endif /* GWDATALOGER_H_ */
