/*
 * gwDevMonitoring.h
 *
 *  Created on: 21. apr. 2010
 *      Author: Ernad
 */

#ifndef GWDEVICE_H_
#define GWDEVICE_H_

extern char* devName[];

enum devType {
	TG = 1, // TODO: 1,
	IL = 2,
	SW = 3, //TODO: 3,
	AP = 4,
	SP = 6
};





typedef struct {
	int adr;
	int type;
	char enab;
	char found;
} DEV;

#endif /* GWDEVICE_H_ */
