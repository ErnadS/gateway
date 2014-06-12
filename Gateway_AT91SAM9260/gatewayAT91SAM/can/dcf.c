/*
This file is part of the NCANbus, a library implementing an light version of the Canopen standard.

Copyright (c) Elreg as 2010


*/

#include "data.h"
#include "sysdep.h"

extern unsigned char _writeNetworkDict (CO_Data* d, unsigned char nodeId, unsigned int index,
                                        unsigned char subIndex, unsigned char count, unsigned char dataType, void *data, SDOCallback_t Callback, unsigned char endianize);


static void send_consise_dcf_loop(CO_Data* d,unsigned char nodeId);

/* Seek to next NodeID's DCF */
#define SEEK_NEXT_DCF() \
  nodeId=(nodeId+1) % d->dcf_odentry->bSubCount; \
    if(nodeId==0) nodeId=1; \
      d->dcf_cursor = NULL;

/**
**
**
** @param d
** @param nodeId
*/
static void CheckSDOAndContinue(CO_Data* d, unsigned char nodeId)
{
  unsigned long abortCode = 0;
  
  if(getWriteResultNetworkDict (d, nodeId, &abortCode) != SDO_FINISHED)
  {
    MSG_ERR(0x1A01, "SDO error in consise DCF", abortCode);
    MSG_WAR(0x2A02, "server node : ", nodeId);
  }
  
  closeSDOtransfer(d, nodeId, SDO_CLIENT);
  /* Timedout ? */
  if(abortCode == SDOABT_TIMED_OUT){
    /* Node may not be ready, try another one */
    /* Warning, this might leed to endless attempts */
    /* if node does never answer */
    SEEK_NEXT_DCF()
  }
  send_consise_dcf_loop(d,nodeId);
}


/**
**
**
** @param d
** @param nodeId
**
** @return
*/
unsigned char send_consise_dcf(CO_Data* d,unsigned char nodeId)
{
  unsigned long szData;
  /* Fetch DCF OD entry, if not already done */
  printf("1\n");
  if(!d->dcf_odentry)
  {
	  printf("2\n");
    unsigned long errorCode;
    ODCallback_t *Callback;
    d->dcf_odentry = (*d->scanIndexOD)(0x1F22, &errorCode, &Callback);
    /* If DCF entry do not exist... Nothing to do.*/
    if (errorCode != OD_SUCCESSFUL) goto DCF_finish;
  }
  printf("3\n");
  szData = d->dcf_odentry->pSubindex[nodeId].size;
  
  /* if the entry for the nodeId is not empty. */
  if(szData!=0){
	  printf("4\n");
    /* if the entry for the nodeId is already been processing, quit.*/
    if(d->dcf_odentry->pSubindex[nodeId].bAccessType & DCF_TO_SEND) return 1;
    
    d->dcf_odentry->pSubindex[nodeId].bAccessType|=DCF_TO_SEND;
    d->dcf_request++;
    if(d->dcf_request==1) {
    	printf("5\n");
      send_consise_dcf_loop(d,nodeId);
    }
    printf("6\n");
    return 1;
  }
  
DCF_finish:
  return 0;
}

static void send_consise_dcf_loop(CO_Data* d,unsigned char nodeId)
{
  if(nodeId > d->dcf_odentry->bSubCount) return;
  /* Loop on all DCF subindexes, corresponding to node ID until there is no request*/
  //while (nodeId < d->dcf_odentry->bSubCount){
  while (d->dcf_request>0){
    if(d->dcf_odentry->pSubindex[nodeId].bAccessType & DCF_TO_SEND){   	 
      unsigned char* dcfend;
      unsigned long nb_entries;
      unsigned long szData = d->dcf_odentry->pSubindex[nodeId].size;
      printf("8\n");
      {
        unsigned char* dcf = *((unsigned char**)d->dcf_odentry->pSubindex[nodeId].pObject);
        dcfend = dcf + szData;
        if (!d->dcf_cursor){
        	printf("9\n");
          d->dcf_cursor = (unsigned char*)dcf + 4;
          d->dcf_entries_count = 0;
        }
        nb_entries = UNS32_LE(*((unsigned long*)dcf));
      }
      printf("10\n");
      /* condition on consise DCF string for NodeID, if big enough */
      if((unsigned char*)d->dcf_cursor + 7 < (unsigned char*)dcfend && d->dcf_entries_count < nb_entries){
    	  printf("11\n");
        unsigned int target_Index;
        unsigned char target_Subindex;
        unsigned long target_Size;
        
        /* DCF data may not be 32/16b aligned, 
        * we cannot directly dereference d->dcf_cursor 
        * as unsigned int or unsigned long 
        * Do it byte per byte taking care on endianess*/
#ifdef CANOPEN_BIG_ENDIAN
        target_Index = *(d->dcf_cursor++) << 8 | 
          *(d->dcf_cursor++);
#else
        memcpy(&target_Index, d->dcf_cursor,2);
        d->dcf_cursor+=2;
#endif
        
        target_Subindex = *(d->dcf_cursor++);
        
#ifdef CANOPEN_BIG_ENDIAN
        target_Size = *(d->dcf_cursor++) << 24 | 
          *(d->dcf_cursor++) << 16 | 
            *(d->dcf_cursor++) << 8 | 
              *(d->dcf_cursor++);
#else
        memcpy(&target_Size, d->dcf_cursor,4);
        d->dcf_cursor+=4;
#endif
        printf("12\n");
        _writeNetworkDict(d, /* CO_Data* d*/
                          nodeId, /* unsigned char nodeId*/
                          target_Index, /* unsigned int index*/
                          target_Subindex, /* unsigned char subindex*/
                          (unsigned char)target_Size, /* unsigned char count*/
                          0, /* unsigned char dataType*/
                          d->dcf_cursor,/* void *data*/
                          CheckSDOAndContinue,/* SDOCallback_t
                          Callback*/
                          0); /* no endianize*/
        /* Push d->dcf_cursor to the end of data*/
        printf("13\n");
        d->dcf_cursor += target_Size;
        d->dcf_entries_count++;
        
        /* send_consise_dcf_loop will be called by CheckSDOAndContinue for next DCF entry*/
        return;
      }
      else
      {
        /* We have finished with the dcf entry. Change the flag, decrement the request
        *  and execute the bootup callback. */
        d->dcf_odentry->pSubindex[nodeId].bAccessType&=~DCF_TO_SEND;
        d->dcf_request--;
        (*d->post_SlaveBootup)(d, nodeId);
      }
    }
    
    SEEK_NEXT_DCF()
  }
  
}
