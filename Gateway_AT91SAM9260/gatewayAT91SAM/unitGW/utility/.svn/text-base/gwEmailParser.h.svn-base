/*
 * gwEmailParser.h
 *
 *  Created on: 7. apr. 2010
 *      Author: Ernad
 */

#include <string.h>
#include <stdio.h>
#include "../model/gwEmail.h"

#ifndef GWEMAILPARSER_H_
#define GWEMAILPARSER_H_

char gwEmail_saveStruct(GW_EMAIL eMailStruct);
char gwEmail_readStruct(GW_EMAIL* peMailStruct);
void gwEmailUtility_clearStruct(GW_EMAIL* eMailStruct);
int gwEmailUtility_updateStructFromBuffer(char* bufferRec, GW_EMAIL* timeStruct, char * separator);
int gwEmail_getXMLfromStruct(GW_EMAIL* tgSetupData, char* bufferXML);

/////////////////////////////////////////////////////
// needs for could send mail as root when SMTP username is not "root"
// Example of "revaliases" file
// root:noratel.imed.secure@elreg.eu:customer-smtp.one.com:2525
/////////////////////////////////////////////////////
char gwEmailUtility_saveRevaliases(GW_EMAIL eMailStruct);

#endif /* GWEMAILPARSER_H_ */
