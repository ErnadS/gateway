/*
This file is part of the NCANbus, a library implementing an light version of the Canopen standard.

Copyright (c) Elreg as 2010


*/



#include "data.h"
#include "lifegrd.h"
#include "ncanbus.h"
#include "dcf.h"
#include "sysdep.h"
#include "syslog.h"
#include "canInterface.h"


//#define CAN_WARN_ENAB   //ES: 2014.03  (could not compile with warn optios)


void ConsumerHearbeatAlarm(CO_Data* d, unsigned long id);
void ProducerHearbeatAlarm(CO_Data* d, unsigned long id);
//unsigned long OnHearbeatProducerUpdate(CO_Data* d, const indextable * unsused_indextable, unsigned char unsused_bSubindex);

/*!
**
**
** d pointer to node data  
** nodeId
**
** Returns: networkNodeState
**/
e_nodeState getNodeState (CO_Data* d, unsigned char nodeId)
{
  e_nodeState networkNodeState = d->NMTable[nodeId];
  return networkNodeState;
}

/*! 
** The Consumer Timer Callback
**
** d pointer to node data
** id
**/
void ConsumerHearbeatAlarm(CO_Data* d, unsigned long id)
{
  /*MSG_WAR(0x00, "ConsumerHearbeatAlarm", 0x00);*/
  
  /* timer have been notified and is now free (non periodic)*/
  /* -> avoid deleting re-assigned timer if message is received too late*/
  d->ConsumerHeartBeatTimers[id]=TIMER_NONE;
  /*! call heartbeat error with NodeId */
  (*d->heartbeatError)(d, (unsigned char)( ((d->ConsumerHeartbeatEntries[id]) & (unsigned long)0x00FF0000) >> (unsigned char)16 ));
}

/*!
**
**
** d pointer to node data
** m
**/
void proceedNODE_GUARD(CO_Data* d, Message* m )
{
  unsigned char nodeId = (unsigned char) GET_NODE_ID((*m));
  
  if((m->rtr == 1) )
    /*!
    ** Notice that only the master can have sent this
    ** node guarding request
    */
  {
    /*!
    ** Receiving a NMT NodeGuarding (request of the state by the
    ** master)
    ** Only answer to the NMT NodeGuarding request, the master is
    ** not checked (not implemented)
    */
    if (nodeId == *d->bDeviceNodeId )
    {
      Message msg;
      unsigned int tmp = *d->bDeviceNodeId + 0x700;
      msg.cob_id = UNS16_LE(tmp);
      msg.len = (unsigned char)0x01;
      msg.rtr = 0;
      msg.data[0] = d->nodeState;
      if (d->toggle)
      {
        msg.data[0] |= 0x80 ;
        d->toggle = 0 ;
      }
      else
        d->toggle = 1 ;
      /* send the nodeguard response. */
#ifdef CAN_WARN_ENAB
      printf("0x3130 Sending NMT Nodeguard to master, state: %u\n", d->nodeState);
#endif

      //MSG_WAR(0x3130, "Sending NMT Nodeguard to master, state: ", d->nodeState);
      canSend( &msg );
    }
    
  }
  else
  { /* Not a request CAN */
#ifdef CAN_WARN_ENAB
      printf("0x3130 Received NMT nodeId :  %u\n", nodeId);
#endif
   // MSG_WAR(0x3110, "Received NMT nodeId : ", nodeId);
    /* the slave's state receievd is stored in the NMTable */
    /* The state is stored on 7 bit */
    d->NMTable[nodeId] = (e_nodeState) ((*m).data[0] & 0x7F) ;
    
    /* Boot-Up frame reception */
    if ( d->NMTable[nodeId] == Initialisation)
    {
      /*
      ** The device send the boot-up message (Initialisation)
      ** to indicate the master that it is entered in
      ** pre_operational mode
      ** Because the  device enter automaticaly in pre_operational
      ** mode,
      ** the pre_operational mode is stored
      ** NMTable[bus_id][nodeId] = Pre_operational
      */
      //MSG_WAR(0x3100, "The NMT is a bootup from node : ", nodeId);
#ifdef CAN_WARN_ENAB
      printf("0x3130 The NMT is a bootup from node : :  %u\n", nodeId);
#endif
      //if(!send_consise_dcf(d,nodeId)){
        /* call post SlaveBootup with NodeId */

			(*d->post_SlaveBootup)(d, nodeId);
      //}
    }
    if( d->NMTable[nodeId] != Unknown_state ) 
    {
      unsigned char index, ConsummerHeartBeat_nodeId ;

      for( index = (unsigned char)0x00; index < *d->ConsumerHeartbeatCount; index++ )
      {
        ConsummerHeartBeat_nodeId = (unsigned char)( ((d->ConsumerHeartbeatEntries[index]) & (unsigned long)0x00FF0000) >> (unsigned char)16 );
        if ( nodeId == ConsummerHeartBeat_nodeId )
        {
          TIMEVAL time = ( (d->ConsumerHeartbeatEntries[index]) & (unsigned long)0x0000FFFF ) ;
          /* Renew alarm for next heartbeat. */
          DelAlarm(d->ConsumerHeartBeatTimers[index]);
          d->ConsumerHeartBeatTimers[index] = SetAlarm(d, index, &ConsumerHearbeatAlarm, MS_TO_TIMEVAL(time), 0);
        }
      }
    }
  }
}

