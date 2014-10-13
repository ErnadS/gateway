/*
 * gwDevMonitLinkedList.h
 *
 *  Created on: 22. apr. 2010
 *      Author: Ernad
 */

#ifndef GWDEVMONITLINKEDLIST_H_
#define GWDEVMONITLINKEDLIST_H_

#include "../device.h"

int deviceLinkedList_addElement(DEV* newDev);
DEV* deviceLinkedList_getDevice(int devAddr);
int deviceLinkedList_getSize(void);
DEV* deviceLinkedList_getElementAt(int pos);
char deviceLinkedList_removeDevice(int devAddr);
void deviceLinkedList_setFound(int nAddr, char bFound);

DEV* deviceLinkedList_getDeviceByType(int devType);

#endif /* GWDEVMONITLINKEDLIST_H_ */
