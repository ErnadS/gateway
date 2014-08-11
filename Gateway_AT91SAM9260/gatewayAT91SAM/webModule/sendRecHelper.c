/*
 * sendRecHelper.c
 *
 *  Created on: Mar 9, 2014
 *      Author: ernad
 */

#include "sendRecHelper.h"
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>

/*  Read a line from a socket  */

ssize_t Readline(int sockd, char *buffer, size_t maxlen) {
	ssize_t n, rc;
	char c;
	size_t nLength = 0;


	if (sockd < 0)
		return 0;

	if ((rc = read(sockd, &c, 1)) == 1) {  // new in version 2014.02
		nLength = ((size_t)c)*256*256;
	} else
		return -1;
	if ((rc = read(sockd, &c, 1)) == 1) {
		nLength += ((size_t)c)*256;
	} else
		return -1;

	if ((rc = read(sockd, &c, 1)) == 1) {
		nLength += ((size_t)c);
	} else
		return -1;
	printf ("-->calc rec len: %d\n", nLength);

	for (n = 1; n <= nLength; n++) {  //n < maxlen; n++) {

		if ((rc = read(sockd, &c, 1)) == 1) {
			/*if ((c == '\n') && (*buffer == '\n')) {
				n--;
				break;
			}*/
			*buffer++ = c;
		} else if (rc == 0) {
			if (n == 1)
				return 0;
			else
				break;
		} else {
			if (errno == EINTR)
				continue;
			return -1;
		}
	}

	*buffer = 0;
#ifdef PRINT_SEND_REC_SOCKET
	printf ("-->buff: %s\n", bufferTest);
#endif
	return nLength; //n;
}

