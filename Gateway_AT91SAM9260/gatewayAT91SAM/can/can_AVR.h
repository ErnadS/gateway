/*
This file is part of the NCAN bus, a library implementing CanOpen Light Stack.

Copyright (c) Elreg as 2010


*/

#ifndef __CAN_AVR__
#define __CAN_AVR__

// AVR implementation of the NCANbus driver includes
// Hardware related macros and ATMEL lib can_drv
#include "config.h"
#include "can_drv.h"
#include "can.h"

// Number of receive MOb
#define NB_RX_MOB                       13	// minimal 8
// Number of transmit MOb
#define NB_TX_MOB                       (NB_MOB - NB_RX_MOB)

#if (NB_TX_MOB < 1)
#error define less RX Mobs, you must have at least 1 TX MOb!
#elif (NB_RX_MOB < 8)
#error define at least 8 RX MObs!
#endif

#define START_TX_MOB                    NB_RX_MOB
#define TX_INT_MSK			((0x7F << (7 - NB_TX_MOB)) & 0x7F)

/************************* To be called by user app ***************************/

unsigned char canInit();
unsigned char canSend(Message *m);
unsigned char canReceive(Message *m);
unsigned char canChangeBaudRate_driver( CAN_HANDLE fd, char* baud);
#endif
