/*
 * gwUrlHelp.c
 *
 *  Created on: 26. mai 2010
 *      Author: Ernad
 */
#include "gwHelpUtility.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../utility/xml.h"
#include "../../utility/paramUtil.h"
#include "urlHelpConfig/helpFile.h"
#include "urlHelpConfig/helpFile.h"
#include "../../config.h"


char gwHelpUtility_saveStruct(GW_HELP *pgwHelp) {
	return  helpFile_saveToFile(pgwHelp);
}

char gwHelpUtility_readStruct(GW_HELP *pgwHelp) {
	helpFile_readFromFile(pgwHelp);
	return 1;
}

int gwHelpUtility_updateStructFromBuffer(char* bufferRec,
		GW_HELP *pgwHelp, char * separator) {
	int nLength = 0;
	int buffRecPos = 0;
	char paramName[80];
	char paramValue[80];

	while (1) {
		nLength = paramUtil_getNextParam(&bufferRec[buffRecPos], separator,
				paramName, paramValue);
		if (nLength < 0)
			break; // TODO: return -1 ???

		if (strcmp(paramName, "u") == 0) {
			strcpy((pgwHelp->u), paramValue);
		} else if (strcmp(paramName, "m") == 0) {
			strcpy((pgwHelp->m), paramValue);
		}

		if (nLength == 0)
			break;
		buffRecPos += nLength;
	}

	return 1;
}

int gwHelpUtility_getXMLfromStruct(GW_HELP *pgwHelp, char* bufferXML) {
	int nLength = 0;
	char chTemp[80];

	sprintf(chTemp, "%s", pgwHelp->u);
	nLength += xml_addTag(&bufferXML[nLength], "u", chTemp);

	sprintf(chTemp, "%s", pgwHelp->m);
	nLength += xml_addTag(&bufferXML[nLength], "m", chTemp);

	return nLength;
}

