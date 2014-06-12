/*
 * apUtility.c
 *
 *  Created on: 25. aug. 2010
 *      Author: Ernad
 */

#include "ap_data.h"
#include "apUtility.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../utility/xml.h"
#include "../utility/paramUtil.h"
#include "../config.h"
#include "../utility/myString.h"

int apUtility_getStructFromRequest(char* bufferRec, apSetup* apSetupData) {
	int nLength = 0;
	int buffRecPos = 0;
	char paramName[MAX_PARAM_NAME_LENGTH];
	char paramValue[MAX_PARAM_VALUE_LENGTH];

	while (1) {
		nLength = paramUtil_getNextParam(&bufferRec[buffRecPos], "&",
				paramName, paramValue);

		//printf("Name: %s, value %s\n", paramName, paramValue);
		if (nLength < 0)
			break; // TODO: return -1 ???


		if (strcmp(paramName, "loc") == 0) {
			memcpy(&(apSetupData->location), paramValue,
					sizeof(apSetupData->location));
		} else if (strcmp(paramName, "pas") == 0) {
			memcpy(&(apSetupData->password), paramValue,
					sizeof(apSetupData->password));
		} else if (strcmp(paramName, "lan") == 0) {
			apSetupData->language = atoi(paramValue);
		} else if (strcmp(paramName, "canAdr") == 0) {
			apSetupData->canBusAddr = atoi(paramValue);
		} else if (strcmp(paramName, "dispC") == 0) {
			apSetupData->displayContrast = atoi(paramValue);
		} else if (strcmp(paramName, "dispT") == 0) {
			apSetupData->displayTimeout = atoi(paramValue);
		}

		if (nLength == 0)
			break;
		buffRecPos += nLength;
	}

	return 1;
}

int apUtility_getXMLfromStruct(apSetup* apSetupData, char* bufferXML) {
	int nLength = 0;
	char chTemp[MAX_PARAM_VALUE_LENGTH];

	//nLength += xml_addTag(&bufferXML[nLength], "err", "Test ERROR");

	sprintf(chTemp, "%s", apSetupData->location);
	nLength += xml_addTag(&bufferXML[nLength], "loc", chTemp);

	sprintf(chTemp, "%s", apSetupData->password);
	nLength += xml_addTag(&bufferXML[nLength], "pas", chTemp);

	sprintf(chTemp, "%d", apSetupData->language);
	nLength += xml_addTag(&bufferXML[nLength], "lan", chTemp);

	sprintf(chTemp, "%d", apSetupData->canBusAddr);
	nLength += xml_addTag(&bufferXML[nLength], "canAdr", chTemp);

	sprintf(chTemp, "%d", apSetupData->displayContrast);
	nLength += xml_addTag(&bufferXML[nLength], "dispC", chTemp);

	sprintf(chTemp, "%d", apSetupData->displayTimeout);
	nLength += xml_addTag(&bufferXML[nLength], "dispT", chTemp);

	return nLength;
}
