/*
This file is part of the NCANbus, a library implementing an light version of the Canopen standard.

Copyright (c) Elreg as 2010

Definitions of the functions that manage EMCY (emergency) messages
*/

#include "data.h"
#include "emcy.h"
#include "ncanbus.h"
#include "sysdep.h"
#include "canInterface.h" // Ernad

#ifdef COMPILE_FOR_IM150
#include "alarm.h" 
#endif

/*
Alarm ids from Fx00 - Fx7F have index to custom defined alarm text in the Object dictionary
To be able to find the Index and subindex the folowing conversion table have been set up
The lower byte of the errorcode is used as ponter to the table
*/
//unsigned int __flash const ALARMPANEL_errorcode_vs_index[] = {
unsigned int const ALARMPANEL_errorcode_vs_index[] = {
  0x0000,0x0000,//Dummy
                //Not finished
  
  
};
//unsigned int __flash const TRAFOGUARD_errorcode_vs_index[] = {
unsigned int const TRAFOGUARD_errorcode_vs_index[] = {
  0x0000,0x0000,//Dummy
                //Not finished
  
  
};
//unsigned int __flash const ISOLOCATOR_errorcode_vs_index[] = {
unsigned int const ISOLOCATOR_errorcode_vs_index[] = {
  0x0000,0x0000,//Dummy
                //Not finished
  
  
};

//unsigned int __flash const SWITCHOVERUNIT_errorcode_vs_index[] = {
unsigned int const SWITCHOVERUNIT_errorcode_vs_index[] = {
  0x0000,0x0000,//Dummy
  0x3000,0x0005,//Index, subindex errorcode: 0xF301 Supply failure Net1
  0x3001,0x0005,//Index, subindex errorcode: 0xF302 Supply failure Net2
  0x3002,0x0005,//Index, subindex errorcode: 0xF303 Supply failure Net3
  0x3003,0x0003,//Index, subindex errorcode: 0xF304 Supply failure Ds
  0x3000,0x0006,//Index, subindex errorcode: 0xF305 Coil failure K1on
  0x3000,0x0007,//Index, subindex errorcode: 0xF306 Coil failure K1off
  0x3001,0x0006,//Index, subindex errorcode: 0xF307 Coil failure K2on
  0x3001,0x0007,//Index, subindex errorcode: 0xF308 Coil failure K2off
  0x3002,0x0006,//Index, subindex errorcode: 0xF307 Coil failure K3on
  0x3002,0x0007,//Index, subindex errorcode: 0xF308 Coil failure K3off
//  0x3003,0x0003,//Index, subindex
//  0x3003,0x0007,//Index, subindex
};

//Table of pointers to above errorcode vs index tables
//unsigned int const __flash * __flash errorcode_vs_index[] = {
unsigned int const *  errorcode_vs_index[] = {
  ALARMPANEL_errorcode_vs_index,
  TRAFOGUARD_errorcode_vs_index,
  ISOLOCATOR_errorcode_vs_index,
  SWITCHOVERUNIT_errorcode_vs_index
};


/*
Alarm ids from Fx80 - FxFF have index to predefined alarm text in the text file
To be able to find the text corresponding with the errorcode the folowing conversion table have been set up
The lower byte of the errorcode is used as ponter to the table
*/

unsigned long OnNumberOfErrorsUpdate(CO_Data* d, const indextable * unsused_indextable, unsigned char unsused_bSubindex);
unsigned char sendEMCY(CO_Data* d, unsigned int errCode, unsigned char errRegister, unsigned int addinfo);

/*! This is called when Index 0x1003 is updated.
**
**
** @param d
** @param unsused_indextable
** @param unsused_bSubindex
**
** @return
**/
unsigned long OnNumberOfErrorsUpdate(CO_Data* d, const indextable * unsused_indextable, unsigned char unsused_bSubindex)
{
  unsigned char index;
  // if 0, reset Pre-defined Error Field
  // else, don't change and give an abort message (eeror code: 0609 0030h)
  if (*d->error_number == 0)
    for (index = 0; index < d->error_history_size; ++index)
      *(d->error_first_element + index) = 0;		/* clear all the fields in Pre-defined Error Field (1003h) */
  else
    ;// abort message
  return 0;
}

/*! start the EMCY mangagement.
**
**
** d pointer to node data
**/
void emergencyInit(CO_Data* d)
{
  RegisterSetODentryCallBack(d, 0x1003, 0x00, &OnNumberOfErrorsUpdate);
  
  *d->error_number = 0;
}

