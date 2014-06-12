/*
This file is part of the NCAN bus, a library implementing CanOpen Light Stack.

Copyright (c) Elreg as 2010

This file contains functions for accessing the object dictionary and
variables that are contained by the object dictionary.
Accessing the object dictionary contains setting local variables
as PDOs and accessing (read/write) all entries of the object dictionary
Warning: Only the basic entries of an object dictionary are included
at the moment.
The Object Dictionary is the heart of each NCANbus device containing all communication and application objects.

*/
 
#ifndef __objacces_h__
#define __objacces_h__

#include "applicfg.h"


#ifdef __cplusplus
extern "C" {
#endif


typedef unsigned long (*valueRangeTest_t)(unsigned char typeValue, void *Value);
typedef void (* storeODSubIndex_t)(CO_Data* d, unsigned int wIndex, unsigned char bSubindex);
void _storeODSubIndex (CO_Data* d, unsigned int wIndex, unsigned char bSubindex);

/**
 * Print MSG_WAR (s) if error to the access to the object dictionary occurs.
 * 
 * You must uncomment the lines in the file objaccess.c :
 * //#define DEBUG_CAN
 * //#define DEBUG_WAR_CONSOLE_ON
 * //#define DEBUG_ERR_CONSOLE_ON
 * @param index
 * @param subIndex
 * @param sizeDataDict Size of the data defined in the dictionary
 * @param sizeDataGiven Size data given by the user.
 * @param code error code to print. (SDO abort code. See file def.h)
 * @return
 */ 
unsigned char accessDictionaryError(unsigned int index, unsigned char subIndex, 
			   unsigned long sizeDataDict, unsigned long sizeDataGiven, unsigned long code);


/* _getODentry() Reads an entry from the object dictionary.\n
 * 
 *    use getODentry() macro to read from object and endianize
 *    use readLocalDict() macro to read from object and not endianize   
 *
 * 
 * // Example:
 * unsigned char  *pbData;
 * unsigned char length;
 * unsigned long returnValue;
 *
 * returnValue = getODentry( (unsigned int)0x100B, (unsigned char)1, 
 * (void * *)&pbData, (unsigned char *)&length );
 * if( returnValue != SUCCESSFUL )
 * {
 *     // error handling
 * }
 *  
 * *d Pointer to a CAN object data structure
 * wIndex The index in the object dictionary where you want to read
 *               an entry
 * bSubindex The subindex of the Index. e.g. mostly subindex 0 is
 *                  used to tell you how many valid entries you can find
 *                  in this index. Look at the NCANbus specification for further
 *                  information
 * *pDestData Pointer to the pointer which points to the variable where
 *                   the value of this object dictionary entry should be copied
 * pExpectedSize This function writes the size of the copied value (in Byte)
 *                      into this variable.
 * *pDataType Pointer to the type of the data. See objdictdef.h
 * CheckAccess if other than 0, do not read if the data is Write Only
 *                    [Not used today. Put always 0].
 * Endianize  When not 0, data is endianized into network byte order
 *                   when 0, data is not endianized and copied in machine native
 *                   endianness 
 * Returns: 
 * - OD_SUCCESSFUL is returned upon success. 
 * - SDO abort code is returned if error occurs . (See file def.h)
 */
unsigned long _getODentry( CO_Data* d,
                   unsigned int wIndex,
                   unsigned char bSubindex,
                   void * pDestData,
                   unsigned long * pExpectedSize,
                   unsigned char * pDataType,
                   unsigned char checkAccess,
                   unsigned char endianize);

/** 
 * getODentry() to read from object and endianize
 * OD Pointer to a CAN object data structure
 * wIndex The index in the object dictionary where you want to read
 *                an entry
 * bSubindex The subindex of the Index. e.g. mostly subindex 0 is
 *                  used to tell you how many valid entries you can find
 *                  in this index. Look at the NCANbus specification for further
 *                  information
 * *pDestData Pointer to the pointer which points to the variable where
 *                   the value of this object dictionary entry should be copied
 * pExpectedSize This function writes the size of the copied value (in Byte)
 *                      into this variable.
 * *pDataType Pointer to the type of the data. See objdictdef.h
 * checkAccess Flag that indicate if a check rights must be perfomed (0 : no , other than 0 : yes)
 * endianize  Set to 1 : endianized into network byte order 
 * Returns: 
 * - OD_SUCCESSFUL is returned upon success. 
 * - SDO abort code is returned if error occurs . (See file def.h)
 */
#define getODentry( OD, wIndex, bSubindex, pDestData, pExpectedSize, \
		          pDataType,  checkAccess)                         \
       _getODentry( OD, wIndex, bSubindex, pDestData, pExpectedSize, \
		          pDataType,  checkAccess, 1)            

/** 
 * readLocalDict() reads an entry from the object dictionary, but in 
 * contrast to getODentry(), readLocalDict() doesn't endianize entry and reads
 * entry in machine native endianness.
 *
 * OD Pointer to a CAN object data structure
 * wIndex The index in the object dictionary where you want to read
 *                an entry
 * bSubindex The subindex of the Index. e.g. mostly subindex 0 is
 *                  used to tell you how many valid entries you can find
 *                  in this index. Look at the NCANbus specification for further
 *                  information
 * *pDestData Pointer to the pointer which points to the variable where
 *                   the value of this object dictionary entry should be copied
 * pExpectedSize This function writes the size of the copied value (in Byte)
 *                      into this variable.
 * *pDataType Pointer to the type of the data. See objdictdef.h
 * checkAccess if other than 0, do not read if the data is Write Only
 *                    [Not used today. Put always 0].
 * endianize Set to 0, data is not endianized and copied in machine native
 *                  endianness 
 * Returns: 
 * - OD_SUCCESSFUL is returned upon success. 
 * - SDO abort code is returned if error occurs . (See file def.h)
 */
