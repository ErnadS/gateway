/*
 * canInterface.h
 *
 *  Created on: 15. juni 2010
 *      Author: Ernad
 */

#ifndef CANDRIVERINTERFACE_H_
#define CANDRIVERINTERFACE_H_

#include "can.h"

#define MAJOR_NUM  				98

enum SJA1000_REG {
	CONTROL_REG = 0,
	COMMAND_REG,
	STATUS_REG,
	INTERRUPT_REG,
	ACCEPTANCE_CODE_REG = 4,
	ACCEPTANCE_MASK_REG,
	BUS_TIMING_0_REG,
	BUS_TIMING_1_REG,
	OUTPUT_CONTR_REG,
	TEST_REG,
	TRANSMIT_IDENTIFIER_H_REG = 10,
	TRANSMIT_IDENTIFEIER_L_REG,
	TRANSMIT_BYTE_1_REG,
	TRANSMIT_BYTE_2_REG,
	TRANSMIT_BYTE_3_REG,
	TRANSMIT_BYTE_4_REG,
	TRANSMIT_BYTE_5_REG,
	TRANSMIT_BYTE_6_REG,
	TRANSMIT_BYTE_7_REG,
	TRANSMIT_BYTE_8_REG,
	RECEIVE_IDENTIFIER_H_REG = 20,
	RECEIVE_IDENTIFEIER_L_REG,
	RECEIVE_BYTE_1_REG,
	RECEIVE_BYTE_2_REG,
	RECEIVE_BYTE_3_REG,
	RECEIVE_BYTE_4_REG,
	RECEIVE_BYTE_5_REG,
	RECEIVE_BYTE_6_REG,
	RECEIVE_BYTE_7_REG,
	RECEIVE_BYTE_8_REG,
	NOT_USED_REG,
	CLOCK_DIVIDER_REG
};

#define IOCTL_GET  _IOWR(MAJOR_NUM, 1, struct canControlMsg)
#define IOCTL_SET  _IOWR(MAJOR_NUM, 2, struct canControlMsg)
#define IOCTL_TEST _IOWR(MAJOR_NUM, 3, int)

struct canControlMsg {
   char address;
   char newValue;
   char result[8];
   char resultLength;
   int id;
};

char canDriverInterface_init(void);
unsigned char canDriverInterface_sendMessage(int nAddress, char* buffer, char msgLength);
unsigned char canDriverInterface_readMessage(Message *m);

#endif /* CANDRIVERINTERFACE_H_ */

