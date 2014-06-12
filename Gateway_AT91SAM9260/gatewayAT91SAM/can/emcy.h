/*
This file is part of the NCAN bus, a library implementing CanOpen Light Stack.

Copyright (C): Elreg A.S, Terje Sparengen

Emergency Object is used to communicate device and application failures.
*/
					 
#ifndef __emcy_h__
#define __emcy_h__


#include "applicfg.h"
#include "data.h"

/* The error states 
 * ----------------- */
typedef enum enum_errorState {
  Error_free		= 0x00, 
  Error_occurred	= 0x01
} e_errorState;

typedef struct {
	unsigned int errCode;
	unsigned char errRegMask;
	unsigned char active;
} s_errors;



typedef void (*post_emcy_t)(CO_Data* d, unsigned char nodeID, unsigned int errCode, unsigned char errReg,unsigned int idx,unsigned char subidx);
//void _post_emcy(CO_Data* d, unsigned char nodeID, unsigned int errCode, unsigned char errReg);
void _post_emcy(CO_Data* d, unsigned char nodeID, unsigned int errCode, unsigned char errReg,unsigned int idx,unsigned char subidx);
/*************************************************************************
 * Functions
 *************************************************************************/

/** 
 * Sets a new error with code errCode. Also sets corresponding bits in Error register (1001h)
 * *d Pointer on a CAN object data structure
 * errCode The error code
 * errRegMask
 * addInfo
 * Returns:
 */
unsigned char EMCY_setError(CO_Data* d, unsigned int errCode, unsigned char errRegMask, unsigned int addInfo);

/** 
 * Indicates it has recovered from error errCode. Also clears corresponding bits in Error register (1001h)
 * *d Pointer on a CAN object data structure
 * errCode The error code
 */
void EMCY_errorRecovered(CO_Data* d, unsigned int errCode);

/**
 * Start EMCY consumer and producer
 * *d Pointer on a CAN object data structure
 */
void emergencyInit(CO_Data* d);

/** 
 * Stop EMCY producer and consumer
 * *d Pointer on a CAN object data structure 
 */
void emergencyStop(CO_Data* d);

/** 
 * This function is responsible to process an EMCY NCANbus-message
 * *d Pointer on a CAN object data structure 
 * *m Pointer on the CAN-message which has to be analysed.
 */
void proceedEMCY(CO_Data* d, Message* m);

#endif /*__emcy_h__ */
