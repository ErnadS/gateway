#include "helper.h"
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>

/*  Read a line from a socket  */

// !!! "gatewayAT91SAM has it's own function for receiving ("sendRecHelpers.c)
ssize_t Readline(int sockd, char *buffer, size_t maxlen) {
	ssize_t n, rc;
	char c;
	size_t nLength = 0;
#ifdef PRINT_SEND_REC_SOCKET
	char *bufferTest = buffer;
#endif

	if (sockd < 0)
		return 0;

	if ((rc = read(sockd, &c, 1)) == 1) {  // new in version 2014.02
		nLength = ((size_t)c)*256*256;
#ifdef PRINT_SEND_REC_SOCKET
		printf ("-->len 1: %d\n", c);
#endif
	}
	if ((rc = read(sockd, &c, 1)) == 1) {
		nLength += ((size_t)c)*256;
#ifdef PRINT_SEND_REC_SOCKET
		printf ("-->len 2: %d\n", c);
#endif
	}

	if ((rc = read(sockd, &c, 1)) == 1) {
		nLength += ((size_t)c);
#ifdef PRINT_SEND_REC_SOCKET
		printf ("-->len 3: %d\n", c);
#endif
	}
#ifdef PRINT_SEND_REC_SOCKET
	printf ("-->calc rec len: %d\n", nLength);
#endif

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

// !!! "gatewayAT91SAM has it's own function for sending ("sendRecHelpers.c)
ssize_t Writeline(int sockd, const void *vptr, size_t nSize) {
	ssize_t nwritten;
	ssize_t nWrittenTot = 0;
	const void * buffer = vptr;
	char length[3];

	size_t nTempSize = nSize;

	length[0] = nTempSize / (256*256);
	nTempSize = nTempSize - length[0]*(256*256);
	length[1] = nTempSize / 256;
	length[2] = nTempSize - length[1] * 256;

	if (sockd < 0)
		return 0;

	write(sockd, length, 3);
#ifdef PRINT_SEND_REC_SOCKET
	printf ("<--len 3: %d  %d  %d\n", length[0], length[1], length[2]);
#endif

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
