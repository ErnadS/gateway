/*
This file is part of the NCAN bus, a library implementing CanOpen Light Stack.

Copyright (c) Elreg as 2010


*/

#ifndef __objdictdef_h__
#define __objdictdef_h__

/************************* CONSTANTS **********************************/
/** this are static defined datatypes taken fCODE the NCANbus specification. They
 *  are located at index 0x0001 to 0x001B. As described in the specification, they
 *  are in the object dictionary for definition purpose only. a device does not
 *  to support all of this datatypes.
 */
#define boolean         0x01
#define int8            0x02
#define int16           0x03
#define int32           0x04
#define uint8           0x05
#define uint16          0x06
#define uint32          0x07
#define real32          0x08
#define visible_string  0x09
#define octet_string    0x0A
#define unicode_string  0x0B
#define time_of_day     0x0C
#define time_difference 0x0D

#define domain          0x0F
#define int24           0x10
#define real64          0x11
#define int40           0x12
#define int48           0x13
#define int56           0x14
#define int64           0x15
#define uint24          0x16

#define uint40          0x18
#define uint48          0x19
#define uint56          0x1A
#define uint64          0x1B

#define pdo_communication_parameter 0x20
#define pdo_mapping                 0x21
#define sdo_parameter               0x22
#define identity                    0x23

/* NCANbus is using 0x24 to 0xFF to define some types containing a 
 value range (See how it works in objdict.c)
 */


/** Each entry of the object dictionary can be READONLY (RO), READ/WRITE (RW),
 *  WRITE-ONLY (WO)
 *  In addition, parameters which should be stored to eeprom have the flag TO_BE_SAVE set
 */
#define RW     0x00  
#define WO     0x01
#define RO     0x02

#define TO_BE_SAVE  0x04 //
#define DCF_TO_SEND 0x08

/************************ STRUCTURES ****************************/
/** This are some structs which are neccessary for creating the entries
 *  of the object dictionary.
 */
//typedef struct td_subindex
//{
//    unsigned char    bAccessType;
//    unsigned char    bDataType; /* Defines of what datatype the entry is */
//    unsigned long    size;      /* The size (in Byte) of the variable */
//    void*            pObject;   /* This is the pointer of the Variable */
//} subindex;

typedef struct td_subindex
{
   unsigned char      bAccessType;
   unsigned char      bDataType; /* Defines of what datatype the entry is */
   unsigned long      size;      /* The size (in Byte) of the variable */
   void *             pObject;   /* This is the pointer of the Variable */
  // void __eeprom *    pObjectEEPROM;/* This is the pointer of the Variable in eeprom */
//   void *  pObjectEEPROM;/* This is the pointer of the Variable in eeprom */
} subindex;

/** Struct for creating entries in the communictaion profile
 */
typedef struct td_indextable
{
	//__flash*
    subindex*    pSubindex;   /* Pointer to the subindex */
    unsigned char   bSubCount;   /* the count of valid entries for this subindex
                         * This count here defines how many memory has been
                         * allocated. this memory does not have to be used.
                         */
    unsigned int   index;
} indextable;

typedef struct s_quick_index{
	unsigned int SDO_SVR;
	unsigned int SDO_CLT;
	unsigned int PDO_RCV;
	unsigned int PDO_RCV_MAP;
	unsigned int PDO_TRS;
	unsigned int PDO_TRS_MAP;
}quick_index;


/*typedef struct struct_CO_Data CO_Data; */
typedef unsigned long (*ODCallback_t)(CO_Data* d, const indextable *, unsigned char bSubindex);
typedef const indextable* (*scanIndexOD_t)(unsigned int wIndex, unsigned long * errorCode, ODCallback_t **Callback);
//typedef indextable const __flash* (*scanIndexOD_t)(unsigned int wIndex, unsigned long * errorCode, ODCallback_t **Callback);
/************************** MACROS *********************************/

/* NCANbus usefull helpers */
#define GET_NODE_ID(m)         (UNS16_LE(m.cob_id) & 0x7f)
#define GET_FUNCTION_CODE(m)   (UNS16_LE(m.cob_id) >> 7)

 
#endif /* __objdictdef_h__ */
