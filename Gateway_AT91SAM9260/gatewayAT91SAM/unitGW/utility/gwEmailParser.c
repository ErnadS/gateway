/*
 * gwEmailParser.c
 *
 *  Created on: 7. apr. 2010
 *      Author: Ernad
 */
#include "gwEmailParser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../utility/xml.h"
#include "../../utility/paramUtil.h"

#include "emailConfig/emailConfig.h"
#include "../../services/smtpConfig.h"
#include "../../config.h"

char gwEmail_saveStruct(GW_EMAIL eMailStruct) {
	char result1, result2;
	result1 = smtpConfig_save(eMailStruct.ssmtp);
	result2 = emailConfig_saveToFile(eMailStruct.enabled, eMailStruct.usName, eMailStruct.recip1,
			eMailStruct.recip2, eMailStruct.recip3, eMailStruct.recip4,
			eMailStruct.recip5);

	if ((result1 == 0) && (result2 == 0)) {
		return 0;
	}
	return 1; // ERROR
}

char gwEmail_readStruct(GW_EMAIL* peMailStruct) {
	gwEmailUtility_clearStruct(peMailStruct);

	smtpConfig_read(&peMailStruct->ssmtp);
	emailConfig_readFromFile(peMailStruct);
	return 1;
}

int gwEmailUtility_updateStructFromBuffer(char* bufferRec,
		GW_EMAIL* eMailStruct, char * separator) {
	int nLength = 0;
	int buffRecPos = 0;
	char paramName[MAX_PARAM_NAME_LENGTH];
	char paramValue[MAX_PARAM_VALUE_LENGTH];


	while (1) {
		nLength = paramUtil_getNextParam(&bufferRec[buffRecPos], separator,
				paramName, paramValue);
		if (nLength < 0)
			break; // TODO: return -1 ???

		//gw_enab smtp port usr pass send r1 r2 r3 r4 r5
		if (strcmp(paramName, "gw_enab") == 0) {
			memcpy(&(eMailStruct->enabled), paramValue, sizeof(eMailStruct->enabled));
		} else if (strcmp(paramName, "smtp") == 0) { //&
			memcpy((eMailStruct->ssmtp.smtpAddress), paramValue, sizeof(eMailStruct->ssmtp.smtpAddress));
		} else if (strcmp(paramName, "port") == 0) {
			eMailStruct->ssmtp.port = atoi(paramValue);
		} else if (strcmp(paramName, "usr") == 0) { //&
			memcpy((eMailStruct->ssmtp.user), paramValue, sizeof(eMailStruct->ssmtp.user));
		} else if (strcmp(paramName, "pass") == 0) { //&
			memcpy((eMailStruct->ssmtp.pass), paramValue, sizeof(eMailStruct->ssmtp.pass));
		} else if (strcmp(paramName, "usName") == 0) { //&
			memcpy((eMailStruct->usName), paramValue, sizeof(eMailStruct->usName));
		} else if (strcmp(paramName, "send") == 0) {//&
			memcpy((eMailStruct->ssmtp.sender), paramValue,
					sizeof(eMailStruct->ssmtp.sender));
		} else if (strcmp(paramName, "r1") == 0) { //&
			memcpy((eMailStruct->recip1), paramValue,
					sizeof(eMailStruct->recip1));
		} else if (strcmp(paramName, "r2") == 0) { //&
			memcpy((eMailStruct->recip2), paramValue,
					sizeof(eMailStruct->recip2));
		} else if (strcmp(paramName, "r3") == 0) { //&
			memcpy((eMailStruct->recip3), paramValue,
					sizeof(eMailStruct->recip3));
		} else if (strcmp(paramName, "r4") == 0) { //&
			memcpy((eMailStruct->recip4), paramValue,
					sizeof(eMailStruct->recip4));
		} else if (strcmp(paramName, "r5") == 0) { //&
			memcpy((eMailStruct->recip5), paramValue,
					sizeof(eMailStruct->recip5));
		}

		if (nLength == 0)
			break;
		buffRecPos += nLength;
	}

	return 1;
}

