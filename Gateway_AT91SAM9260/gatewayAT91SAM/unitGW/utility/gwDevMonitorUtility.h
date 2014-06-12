/*
 * gwDevMonitorUtility.h
 *
 *  Created on: 22. apr. 2010
 *      Author: Ernad
 */

#ifndef GWDEVMONITORUTILITY_H_
#define GWDEVMONITORUTILITY_H_

#include "../model/device.h"

void gwDevMonitorUtility_clearStruct(DEV* device);
void gwDevMonitorUtility_updateDevices(void);
int gwDevMonitorUtility_updateListFromWebBuffer(char* bufferRec, char* separator);
int gwDevMonitorUtility_updateListFromFileBuffer(char* bufferRec, char* separator);
int gwDevMonitorUtility_getXML(char* bufferXML);

#endif /* GWDEVMONITORUTILITY_H_ */
