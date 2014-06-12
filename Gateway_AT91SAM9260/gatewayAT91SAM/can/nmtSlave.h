/*
This file is part of the NCAN bus, a library implementing CanOpen Light Stack.

Copyright (c) Elreg as 2010

The NMT Slave methods are called automatically when a NMT message from Master are received.
*/
 
#ifndef __nmtSlave_h__
#define __nmtSlave_h__

#include "applicfg.h"
#include "data.h"

/** 
 * Threat the reception of a NMT message from the master.
 * *d Pointer to the CAN data structure
 * *m Pointer to the message received
 * Returns: 
 *  -  0 if OK 
 *  - -1 if the slave is not allowed, by its state, to receive the message
 */
void proceedNMTstateChange (CO_Data* d, Message * m);

/** 
 * Transmit the boot-Up frame when the slave is moving from initialization
 * state to pre_operational state.
 * *d Pointer on the CAN data structure
 * Returns: canSend(bus_id,&m)
 */
unsigned char slaveSendBootUp (CO_Data* d);


#endif /* __nmtSlave_h__ */
