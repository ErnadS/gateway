/*
This file is part of the NCAN bus, a library implementing CanOpen Light Stack.

Copyright (c) Elreg as 2010


*/

#ifndef __can_h__
#define __can_h__

#include "applicfg.h"

/*
union SHORT_CAN {
  struct { unsigned char b0,b1; } b;
  unsigned long w;
};
*/
/*
typedef struct {
  unsigned long w;
} SHORT_CAN;
*/

/** 
 * The CAN message structure 
 * 
 */
typedef struct {
  unsigned int cob_id;	/**< message's ID */
  unsigned char rtr;		/**< remote transmission request. (0 if not rtr message, 1 if rtr message) */
  unsigned char len;		/**< message's length (0 to 8) */
  unsigned char data[8]; /**< message's datas */
} Message;

#define Message_Initializer {0,0,0,{0,0,0,0,0,0,0,0}}

typedef unsigned char (*canSend_t)(Message *);

#endif /* __can_h__ */
