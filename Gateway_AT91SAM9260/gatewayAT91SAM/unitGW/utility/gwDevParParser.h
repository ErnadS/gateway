/*
 * gwDevParParser.h
 *
 *  Created on: 6. apr. 2010
 *      Author: Ernad
 */

#include <string.h>
#include <stdio.h>
#include "../model/gwDevParData.h"

#ifndef GWDEVPARPARSER_H_
#define GWDEVPARPARSER_H_

void gwDevParParser_clearStruct(GW_TIME* eMailStruct);
int gwDevParParser__getXMLfromStruct(GW_TIME* timeStruct, char* bufferXML, char command);
int gwDevParParser_getStructFromRequest(char* bufferRec, GW_TIME* timeStruct);

#endif /* GWDEVPARPARSER_H_ */
