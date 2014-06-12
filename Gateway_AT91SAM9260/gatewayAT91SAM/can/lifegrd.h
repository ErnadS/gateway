/*
This file is part of the NCAN bus, a library implementing CanOpen Light Stack.

Copyright (c) Elreg as 2010

  The heartbeat mechanism for a device is established through cyclically transmitting a message by a
  heartbeat producer. One or more devices in the network are aware of this heartbeat message. If the
  heartbeat cycle fails for the heartbeat producer the local application on the heartbeat consumer will be
  informed about that event.
*/

					 
#ifndef __lifegrd_h__
#define __lifegrd_h__


#include "applicfg.h"

typedef void (*heartbeatError_t)(CO_Data*, unsigned char);
void _heartbeatError(CO_Data* d, unsigned char heartbeatID);

typedef void (*post_SlaveBootup_t)(CO_Data*, unsigned char);
void _post_SlaveBootup(CO_Data* d, unsigned char SlaveID);

#include "data.h"

/*************************************************************************
 * Functions
 *************************************************************************/

/** 
 * To read the state of a node
 * This can be used by the master after having sent a life guard request,
 * of by any node if it is waiting for heartbeat.
 *
 * *d Pointer on a CAN object data structure
 * nodeId Id of a node
 * Returns: e_nodeState State of the node corresponding to the nodeId
 */
e_nodeState getNodeState (CO_Data* d, unsigned char nodeId);

/** 
 * Start heartbeat consumer and producer
 * with respect to 0x1016 and 0x1017
 * object dictionary entries
 *
 * *d Pointer on a CAN object data structure
 */
void heartbeatInit(CO_Data* d);

/** 
 * Stop heartbeat consumer and producer
 * *d Pointer on a CAN object data structure
 */
void heartbeatStop(CO_Data* d);

/** 
 * This function is responsible to process a NCANbus-message which seams to be an NMT Error Control
 * Messages. At them moment we assume that every NMT error control message
 * is a heartbeat message.
 * If a BootUp message is detected, it will return the nodeId of the Slave who booted up
 * *d Pointer on a CAN object data structure 
 * *m Pointer on the CAN-message which has to be analysed.
 */
void proceedNODE_GUARD (CO_Data* d, Message* m);


char* lifegrd_getErrorArray();
void lifegrd_clearError(int nIndex);
void lifegrd_clearErrors();

char errorHeartbeat[126];

#endif /*__lifegrd_h__ */