/*!
**
**
** d pointer to node data
**/
void emergencyStop(CO_Data* d)
{
  
}

/*!                                                                                                
**                                                                                                 
**                                                                                                 
** d pointer to node data                                                                              
** cob_id                                                                                   
**                                                                                                 
** Returns: canSend(&m);                                                                                         
**/  
unsigned char sendEMCY(CO_Data* d, unsigned int errCode, unsigned char errRegister, unsigned int addinfo)
{
  Message m;
  //unsigned int const __flash *ptr;
  unsigned int const *ptr;
  MSG_WAR(0x3051, "sendEMCY", 0);
  
  
  m.cob_id = (unsigned int)UNS16_LE(*(unsigned long*)d->error_cobid);
  m.rtr = NOT_A_REQUEST;
  m.len = 8;
  m.data[0] = errCode & 0xFF;        /* LSB */
  m.data[1] = (errCode >> 8) & 0xFF; /* MSB */
  m.data[2] = errRegister;
  m.data[3] = addinfo & 0xFF;        /* LSB */;	
  m.data[4] = (addinfo >> 8) & 0xFF; /* MSB */      
  m.data[5] = 0;        
  m.data[6] = 0;        
  m.data[7] = 0;  
  
  if((m.data[1]&0xF0)==0xF0)//Device specific error code?
  {
    if(m.data[0]<128)//Device specific error codes with the LSB byte lower than 128 should include index and subindex of objdict text
    {
      /*
      m.data[1]   ==0xF0)   AlarmPanel alarm
      m.data[1]   ==0xF1)   TrafoGuard alarm
      if(m.data[1]==0xF2)   Isolocator alarm
      if(m.data[1]==0xF3)   SwitchoverUnit alarm
      if(m.data[1]==0xF4)   GateWay alarm 
      */
      ptr=errorcode_vs_index[m.data[1]&0x0F];//Set pointer to correct index table
      ptr+=(m.data[0]*2);//Set pointer to correct index and subindex values in table
      m.data[3] = (unsigned char)*ptr &0xFF;      /* LSB of index*/
      m.data[4] = (unsigned char)(*ptr++>>8)&0xFF;/* MSB of index*/
      m.data[5] = (unsigned char)*ptr;            /* subindex*/
      
    }
  }
  
  return canSend(&m);
}

/*! Sets a new error with code errCode. Also sets corresponding bits in Error register (1001h)
**                                                                                                 
**  
** d pointer to node data  
** errCode Code of the error                                                                                        
** errRegister Bits of Error register (1001h) to be set.
** Returns 1 if error, 0 if successful
*/
unsigned char EMCY_setError(CO_Data* d, unsigned int errCode, unsigned char errRegMask, unsigned int addInfo)
{
  unsigned char index;
  unsigned char errRegister_tmp;
  
  for (index = 0; index < EMCY_MAX_ERRORS; ++index)
  {
    if (d->error_data[index].errCode == errCode)		/* error already registered */
    {
      if (d->error_data[index].active)
      {
        MSG_WAR(0x3052, "EMCY message already sent", 0);
        return 0;
      } else d->error_data[index].active = 1;		/* set as active error */
      break;
    }
  }
  
  if (index == EMCY_MAX_ERRORS)		/* if errCode not already registered */
    for (index = 0; index < EMCY_MAX_ERRORS; ++index) if (d->error_data[index].active == 0) break;	/* find first inactive error */
  
  if (index == EMCY_MAX_ERRORS)		/* error_data full */
  {
    MSG_ERR(0x3053, "error_data full", 0);
    return 1;
  }
  
  d->error_data[index].errCode = errCode;
  d->error_data[index].errRegMask = errRegMask;
  d->error_data[index].active = 1;
  
  /* set the new state in the error state machine */
  d->error_state = Error_occurred;
  
  /* set Error Register (1001h) */
  for (index = 0, errRegister_tmp = 0; index < EMCY_MAX_ERRORS; ++index)
    if (d->error_data[index].active == 1) errRegister_tmp |= d->error_data[index].errRegMask;
  *d->error_register = errRegister_tmp;
  
  /* set Pre-defined Error Field (1003h) */
  for (index = d->error_history_size - 1; index > 0; --index)
    *(d->error_first_element + index) = *(d->error_first_element + index - 1);
  *(d->error_first_element) = errCode | ((unsigned long)addInfo << 16);
  if(*d->error_number < d->error_history_size) ++(*d->error_number);
  
  /* send EMCY message */
  if (d->CurrentCommunicationState.csEmergency)
    return sendEMCY(d, errCode, *d->error_register,0x00);
  else return 1;
}

