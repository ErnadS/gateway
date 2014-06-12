/*
 * gwPassUtility.h
 *
 *  Created on: 21. apr. 2010
 *      Author: Ernad
 */

#ifndef GWPASSUTILITY_H_
#define GWPASSUTILITY_H_

#include "../model/gwPass.h"

char gwPassUtility_saveStruct(GW_PASS* passStruct);
char gwPassUtility_readStruct(GW_PASS* passStruct);
int gwPassUtility_updateStructFromBuffer(char* bufferRec, GW_PASS* passOld,
		GW_PASS* passNew, char * separator);
int gwPassUtility_getXMLfromStruct(GW_PASS* pdhcpStruct, char* bufferXML);
void gwPassUtility_clearStruct(GW_PASS* pdhcpStruct);
void gwPassUtility_setDefault(GW_PASS* pdhcpStruct);

#endif /* GWPASSUTILITY_H_ */
