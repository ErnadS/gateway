/*
 * gwDataLoger.c
 *
 *  Created on: 23. apr. 2010
 *      Author: Ernad
 */

#include "gwDataLogerUtility.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../utility/xml.h"
#include "../../utility/paramUtil.h"
#include "../../config.h"
#include "../model/gwDataLoger.h"
#include "../utility/dataLoger/dataLogerSaver.h"
#include "../../utility/myString.h"
#include "../model/linkedList/deviceLinkedList.h"



DEV_MEASSUR_TASK devMassuringsTask[MAX_LOG_TASKS];

char gwDataLogerUtility_saveStruct(void) {
	return dataLogerSaver_saveToFile((DEV_MEASSUR_TASK*) &devMassuringsTask);
}

char gwDataLogerUtility_readStruct(void) {
	gwDataLogerUtility_clearStruct((DEV_MEASSUR_TASK*) &devMassuringsTask);
	dataLogerSaver_readFromFile((DEV_MEASSUR_TASK*) &devMassuringsTask);
	return 1;
}

/**
 * user has changed Can bus addresss
 */
void gwDataLogerUtility_changeCanID(int oldID, int newID) {
	int i;

	for (i = 0; i < MAX_LOG_TASKS; i++) {
		if (devMassuringsTask[i].adr == oldID) {
			devMassuringsTask[i].adr = newID;
			gwDataLogerUtility_saveStruct();
			return;
		}
	}
}

void gwDataLogerUtility_removeCanID(int oldID) {
	int i;

	for (i = 0; i < MAX_LOG_TASKS; i++) {
		if (devMassuringsTask[i].adr == oldID) {
			devMassuringsTask[i].adr = 0; // nu = not used
			devMassuringsTask[i].msid = 0;
			devMassuringsTask[i].chan = 0;
			devMassuringsTask[i].per = 0;
			// new in version 2.07
			devMassuringsTask[i].meassuringsList.currentIndex = 0;
			devMassuringsTask[i].meassuringsList.structSize = 0;

			gwDataLogerUtility_saveStruct();
			return;
		}
	}
}

DEV_MEASSUR_TASK* gwDagaLogerUtility_getStruct(void) {
	return (DEV_MEASSUR_TASK*) &devMassuringsTask;
}

int gwDataLogerUtility_updateStructFromBuffer(char* bufferRec,
		DEV_MEASSUR_TASK* pdevMeassuringsTask, char * separator) {
	int nLength = 0;
	int nNo;
	int buffRecPos = 0;
	char paramName[MAX_PARAM_NAME_LENGTH];
	char paramValue[MAX_PARAM_VALUE_LENGTH];

	while (1) {
		nLength = paramUtil_getNextParam(&bufferRec[buffRecPos], separator,
				paramName, paramValue);
		if (nLength < 0)
			break; // TODO: return -1 ???

		if (nameStartsWith(paramName, "adr") == 0) {
			nNo = atoi(&paramName[3]);
			pdevMeassuringsTask[nNo - 1].adr = atoi(paramValue);
		} else if (nameStartsWith(paramName, "chan") == 0) {
			nNo = atoi(&paramName[4]);
			pdevMeassuringsTask[nNo- 1].chan = atoi(paramValue);
		} else if (nameStartsWith(paramName, "msid") == 0) {
			nNo = atoi(&paramName[4]);
			pdevMeassuringsTask[nNo- 1].msid = atoi(paramValue);
		} else if (nameStartsWith(paramName, "per") == 0) {
			nNo = atoi(&paramName[3]);
			pdevMeassuringsTask[nNo- 1].per = atoi(paramValue);
			pdevMeassuringsTask[nNo- 1].timer = (pdevMeassuringsTask[nNo].per + 1) * 10;
		}

		if (nLength == 0)
			break;
		buffRecPos += nLength;
	}

	return 1;
}

int gwDataLogerUtility_getXMLfromStruct(DEV_MEASSUR_TASK* pdevMassuringsTask,
		char* bufferXML) {
	int i;
	int j;
	int nPos = -1;
	int nLength = 0;
	char parName[10];
	char temp[30];
	char temp2[10];
	DEV * dev;

	int deviceCount = deviceLinkedList_getSize();
	int nDataLogerCount = 0;
	for (i = 0; i < deviceCount; i++) {
		dev = deviceLinkedList_getElementAt(i);
		if (dev->enab && ((dev->type == TG) || (dev->type == IL) || (dev->type
				== SW))) {
			nDataLogerCount++;
		}
	}

	sprintf(temp, "%d", nDataLogerCount + 1);
	nLength += xml_addTag(&bufferXML[nLength], "size", temp);

	nLength += xml_addTag(&bufferXML[nLength], "parN0", "not used");

	for (i = 0; i < deviceCount; i++) {
		dev = deviceLinkedList_getElementAt(i);
		if (dev->enab && ((dev->type == TG) || (dev->type == IL) || (dev->type
				== SW))) {
			nPos++;
			sprintf(temp, "Addr %d: %s", dev->adr, devName[dev->type]);
			sprintf(temp2, "parN%d", nPos + 1);
			nLength += xml_addTag(&bufferXML[nLength], temp2, temp);
		}
	}

	char tt [5];
	for (i = 0; i < MAX_LOG_TASKS; i++) {
		nPos = 0;
		sprintf(parName, "adr%u", (i + 1));
		sprintf(tt, "%d", 0); // if not found
		for (j = 0; j < deviceCount; j++) {
			dev = deviceLinkedList_getElementAt(j);
			if (dev->enab && ((dev->type == TG) || (dev->type == IL)
					|| (dev->type == SW))) {
				nPos++;
				if (dev->adr == pdevMassuringsTask[i].adr) {
					sprintf(tt, "%d", nPos);
					break;
				}
			}
		}
		nLength += xml_addTag(&bufferXML[nLength], parName, tt);

		char temp[2];
		sprintf(parName, "chan%u", (i+1));
		sprintf(temp, "%u", pdevMassuringsTask[i].chan);
		nLength += xml_addTag(&bufferXML[nLength], parName, temp);

		sprintf(parName, "msid%u", (i+1));
		sprintf(temp, "%u", pdevMassuringsTask[i].msid);
		nLength += xml_addTag(&bufferXML[nLength], parName, temp);

		sprintf(parName, "per%u", (i+1));
		sprintf(temp, "%u", pdevMassuringsTask[i].per);
		nLength += xml_addTag(&bufferXML[nLength], parName, temp);
	}

	return nLength;
}

void gwDataLogerUtility_clearStruct(DEV_MEASSUR_TASK* pdevMassuringsTask) {
	int i;

	for (i = 0; i < MAX_LOG_TASKS; i++) {
		pdevMassuringsTask[i].adr = 0; // nu = not used
		pdevMassuringsTask[i].msid = 0;
		pdevMassuringsTask[i].chan = 0;
		pdevMassuringsTask[i].per = 0;
	}
}

