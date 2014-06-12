/*
This file is part of the NCANbus, a library implementing an light version of the Canopen standard.

Copyright (c) Elreg as 2010


*/


/* #define DEBUG_WAR_CONSOLE_ON */
/* #define DEBUG_ERR_CONSOLE_ON */


#include "data.h"

#ifdef DEBUG_WAR_CONSOLE_ON
unsigned char accessDictionaryError(unsigned int index, unsigned char subIndex,
                                    unsigned long sizeDataDict, unsigned long sizeDataGiven, unsigned long code)
{
  MSG_WAR(0x2B09,"Dictionary index : ", index);
  MSG_WAR(0X2B10,"           subindex : ", subIndex);
  switch (code) {
  case  OD_NO_SUCH_OBJECT:
    MSG_WAR(0x2B11,"Index not found ", index);
    break;
  case OD_NO_SUCH_SUBINDEX :
    MSG_WAR(0x2B12,"SubIndex not found ", subIndex);
    break;
  case OD_WRITE_NOT_ALLOWED :
    MSG_WAR(0x2B13,"Write not allowed, data is read only ", index);
    break;
  case OD_LENGTH_DATA_INVALID :
    MSG_WAR(0x2B14,"Conflict size data. Should be (bytes)  : ", sizeDataDict);
    MSG_WAR(0x2B15,"But you have given the size  : ", sizeDataGiven);
    break;
  case OD_NOT_MAPPABLE :
    MSG_WAR(0x2B16,"Not mappable data in a PDO at index    : ", index);
    break;
  case OD_VALUE_TOO_LOW :
    MSG_WAR(0x2B17,"Value range error : value too low. SDOabort : ", code);
    break;
  case OD_VALUE_TOO_HIGH :
    MSG_WAR(0x2B18,"Value range error : value too high. SDOabort : ", code);
    break;
  default :
    MSG_WAR(0x2B20, "Unknown error code : ", code);
  }
  return 0;
}
#else
#define accessDictionaryError(index, subIndex, sizeDataDict, sizeDataGiven, code)
#endif

unsigned long _getODentry( CO_Data* d,
                          unsigned int wIndex,
                          unsigned char bSubindex,
                          void * pDestData,
                          unsigned long * pExpectedSize,
                          unsigned char * pDataType,
                          unsigned char checkAccess,
                          unsigned char endianize)
{ 
  
  unsigned long errorCode;
  unsigned long szData;
  const indextable *ptrTable;
  ODCallback_t *Callback;
  
  ptrTable = (*d->scanIndexOD)(wIndex, &errorCode, &Callback);
  
  if (errorCode != OD_SUCCESSFUL)
    return errorCode;
  if( ptrTable->bSubCount <= bSubindex ) {
    /* Subindex not found */
    accessDictionaryError(wIndex, bSubindex, 0, 0, OD_NO_SUCH_SUBINDEX);
    return OD_NO_SUCH_SUBINDEX;
  }
  
  if (checkAccess && (ptrTable->pSubindex[bSubindex].bAccessType & WO)) {
    //printf("_getODentry Access Type : %u\n", ptrTable->pSubindex[bSubindex].bAccessType);
    accessDictionaryError(wIndex, bSubindex, 0, 0, OD_READ_NOT_ALLOWED);
    return OD_READ_NOT_ALLOWED;
  }
  
  *pDataType = ptrTable->pSubindex[bSubindex].bDataType;
  szData = ptrTable->pSubindex[bSubindex].size;
  
  if(*pExpectedSize == 0 ||
     *pExpectedSize == szData ||
       /* allow to fetch a shorter string than expected */
       (*pDataType >= visible_string && *pExpectedSize < szData)) { 
         
#  ifdef CANOPEN_BIG_ENDIAN
         if(endianize && *pDataType > boolean && !(
                                                   *pDataType >= visible_string && 
                                                     *pDataType <= domain)) {
                                                       /* data must be transmited with low byte first */
                                                       unsigned char i, j = 0;
                                                       MSG_WAR(boolean, "data type ", *pDataType);
                                                       MSG_WAR(visible_string, "data type ", *pDataType);
                                                       for ( i = szData ; i > 0 ; i--) {
                                                         MSG_WAR(i," ", j);
                                                         ((unsigned char*)pDestData)[j++] =
                                                           ((unsigned char*)ptrTable->pSubindex[bSubindex].pObject)[i-1];
                                                       }
                                                       *pExpectedSize = szData;
                                                     }
         else /* no endianisation change */
#  endif
           if(*pDataType != visible_string) {
             memcpy(pDestData, ptrTable->pSubindex[bSubindex].pObject,szData);
             *pExpectedSize = szData;
           }else{
             /* TODO : CONFORM TO CAN : 
             *  - stop using NULL terminated strings
             *  - store string size in td_subindex 
             * */
             /* Copy null terminated string to user, and return discovered size */
             unsigned char *ptr = (unsigned char*)ptrTable->pSubindex[bSubindex].pObject;
             unsigned char *ptr_start = ptr;
             /* *pExpectedSize IS < szData . if null, use szData */
             unsigned char *ptr_end = ptr + (*pExpectedSize ? *pExpectedSize : szData) ; 
             unsigned char *ptr_dest = (unsigned char*)pDestData;
             while( *ptr && ptr < ptr_end){
               *(ptr_dest++) = *(ptr++);
             } 
             
             *pExpectedSize = ptr - ptr_start;
             /* terminate string if not maximum length */
             if (*pExpectedSize < szData) 
               *(ptr) = 0; 
           }
         //printf("_getODentry TYPE: %lu, data: %lu\n", (*((long*)(ptrTable->pSubindex[bSubindex].pObject))), *((long*)pDestData));
         return OD_SUCCESSFUL;
       }
  else { /* Error ! */
    *pExpectedSize = szData;
    accessDictionaryError(wIndex, bSubindex, szData,
                          *pExpectedSize, OD_LENGTH_DATA_INVALID);
    return OD_LENGTH_DATA_INVALID;
  }
}

