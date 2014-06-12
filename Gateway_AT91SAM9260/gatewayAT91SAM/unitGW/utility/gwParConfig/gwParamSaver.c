/*
 * gwParamSaver.c
 *
 *  Created on: 29. mai 2010
 *      Author: Ernad
 */
#include <string.h>
#include <stdio.h>
#include<sys/stat.h>

#include "gwParamSaver.h"
#include "../gwHelpUtility.h"
#include "../../../config.h"
#include "../../../can/ObjDict.h"
#include "../../../services/sysTime.h"
#include "../../../utility/myString.h"

GW_TIME m_gwTimeStruct;

GW_TIME* getTimeStruct(void) {
	return &m_gwTimeStruct;
}

char gwParamSaver_init(void) {
	FILE * hFile;
	//GW_TIME gwTimeStruct;

	// If file not found, write default name
	hFile = fopen(GW_PARAM_FILE_PATH, "r");
	if (hFile == NULL) { // FILE NOT EXIST, CREATE
		strcpy(m_gwTimeStruct.name_CAN_format, "unknown");
		strcpy(m_gwTimeStruct.name_UTF8_format, "unknown");
		gwParamSaver_saveToFile(&m_gwTimeStruct); // save if not exist
	} else {
		fclose(hFile);
	}

	// Update ObjDict struct
	gwParamSaver_readFromFile();
	///////////////////////////////////////////////

	///////////////////////////////////////////////
	//strcpy((char*)ObjDict_obj2002, gwTimeStruct.n);
	return 1;
}

char gwParamSaver_readFromFile() {
	int i = 0;
	int maxSize = 50;
	char buff[maxSize];

	FILE * hFile;
	hFile = fopen(GW_PARAM_FILE_PATH, "r");
	if (hFile == NULL) {
		return 0;
	}

	int c;
	while ((c = fgetc(hFile)) != EOF && i < maxSize) {
		if (c == '\n')
			break;
		buff[i++] = (char) c;
	}
	buff[i] = 0;

	strcpy(m_gwTimeStruct.name_UTF8_format, buff);

	myString_convertUTF8toCAN(m_gwTimeStruct.name_UTF8_format, m_gwTimeStruct.name_CAN_format, 49);
	strcpy((char*) ObjDict_obj2002, m_gwTimeStruct.name_CAN_format);

	fclose(hFile);
	return 1;
}

char gwParamSaver_saveToFile() {

	FILE * hFile;
	hFile = fopen(GW_PARAM_FILE_PATH, "w");
	if (hFile == NULL) {
		return 0;
	}

	fputs(m_gwTimeStruct.name_UTF8_format, hFile);

	fclose(hFile);

	return 1;
}
