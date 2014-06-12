/*
 * dataLogerLogic.h
 *
 *  Created on: 19. mai 2010
 *      Author: Ernad
 */

#ifndef DATALOGERLOGIC_H_
#define DATALOGERLOGIC_H_

#include "../unitGW/model/gwDataLoger.h"

extern char * messageTextTg[];
extern char * messageTextSw[];

void dataLogerLogic_init(void);
void dataLogerLogic_checkTimers(void);
int dataLogerLogic_getExcel(int nCanAddress, int nMsgId, char* bufferXML);
void dataLogerLogic_removeCanID(int id);

void dataLogerLogic_onChangedStruct(DEV_MEASSUR_TASK * devMeassuringsTaskNew);
void removeUnusedMeasureFiles(void);

#endif /* DATALOGERLOGIC_H_ */
