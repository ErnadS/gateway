/*
This file is part of the NCANbus, a library implementing CanOpenLight Stack.

Copyright (c) Elreg as 2010


*/

#ifndef __APPLICFG_AVR__
#define __APPLICFG_AVR__

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

//#define DEBUG_ERR_CONSOLE_ON
//#define DEBUG_WAR_CONSOLE_ON

// Integers
#define INTEGER8 signed char
#define INTEGER16 short
//#define INTEGER24
#define INTEGER32 long
//#define INTEGER40
//#define INTEGER48
//#define INTEGER56
#define INTEGER64

// Unsigned integers
//#define UNS8   unsigned char
//#define UNS16  unsigned short
//#define UNS32  unsigned long
/*
#define UNS24
#define UNS40
#define UNS48
#define UNS56
#define UNS64
*/ 


// Reals
#define REAL32	float
#define REAL64 double
#include "can.h"
//#include <gdisp.h>  //Grapic display functions to be able to write MSGERRors and MSGWARnings to the display for debug purposes

// MSG functions for debug purpose
// not finished, the strings have to be placed to the flash and printed out 
// using the printf_P function or to OLED display using the gputs_f function
/// Definition of MSG_ERR
// ---------------------
#ifdef DEBUG_ERR_CONSOLE_ON
#define MSG_ERR(num, str, val)      \
          printf(num, ' ');	\
          printf(str);		\
          printf(val);		\
		  printf('\n');

#else
#    define MSG_ERR(num, str, val)
#endif

/// Definition of MSG_WAR
// ---------------------
#ifdef DEBUG_WAR_CONSOLE_ON
#define MSG_WAR(num, str, val)      \
          printf(num, ' ');	\
          printf(str);		\
          printf(val);		\
          printf('\n');
#else
#    define MSG_WAR(num, str, val)
#endif

typedef void* CAN_HANDLE; //In case we have more than one can port...

typedef void* CAN_PORT;

#endif




