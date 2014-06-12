/*
 * ap_data.h
 *
 *  Created on: 25. aug. 2010
 *      Author: Ernad
 */

#ifndef AP_DATA_H_
#define AP_DATA_H_

typedef struct {
	char location[50];
	char password[5];
	char language;
	int canBusAddr;
	char displayContrast;
	int displayTimeout;
} apSetup;


#endif /* AP_DATA_H_ */
