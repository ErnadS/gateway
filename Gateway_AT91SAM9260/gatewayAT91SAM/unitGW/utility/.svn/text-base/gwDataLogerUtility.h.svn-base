/*
 * gwDataLogerUtility.h
 *
 *  Created on: 23. apr. 2010
 *      Author: Ernad
 */

#ifndef GWDATALOGERUTILITY_H_
#define GWDATALOGERUTILITY_H_

#include "../model/gwDataLoger.h"


void gwDataLogerUtility_clearStruct(DEV_MEASSUR_TASK* ppdataLogerStruct);
void gwDataLogerUtility_removeCanID(int oldID);
void gwDataLogerUtility_changeCanID(int oldID, int newID);
int gwDataLogerUtility_getXMLfromStruct(DEV_MEASSUR_TASK* ppdataLogerStruct,
		char* bufferXML);
int gwDataLogerUtility_updateStructFromBuffer(char* bufferRec,
		DEV_MEASSUR_TASK* ppdataLogerStruct, char * separator);
char gwDataLogerUtility_readStruct(void);
char gwDataLogerUtility_saveStruct(void);
DEV_MEASSUR_TASK* gwDagaLogerUtility_getStruct(void);
#endif /* GWDATALOGERUTILITY_H_ */