/*! Deletes error errCode. Also clears corresponding bits in Error register (1001h)
**                                                                                                 
**  
** d pointer to node data  
** errCode Code of the error                                                                                        
** errRegister Bits of Error register (1001h) to be set.
** Retuirns 1 if error, 0 if successful
*/
void EMCY_errorRecovered(CO_Data* d, unsigned int errCode)
{
  unsigned char index;
  unsigned char errRegister_tmp;
  unsigned char anyActiveError = 0;
  
  for (index = 0; index < EMCY_MAX_ERRORS; ++index)
    if (d->error_data[index].errCode == errCode) break;	/* find the position of the error */
  
  
  if ((index != EMCY_MAX_ERRORS) && (d->error_data[index].active == 1))
  {
    d->error_data[index].active = 0;
    
    /* set Error Register (1001h) and check error state machine */
    for (index = 0, errRegister_tmp = 0; index < EMCY_MAX_ERRORS; ++index)
      if (d->error_data[index].active == 1)
      {
        anyActiveError = 1;
        errRegister_tmp |= d->error_data[index].errRegMask;
      }
    
    if(anyActiveError == 0)
    {
      d->error_state = Error_free;
      /* send a EMCY message with code "Error Reset or No Error" */
      if (d->CurrentCommunicationState.csEmergency)
        sendEMCY(d, 0x0000, 0x00,errCode);//Include recovered errorcode in emcy message
    }
    *d->error_register = errRegister_tmp;
  }
  else
    MSG_WAR(0x3054, "recovered error was not active", 0);
}

/*! This function is responsible to process an EMCY NCANbus-message.
**
**
** d pointer to node data  
** m The CAN-message which has to be analysed.
**
**/
void proceedEMCY(CO_Data* d, Message* m)
{
  unsigned char nodeID;
  unsigned int errCode;
  unsigned char errReg;
  unsigned int erridx;
  unsigned char errsubidx;
  
  MSG_WAR(0x3055, "EMCY received. Proceed. ", 0);
  
  /* Test if the size of the EMCY is ok */
  if ( m->len != 8) 
  {
    MSG_ERR(0x1056, "Error size EMCY. CobId  : ", m->cob_id);
    return;
  }
  
  /* post the received EMCY */
  nodeID = UNS16_LE(m->cob_id) & 0x7F;
  errCode = m->data[0] | ((unsigned int)m->data[1] << 8);
  errReg = m->data[2];
  erridx = m->data[3] | ((unsigned int)m->data[4] << 8);
  errsubidx = m->data[5]; 
  (*d->post_emcy)(d, nodeID, errCode, errReg,erridx,errsubidx);
}

/*! This callback function is run when we have received a valid EMCY NCANbus-message.
**
**
** d pointer to node data  
** nodeID 
** errCode
** errReg
**/
void _post_emcy(CO_Data* d, unsigned char nodeID, unsigned int errCode, unsigned char errReg,unsigned int idx,unsigned char subidx)
{
	canInterface_onReceivedAlarm(nodeID, errCode, errReg, idx, subidx);
#ifdef COMPILE_FOR_IM150
  unsigned char pos;
  
  if(errCode!=NO_ERROR)
  {
    
    if(GetErrId (errCode)==0xFF)//Test if we already have this error in the alarm table 
    {
      //We have received a new alarm message 
      pos=GetFreeAlarm();
      if(pos!=0xFF)//If free returns 0xFF, too many alarms in table...sorry we have to skip this alarm 
      {
        SetErrorId (pos,errCode);
        SetAlarmNodeId (pos,nodeID);
        SetAlarmIndex (pos,idx);
        SetAlarmSubIndex (pos,subidx);
        SetAlarmStatus (pos,NEW_ALARM);
      }
    }  
  }
  else //We have receivesd a error cleared message or no error message.
  {
    //Find the alarm in the table which corresponds with the error code...
    //The idx is holding the error code when we receive an NO_ERROR emcy message
    pos=GetErrId (idx);
    if(pos!=0xFF)//If the error code is found..
    {
      // ....Set status to alarm recovered but not reset
      SetAlarmStatus (pos,ALARM_RECOVERED); 
    }    
  } 
#endif
}
