/*
 * gwUrlHelp.h
 *
 *  Created on: 26. mai 2010
 *      Author: Ernad
 */

#ifndef GWURLHELP_H_
#define GWURLHELP_H_

#include "../model/gwHelp.h"
char gwHelpUtility_saveStruct(GW_HELP *pgwHelp);
char gwHelpUtility_readStruct(GW_HELP *pgwHelp);
int gwHelpUtility_updateStructFromBuffer(char* bufferRec,
		GW_HELP *pgwHelp, char * separator);
int gwHelpUtility_getXMLfromStruct(GW_HELP *pgwHelp, char* bufferXML);
#endif /* GWURLHELP_H_ */
