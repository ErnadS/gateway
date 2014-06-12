/*
 * postHeader.c
 *
 *  Created on: 29. jan. 2010
 *      Author: Ernad
 */

#include "postHeader.h"
#include "../utility/paramUtil.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../utility/xml.h"
#include "../config.h"



int postHeader_getData(char* bufferRec, postHeader* postHeaderData) {
	int nLength = 0;
	int buffRecPos = 0;
	char paramName[MAX_PARAM_NAME_LENGTH];
	char paramValue[MAX_PARAM_VALUE_LENGTH];

	while (1) {
		nLength = paramUtil_getNextParam(&bufferRec[buffRecPos], "&", paramName,
				paramValue);
		if (nLength < 0)
			return -1;

		buffRecPos += nLength;
		/*
#ifdef DEBUG_CLIENT
		fprintf(stderr, "rec header param: ");
#endif*/
		fprintf(stderr, paramName);
	/*
#ifdef DEBUG_CLIENT
		fprintf(stderr, ", rec header value: ");
#endif*/
		fprintf(stderr, paramValue);


		if (strcmp(paramName, "comm") == 0) {
			postHeaderData->command = paramValue[0];
		} else if (strcmp(paramName, "formID") == 0) {
			memcpy(postHeaderData->formID, paramValue, strlen(paramValue));
			postHeaderData->formID[strlen(paramValue)] = 0;
		} else if (strcmp(paramName, "canID") == 0) {
			postHeaderData->canID = atoi(paramValue);
		} else if (strcmp(paramName, "loginID") == 0) {
			memcpy(postHeaderData->loginID, paramValue, strlen(paramValue));
			postHeaderData->loginID[strlen(paramValue)] = 0;
		} else if (strcmp(paramName, "END") == 0) {
			return buffRecPos;
		}
	}
	return -1;
}