#define readLocalDict( OD, wIndex, bSubindex, pDestData, pExpectedSize, \
		          pDataType,  checkAccess)                         \
       _getODentry( OD, wIndex, bSubindex, pDestData, pExpectedSize, \
		          pDataType,  checkAccess, 0)

/* By this function you can write an entry into the object dictionary
 *
 * *d Pointer to a CAN object data structure
 * wIndex The index in the object dictionary where you want to write
 *               an entry
 * bSubindex The subindex of the Index. e.g. mostly subindex 0 is
 *                  used to tell you how many valid entries you can find
 *                  in this index. Look at the NCANbus specification for further
 *                  information
 * *pSourceData Pointer to the variable that holds the value that should
 *                     be copied into the object dictionary
 * *pExpectedSize The size of the value (in Byte).
 * checkAccess Flag that indicate if a check rights must be perfomed (0 : no , other than 0 : yes)
 * endianize When not 0, data is endianized into network byte order
 *                  when 0, data is not endianized and copied in machine native
 *                  endianness   
 * Returns: 
 * - OD_SUCCESSFUL is returned upon success. 
 * - SDO abort code is returned if error occurs . (See file def.h)
 */
unsigned long _setODentry( CO_Data* d,
                   unsigned int wIndex,
                   unsigned char bSubindex,
                   void * pSourceData,
                   unsigned long * pExpectedSize,
                   unsigned char checkAccess,
                   unsigned char endianize);

/**
 * setODentry converts SourceData from network byte order to machine native 
 * format, and writes that to OD.
 * 
 * // Example:
 * unsigned char B;
 * B = 0xFF; // set transmission type
 *
 * retcode = setODentry( (unsigned int)0x1800, (unsigned char)2, &B, sizeof(unsigned char), 1 );
 * 
 * d Pointer to a CAN object data structure
 * wIndex The index in the object dictionary where you want to write
 *               an entry
 * bSubindex The subindex of the Index. e.g. mostly subindex 0 is
 *                  used to tell you how many valid entries you can find
 *                  in this index. Look at the NCANbus specification for further
 *                  information
 * *pSourceData Pointer to the variable that holds the value that should
 *                     be copied into the object dictionary
 * *pExpectedSize The size of the value (in Byte).
 * checkAccess Flag that indicate if a check rights must be perfomed (0 : no , other than 0 : yes)
 * endianize Set to 1 : endianized into network byte order
 * Returns: 
 * - OD_SUCCESSFUL is returned upon success. 
 * - SDO abort code is returned if error occurs . (See file def.h)
 */
#define setODentry( d, wIndex, bSubindex, pSourceData, pExpectedSize, \
                  checkAccess) \
       _setODentry( d, wIndex, bSubindex, pSourceData, pExpectedSize, \
                  checkAccess, 1)

/** unsigned long writeLocalDict(d, wIndex, bSubindex, pSourceData, pExpectedSize, checkAccess)
 * @hideinitializer
 * Writes machine native SourceData to OD.
 *
 * d Pointer to a CAN object data structure
 * wIndex The index in the object dictionary where you want to write
 *               an entry
 * bSubindex The subindex of the Index. e.g. mostly subindex 0 is
 *                  used to tell you how many valid entries you can find
 *                  in this index. Look at the NCANbus specification for further
 *                  information
 * *pSourceData Pointer to the variable that holds the value that should
 *                     be copied into the object dictionary
 * *pExpectedSize The size of the value (in Byte).
 * checkAccess Flag that indicate if a check rights must be perfomed (0 : no , other than 0 : yes)
 * endianize Data is not endianized and copied in machine native endianness 
 * Returns: 
 * - OD_SUCCESSFUL is returned upon success. 
 * - SDO abort code is returned if error occurs . (See file def.h)
 * 
 * 
 * // Example :
 * unsigned char B;
 * B = 0xFF; // set transmission type
 *
 * retcode = writeLocalDict( (unsigned int)0x1800, (unsigned char)2, &B, sizeof(unsigned char), 1 );
 * 
 */
#define writeLocalDict( d, wIndex, bSubindex, pSourceData, pExpectedSize, checkAccess) \
       _setODentry( d, wIndex, bSubindex, pSourceData, pExpectedSize, checkAccess, 0)



/**
 * Scan the index of object dictionary. Used only by setODentry and getODentry.
 * *d Pointer to a CAN object data structure
 * wIndex
 * *errorCode :  OD_SUCCESSFUL if index foundor SDO abort code. (See file def.h)
 * **Callback
 * Returns:
 * NULL if index not found. Else : return the table part of the object dictionary.
 */
 const indextable * scanIndexOD (CO_Data* d, unsigned int wIndex, unsigned long *errorCode, ODCallback_t **Callback);

unsigned long RegisterSetODentryCallBack(CO_Data* d, unsigned int wIndex, unsigned char bSubindex, ODCallback_t Callback);

#ifdef __cplusplus
}
#endif

#endif /* __objacces_h__ */
