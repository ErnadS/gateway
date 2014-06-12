/*
 * ilSetupParser.c
 *
 *  Created on: 4. mai 2010
 *      Author: Ernad
 */

#include "ilSetupParser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../utility/xml.h"
#include "../../utility/paramUtil.h"
#include "../../config.h"
#include "../../utility/myString.h"

int ilSetupParser_getStructFromRequest(char* bufferRec, ilSetup* ilSetupData) {
	int nLength = 0;
	int buffRecPos = 0;
	char paramName[MAX_PARAM_NAME_LENGTH];
	char paramValue[MAX_PARAM_VALUE_LENGTH];
	int nIndex;

	while (1) {
		nLength = paramUtil_getNextParam(&bufferRec[buffRecPos], "&",
				paramName, paramValue);
		if (nLength < 0)
			break; // TODO: return -1 ???


		if (strcmp(paramName, "loc") == 0) {
			memcpy(&(ilSetupData->location), paramValue,
					sizeof(ilSetupData->location));
		} else if (strcmp(paramName, "pas") == 0) {
			memcpy(&(ilSetupData->password), paramValue,
					sizeof(ilSetupData->password));
		} else if (strcmp(paramName, "lan") == 0) {
			ilSetupData->language = atoi(paramValue);
		} else if (strcmp(paramName, "canAdr") == 0) {
			ilSetupData->canBusAddr = atoi(paramValue);
		} else if (strcmp(paramName, "dispC") == 0) {
			ilSetupData->displayContrast = atoi(paramValue);
		} else if (strcmp(paramName, "dispT") == 0) {
			ilSetupData->displayTimeout = atoi(paramValue);
		}

		else if (nameStartsWith(paramName, "En") == 0) {
			nIndex = atoi(&paramName[2]);
			ilSetupData->En[nIndex - 1] = atoi(paramValue);
		} else if (nameStartsWith(paramName, "AL") == 0) {
			nIndex = atoi(&paramName[2]);
			ilSetupData->AL[nIndex - 1] = atoi(paramValue);
		} else if (nameStartsWith(paramName, "AT") == 0) {
			nIndex = atoi(&paramName[2]);
			memcpy(&(ilSetupData->AT[nIndex - 1]), paramValue,
					sizeof(ilSetupData->AT[0]));
		}

		if (nLength == 0)
			break;
		buffRecPos += nLength;
	}

	return 1;
}

int ilSetupParser_getXMLfromStruct(ilSetup* ilSetupData, char* bufferXML) {
	int nLength = 0;
	char chTemp[MAX_PARAM_VALUE_LENGTH];

	sprintf(chTemp, "%s", ilSetupData->location);
	nLength += xml_addTag(&bufferXML[nLength], "loc", chTemp);

	sprintf(chTemp, "%s", ilSetupData->password);
	nLength += xml_addTag(&bufferXML[nLength], "pas", chTemp);

	sprintf(chTemp, "%d", ilSetupData->language);
	nLength += xml_addTag(&bufferXML[nLength], "lan", chTemp);

	sprintf(chTemp, "%d", ilSetupData->canBusAddr);
	nLength += xml_addTag(&bufferXML[nLength], "canAdr", chTemp);

	sprintf(chTemp, "%d", ilSetupData->displayContrast);
	nLength += xml_addTag(&bufferXML[nLength], "dispC", chTemp);

	sprintf(chTemp, "%d", ilSetupData->displayTimeout);
	nLength += xml_addTag(&bufferXML[nLength], "dispT", chTemp);

	int i;
	char chTemp2[8];

	for (i = 0; i < 16; i++) {
		sprintf(chTemp, "%d", ilSetupData->En[i]);
		sprintf(chTemp2, "En%d", i + 1);
		nLength += xml_addTag(&bufferXML[nLength], chTemp2, chTemp);

		sprintf(chTemp, "%d", ilSetupData->AL[i]);
		sprintf(chTemp2, "AL%d", i + 1);
		nLength += xml_addTag(&bufferXML[nLength], chTemp2, chTemp);

		sprintf(chTemp2, "AT%d", i + 1);
		nLength += xml_addTag(&bufferXML[nLength], chTemp2, ilSetupData->AT[i]);
	}

	return nLength;
}

int ilMesParser_getXMLfromStruct(ilMes* ilMesData, char* bufferXML) {
	int nLength = 0;
	char chTemp[MAX_PARAM_VALUE_LENGTH];

	sprintf(chTemp, "%u", ilMesData->ch1);
	nLength += xml_addTag(&bufferXML[nLength], "ch1", chTemp);

	sprintf(chTemp, "%u", ilMesData->ch2);
	nLength += xml_addTag(&bufferXML[nLength], "ch2", chTemp);

	sprintf(chTemp, "%u", ilMesData->ch3);
	nLength += xml_addTag(&bufferXML[nLength], "ch3", chTemp);

	sprintf(chTemp, "%u", ilMesData->ch4);
	nLength += xml_addTag(&bufferXML[nLength], "ch4", chTemp);

	sprintf(chTemp, "%u", ilMesData->ch5);
	nLength += xml_addTag(&bufferXML[nLength], "ch5", chTemp);

	sprintf(chTemp, "%u", ilMesData->ch6);
	nLength += xml_addTag(&bufferXML[nLength], "ch6", chTemp);

	sprintf(chTemp, "%u", ilMesData->ch7);
	nLength += xml_addTag(&bufferXML[nLength], "ch7", chTemp);

	sprintf(chTemp, "%u", ilMesData->ch8);
	nLength += xml_addTag(&bufferXML[nLength], "ch8", chTemp);

	sprintf(chTemp, "%u", ilMesData->ch9);
	nLength += xml_addTag(&bufferXML[nLength], "ch9", chTemp);

	sprintf(chTemp, "%u", ilMesData->ch10);
	nLength += xml_addTag(&bufferXML[nLength], "ch10", chTemp);

	sprintf(chTemp, "%u", ilMesData->ch11);
	nLength += xml_addTag(&bufferXML[nLength], "ch11", chTemp);

	sprintf(chTemp, "%u", ilMesData->ch12);
	nLength += xml_addTag(&bufferXML[nLength], "ch12", chTemp);

	sprintf(chTemp, "%u", ilMesData->ch13);
	nLength += xml_addTag(&bufferXML[nLength], "ch13", chTemp);

	sprintf(chTemp, "%u", ilMesData->ch14);
	nLength += xml_addTag(&bufferXML[nLength], "ch14", chTemp);

	sprintf(chTemp, "%u", ilMesData->ch15);
	nLength += xml_addTag(&bufferXML[nLength], "ch15", chTemp);

	sprintf(chTemp, "%u", ilMesData->ch16);
	nLength += xml_addTag(&bufferXML[nLength], "ch16", chTemp);

	return nLength;
}
