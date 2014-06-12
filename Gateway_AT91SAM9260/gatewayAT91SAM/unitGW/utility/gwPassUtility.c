/*
 * gwPassUtility.c
 *
 *  Created on: 21. apr. 2010
 *      Author: Ernad
 */


#include "gwPassUtility.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../utility/xml.h"
#include "../../utility/paramUtil.h"

#include "passFile/passFile.h"
#include "../../config.h"



char gwPassUtility_saveStruct(GW_PASS *passStruct) {
	return  passFile_saveToFile(passStruct);
}

char gwPassUtility_readStruct(GW_PASS* passStruct) {
	gwPassUtility_clearStruct(passStruct);
	passFile_readFromFile(passStruct);
	return 1;
}

int gwPassUtility_updateStructFromBuffer(char* bufferRec, GW_PASS* passOld,
		GW_PASS* passNew, char * separator) {
	int nLength = 0;
	int buffRecPos = 0;
	char paramName[MAX_PARAM_NAME_LENGTH];
	char paramValue[MAX_PARAM_VALUE_LENGTH];

	while (1) {
		nLength = paramUtil_getNextParam(&bufferRec[buffRecPos], separator,
				paramName, paramValue);
		if (nLength < 0)
			break; // TODO: return -1 ???

		if (strcmp(paramName, "lpass1") == 0) {
			strcpy((passOld->lpass1), paramValue);
		} else if (strcmp(paramName, "lpass2") == 0) {
			strcpy((passNew->lpass1), paramValue);
		} else if (strcmp(paramName, "apass1") == 0) {
			strcpy((passOld->apass1), paramValue);
		}  else if (strcmp(paramName, "apass2") == 0) {
			 strcpy((passNew->apass1), paramValue);
		}

		if (nLength == 0)
			break;
		buffRecPos += nLength;
	}

	return 1;
}

int gwPassUtility_getXMLfromStruct(GW_PASS* pdhcpStruct, char* bufferXML) {
	int nLength = 0;
	char chTemp[MAX_PARAM_VALUE_LENGTH];

	sprintf(chTemp, "%s", " ");
	nLength += xml_addTag(&bufferXML[nLength], "lpass1", chTemp);

	// sprintf(chTemp, "%s", " ");// ny endring
	// nLength += xml_addTag(&bufferXML[nLength], "lpass2", chTemp);// ny endring

	sprintf(chTemp, "%s", " ");
	nLength += xml_addTag(&bufferXML[nLength], "apass1", chTemp);

	// sprintf(chTemp, "%s", " ");// ny endring
	// nLength += xml_addTag(&bufferXML[nLength], "apass2", chTemp);// ny endring


	return nLength;
}

void gwPassUtility_clearStruct(GW_PASS* pdhcpStruct) {
	memset(pdhcpStruct->lpass1, 0, sizeof(pdhcpStruct->lpass1));
	// memset(pdhcpStruct->lpass2, 0, sizeof(pdhcpStruct->lpass2)); // ny endring
	memset(pdhcpStruct->apass1, 0, sizeof(pdhcpStruct->apass1));
	// memset(pdhcpStruct->apass2, 0, sizeof(pdhcpStruct->apass2)); // ny endring
}

void gwPassUtility_setDefault(GW_PASS* pdhcpStruct) {
	strcpy(pdhcpStruct->lpass1, "IMED02");
	//strcpy(pdhcpStruct->lpass2, "IMED02"); // ny endring
	strcpy(pdhcpStruct->apass1, "IMED01");
	// strcpy(pdhcpStruct->apass2, "gateway"); // ny endring
}
