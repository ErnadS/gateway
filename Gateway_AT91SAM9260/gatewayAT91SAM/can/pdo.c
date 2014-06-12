/*
This file is part of the NCANbus, a library implementing an light version of the Canopen standard.

Copyright (c) Elreg as 2010


*/
#include "pdo.h"
#include "objacces.h"
#include "ncanbus.h"
#include "sysdep.h"



/*!
**
**
** d pointer to node data        
** TPDO_com TPDO communication parameters OD entry
** TPDO_map TPDO mapping parameters OD entry
**
** Returns: 
**/
unsigned char buildPDO (CO_Data * d, unsigned char numPdo, Message * pdo)
{
  const indextable *TPDO_com = d->objdict + d->firstIndex->PDO_TRS + numPdo;
  const indextable *TPDO_map = d->objdict + d->firstIndex->PDO_TRS_MAP + numPdo;
  
  unsigned char prp_j = 0x00;
  unsigned long offset = 0x00000000;
  const unsigned char *pMappingCount = (unsigned char *) TPDO_map->pSubindex[0].pObject;
  
  pdo->cob_id = UNS16_LE(*(unsigned long*)TPDO_com->pSubindex[1].pObject & 0x7FF);
  pdo->rtr = NOT_A_REQUEST;
  
  MSG_WAR (0x3009, "  PDO CobId is : ",
           *(unsigned long *) TPDO_com->pSubindex[1].pObject);
  MSG_WAR (0x300D, "  Number of objects mapped : ", *pMappingCount);
  
  do
  {
    unsigned char dataType;            /* Unused */
    unsigned char tmp[] = { 0, 0, 0, 0, 0, 0, 0, 0 };  /* temporary space to hold bits */
    
    /* pointer fo the var which holds the mapping parameter of an mapping entry  */
    unsigned long *pMappingParameter =
      (unsigned long *) TPDO_map->pSubindex[prp_j + 1].pObject;
    unsigned int index = (unsigned int) ((*pMappingParameter) >> 16);
    unsigned long Size = (unsigned long) (*pMappingParameter & (unsigned long) 0x000000FF);     /* Size in bits */
    
    /* get variable only if Size != 0 and Size is lower than remaining bits in the PDO */
    if (Size && ((offset + Size) <= 64))
    {
      unsigned long ByteSize = 1 + ((Size - 1) >> 3);        /*1->8 => 1 ; 9->16 => 2, ... */
      unsigned char subIndex =
        (unsigned char) (((*pMappingParameter) >> (unsigned char) 8) & (unsigned long) 0x000000FF);
      
      MSG_WAR (0x300F, "  got mapping parameter : ", *pMappingParameter);
      MSG_WAR (0x3050, "    at index : ", TPDO_map->index);
      MSG_WAR (0x3051, "    sub-index : ", prp_j + 1);
      
      if (getODentry (d, index, subIndex, tmp, &ByteSize, &dataType, 0) !=
          OD_SUCCESSFUL)
      {
        MSG_ERR (0x1013,
                 " Couldn't find mapped variable at index-subindex-size : ",
                 (unsigned int) (*pMappingParameter));
        return 0xFF;
      }
      /* copy bit per bit in little endian */
      CopyBits ((unsigned char) Size, ((unsigned char *) tmp), 0, 0,
                (unsigned char *) & pdo->data[offset >> 3], (unsigned char)(offset % 8), 0);
      
      offset += Size;
    }
    prp_j++;
  }
  while (prp_j < *pMappingCount);
  
  pdo->len = (unsigned char)(1 + ((offset - 1) >> 3));
  
  MSG_WAR (0x3015, "  End scan mapped variable", 0);
  
  return 0;
}

