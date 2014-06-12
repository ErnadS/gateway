/*
This file is part of the NCAN bus, a library implementing CanOpen Light Stack.

Copyright (c) Elreg as 2010


*/


#ifndef __NCAN_Bus__
#define __NCAN_Bus__
#include "applicfg.h"
#include "data.h"





// ---------  to be called by user app ---------

void initTimer(void);
unsigned char  canSend(Message *m);
unsigned char  canChangeBaudRate(CAN_PORT port, char* baud);

#endif