unsigned long _setODentry( CO_Data* d,
                          unsigned int wIndex,
                          unsigned char bSubindex,
                          void * pSourceData,
                          unsigned long * pExpectedSize,
                          unsigned char checkAccess,
                          unsigned char endianize)
{
  unsigned long szData;
  unsigned char dataType;
  unsigned long errorCode;
  const indextable *ptrTable;
  ODCallback_t *Callback;

  ptrTable =(*d->scanIndexOD)(wIndex, &errorCode, &Callback);
  if (errorCode != OD_SUCCESSFUL)
    return errorCode;
  
  if( ptrTable->bSubCount <= bSubindex ) {
    /* Subindex not found */
    accessDictionaryError(wIndex, bSubindex, 0, *pExpectedSize, OD_NO_SUCH_SUBINDEX);
    return OD_NO_SUCH_SUBINDEX;
  }
  if (checkAccess && (ptrTable->pSubindex[bSubindex].bAccessType == RO)) {
    accessDictionaryError(wIndex, bSubindex, 0, *pExpectedSize, OD_WRITE_NOT_ALLOWED);
    return OD_WRITE_NOT_ALLOWED;
  }
  
  dataType = ptrTable->pSubindex[bSubindex].bDataType;
  szData = ptrTable->pSubindex[bSubindex].size;
  
  if( *pExpectedSize == 0 ||
     *pExpectedSize == szData ||
       /* allow to store a shorter string than entry size */
       (dataType == visible_string && *pExpectedSize < szData))
  {
#ifdef CANOPEN_BIG_ENDIAN
    /* re-endianize do not occur for bool, strings time and domains */
    if(endianize && dataType > boolean && !(
                                            dataType >= visible_string && 
                                              dataType <= domain))
    {
      /* we invert the data source directly. This let us do range
      testing without */
      /* additional temp variable */
      unsigned char i;
      for ( i = 0 ; i < ( ptrTable->pSubindex[bSubindex].size >> 1)  ; i++)
      {
        unsigned char tmp =((unsigned char *)pSourceData) [(ptrTable->pSubindex[bSubindex].size - 1) - i];
        ((unsigned char *)pSourceData) [(ptrTable->pSubindex[bSubindex].size - 1) - i] = ((unsigned char *)pSourceData)[i];
        ((unsigned char *)pSourceData)[i] = tmp;
      }
    }
#endif
    errorCode = (*d->valueRangeTest)(dataType, pSourceData);
    if (errorCode) {
      accessDictionaryError(wIndex, bSubindex, szData, *pExpectedSize, errorCode);
      return errorCode;
    }
    memcpy(ptrTable->pSubindex[bSubindex].pObject,pSourceData, *pExpectedSize);
    /* TODO : CONFORM TO CAN, DeviceNet, and Profibus : 
    *  - stop using NULL terminated strings
    *  - store string size in td_subindex 
    * */
    /* terminate visible_string with '\0' */
    if(dataType == visible_string && *pExpectedSize < szData)
      ((unsigned char*)ptrTable->pSubindex[bSubindex].pObject)[*pExpectedSize] = 0;
    
    *pExpectedSize = szData;
    
    /* Callbacks */
    if(Callback && Callback[bSubindex]){
      errorCode = (Callback[bSubindex])(d, ptrTable, bSubindex);
      if(errorCode != OD_SUCCESSFUL)
      {
        return errorCode;
      }
    }
    
    /* TODO : Store dans EEPROM */
    if (ptrTable->pSubindex[bSubindex].bAccessType & TO_BE_SAVE){
      (*d->storeODSubIndex)(d, wIndex, bSubindex);
    }
    return OD_SUCCESSFUL;
  }else{
    *pExpectedSize = szData;
    accessDictionaryError(wIndex, bSubindex, szData, *pExpectedSize, OD_LENGTH_DATA_INVALID);
    return OD_LENGTH_DATA_INVALID;
  }
}




const indextable * scanIndexOD (CO_Data* d, unsigned int wIndex, unsigned long *errorCode, ODCallback_t **Callback)
{
  return (*d->scanIndexOD)(wIndex, errorCode, Callback);
}




unsigned long RegisterSetODentryCallBack(CO_Data* d, unsigned int wIndex, unsigned char bSubindex, ODCallback_t Callback)
{
  unsigned long errorCode;
  ODCallback_t *CallbackList;
  const indextable *odentry;
  
  odentry = scanIndexOD (d, wIndex, &errorCode, &CallbackList);
  if(errorCode == OD_SUCCESSFUL  &&  CallbackList  &&  bSubindex < odentry->bSubCount) 
    CallbackList[bSubindex] = Callback;
  return errorCode;
}

void _storeODSubIndex (CO_Data* d, unsigned int wIndex, unsigned char bSubindex){}
