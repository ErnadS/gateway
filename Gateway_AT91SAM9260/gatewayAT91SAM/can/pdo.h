/*
This file is part of the NCAN bus, a library implementing CanOpen Light Stack.

Copyright (c) Elreg as 2010

PDO is a communication object defined by the DPO communication parameter and PDA mapping parameter objects.
It is an uncomfirmed communication service without protocol overhead.
*/
 
#ifndef __pdo_h__
#define __pdo_h__

#include "applicfg.h"
#include "def.h"

#include "can.h"

typedef struct struct_s_PDO_status s_PDO_status;

#include "data.h"

/* Status of the TPDO : */
#define PDO_INHIBITED 0x01
#define PDO_RTR_SYNC_READY 0x01

/** The PDO structure */
struct struct_s_PDO_status {
  unsigned char transmit_type_parameter;
  TIMER_HANDLE event_timer;
  TIMER_HANDLE inhibit_timer;
  Message last_message;
};

#define s_PDO_status_Initializer {0, TIMER_NONE, TIMER_NONE, Message_Initializer}

/** definitions of the different types of PDOs' transmission
 * 
 * SYNCHRO(n) means that the PDO will be transmited every n SYNC signal.
 */
#define TRANS_EVERY_N_SYNC(n) (n) /*n = 1 to 240 */
#define TRANS_SYNC_ACYCLIC    0    /* Trans after reception of n SYNC. n = 1 to 240 */
#define TRANS_SYNC_MIN        1    /* Trans after reception of n SYNC. n = 1 to 240 */
#define TRANS_SYNC_MAX        240  /* Trans after reception of n SYNC. n = 1 to 240 */
#define TRANS_RTR_SYNC        252  /* Transmission on request */
#define TRANS_RTR             253  /* Transmission on request */
#define TRANS_EVENT_SPECIFIC  254  /* Transmission on event */
#define TRANS_EVENT_PROFILE   255  /* Transmission on event */

/** 
 * Copy all the data to transmit in process_var
 * Prepare the PDO defined at index to be sent
 * *pwCobId : returns the value of the cobid. (subindex 1)
 * *d Pointer on a CAN object data structure
 * numPdo The PDO number
 * *pdo Pointer on a CAN message structure
 * Returns 0 or 0xFF if error.
 */
unsigned char buildPDO(CO_Data* d, unsigned char numPdo, Message *pdo);

/** 
 * Transmit a PDO request frame on the network to the slave.
 * *d Pointer on a CAN object data structure
 * RPDOIndex Index of the receive PDO
 * Returns:
 *       - NCANbus file descriptor is returned upon success.
 *       - 0xFF is returned if RPDO Index is not found.
 
 * @return 0xFF if error, other in success.
 */
unsigned char sendPDOrequest( CO_Data* d, unsigned int RPDOIndex );

/**
 * Compute a PDO frame reception
 * bus_id is hardware dependant
 * *d Pointer on a CAN object data structure
 * *m Pointer on a CAN message structure
 * Returns: 0xFF if error, else return 0
 */
unsigned char proceedPDO (CO_Data* d, Message *m);

/** 
 * Used by the application to signal changes in process data
 * that could be mapped to some TPDO.
 * This do not necessarily imply PDO emission.
 * Function iterates on all TPDO and look TPDO transmit 
 * type and content change before sending it.    
 * *d Pointer on a CAN object data structure
 */
unsigned char sendPDOevent (CO_Data* d);
unsigned char sendOnePDOevent (CO_Data* d, unsigned long pdoNum);

/** 
 * Function iterates on all TPDO and look TPDO transmit 
 * type and content change before sending it.
 * *d Pointer on a CAN object data structure
 * isSyncEvent
 */
unsigned char _sendPDOevent(CO_Data* d, unsigned char isSyncEvent);

/** 
 * Initialize PDO feature 
 * *d Pointer on a CAN object data structure
 */
void PDOInit(CO_Data* d);

/** 
 * Stop PDO feature 
 * *d Pointer on a CAN object data structure
 */
void PDOStop(CO_Data* d);

/** 
 * Set timer for PDO event
 * *d Pointer on a CAN object data structure
 * pdoNum The PDO number
 */
void PDOEventTimerAlarm(CO_Data* d, unsigned long pdoNum);

/** 
 * Inhibit timer for PDO event
 * *d Pointer on a CAN object data structure
 * pdoNum The PDO number
 */
void PDOInhibitTimerAlarm(CO_Data* d, unsigned long pdoNum);

/* copy bit per bit in little endian */
void CopyBits(unsigned char NbBits, unsigned char* SrcByteIndex, unsigned char SrcBitIndex, unsigned char SrcBigEndian, unsigned char* DestByteIndex, unsigned char DestBitIndex, unsigned char DestBigEndian);
#endif
