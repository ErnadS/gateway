/*
 * swSetupParser.h
 *
 *  Created on: 6. mai 2010
 *      Author: Ernad
 */

#ifndef SWSETUPPARSER_H_
#define SWSETUPPARSER_H_

#include "../model/swData.h"

int swSetupParser_getStructFromRequest(char* bufferRec, swSetup* swSetupData);
int swSetupParser_getXMLfromStruct(swSetup* swSetupData, char* bufferXML);
int swMesParser_getXMLfromStruct(swMes* swMesData, char* bufferXML);

#endif /* SWSETUPPARSER_H_ */
