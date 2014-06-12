/*
 * gwPass.h
 *
 *  Created on: 21. apr. 2010
 *      Author: Ernad
 */

#ifndef GWPASS_H_
#define GWPASS_H_

// received when changing password
typedef struct {
	// vanlig user
	char lpass1[40]; // current
	// char lpass2[40]; // new // ny endring
	// admin
	char apass1[40]; // current
	// char apass2[40]; // new // ny endring
} GW_PASS;

#endif /* GWPASS_H_ */
