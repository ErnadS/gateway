/*
 * gwDevMonitorUtility.c
 *
 *  Created on: 22. apr. 2010
 *      Author: Ernad
 */

#include "gwDevMonitorUtility.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../utility/xml.h"
#include "../../utility/paramUtil.h"
#include "../../config.h"
#include "../model/linkedList/deviceLinkedList.h"
#include "../../utility/myString.h"
#include "devMonitor/devMonitorSaver.h"

#include "../../log/dataLogerLogic.h"

void gwDevMonitorUtility_clearStruct(DEV* device) {
	device->adr = 0;
	device->enab = 0;
	device->type = 0;
}

void gwDevMonitorUtility_updateDevices(void) {
	int nSize = deviceLinkedList_getSize();
	int i;
	DEV* dev;

	for (i = 0; i < nSize; i++) {
		dev = deviceLinkedList_getElementAt(i);
		if ((dev->enab == 0) && (dev->found == 0)) {
			if (deviceLinkedList_removeDevice(dev->adr)) {
				nSize--;
				i--;
			}
		}
	}

	devMonitorSaver_saveToFile();
}

int gwDevMonitorUtility_updateListFromWebBuffer(char* bufferRec,
		char* separator) {
	int nLength = 0;
	int buffRecPos = 0;
	char paramName[MAX_PARAM_NAME_LENGTH];
	char paramValue[MAX_PARAM_VALUE_LENGTH];
	DEV * device;

	int nAdr;

	if (*bufferRec == 0)
		return -1;

	while (1) {
		nLength = paramUtil_getNextParam(&bufferRec[buffRecPos], separator,
				paramName, paramValue);
		if (nLength < 0)
			break; // TODO: return -1 ???


		if (nameStartsWith(paramName, "enab") == 0) {
			nAdr = atoi(&paramName[4]);
			device = deviceLinkedList_getDevice(nAdr);
			if (device != NULL) {
				if (device->enab == 1 && atoi(paramValue) == 0) {
					printf("111. Removing %d\n", nAdr);
					dataLogerLogic_removeCanID(device->adr);
				}
				device->enab = atoi(paramValue);
			} else {
				printf("111. Dev %d is null\n", nAdr);
			}
		}

		if (nLength == 0)
			break;
		buffRecPos += nLength;
	}

	return 1;
}

int gwDevMonitorUtility_updateListFromFileBuffer(char* bufferRec,
		char* separator) {
	int nLength = 0;
	int buffRecPos = 0;
	char paramName[MAX_PARAM_NAME_LENGTH];
	char paramValue[MAX_PARAM_VALUE_LENGTH];
	DEV device;

	if (*bufferRec == 0)
		return -1;

	while (1) {
		nLength = paramUtil_getNextParam(&bufferRec[buffRecPos], separator,
				paramName, paramValue);
		if (nLength < 0)
			break; // TODO: return -1 ???

		if (nameStartsWith(paramName, "adr") == 0) {
			device.adr = atoi(paramValue);
		} else if (nameStartsWith(paramName, "type") == 0) {
			device.type = atoi(paramValue);
		} else if (nameStartsWith(paramName, "enab") == 0) {
			device.enab = atoi(paramValue);
			device.found = 0;
			deviceLinkedList_addElement(&device);
		}

		if (nLength == 0)
			break;
		buffRecPos += nLength;
	}

	return 1;
}

int gwDevMonitorUtility_getXML(char* bufferXML) {
	int i;
	int nLength = 0;
	int nDevCount = deviceLinkedList_getSize();
	DEV* dev;
	char parName[10];

	for (i = 0; i < nDevCount; i++) {
		dev = deviceLinkedList_getElementAt(i);
		/*	sprintf(parName, "dev%u", i);
		 nLength += xml_addTag(&bufferXML[nLength], parName, dev->dev);

		 sprintf(parName, "adr%u", i);
		 nLength += xml_addTag(&bufferXML[nLength], parName, dev->adr);
		 */
		sprintf(parName, "enab%u", i);
		if (dev->enab == 1)
			nLength += xml_addTag(&bufferXML[nLength], parName, "1");
		else
			nLength += xml_addTag(&bufferXML[nLength], parName, "0");
	}

	return nLength;
}

