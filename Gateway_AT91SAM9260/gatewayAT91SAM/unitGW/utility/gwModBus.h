/*
 * gwModBus.h
 *
 *  Created on: 12. okt. 2010
 *      Author: Ernad
 */

#ifndef GWMODBUS_H_
#define GWMODBUS_H_

typedef struct {
	unsigned char gw_e; // mod bus enabled
	unsigned char add; 	// mod bus address
	unsigned char br; 	// baud rate
	unsigned char p; 	// parity
	unsigned char d; 	// data bits
	unsigned char s; 	// stop bit
} GW_MODBUS;

char gwModBus_init(void);
void gwModBus_clearStruct(GW_MODBUS *gwMB);
int gwModBus_getStructFromRequest(char* buf, GW_MODBUS *gwMB, char * separator);
char gwModBus_saveToFile(GW_MODBUS gwMB);
char gwModBus_readFromFile(GW_MODBUS *gwMB);
int gwModBus_getXMLfromStruct(GW_MODBUS *gwMB, char* bufferSend);
#endif /* GWMODBUS_H_ */
