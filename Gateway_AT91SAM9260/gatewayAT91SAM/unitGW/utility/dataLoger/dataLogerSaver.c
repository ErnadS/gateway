/*
 * dataLogerSaver.c
 *
 *  Created on: 23. apr. 2010
 *      Author: Ernad
 */

#include <stdio.h>
#include<sys/stat.h>

#include "dataLogerSaver.h"
#include "../gwDataLogerUtility.h"
#include "../../../config.h"
#include <string.h>



char dataLogerSaver_init(void) {
	FILE * hFile;
	hFile = fopen(DATA_LOGER_CONFIG_PATH, "r");
	if (hFile == NULL) { // FILE NOT EXIST, CREATE
		struct stat st;
		if (stat("/gateway", &st) != 0) {
			mkdir("/gateway", 0777);
			mkdir("/gateway/Config", 0777);
		} else if (stat("/gateway/Config", &st) != 0) {
			mkdir("/gateway/Config", 0777);
		}

		hFile = fopen(DATA_LOGER_CONFIG_PATH, "w"); // create file if not exist

		DEV_MEASSUR_TASK dataLogerStruct [MAX_LOG_TASKS];
		gwDataLogerUtility_clearStruct((DEV_MEASSUR_TASK*)dataLogerStruct);
		dataLogerSaver_saveToFile((DEV_MEASSUR_TASK*)dataLogerStruct);
	}

	if (hFile == NULL) {
		return 0;
	}

	fclose(hFile);
	return 1;
}

char dataLogerSaver_readFromFile(DEV_MEASSUR_TASK* ppdataLogerStructs) {
	int i = 0;
	int maxSize = 1000;
	char buff[maxSize];

	FILE * hFile;
	hFile = fopen(DATA_LOGER_CONFIG_PATH, "r");
	if (hFile == NULL) {
		return 0;
	}

	int c;
	while ((c = fgetc(hFile)) != EOF && i < maxSize) {
		buff[i++] = (char) c;
	}
	buff[i] = 0;

	gwDataLogerUtility_updateStructFromBuffer(buff, ppdataLogerStructs, "\n");

	fclose(hFile);
	return 1;
}

char dataLogerSaver_saveToFile(DEV_MEASSUR_TASK* ppdataLogerStructs) {
	FILE * hFile;
//	GW_DLOG* ppDLStructs = ppdataLogerStructs;
	DEV_MEASSUR_TASK* pdataLogerStruct= ppdataLogerStructs;
	int i;
	hFile = fopen(DATA_LOGER_CONFIG_PATH, "w");
	if (hFile == NULL) {
		return 0;
	}

	char temp[12];

	for (i = 0; i < MAX_LOG_TASKS; i++) {
		//pdataLogerStruct = *ppDLStructs;
		//ppDLStructs++;
		sprintf(temp, "adr%u=%u\n", (i+1), pdataLogerStruct->adr);
		fputs(temp, hFile);

		sprintf(temp, "msid%u=%u\n", (i+1), pdataLogerStruct->msid);
		fputs(temp, hFile);

		sprintf(temp, "chan%u=%u\n", (i+1), pdataLogerStruct->chan);
		fputs(temp, hFile);

		sprintf(temp, "per%u=%u\n", (i+1), pdataLogerStruct->per);
		fputs(temp, hFile);

		pdataLogerStruct++;
	}

	fclose(hFile);

	return 1;
}


