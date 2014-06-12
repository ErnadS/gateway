/*
 * tcpPipe.h
 *
 *  Created on: 27. jan. 2010
 *      Author: Ernad
 */

#ifndef TCPPIPE_H_
#define TCPPIPE_H_

#define MAX_REQUEST_LENGTH	2000
#define MAX_RESPONSE_LENGTH	150000

void socketClientPipe_handleRequest(int conn_s);
void socketClientPipe_setMutex();
void socketClientPipe_clearMutex();

#endif /* TCPPIPE_H_ */
