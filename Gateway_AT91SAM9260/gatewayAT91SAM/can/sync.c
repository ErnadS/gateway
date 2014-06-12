/*
This file is part of the NCANbus, a library implementing an light version of the Canopen standard.

Copyright (c) Elreg as 2010


*/

#include "data.h"
#include "sync.h"
#include "ncanbus.h"
#include "sysdep.h"

/* Prototypes for internals functions */

/*!                                                                                                
**                                                                                                 
**                                                                                                 
** @param d                                                                                        
** @param id                                                                                       
**/  
void SyncAlarm(CO_Data* d, unsigned long id);
unsigned long OnCOB_ID_SyncUpdate(CO_Data* d, const indextable * unsused_indextable, unsigned char unsused_bSubindex);

/*!                                                                                                
**                                                                                                 
**                                                                                                 
** @param d                                                                                        
** @param id                                                                                       
**/   
void SyncAlarm(CO_Data* d, unsigned long id)
{
  sendSYNC(d) ;
}

/*!                                                                                                
** This is called when Index 0x1005 is updated.                                                                                                
**                                                                                                 
** @param d                                                                                        
** @param unsused_indextable                                                                       
** @param unsused_bSubindex                                                                        
**                                                                                                 
** @return                                                                                         
**/  
unsigned long OnCOB_ID_SyncUpdate(CO_Data* d, const indextable * unsused_indextable, unsigned char unsused_bSubindex)
{
  startSYNC(d);
  return 0;
}

/*!                                                                                                
**                                                                                                 
**                                                                                                 
** @param d                                                                                        
**/ 
void startSYNC(CO_Data* d)
{
  if(d->syncTimer != TIMER_NONE)
  {
    stopSYNC(d);
  }
  
  RegisterSetODentryCallBack(d, 0x1005, 0, &OnCOB_ID_SyncUpdate);
  RegisterSetODentryCallBack(d, 0x1006, 0, &OnCOB_ID_SyncUpdate);
  
  if(*d->COB_ID_Sync & 0x40000000ul && *d->Sync_Cycle_Period)
  {
    d->syncTimer = SetAlarm(
                            d,
                            0 /*No id needed*/,
                            &SyncAlarm,
                            US_TO_TIMEVAL(*d->Sync_Cycle_Period), 
                            US_TO_TIMEVAL(*d->Sync_Cycle_Period));
  }
}

/*!                                                                                                
**                                                                                                 
**                                                                                                 
** @param d                                                                                        
**/   
void stopSYNC(CO_Data* d)
{
  RegisterSetODentryCallBack(d, 0x1005, 0, NULL);
  RegisterSetODentryCallBack(d, 0x1006, 0, NULL);
  d->syncTimer = DelAlarm(d->syncTimer);
}


/*!                                                                                                
**                                                                                                 
**                                                                                                 
** @param d                                                                                        
** @param cob_id                                                                                   
**                                                                                                 
** @return                                                                                         
**/  
unsigned char sendSYNCMessage(CO_Data* d)
{
  Message m;
  
  MSG_WAR(0x3001, "sendSYNC ", 0);
  
  m.cob_id = (unsigned int)UNS16_LE(*d->COB_ID_Sync);
  m.rtr = NOT_A_REQUEST;
  m.len = 0;
  
  return canSend(&m);
}


/*!                                                                                                
**                                                                                                 
**                                                                                                 
** @param d                                                                                        
** @param cob_id                                                                                   
**                                                                                                 
** @return                                                                                         
**/  
unsigned char sendSYNC(CO_Data* d)
{
  unsigned char res;
  res = sendSYNCMessage(d);
  proceedSYNC(d) ; 
  return res ;
}

/*!                                                                                                
**                                                                                                 
**                                                                                                 
** @param d                                                                                        
** @param m                                                                                        
**                                                                                                 
** @return                                                                                         
**/ 
unsigned char proceedSYNC(CO_Data* d)
{
  
  unsigned char res;
  
  MSG_WAR(0x3002, "SYNC received. Proceed. ", 0);
  
  (*d->post_sync)(d);
  
  /* only operational state allows PDO transmission */
  if(! d->CurrentCommunicationState.csPDO) 
    return 0;
  
  res = _sendPDOevent(d, 1 /*isSyncEvent*/ );
  
  /*Call user app callback*/
  (*d->post_TPDO)(d);
  
  return res;
  
}


void _post_sync(CO_Data* d){}
void _post_TPDO(CO_Data* d){}
