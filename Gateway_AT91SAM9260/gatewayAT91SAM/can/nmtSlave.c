/*
This file is part of the NCANbus, a library implementing an light version of the Canopen standard.

Copyright (c) Elreg as 2010


*/


#include "nmtSlave.h"
#include "states.h"
#include "ncanbus.h"
#include "sysdep.h"

/*!
** Put the slave in the state wanted by the master
**
** d pointer to node data        
** m pointer to can message
**/
void proceedNMTstateChange(CO_Data* d, Message *m)
{
  if( d->nodeState == Pre_operational ||
     d->nodeState == Operational ||
       d->nodeState == Stopped ) {
         
         MSG_WAR(0x3400, "NMT received. for node :  ", (*m).data[1]);
         
         /* Check if this NMT-message is for this node */
         /* byte 1 = 0 : all the nodes are concerned (broadcast) */
         
         if( ( (*m).data[1] == 0 ) || ( (*m).data[1] == *d->bDeviceNodeId ) ){
           
           switch( (*m).data[0]){ /* command specifier (cs) */
           case NMT_Start_Node:
             if ( (d->nodeState == Pre_operational) || (d->nodeState == Stopped) )
               setState(d,Operational);
             break;
             
           case NMT_Stop_Node:
             if ( d->nodeState == Pre_operational ||
                 d->nodeState == Operational ) {
               setState(d,Stopped);
               printf("*** CAN STOPPED\n");
             }
             break;
             
           case NMT_Enter_PreOperational:
             if ( d->nodeState == Operational ||
                 d->nodeState == Stopped ) {
               setState(d,Pre_operational);
               printf("*** CAN STARTED\n");
             }
             break;
             
           case NMT_Reset_Node:
             if(d->NMT_Slave_Node_Reset_Callback != NULL)
               d->NMT_Slave_Node_Reset_Callback(d);
             setState(d,Initialisation);
             break;
             
           case NMT_Reset_Comunication:
             {
               unsigned char currentNodeId = getNodeId(d);
               
               if(d->NMT_Slave_Communications_Reset_Callback != NULL)
                 d->NMT_Slave_Communications_Reset_Callback(d);
#ifdef CO_ENABLE_LSS
               // LSS changes NodeId here in case lss_transfer.nodeID doesn't 
               // match current getNodeId()
               if(currentNodeId!=d->lss_transfer.nodeID)
                 currentNodeId = d->lss_transfer.nodeID;
#endif
               
               // clear old NodeId to make SetNodeId reinitializing
               // SDO, EMCY and other COB Ids
               *d->bDeviceNodeId = 0xFF; 
               
               setNodeId(d, currentNodeId);
             }
             setState(d,Initialisation);
             break;
             
           }/* end switch */
           
         }/* end if( ( (*m).data[1] == 0 ) || ( (*m).data[1] ==
         bDeviceNodeId ) ) */
       }
}


/*!
**
**
** d pointer to node data        
**
** Returns:
**/
unsigned char slaveSendBootUp(CO_Data* d)
{
  Message m;
  
#ifdef CO_ENABLE_LSS
  if(*d->bDeviceNodeId==0xFF)return 0;
#endif
  
  MSG_WAR(0x3407, "Send a Boot-Up msg ", 0);
  
  /* message configuration */
  {
    unsigned int tmp = NODE_GUARD << 7 | *d->bDeviceNodeId; 
    m.cob_id = UNS16_LE(tmp);
  }
  m.rtr = NOT_A_REQUEST;
  m.len = 1;
  m.data[0] = 0x00;
  
  return canSend(&m);
}

