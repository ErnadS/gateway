/*
This file is part of the NCANbus, a library implementing an light version of the Canopen standard.

Copyright (c) Elreg as 2010


*/

#include "nmtMaster.h"
#include "ncanbus.h"
#include "sysdep.h"

/*!
**
**
**
** Node_ID
** cs
**
** Returns: canSend(&m);
**/
unsigned char masterSendNMTstateChange(CO_Data* d, unsigned char Node_ID, unsigned char cs)
{
  Message m;
  
  MSG_WAR(0x3501, "Send_NMT cs : ", cs);
  MSG_WAR(0x3502, "    to node : ", Node_ID);
  /* message configuration */
  m.cob_id = 0x0000; /*(NMT) << 7*/
  m.rtr = NOT_A_REQUEST;
  m.len = 2;
  m.data[0] = cs;
  m.data[1] = Node_ID;
  
  return canSend(&m);
}


/*!
**
**
** d pointer to node data        
** nodeId
**
** Returns: canSend(&m);
**/
unsigned char masterSendNMTnodeguard(CO_Data* d, unsigned char nodeId)
{
  Message m;
  
  /* message configuration */
  unsigned int tmp = nodeId | (NODE_GUARD << 7); 
  m.cob_id = UNS16_LE(tmp);
  m.rtr = REQUEST;
  m.len = 0;
  
  MSG_WAR(0x3503, "Send_NODE_GUARD to node : ", nodeId);
  
  return canSend(&m);
}

/*!
**
**
** d pointer to node data        
** nodeId
**/
void masterRequestNodeState(CO_Data* d, unsigned char nodeId)
{
  /* NOT FINISHED: should warn for bad toggle bit. */
  
  /* NMTable configuration to indicate that the master is waiting
  for a Node_Guard frame from the slave whose node_id is ID
  */
  d->NMTable[nodeId] = Unknown_state; /* A state that does not exist */
  
  if (nodeId == 0) { /* NMT broadcast */
    unsigned char i = 0;
    for (i = 0 ; i < NMT_MAX_NODE_ID ; i++) {
      d->NMTable[i] = Unknown_state;
    }
  }
  masterSendNMTnodeguard(d,nodeId);
}

