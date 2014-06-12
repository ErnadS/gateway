/*
 * gwModBus.c
 *
 *  Created on: 12. okt. 2010
 *      Author: Ernad
 */

#include "gwModBus.h"

#include <stdio.h>
#include<sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include "../../utility/xml.h"
#include "../../utility/paramUtil.h"

#include "../../config.h"

void gwModBus_clearStruct(GW_MODBUS *gwMB) {
	gwMB->gw_e=0;
	gwMB->add=1;
	gwMB->br=3;
	gwMB->p=0;
	gwMB->d=3;
	gwMB->s=0;
}
int gwModBus_getStructFromRequest(char* bufferRec, GW_MODBUS *gwMB, char * separator) {
	int nLength = 0;
	int buffRecPos = 0;
	char paramName[MAX_PARAM_NAME_LENGTH];
	char paramValue[MAX_PARAM_VALUE_LENGTH];

	while (1) {
		nLength = paramUtil_getNextParam(&bufferRec[buffRecPos], separator,
				paramName, paramValue);
		if (nLength < 0)
			break; // TODO: return -1 ???

		if (strcmp(paramName, "gw_e") == 0) {
			gwMB->gw_e=atoi(paramValue);
		} else if (strcmp(paramName, "add") == 0) {
			gwMB->add=atoi(paramValue);
		} else if (strcmp(paramName, "br") == 0) {
			gwMB->br=atoi(paramValue);
		} else if (strcmp(paramName, "p") == 0) {
			gwMB->p=atoi(paramValue);
		} else if (strcmp(paramName, "d") == 0) {
			gwMB->d=atoi(paramValue);
		} else if (strcmp(paramName, "s") == 0) {
			gwMB->s=atoi(paramValue);
		}
		if (nLength == 0)
			break;
		buffRecPos += nLength;
	}

	return 1;
}

char gwModBus_init(void) {
	FILE * hFile;
	hFile = fopen(MODBUS_CONFIG_PATH, "r");
	if (hFile == NULL) { // FILE NOT EXIST, CREATE
		struct stat st;
		if (stat("/gateway", &st) != 0) {
			mkdir("/gateway", 0777);
			mkdir("/gateway/Config", 0777);
		} else if (stat("/gateway/Config", &st) != 0) {
			mkdir("/gateway/Config", 0777);
		}

		hFile = fopen(MODBUS_CONFIG_PATH, "w"); // create file if not exist

		GW_MODBUS gwMB;
		gwModBus_clearStruct(&gwMB);
		gwModBus_saveToFile(gwMB);
	}

	if (hFile == NULL) {
		return 0;
	}

	fclose(hFile);
	return 1;
}

char gwModBus_readFromFile(GW_MODBUS *gwMB) {
	int i = 0;
	int maxSize = 500;
	char buff[maxSize];

	FILE * hFile;
	hFile = fopen(MODBUS_CONFIG_PATH, "r");
	if (hFile == NULL) {
		return 0;
	}

	int c;
	while ((c = fgetc(hFile)) != EOF && i < maxSize) {
		buff[i++] = (char) c;
	}
	buff[i] = 0;

	gwModBus_getStructFromRequest(buff, gwMB, "\n");

	fclose(hFile);
	return 1;
}

char gwModBus_saveToFile(GW_MODBUS gwMB) {
	char chTemp[5];

	FILE * hFile;
	hFile = fopen(MODBUS_CONFIG_PATH, "w");
	if (hFile == NULL) {
		return 0;
	}

	fputs("gw_e=", hFile);
	sprintf(chTemp, "%u", gwMB.gw_e); // write asci "enabled".
	fputs(chTemp, hFile);
	fputs("\n", hFile);

	fputs("add=", hFile);
	sprintf(chTemp, "%u", gwMB.add); // write asci
	fputs(chTemp, hFile);
	fputs("\n", hFile);

	fputs("br=", hFile);
	sprintf(chTemp, "%u", gwMB.br); // write asci
	fputs(chTemp, hFile);
	fputs("\n", hFile);

	fputs("p=", hFile);
	sprintf(chTemp, "%u", gwMB.p); // write asci.
	fputs(chTemp, hFile);
	fputs("\n", hFile);

	fputs("d=", hFile);
	sprintf(chTemp, "%u", gwMB.d); // write asci
	fputs(chTemp, hFile);
	fputs("\n", hFile);

	fputs("s=", hFile);
	sprintf(chTemp, "%u", gwMB.s); // write asci
	fputs(chTemp, hFile);
	fputs("\n", hFile);



	fclose(hFile);

	return 1;
}

int gwModBus_getXMLfromStruct(GW_MODBUS *gwMB, char* bufferXML) {
	int nLength = 0;
	char chTemp[MAX_PARAM_VALUE_LENGTH];

	sprintf(chTemp, "%u", gwMB->gw_e);
	nLength += xml_addTag(&bufferXML[nLength], "gw_e", chTemp);

	sprintf(chTemp, "%u", gwMB->add);
	nLength += xml_addTag(&bufferXML[nLength], "add", chTemp);

	sprintf(chTemp, "%u", gwMB->br);
	nLength += xml_addTag(&bufferXML[nLength], "br", chTemp);

	sprintf(chTemp, "%u", gwMB->p);
	nLength += xml_addTag(&bufferXML[nLength], "p", chTemp);

	sprintf(chTemp, "%u", gwMB->d);
	nLength += xml_addTag(&bufferXML[nLength], "d", chTemp);

	sprintf(chTemp, "%u", gwMB->s);
	nLength += xml_addTag(&bufferXML[nLength], "s", chTemp);

	return nLength;
}