/*! The Consumer Timer Callback
**
**
** d pointer to node data
** id
**/
void ProducerHearbeatAlarm(CO_Data* d, unsigned long id)
{
  if(*d->ProducerHeartBeatTime)
  {
    Message msg;
    /* Time expired, the heartbeat must be sent immediately
    ** generate the correct node-id: this is done by the offset 1792
    ** (decimal) and additionaly
    ** the node-id of this device.
    */
    unsigned int tmp = *d->bDeviceNodeId + 0x700;
    msg.cob_id = UNS16_LE(tmp);
    msg.len = (unsigned char)0x01;
    msg.rtr = 0;
    msg.data[0] = d->nodeState; /* No toggle for heartbeat !*/
    /* send the heartbeat */
    //MSG_WAR(0x3130, "Producing heartbeat: ", d->nodeState);
#ifdef CAN_WARN_ENAB
      printf("0x3130 Producing heartbeat:   %u\n", d->nodeState);
#endif
    canSend(&msg );
    
  }
  else
  {
    d->ProducerHeartBeatTimer = DelAlarm(d->ProducerHeartBeatTimer);
  }
}

/*! This is called when Index 0x1017 is updated.
**
**
** d pointer to node data
** unsused_indextable
** unsused_bSubindex
**
** Returns 0
**/
unsigned long OnHeartbeatProducerUpdate(CO_Data* d, const indextable * unsused_indextable, unsigned char unsused_bSubindex)
{
  heartbeatStop(d);
  heartbeatInit(d);
  return 0;
}

/*!
**
**
** d pointer to node data
**/
void heartbeatInit(CO_Data* d)
{
  
  unsigned char index; /* Index to scan the table of heartbeat consumers */
  RegisterSetODentryCallBack(d, 0x1017, 0x00, &OnHeartbeatProducerUpdate);
  
  d->toggle = 0;
  
  for( index = (unsigned char)0x00; index < *d->ConsumerHeartbeatCount; index++ )
  {
    TIMEVAL time = (unsigned int) ( (d->ConsumerHeartbeatEntries[index]) & (unsigned long)0x0000FFFF ) ;
    /* MSG_WAR(0x3121, "should_time : ", should_time ) ; */
    if ( time )
    {
      d->ConsumerHeartBeatTimers[index] = SetAlarm(d, index, &ConsumerHearbeatAlarm, MS_TO_TIMEVAL(time), 0);
    }
  }
  
  if ( *d->ProducerHeartBeatTime )
  {
    TIMEVAL time = *d->ProducerHeartBeatTime;
    d->ProducerHeartBeatTimer = SetAlarm(d, 0, &ProducerHearbeatAlarm, MS_TO_TIMEVAL(time), MS_TO_TIMEVAL(time));
  }
}

/*!
**
**
** d pointer to node data
**/
void heartbeatStop(CO_Data* d)
{
  unsigned char index;
  for( index = (unsigned char)0x00; index < *d->ConsumerHeartbeatCount; index++ )
  {
    d->ConsumerHeartBeatTimers[index] = DelAlarm(d->ConsumerHeartBeatTimers[index]);
  }
  
  d->ProducerHeartBeatTimer = DelAlarm(d->ProducerHeartBeatTimer);
}

/*!
**
**
** heartbeatID
**/
void _heartbeatError(CO_Data* d, unsigned char heartbeatID){
	syslog(LOG_ERR, "Error CAN node id: %d\n", heartbeatID);
	errorHeartbeat[heartbeatID] = (char)1;
} //Not finished


char* lifegrd_getErrorArray(){
	return errorHeartbeat;
}

void lifegrd_clearError(int nIndex){
	errorHeartbeat[nIndex] = (char)0;
}

void lifegrd_clearErrors(){
	int i;
	for(i = 0; i < 126; i++)
		errorHeartbeat[i] = (char)0;
}

void _post_SlaveBootup(CO_Data* d, unsigned char SlaveID) {
	syslog(LOG_ERR, "Slave bootup ID: %u", SlaveID);
	onSlaveBootup(SlaveID);
	lifegrd_clearError(SlaveID);
}  //Not finished