/*!
**
**
** d pointer to node data 
**  cobId
**
** Returns: 
**/
unsigned char
sendPDOrequest (CO_Data * d, unsigned int RPDOIndex)
{
  unsigned long *pwCobId;
  unsigned int offset = d->firstIndex->PDO_RCV;
  unsigned int lastIndex = d->lastIndex->PDO_RCV;
  
  if (!d->CurrentCommunicationState.csPDO)
  {
    return 0;
  }
  
  /* Sending the request only if the cobid have been found on the PDO
  receive */
  /* part dictionary */
  
  MSG_WAR (0x3930, "sendPDOrequest RPDO Index : ", RPDOIndex);
  
  if (offset && RPDOIndex >= 0x1400)
  {
    offset += RPDOIndex - 0x1400;
    if (offset <= lastIndex)
    {
      /* get the CobId */
      pwCobId = d->objdict[offset].pSubindex[1].pObject;
      
      MSG_WAR (0x3930, "sendPDOrequest cobId is : ", *pwCobId);
      {
        Message pdo;
        pdo.cob_id = (unsigned int)UNS16_LE(*pwCobId);
        pdo.rtr = REQUEST;
        pdo.len = 0;
        return canSend (&pdo);
      }
    }
  }
  MSG_ERR (0x1931, "sendPDOrequest : RPDO Index not found : ", RPDOIndex);
  return 0xFF;
}


