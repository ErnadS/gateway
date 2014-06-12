/*
 * swSetupParser.c
 *
 *  Created on: 6. mai 2010
 *      Author: Ernad
 */

#include "swSetupParser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../utility/xml.h"
#include "../../utility/paramUtil.h"
#include "../../config.h"
#include "../../utility/myString.h"

int swSetupParser_getStructFromRequest(char* bufferRec, swSetup* swSetupData) {
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
			memcpy(&(swSetupData->location), paramValue,
					sizeof(swSetupData->location));
		} else if (strcmp(paramName, "pas") == 0) {
			memcpy(&(swSetupData->password), paramValue,
					sizeof(swSetupData->password));
		} else if (strcmp(paramName, "lan") == 0) {
			swSetupData->language = atoi(paramValue);
		} else if (strcmp(paramName, "canAdr") == 0) {
			swSetupData->canBusAddr = atoi(paramValue);
		} else if (strcmp(paramName, "dispC") == 0) {
			swSetupData->displayContrast = atoi(paramValue);
		} else if (strcmp(paramName, "dispT") == 0) {
			swSetupData->displayTimeout = atoi(paramValue);
		}

		else if (strcmp(paramName, "uv") == 0) {
			swSetupData->uv = atoi(paramValue);
		} else if (strcmp(paramName, "ov") == 0) {
			swSetupData->ov = atoi(paramValue);
		} else if (strcmp(paramName, "k1On") == 0) {
			swSetupData->k1On = atoi(paramValue);
		} else if (strcmp(paramName, "k1Of") == 0) {
			swSetupData->k1Of = atoi(paramValue);
		} else if (strcmp(paramName, "pOn1") == 0) {
			swSetupData->pOn1 = atoi(paramValue);
		} else if (strcmp(paramName, "pOf1") == 0) {
			swSetupData->pOf1 = atoi(paramValue);
		} else if (strcmp(paramName, "ocOn1") == 0) {
			swSetupData->ocOn1 = atoi(paramValue);
		} else if (strcmp(paramName, "ocOf1") == 0) {
			swSetupData->ocOf1 = atoi(paramValue);
		}

		else if (strcmp(paramName, "k2On") == 0) {
			swSetupData->k2On = atoi(paramValue);
		} else if (strcmp(paramName, "k2Of") == 0) {
			swSetupData->k2Of = atoi(paramValue);
		} else if (strcmp(paramName, "pOn2") == 0) {
			swSetupData->pOn2 = atoi(paramValue);
		} else if (strcmp(paramName, "pOf2") == 0) {
			swSetupData->pOf2 = atoi(paramValue);
		} else if (strcmp(paramName, "ocOn2") == 0) {
			swSetupData->ocOn2 = atoi(paramValue);
		} else if (strcmp(paramName, "ocOf2") == 0) {
			swSetupData->ocOf2 = atoi(paramValue);
		}

		else if (strcmp(paramName, "k3On") == 0) {
			swSetupData->k3On = atoi(paramValue);
		} else if (strcmp(paramName, "k3Of") == 0) {
			swSetupData->k3Of = atoi(paramValue);
		} else if (strcmp(paramName, "pOn3") == 0) {
			swSetupData->pOn3 = atoi(paramValue);
		} else if (strcmp(paramName, "pOf3") == 0) {
			swSetupData->pOf3 = atoi(paramValue);
		} else if (strcmp(paramName, "ocOn3") == 0) {
			swSetupData->ocOn3 = atoi(paramValue);
		} else if (strcmp(paramName, "ocOf3") == 0) {
			swSetupData->ocOf3 = atoi(paramValue);
		} else if (strcmp(paramName, "rd") == 0) {
			swSetupData->rd = atoi(paramValue);
		} else if (strcmp(paramName, "ofP") == 0) {
			swSetupData->ofP = atoi(paramValue);
		} else if (strcmp(paramName, "rt") == 0) {
			swSetupData->rt = atoi(paramValue);
		} else if (strcmp(paramName, "f1") == 0) {
			strcpy(swSetupData->f1, paramValue);
		} else if (strcmp(paramName, "f2") == 0) {
			strcpy(swSetupData->f2, paramValue);
		} else if (strcmp(paramName, "f3") == 0) {
			strcpy(swSetupData->f3, paramValue);
		} else if (strcmp(paramName, "fd") == 0) {
			strcpy(swSetupData->fd, paramValue);
		} else if (strcmp(paramName, "f1On") == 0) {
			strcpy(swSetupData->f1On, paramValue);
		} else if (strcmp(paramName, "f1Of") == 0) {
			strcpy(swSetupData->f1Of, paramValue);
		} else if (strcmp(paramName, "f2On") == 0) {
			strcpy(swSetupData->f2On, paramValue);
		} else if (strcmp(paramName, "f2Of") == 0) {
			strcpy(swSetupData->f2Of, paramValue);
		} else if (strcmp(paramName, "f3On") == 0) {
			strcpy(swSetupData->f3On, paramValue);
		} else if (strcmp(paramName, "f3Of") == 0) {
			strcpy(swSetupData->f3Of, paramValue);
		}

		if (nLength == 0)
			break;
		buffRecPos += nLength;
	}

	return 1;
}


