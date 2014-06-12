/*
This file is part of the NCAN bus, a library implementing CanOpen Light Stack.

Copyright (c) Elreg as 2010

 SYNC object is a NCANbus message forcing the receiving nodes to sample the inputs mapped into synchronous TPDOS.
 Receiving this message cause the node to set the outputs to values received in the previous synchronous RPDO.
*/
					 
#ifndef __SYNC_h__
#define __SYNC_h__

void startSYNC(CO_Data* d);

void stopSYNC(CO_Data* d);

typedef void (*post_sync_t)(CO_Data*);
void _post_sync(CO_Data* d);

typedef void (*post_TPDO_t)(CO_Data*);
void _post_TPDO(CO_Data* d);

/** 
 * Transmit a SYNC message and trigger sync TPDOs
 * *d Pointer on a CAN object data structure
 * Returns...
 */
unsigned char sendSYNC (CO_Data* d);

/** 
 * Transmit a SYNC message on CAN bus
 * *d Pointer on a CAN object data structure
 * Returns...
 */
unsigned char sendSYNCMessage(CO_Data* d);

/** 
 * This function is called when the node is receiving a SYNC message (cob-id = 0x80).
 *  - Check if the node is in OERATIONAL mode. (other mode : return 0 but does nothing).
 *  - Get the SYNC cobId by reading the dictionary index 1005, check it does correspond to the received cobId
 *  - Trigger sync TPDO emission 
 * *d Pointer on a CAN object data structure
 * Returns 0 if OK, 0xFF if error 
 */
unsigned char proceedSYNC (CO_Data* d);

#endif
