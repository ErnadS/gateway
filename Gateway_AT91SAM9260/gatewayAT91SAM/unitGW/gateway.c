/*
 * gateway.c
 *
 *  Created on: 9. apr. 2010
 *      Author: Ernad
 */

#include "model/gwData.h"

gwSetup gwSetupStruct;

int gateway_init(void) {
	gwSetupStruct.canBusAddr = 127;
	return 1;
}

int gateway_getCanAddress(void) {
	return gwSetupStruct.canBusAddr;
}