int swSetupParser_getXMLfromStruct(swSetup* swSetupData, char* bufferXML) {
	int nLength = 0;
	char chTemp[MAX_PARAM_VALUE_LENGTH];

	//nLength += xml_addTag(&bufferXML[nLength], "err", "Test ERROR");

	sprintf(chTemp, "%s", swSetupData->location);
	nLength += xml_addTag(&bufferXML[nLength], "loc", chTemp);

	sprintf(chTemp, "%s", swSetupData->password);
	nLength += xml_addTag(&bufferXML[nLength], "pas", chTemp);

	sprintf(chTemp, "%d", swSetupData->language);
	nLength += xml_addTag(&bufferXML[nLength], "lan", chTemp);

	sprintf(chTemp, "%d", swSetupData->canBusAddr);
	nLength += xml_addTag(&bufferXML[nLength], "canAdr", chTemp);

	sprintf(chTemp, "%d", swSetupData->displayContrast);
	nLength += xml_addTag(&bufferXML[nLength], "dispC", chTemp);

	sprintf(chTemp, "%d", swSetupData->displayTimeout);
	nLength += xml_addTag(&bufferXML[nLength], "dispT", chTemp);

	sprintf(chTemp, "%d", swSetupData->uv);
	nLength += xml_addTag(&bufferXML[nLength], "uv", chTemp);
	sprintf(chTemp, "%d", swSetupData->ov);
	nLength += xml_addTag(&bufferXML[nLength], "ov", chTemp);

	sprintf(chTemp, "%d", swSetupData->k1On);
	nLength += xml_addTag(&bufferXML[nLength], "k1On", chTemp);
	sprintf(chTemp, "%d", swSetupData->k1Of);
	nLength += xml_addTag(&bufferXML[nLength], "k1Of", chTemp);
	sprintf(chTemp, "%d", swSetupData->pOn1);
	nLength += xml_addTag(&bufferXML[nLength], "pOn1", chTemp);
	sprintf(chTemp, "%d", swSetupData->pOf1);
	nLength += xml_addTag(&bufferXML[nLength], "pOf1", chTemp);
	sprintf(chTemp, "%d", swSetupData->ocOn1);
	nLength += xml_addTag(&bufferXML[nLength], "ocOn1", chTemp);
	sprintf(chTemp, "%d", swSetupData->ocOf1);
	nLength += xml_addTag(&bufferXML[nLength], "ocOf1", chTemp);

	sprintf(chTemp, "%d", swSetupData->k2On);
	nLength += xml_addTag(&bufferXML[nLength], "k2On", chTemp);
	sprintf(chTemp, "%d", swSetupData->k2Of);
	nLength += xml_addTag(&bufferXML[nLength], "k2Of", chTemp);
	sprintf(chTemp, "%d", swSetupData->pOn2);
	nLength += xml_addTag(&bufferXML[nLength], "pOn2", chTemp);
	sprintf(chTemp, "%d", swSetupData->pOf2);
	nLength += xml_addTag(&bufferXML[nLength], "pOf2", chTemp);
	sprintf(chTemp, "%d", swSetupData->ocOn2);
	nLength += xml_addTag(&bufferXML[nLength], "ocOn2", chTemp);
	sprintf(chTemp, "%d", swSetupData->ocOf2);
	nLength += xml_addTag(&bufferXML[nLength], "ocOf2", chTemp);

	sprintf(chTemp, "%d", swSetupData->k3On);
	nLength += xml_addTag(&bufferXML[nLength], "k3On", chTemp);
	sprintf(chTemp, "%d", swSetupData->k3Of);
	nLength += xml_addTag(&bufferXML[nLength], "k3Of", chTemp);
	sprintf(chTemp, "%d", swSetupData->pOn3);
	nLength += xml_addTag(&bufferXML[nLength], "pOn3", chTemp);
	sprintf(chTemp, "%d", swSetupData->pOf3);
	nLength += xml_addTag(&bufferXML[nLength], "pOf3", chTemp);
	sprintf(chTemp, "%d", swSetupData->ocOn3);
	nLength += xml_addTag(&bufferXML[nLength], "ocOn3", chTemp);
	sprintf(chTemp, "%d", swSetupData->ocOf3);
	nLength += xml_addTag(&bufferXML[nLength], "ocOf3", chTemp);

	sprintf(chTemp, "%d", swSetupData->rd);
	nLength += xml_addTag(&bufferXML[nLength], "rd", chTemp);
	sprintf(chTemp, "%d", swSetupData->ofP);
	nLength += xml_addTag(&bufferXML[nLength], "ofP", chTemp);
	sprintf(chTemp, "%d", swSetupData->rt);
	nLength += xml_addTag(&bufferXML[nLength], "rt", chTemp);

	nLength += xml_addTag(&bufferXML[nLength], "f1", swSetupData->f1);
	nLength += xml_addTag(&bufferXML[nLength], "f2", swSetupData->f2);
	nLength += xml_addTag(&bufferXML[nLength], "f3", swSetupData->f3);
	nLength += xml_addTag(&bufferXML[nLength], "fd", swSetupData->fd);

	nLength += xml_addTag(&bufferXML[nLength], "f1On", swSetupData->f1On);
	nLength += xml_addTag(&bufferXML[nLength], "f1Of", swSetupData->f1Of);
	nLength += xml_addTag(&bufferXML[nLength], "f2On", swSetupData->f2On);
	nLength += xml_addTag(&bufferXML[nLength], "f2Of", swSetupData->f2Of);
	nLength += xml_addTag(&bufferXML[nLength], "f3On", swSetupData->f3On);
	nLength += xml_addTag(&bufferXML[nLength], "f3Of", swSetupData->f3Of);

	return nLength;
}

int swMesParser_getXMLfromStruct(swMes* swMesData, char* bufferXML) {
	int nLength = 0;
	char chTemp[MAX_PARAM_VALUE_LENGTH];

	sprintf(chTemp, "%u", swMesData->su1);
	nLength += xml_addTag(&bufferXML[nLength], "su1", chTemp);

	sprintf(chTemp, "%u", swMesData->su2);
	nLength += xml_addTag(&bufferXML[nLength], "su2", chTemp);

	sprintf(chTemp, "%u", swMesData->su3);
	nLength += xml_addTag(&bufferXML[nLength], "su3", chTemp);

	sprintf(chTemp, "%u", swMesData->dv);
	nLength += xml_addTag(&bufferXML[nLength], "dv", chTemp);

	return nLength;
}

