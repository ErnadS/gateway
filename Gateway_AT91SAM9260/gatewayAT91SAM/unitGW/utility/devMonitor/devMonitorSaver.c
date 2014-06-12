/*
 * devMonitorSaver.c
 *
 *  Created on: 23. apr. 2010
 *      Author: Ernad
 */

#include <stdio.h>
#include<sys/stat.h>

#include "devMonitorSaver.h"
#include "../gwDevMonitorUtility.h"
#include "../../../config.h"
#include "../../model/linkedList/deviceLinkedList.h"

char devMonitorSaver_init(void) {
	FILE * hFile;
	hFile = fopen(DEV_FILE_PATH, "r");
	if (hFile == NULL) { // FILE NOT EXIST, CREATE
		struct stat st;
		if (stat("/gateway", &st) != 0) {
			mkdir("/gateway", 0777);
			mkdir("/gateway/Config", 0777);
		} else if (stat("/gateway/Config", &st) != 0) {
			mkdir("/gateway/Config", 0777);
		}

		hFile = fopen(DEV_FILE_PATH, "w"); // create file if not exist
	}

	if (hFile == NULL) {
		return 0;
	}

	fclose(hFile);
	return 1;
}

char devMonitorSaver_updateDevicesFromFile(void) {
	int i = 0;
	int maxSize = 500;
	char buff[maxSize];

	FILE * hFile;
	hFile = fopen(DEV_FILE_PATH, "r");
	if (hFile == NULL) {
		return 0;
	}

	int c;
	while ((c = fgetc(hFile)) != EOF && i < maxSize) {
		buff[i++] = (char) c;
	}
	buff[i] = 0;

	gwDevMonitorUtility_updateListFromFileBuffer(buff, "\n");

	fclose(hFile);
	return 1;
}

char devMonitorSaver_saveToFile(void) {
	int i;
	DEV * pdev;
	FILE * hFile;
	char temp[20];
	hFile = fopen(DEV_FILE_PATH, "w");
	if (hFile == NULL) {
		return 0;
	}
	int nDevCount = deviceLinkedList_getSize();

	for (i = 0; i < nDevCount; i++) {
		pdev = deviceLinkedList_getElementAt(i);

		sprintf(temp, "adr%u=", pdev->adr);
		fputs(temp, hFile);
		sprintf(temp, "%d", pdev->adr);
		fputs(temp, hFile);
		fputs("\n", hFile);

		sprintf(temp, "type%u=", pdev->adr);
		fputs(temp, hFile);
		sprintf(temp, "%d", pdev->type);
				fputs(temp, hFile);
		fputs("\n", hFile);


		sprintf(temp, "enab%u=", pdev->adr);
		fputs(temp, hFile);
		if (pdev->enab == 1)
			fputs("1", hFile);
		else
			fputs("0", hFile);
		fputs("\n", hFile);
	}

	fclose(hFile);

	return 1;
}
