/*
This file is part of the NCAN bus, a library implementing CanOpen Light Stack.

Copyright (c) Elreg as 2010


*/

#ifndef _CONFIG_H_
#define _CONFIG_H_

//#include <ioavr.h>


//#define WD_SLEEP

#define FOSC           8000        // 8 MHz External chrystal

#define CAN_BAUDRATE    250         //Default CAN baudrate



// Needed defines by NCAN bus lib
#define MAX_CAN_BUS_ID 1
#define SDO_MAX_LENGTH_TRANSFERT 32
#define SDO_MAX_SIMULTANEOUS_TRANSFERTS 1
#define NMT_MAX_NODE_ID 128
#define SDO_TIMEOUT_MS 3000U   //3000U
#define MAX_NB_TIMER 16

// CANOPEN_BIG_ENDIAN is not defined
#define CANOPEN_LITTLE_ENDIAN 1

#define US_TO_TIMEVAL_FACTOR 8

#define REPEAT_SDO_MAX_SIMULTANEOUS_TRANSFERTS_TIMES(repeat)\
repeat

#define REPEAT_NMT_MAX_NODE_ID_TIMES(repeat)\
repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat \
repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat \
repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat \
repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat \
repeat repeat

#define EMCY_MAX_ERRORS 8
#define REPEAT_EMCY_MAX_ERRORS_TIMES(repeat)\
repeat repeat repeat repeat repeat repeat repeat repeat


#endif /* _CONFIG_H_ */
