/*
 * emailFile.h
 *
 *  Created on: 7. apr. 2010
 *      Author: Ernad
 */

#ifndef EMAILCONFIG_H_
#define EMAILCONFIG_H_

#include "../../model/gwEmail.h"
//#include "../gwEmailParser.h"

char emailConfig_init(void);
char emailConfig_readFromFile(GW_EMAIL * peMailStruct);
char emailConfig_saveToFile(char* enabled, char* usName, char* recip1, char* recip2, char* recip3, char* recip4, char* recip5);

#endif /* EMAILCONFIG_H_ */
