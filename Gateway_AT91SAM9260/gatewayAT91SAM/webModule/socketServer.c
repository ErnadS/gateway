#include <sys/socket.h>       /*  socket definitions        */
#include <sys/types.h>        /*  socket types              */
#include <arpa/inet.h>        /*  inet (3) funtions         */
#include <unistd.h>           /*  misc. UNIX functions      */


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "socketClientPipe.h"

/*  Global constants  */

#define MAX_LINE_LENGTH           	(1000)
#define SERVER_PORT   				 2004
/*
int readParams(int conn_s);
int getNextParam(char* buffer, char* paramName, char* paramValue);
int readAllParams(int conn_s);
*/
int serverSocket_start(void) {
	int list_s; /*  listening socket          */
	int conn_s; /*  connection socket         */
	struct sockaddr_in servaddr; /*  socket address structure  */

	unsigned int addrlen;

	if ((list_s = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		fprintf(stderr, "TCP_SERVER: Error creating listening socket.\n");
		return -1;
	}

	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERVER_PORT);

	if (bind(list_s, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
		fprintf(stderr, "TCP_SERVER: Error calling bind()\n");
		return -2;
	}
	fprintf(stderr, "TCP_SERVER: bind successed\n");

	if (listen(list_s, 1) < 0) {
	//if (listen(list_s, LISTENQ) < 0) {
		fprintf(stderr, "TCP_SERVER: Error calling listen()\n");
		return -3;
	}
	fprintf(stderr, "TCP_SERVER: Listen successed\n");

	while (1) {

		/*  Wait for a connection, then accept() it  */
		addrlen = sizeof(servaddr);
		if ((conn_s = accept(list_s, (struct sockaddr *) &servaddr, &addrlen))
				< 0) {
			fprintf(stderr, "TCP_SERVER: Error calling accept()\n");
			return -4;
		}
		//fprintf(stderr, "TCP_SERVER, Client accepted\n");

		socketClientPipe_handleRequest(conn_s);
	}

	return 0;
}