/*!
**
**
** d pointer to node data 
**  m
**
** Returns: 
**/
unsigned char proceedPDO (CO_Data * d, Message * m)
{
  unsigned char numPdo;
  unsigned char numMap;                  /* Number of the mapped varable */
  unsigned char *pMappingCount = NULL;   /* count of mapped objects... */
  /* pointer to the var which is mapped to a pdo... */
  /*  void *     pMappedAppObject = NULL;   */
  /* pointer fo the var which holds the mapping parameter of an
  mapping entry */
  unsigned long *pMappingParameter = NULL;
  unsigned char *pTransmissionType = NULL;       /* pointer to the transmission
  type */
  unsigned long *pwCobId = NULL;
  unsigned char Size;
  unsigned char offset;
  unsigned char status;
  unsigned long objDict;
  unsigned int offsetObjdict;
  unsigned int lastIndex;
  
  status = state2;
  
  MSG_WAR (0x3935, "proceedPDO, cobID : ", (UNS16_LE(m->cob_id) & 0x7ff));
  offset = 0x00;
  numPdo = 0;
  numMap = 0;
  if ((*m).rtr == NOT_A_REQUEST)
  {                           /* The PDO received is not a
    request. */
    
    offsetObjdict = d->firstIndex->PDO_RCV;
    lastIndex = d->lastIndex->PDO_RCV;
    
    /* study of all the PDO stored in the dictionary */
    if (offsetObjdict)
      while (offsetObjdict <= lastIndex)
      {
        
        switch (status)
        {
          
        case state2:
          /* get CobId of the dictionary correspondant to the received
          PDO */
          pwCobId =
            d->objdict[offsetObjdict].pSubindex[1].pObject;
          /* check the CobId coherance */
          /*pwCobId is the cobId read in the dictionary at the state 3
          */
          if (*pwCobId == UNS16_LE(m->cob_id))
          {
            /* The cobId is recognized */
            status = state4;
            MSG_WAR (0x3936, "cobId found at index ",
                     0x1400 + numPdo);
            break;
          }
          else
          {
            /* cobId received does not match with those written in the
            dictionnary */
            numPdo++;
            offsetObjdict++;
            status = state2;
            break;
          }
          
        case state4:     /* Get Mapped Objects Number */
          /* The cobId of the message received has been found in the
          dictionnary. */
          offsetObjdict = d->firstIndex->PDO_RCV_MAP;
          lastIndex = d->lastIndex->PDO_RCV_MAP;
          pMappingCount =
            (unsigned char *) (d->objdict + offsetObjdict +
                               numPdo)->pSubindex[0].pObject;
          numMap = 0;
          while (numMap < *pMappingCount)
          {
            unsigned char tmp[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
            unsigned long ByteSize;
            pMappingParameter =
              (unsigned long *) (d->objdict + offsetObjdict +
                                 numPdo)->pSubindex[numMap + 1].pObject;
            if (pMappingParameter == NULL)
            {
              MSG_ERR (0x1937, "Couldn't get mapping parameter : ",
                       numMap + 1);
              return 0xFF;
            }
            /* Get the addresse of the mapped variable. */
            /* detail of *pMappingParameter : */
            /* The 16 hight bits contains the index, the medium 8 bits
            contains the subindex, */
            /* and the lower 8 bits contains the size of the mapped
            variable. */
            
            Size = (unsigned char) (*pMappingParameter & (unsigned long) 0x000000FF);
            
            /* set variable only if Size != 0 and 
            * Size is lower than remaining bits in the PDO */
            if (Size && ((offset + Size) <= (m->len << 3)))
            {
              /* copy bit per bit in little endian */
              CopyBits (Size, (unsigned char *) & m->data[offset >> 3],
                        offset % 8, 0, ((unsigned char *) tmp), 0, 0);
              /*1->8 => 1 ; 9->16 =>2, ... */
              ByteSize = (unsigned long)(1 + ((Size - 1) >> 3));
              
              objDict =
                setODentry (d, (unsigned int) ((*pMappingParameter) >> 16),
                            (unsigned char) (((*pMappingParameter) >> 8) &
                                             0xFF), tmp, &ByteSize, 0);
              
              if (objDict != OD_SUCCESSFUL)
              {
                MSG_ERR (0x1938,
                         "error accessing to the mapped var : ",
                         numMap + 1);
                MSG_WAR (0x2939, "         Mapped at index : ",
                         (*pMappingParameter) >> 16);
                MSG_WAR (0x2940, "                subindex : ",
                         ((*pMappingParameter) >> 8) & 0xFF);
                return 0xFF;
              }
              
              MSG_WAR (0x3942,
                       "Variable updated by PDO cobid : ",
                       UNS16_LE(m->cob_id));
              MSG_WAR (0x3943, "         Mapped at index : ",
                       (*pMappingParameter) >> 16);
              MSG_WAR (0x3944, "                subindex : ",
                       ((*pMappingParameter) >> 8) & 0xFF);
              offset += Size;
            }
            numMap++;
          }             /* end loop while on mapped variables */
          
          return 0;
          
        }                 /* end switch status */
      }                     /* end while */
  }                           /* end if ((*m).rtr == NOT_A_REQUEST) */
  else if ((*m).rtr == REQUEST)
  {
    MSG_WAR (0x3946, "Receive a PDO request cobId : ", UNS16_LE(m->cob_id));
    status = state1;
    offsetObjdict = d->firstIndex->PDO_TRS;
    lastIndex = d->lastIndex->PDO_TRS;
    if (offsetObjdict)
      while (offsetObjdict <= lastIndex)
      {
        /* study of all PDO stored in the objects dictionary */
        
        switch (status)
        {
          
        case state1:     /* check the CobId */
          /* get CobId of the dictionary which match to the received PDO
          */
          pwCobId =
            (d->objdict +
             offsetObjdict)->pSubindex[1].pObject;
          if (*pwCobId == UNS16_LE(m->cob_id))
          {
            status = state4;
            break;
          }
          else
          {
            numPdo++;
            offsetObjdict++;
          }
          status = state1;
          break;
          
          
        case state4:     /* check transmission type */
          pTransmissionType =
            (unsigned char *) d->objdict[offsetObjdict].pSubindex[2].pObject;
          /* If PDO is to be sampled and send on RTR, do it */
          if ((*pTransmissionType == TRANS_RTR))
          {
            status = state5;
            break;
          }
          /* RTR_SYNC means data prepared at SYNC, transmitted on RTR */
          else if ((*pTransmissionType == TRANS_RTR_SYNC))
          {
            if (d->PDO_status[numPdo].
                transmit_type_parameter & PDO_RTR_SYNC_READY)
            {
              /*Data ready, just send */
              canSend (&d->PDO_status[numPdo].last_message);
              return 0;
            }
            else
            {
              /* if SYNC did never occur, transmit current data */
              MSG_ERR (0x1947,
                       "Not ready RTR_SYNC TPDO send current data : ",
                       UNS16_LE(m->cob_id));
              status = state5;
            }
            break;
          }
          else if ((*pTransmissionType == TRANS_EVENT_PROFILE) ||
                   (*pTransmissionType == TRANS_EVENT_SPECIFIC))
          {
            /* Zap all timers and inhibit flag */
            d->PDO_status[numPdo].event_timer =
              DelAlarm (d->PDO_status[numPdo].event_timer);
            d->PDO_status[numPdo].inhibit_timer =
              DelAlarm (d->PDO_status[numPdo].inhibit_timer);
            d->PDO_status[numPdo].transmit_type_parameter &=
              ~PDO_INHIBITED;
            /* Call  PDOEventTimerAlarm for this TPDO, 
            * this will trigger emission at reset timers */
            PDOEventTimerAlarm (d, numPdo);
            return 0;
          }
          else
          {
            /* The requested PDO is not to send on request. So, does
            nothing. */
            MSG_WAR (0x2947, "PDO is not to send on request : ",
                     UNS16_LE(m->cob_id));
            return 0xFF;
          }
          
        case state5:     /* build and send requested PDO */
          {
            Message pdo;
            if (buildPDO (d, numPdo, &pdo))
            {
              MSG_ERR (0x1948, " Couldn't build TPDO �%&/(!!!/� shit happens!", numPdo);
              return 0xFF;
            }
            canSend (&pdo);
            return 0;
          }
        }                 /* end switch status */
      }                     /* end while */
  }                           /* end if Requete */
  
  return 0;
}

/*!
**
**
**  NbBits
**  SrcByteIndex
**  SrcBitIndex
**  SrcBigEndian
**  DestByteIndex
**  DestBitIndex
**  DestBigEndian
**/
void CopyBits ( unsigned char NbBits, unsigned char * SrcByteIndex, unsigned char SrcBitIndex,
               unsigned char SrcBigEndian, unsigned char * DestByteIndex, unsigned char DestBitIndex,
               unsigned char DestBigEndian)
{
  /* This loop copy as many bits that it can each time, crossing */
  /* successively bytes */
  // boundaries from LSB to MSB.
  while (NbBits > 0)
  {
    /* Bit missalignement between src and dest */
    INTEGER8 Vect = DestBitIndex - SrcBitIndex;
    
    /* We can now get src and align it to dest */
    unsigned char Aligned =
      Vect > 0 ? *SrcByteIndex << Vect : *SrcByteIndex >> -Vect;
      
      /* Compute the nb of bit we will be able to copy */
      unsigned char BoudaryLimit = (Vect > 0 ? 8 - DestBitIndex : 8 - SrcBitIndex);
      unsigned char BitsToCopy = BoudaryLimit > NbBits ? NbBits : BoudaryLimit;
      
      /* Create a mask that will serve in: */
      unsigned char Mask =
        ((0xff << (DestBitIndex + BitsToCopy)) |
         (0xff >> (8 - DestBitIndex)));
      
      /* - Filtering src */
      unsigned char Filtered = Aligned & ~Mask;
      
      /* - and erase bits where we write, preserve where we don't */
      *DestByteIndex &= Mask;
      
      /* Then write. */
      *DestByteIndex |= Filtered;
      
      /*Compute next time cursors for src */
      if ((SrcBitIndex += BitsToCopy) > 7)      /* cross boundary ? */
      {
        SrcBitIndex = 0;      /* First bit */
        SrcByteIndex += (SrcBigEndian ? -1 : 1);      /* Next byte */
      }
      
      
      /*Compute next time cursors for dest */
      if ((DestBitIndex += BitsToCopy) > 7)
      {
        DestBitIndex = 0;     /* First bit */
        DestByteIndex += (DestBigEndian ? -1 : 1);    /* Next byte */
      }
      
      /*And decrement counter. */
      NbBits -= BitsToCopy;
  }
  
}

static void sendPdo(CO_Data * d, unsigned long pdoNum, Message * pdo)
{
  /*store_as_last_message */
  d->PDO_status[pdoNum].last_message = *pdo;
  MSG_WAR (0x396D, "sendPDO cobId :", UNS16_LE(pdo.cob_id));
  MSG_WAR (0x396E, "     Nb octets  : ", pdo.len);
  
  canSend (pdo);
}


/*!
**
**
** d pointer to node data 
**
** Returns: 
**/

unsigned char sendPDOevent (CO_Data * d)
{
  /* Calls _sendPDOevent specifying it is not a sync event */
  return _sendPDOevent (d, 0);
}

unsigned char sendOnePDOevent (CO_Data * d, unsigned long pdoNum)
{
  if (!d->CurrentCommunicationState.csPDO ||
      !(d->PDO_status[pdoNum].transmit_type_parameter & PDO_INHIBITED))
  {
    return 0;
  }
  
  unsigned int offsetObjdict = d->firstIndex->PDO_TRS + pdoNum;
  MSG_WAR (0x3968, "  PDO is on EVENT. Trans type : ",*pTransmissionType);
  Message pdo;
  memset(&pdo, 0, sizeof(pdo));
  if (buildPDO (d, pdoNum, &pdo))
  {
    MSG_ERR (0x3907, " Couldn't build TPDO number : ", pdoNum);
    return 0;
  }
  
  /*Compare new and old PDO */
  if (d->PDO_status[pdoNum].last_message.cob_id == pdo.cob_id
      && d->PDO_status[pdoNum].last_message.len == pdo.len
        && memcmp(d->PDO_status[pdoNum].last_message.data,
                  pdo.data, 8) == 0
          )
  {
    /* No changes -> go to next pdo */
    return 0;
  }
  else
  {
    
    TIMEVAL EventTimerDuration;
    TIMEVAL InhibitTimerDuration;
    
    MSG_WAR (0x306A, "Changes TPDO number : ", pdoNum);
    /* Changes detected -> transmit message */
    EventTimerDuration =
      *(unsigned int *) d->objdict[offsetObjdict].pSubindex[5].
        pObject;
    InhibitTimerDuration =
      *(unsigned int *) d->objdict[offsetObjdict].pSubindex[3].
        pObject;
    
    /* Start both event_timer and inhibit_timer */
    if (EventTimerDuration)
    {
      DelAlarm (d->PDO_status[pdoNum].event_timer);
      d->PDO_status[pdoNum].event_timer =
        SetAlarm (d, pdoNum, &PDOEventTimerAlarm,
                  MS_TO_TIMEVAL (EventTimerDuration), 0);
    }
    
    if (InhibitTimerDuration)
    {
      DelAlarm (d->PDO_status[pdoNum].inhibit_timer);
      d->PDO_status[pdoNum].inhibit_timer =
        SetAlarm (d, pdoNum, &PDOInhibitTimerAlarm,
                  US_TO_TIMEVAL (InhibitTimerDuration *
                                 100), 0);
      /* and inhibit TPDO */
      d->PDO_status[pdoNum].transmit_type_parameter |=
        PDO_INHIBITED;
    }
    
    sendPdo(d, pdoNum, &pdo);
  }
  return 1;
}

void PDOEventTimerAlarm (CO_Data * d, unsigned long pdoNum)
{
  /* This is needed to avoid deletion of re-attribuated timer */
  d->PDO_status[pdoNum].event_timer = TIMER_NONE;
  /* force emission of PDO by artificially changing last emitted */
  d->PDO_status[pdoNum].last_message.cob_id = 0;
  sendOnePDOevent (d, pdoNum);
}

void PDOInhibitTimerAlarm (CO_Data * d, unsigned long pdoNum)
{
  /* This is needed to avoid deletion of re-attribuated timer */
  d->PDO_status[pdoNum].inhibit_timer = TIMER_NONE;
  /* Remove inhibit flag */
  d->PDO_status[pdoNum].transmit_type_parameter &= ~PDO_INHIBITED;
  sendOnePDOevent (d, pdoNum);
}

/*!
**
**
** d pointer to node data 
**  isSyncEvent
**
** Returns: 
**/

unsigned char _sendPDOevent (CO_Data * d, unsigned char isSyncEvent)
{
  unsigned char pdoNum = 0x00;           /* number of the actual processed pdo-nr. */
  unsigned char *pTransmissionType = NULL;
  unsigned char status = state3;
  unsigned int offsetObjdict = d->firstIndex->PDO_TRS;
  unsigned int offsetObjdictMap = d->firstIndex->PDO_TRS_MAP;
  unsigned int lastIndex = d->lastIndex->PDO_TRS;
  
  if (!d->CurrentCommunicationState.csPDO)
  {
    return 0;
  }
  
  
  /* study all PDO stored in the objects dictionary */
  if (offsetObjdict)
  {
    Message pdo;/* = Message_Initializer;*/
    memset(&pdo, 0, sizeof(pdo));
    while (offsetObjdict <= lastIndex)
    {
      switch (status)
      {
      case state3:
        if ( /* bSubCount always 5 with objdictedit -> check disabled */
            /*d->objdict[offsetObjdict].bSubCount < 5 ||*/
            /* check if TPDO is not valid */
            *(unsigned long *) d->objdict[offsetObjdict].pSubindex[1].pObject & 0x80000000)
        {
          MSG_WAR (0x3960, "Not a valid PDO ", 0x1800 + pdoNum);
          /*Go next TPDO */
          status = state11;
          break;
        }
        /* get the PDO transmission type */
        pTransmissionType =
          (unsigned char *) d->objdict[offsetObjdict].pSubindex[2].pObject;
        MSG_WAR (0x3962, "Reading PDO at index : ", 0x1800 + pdoNum);
        
        /* check if transmission type is SYNCRONOUS */
        /* message transmited every n SYNC with n=TransmissionType */
        if (isSyncEvent &&
            (*pTransmissionType >= TRANS_SYNC_MIN) &&
              (*pTransmissionType <= TRANS_SYNC_MAX) &&
                (++d->PDO_status[pdoNum].transmit_type_parameter ==*pTransmissionType))
        {
          /*Reset count of SYNC */
          d->PDO_status[pdoNum].transmit_type_parameter = 0;
          MSG_WAR (0x3964, "  PDO is on SYNCHRO. Trans type : ",*pTransmissionType);
          memset(&pdo, 0, sizeof(pdo));
          /*{
          Message msg_init = Message_Initializer;
          pdo = msg_init;
        }*/
          if (buildPDO (d, pdoNum, &pdo))
          {
            MSG_ERR (0x1906, " Couldn't build TPDO number : ",pdoNum);
            status = state11;
            break;
          }
          status = state5;
          /* If transmission RTR, with data sampled on SYNC */
        }
        else if (isSyncEvent && (*pTransmissionType == TRANS_RTR_SYNC))
        {
          if (buildPDO(d, pdoNum, &d->PDO_status[pdoNum].last_message))
          {
            MSG_ERR (0x1966, " Couldn't build TPDO number : ",pdoNum);
            d->PDO_status[pdoNum].transmit_type_parameter &=~PDO_RTR_SYNC_READY;
          }
          else
          {
            d->PDO_status[pdoNum].transmit_type_parameter |=PDO_RTR_SYNC_READY;
          }
          status = state11;
          break;
          /* If transmission on Event and not inhibited, check for changes */
        }
        else
          if ( (isSyncEvent && (*pTransmissionType == TRANS_SYNC_ACYCLIC))||
              (!isSyncEvent && (*pTransmissionType == TRANS_EVENT_PROFILE || 
                                *pTransmissionType == TRANS_EVENT_SPECIFIC)&&
               !(d->PDO_status[pdoNum].transmit_type_parameter & PDO_INHIBITED)))
          {
            sendOnePDOevent(d, pdoNum);
            status = state11;
          }
          else
          {
            MSG_WAR (0x306C,
                     "  PDO is not on EVENT or synchro or not at this SYNC. Trans type : ",*pTransmissionType);
            status = state11;
          }
        break;
      case state5:       /*Send the pdo */
        sendPdo(d, pdoNum, &pdo);
        status = state11;
        break;
      case state11:      /*Go to next TPDO */
        pdoNum++;
        offsetObjdict++;
        offsetObjdictMap++;
        MSG_WAR (0x3970, "next pdo index : ", pdoNum);
        status = state3;
        break;
        
      default:
        MSG_ERR (0x1972, "Unknown state has been reached : %d", status);
        return 0xFF;
      }                   /* end switch case */
      
    }                       /* end while */
  }
  return 0;
}

/*!
**
**
** d pointer to node data 
**  OD_entry
**  bSubindex
** Returns:  always 0
**/

unsigned long TPDO_Communication_Parameter_Callback (CO_Data * d,const indextable * OD_entry,unsigned char bSubindex)
{
  /* If PDO are actives */
  if (d->CurrentCommunicationState.csPDO)
    switch (bSubindex)
    {
    case 2:                  /* Changed transmition type */
    case 3:                  /* Changed inhibit time */
    case 5:                  /* Changed event time */
      {
        const indextable *TPDO_com = d->objdict + d->firstIndex->PDO_TRS;
        unsigned char numPdo = OD_entry - TPDO_com;    /* number of the actual processed pdo-nr. */
        
        /* Zap all timers and inhibit flag */
        d->PDO_status[numPdo].event_timer =
          DelAlarm (d->PDO_status[numPdo].event_timer);
        d->PDO_status[numPdo].inhibit_timer =
          DelAlarm (d->PDO_status[numPdo].inhibit_timer);
        d->PDO_status[numPdo].transmit_type_parameter = 0;
        /* Call  PDOEventTimerAlarm for this TPDO, this will trigger emission et reset timers */
        PDOEventTimerAlarm (d, numPdo);
        return 0;
      }
      
    default:                 /* other subindex are ignored */
      break;
    }
  return 0;
}

void PDOInit (CO_Data * d)
{
  /* For each TPDO mapping parameters */
  unsigned int pdoIndex = 0x1800;      /* OD index of TDPO */
  unsigned int offsetObjdict = d->firstIndex->PDO_TRS;
  unsigned int lastIndex = d->lastIndex->PDO_TRS;
  if (offsetObjdict)
    while (offsetObjdict <= lastIndex)
    {
      /* Assign callbacks to sensible TPDO mapping subindexes */
      unsigned long errorCode;
      ODCallback_t *CallbackList;
      /* Find callback list */
      scanIndexOD (d, pdoIndex, &errorCode, &CallbackList);
      if (errorCode == OD_SUCCESSFUL && CallbackList)
      {
        /*Assign callbacks to corresponding subindex */
        /* Transmission type */
        CallbackList[2] = &TPDO_Communication_Parameter_Callback;
        /* Inhibit time */
        CallbackList[3] = &TPDO_Communication_Parameter_Callback;
        /* Event timer */
        CallbackList[5] = &TPDO_Communication_Parameter_Callback;
      }
      pdoIndex++;
      offsetObjdict++;
    }
  
  /* Trigger a non-sync event */
  _sendPDOevent (d, 0);
}

void PDOStop (CO_Data * d)
{
  /* For each TPDO mapping parameters */
  unsigned char pdoNum = 0x00;           /* number of the actual processed pdo-nr. */
  unsigned int offsetObjdict = d->firstIndex->PDO_TRS;
  unsigned int lastIndex = d->lastIndex->PDO_TRS;
  if (offsetObjdict)
    while (offsetObjdict <= lastIndex)
    {
      /* Delete TPDO timers */
      d->PDO_status[pdoNum].event_timer =
        DelAlarm (d->PDO_status[pdoNum].event_timer);
      d->PDO_status[pdoNum].inhibit_timer =
        DelAlarm (d->PDO_status[pdoNum].inhibit_timer);
      /* Reset transmit type parameter */
      d->PDO_status[pdoNum].transmit_type_parameter = 0;
      d->PDO_status[pdoNum].last_message.cob_id = 0;
      pdoNum++;
      offsetObjdict++;
    }
}
