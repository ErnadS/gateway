/*
 * gwParamSaver.h
 *
 *  Created on: 29. mai 2010
 *      Author: Ernad
 */

#ifndef GWPARAMSAVER_H_
#define GWPARAMSAVER_H_

#include "../../model/gwDevParData.h"

char gwParamSaver_init(void);
GW_TIME* getTimeStruct( void );
char gwParamSaver_readFromFile();
char gwParamSaver_saveToFile();

#endif /* GWPARAMSAVER_H_ */
