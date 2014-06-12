/*
 * tg_web.h
 *
 *  Created on: 28. jan. 2010
 *      Author: Ernad
 */
#include <string.h>
#include <stdio.h>

#include "../model/tg_data.h"

#ifndef TG_WEB_H_
#define TG_WEB_H_

int tgSetupParser_getStructFromRequest(char* bufferRec, tgSetup* tgSetupData);
int tgSetupParser_getXMLfromStruct(tgSetup* tgSetupData, char* bufferXML);
int tgMesParser_getXMLfromStruct(tgMes* tgMesData, char* bufferXML);

#endif /* TG_WEB_H_ */
