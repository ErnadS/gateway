#include "helper.h"
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// !!! DO NOT USE printf inside this file!!!! IT WILL BE SENT TO HTTP CLIENT

/*  Read a line from a socket  */

// !!! "gatewayAT91SAM has it's own function for receiving ("sendRecHelpers.c)
char* Readline(int sockd, ssize_t *count, size_t maxlen) {
	ssize_t rc;
	char c;
	char* buffer;
	size_t nLength = 0;

	*count = 0; // clear it (in case of error)

	if (sockd < 0)
		return 0;

	if ((rc = read(sockd, &c, 1)) == 1) { // new in version 2014.02
		nLength = ((size_t) c) * 256 * 256;
	} else
		return 0;
	if ((rc = read(sockd, &c, 1)) == 1) {
		nLength += ((size_t) c) * 256;
	} else
		return 0;

	if ((rc = read(sockd, &c, 1)) == 1) {
		nLength += ((size_t) c);
	} else
		return 0;

	if (nLength > maxlen || nLength == 0)
		return 0;

	buffer = (char *) malloc(nLength + 1);
	if (buffer == NULL) // cannot malloc
		return 0;

	buffer[0] = 0; // in case ...
			/*
			 for (n = 1; n <= nLength; n++) {  //n < maxlen; n++) {

			 if ((rc = read(sockd, &c, 1)) == 1) {
			 buffer[n] = c;
			 } else if (rc == 0) {
			 if (n == 1)
			 return 0;
			 else
			 break;
			 } else {
			 if (errno == EINTR)
			 continue;
			 return 0;
			 }
			 }*/

	if ((rc = read(sockd, buffer, nLength)) != nLength) {
		free(buffer);
		return 0;
	}

	buffer[nLength] = 0;

	*count = nLength;

	return buffer;
}

// !!! "gatewayAT91SAM has it's own function for sending ("sendRecHelpers.c)
ssize_t Writeline(int sockd, const void *vptr, size_t nSize) {
	ssize_t nwritten;
	ssize_t nWrittenTot = 0;
	const void * buffer = vptr;
	char length[3];

	size_t nTempSize = nSize;

	length[0] = nTempSize / (256 * 256);
	nTempSize = nTempSize - length[0] * (256 * 256);
	length[1] = nTempSize / 256;
	length[2] = nTempSize - length[1] * 256;

	if (sockd < 0)
		return 0;

	write(sockd, length, 3);

	while (nSize > 0) {
		if ((nwritten = write(sockd, buffer, nSize)) <= 0) {
			if (errno == EINTR)
				nwritten = 0;
			else
				return -1;
		}
		nWrittenTot += nwritten;
		nSize -= nwritten;
		buffer += nwritten;
	}

	//write(sockd, &newLine, 1);
	//write(sockd, &newLine, 1);

	return nWrittenTot;
}
