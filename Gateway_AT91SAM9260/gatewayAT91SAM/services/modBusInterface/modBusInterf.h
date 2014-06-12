/*
 * modBusInterf.h
 *
 *  Created on: 13. okt. 2010
 *      Author: Ernad
 */

#ifndef MODBUSINTERF_H_
#define MODBUSINTERF_H_

#include <pthread.h>

// "/dev/ttyS1", 19200, 'N', 8, 1
char modBusInterf_start(const char* serialName, unsigned char baudRate,
		char parity, unsigned char bytes, unsigned char stopBit, int slaveId);

char modBusInterf_stop();
void modBusInterface_notifyAlarm();


#endif /* MODBUSINTERF_H_ */
