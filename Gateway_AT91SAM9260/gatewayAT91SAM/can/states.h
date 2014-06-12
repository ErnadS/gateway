/*
This file is part of the NCAN bus, a library implementing CanOpen Light Stack.

Copyright (c) Elreg as 2010


*/

/** @defgroup statemachine State Machine
 *  @ingroup userapi
 */
 
#ifndef __states_h__
#define __states_h__

#include "applicfg.h"

/* The nodes states 
 * -----------------
 * values are choosen so, that they can be sent directly
 * for heartbeat messages...
 * Must be coded on 7 bits only
 * */
enum enum_nodeState {
  Initialisation  = 0x00, 
  Disconnected    = 0x01,
  Connecting      = 0x02,
  Preparing       = 0x02,
  Stopped         = 0x04,
  Operational     = 0x05,
  Pre_operational = 0x7F,
  Unknown_state   = 0x0F
};

typedef enum enum_nodeState e_nodeState;

typedef struct
{
	signed char csBoot_Up;
	signed char csSDO;
	signed char csEmergency;
	signed char csSYNC;
	signed char csHeartbeat;
	signed char csPDO;
	signed char csLSS;
} s_state_communication;

/** 
 * Function that user app can overload
 * 
 */
typedef void (*initialisation_t)(CO_Data*);
typedef void (*preOperational_t)(CO_Data*);
typedef void (*operational_t)(CO_Data*);
typedef void (*stopped_t)(CO_Data*);

/** 
 * Function that user app can overload
 * *d Pointer on a CAN object data structure
 */
void _initialisation(CO_Data* d);

/** 
 *  Function that user app can overload
 *  *d Pointer on a CAN object data structure
 */
void _preOperational(CO_Data* d);

/**
 *  Function that user app can overload
 *  *d Pointer on a CAN object data structure
 */
void _operational(CO_Data* d);

/** 
 *  Function that user app can overload
 *  *d Pointer on a CAN object data structure
 */
void _stopped(CO_Data* d);

#include "data.h"

/************************* prototypes ******************************/

/** 
 *  Called by driver/app when receiving messages
 *  *d Pointer on a CAN object data structure
 *  *m Pointer on a CAN message structure
 */
void canDispatch(CO_Data* d, Message *m);

/** 
 *  Returns the state of the node
 *  *d Pointer on a CAN object data structure
 * @return The node state
 */
e_nodeState getState (CO_Data* d);

/** 
 *  Change the state of the node 
 *  *d Pointer on a CAN object data structure
 *  newState The state to assign
 * @return 
 */
unsigned char setState (CO_Data* d, e_nodeState newState);

/**
 *  Returns the nodId 
 *  *d Pointer on a CAN object data structure
 * @return
 */
unsigned char getNodeId (CO_Data* d);

/** 
 *  Define the node ID. Initialize the object dictionary
 *  *d Pointer on a CAN object data structure
 *  nodeId The node ID to assign
 */
void setNodeId (CO_Data* d, unsigned char nodeId);

/** 
 *  Some stuff to do when the node enter in pre-operational mode
 *  *d Pointer on a CAN object data structure
 */
void initPreOperationalMode (CO_Data* d);

#endif