int gwEmail_getXMLfromStruct(GW_EMAIL* eMailStruct, char* bufferXML) {
	int nLength = 0;
	char chTemp[MAX_PARAM_VALUE_LENGTH];

	sprintf(chTemp, "%s", eMailStruct->enabled);
	nLength += xml_addTag(&bufferXML[nLength], "gw_enab", chTemp);

	sprintf(chTemp, "%s", eMailStruct->ssmtp.smtpAddress);
	nLength += xml_addTag(&bufferXML[nLength], "smtp", chTemp);

	sprintf(chTemp, "%d", eMailStruct->ssmtp.port);
	nLength += xml_addTag(&bufferXML[nLength], "port", chTemp);

	if (eMailStruct->ssmtp.user != 0) {
		sprintf(chTemp, "%s", eMailStruct->ssmtp.user);
		nLength += xml_addTag(&bufferXML[nLength], "usr", chTemp);
	}

	if (eMailStruct->ssmtp.pass != 0) {
		sprintf(chTemp, "%s", eMailStruct->ssmtp.pass);
		nLength += xml_addTag(&bufferXML[nLength], "pass", chTemp);
	}

	if (eMailStruct->usName != 0) {
			sprintf(chTemp, "%s", eMailStruct->usName);
			nLength += xml_addTag(&bufferXML[nLength], "usName", chTemp);
		}

	if (eMailStruct->ssmtp.sender != 0) {
		sprintf(chTemp, "%s", eMailStruct->ssmtp.sender);
		nLength += xml_addTag(&bufferXML[nLength], "send", chTemp);
	}

	sprintf(chTemp, "%s", eMailStruct->recip1);
	nLength += xml_addTag(&bufferXML[nLength], "r1", chTemp);

	sprintf(chTemp, "%s", eMailStruct->recip2);
	nLength += xml_addTag(&bufferXML[nLength], "r2", chTemp);

	sprintf(chTemp, "%s", eMailStruct->recip3);
	nLength += xml_addTag(&bufferXML[nLength], "r3", chTemp);

	sprintf(chTemp, "%s", eMailStruct->recip4);
	nLength += xml_addTag(&bufferXML[nLength], "r4", chTemp);

	sprintf(chTemp, "%s", eMailStruct->recip5);
	nLength += xml_addTag(&bufferXML[nLength], "r5", chTemp);

	printf(bufferXML);
	printf("\n");
	return nLength;
}

void gwEmailUtility_clearStruct(GW_EMAIL* eMailStruct) {
	smtpConfig_clearStruct(&eMailStruct->ssmtp);

	eMailStruct->ssmtp.sender[0] = 0;
	eMailStruct->ssmtp.pass[0] = 0;
	eMailStruct->ssmtp.user[0] = 0;
	eMailStruct->ssmtp.sender[0] = 0;
	eMailStruct->usName[0] = 0;
	eMailStruct->recip1[0] = 0;
	eMailStruct->recip2[0] = 0;
	eMailStruct->recip3[0] = 0;
	eMailStruct->recip4[0] = 0;
	eMailStruct->recip5[0] = 0;
}

/////////////////////////////////////////////////////
// needs for could send mail as root when SMTP username is not "root"
// Example of "revaliases" file
// root:noratel.imed.secure@elreg.eu:customer-smtp.one.com:2525
/////////////////////////////////////////////////////
char gwEmailUtility_saveRevaliases(GW_EMAIL eMailStruct) {
	char tempPort[10];
	FILE * hFile;
	hFile = fopen(SMTP_REVALIASES, "w");
	if (hFile == NULL) {
		return 0;
	}

	fputs("root:", hFile);

	fputs(eMailStruct.ssmtp.user, hFile);
	fputs(":", hFile);
	fputs(eMailStruct.ssmtp.smtpAddress, hFile);
	if(eMailStruct.ssmtp.port != 25) {
		fputs(":", hFile);
		sprintf(tempPort, "%u\n", eMailStruct.ssmtp.port);
		fputs(tempPort, hFile);
	}
	fclose(hFile);

	return 1;
}


