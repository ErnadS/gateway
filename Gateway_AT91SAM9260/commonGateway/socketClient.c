#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h> /* close */
#include <string.h>
#include <errno.h>
#include "socketClient.h"
#include "helper.h"           				/*  Our own helper functions  */

#define SERVER_PORT   2004

int       conn_s;                			/*  connection socket         */
 
 
int connectToServer(void) {
    struct    sockaddr_in servaddr;  		/*  socket address structure  */
    char     *szAddress;             		/*  Holds remote IP address   */

	szAddress = "127.0.0.1";
	//szAddress = "192.168.1.222";
	
    if ( (conn_s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1 ) {
	//fprintf(stderr, "TCP_CLIENT: Error creating client socket. Errno: %d\n", errno);
	return -1;
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family      = AF_INET;
    servaddr.sin_port        = htons(SERVER_PORT);
    
    if ( inet_aton(szAddress, &servaddr.sin_addr) <= 0 ) {
	//printf("TCP_CLIENT: Invalid remote IP address.\n");
	return -3;
    }

    if ( connect(conn_s, (struct sockaddr *) &servaddr, sizeof(servaddr) ) < 0 ) {
    	return -4;
    }

    return 1;
}

int sendCookie(char* psw)
{
	Writeline(conn_s, psw, strlen(psw));
	return 1;
}

int getParam(char* paramName, char* param, size_t maxRecLength)
{
	Writeline(conn_s, paramName, strlen(paramName));
	Readline(conn_s, param, maxRecLength);
	return 1;
}

int getParams(char* paramName, char* param1, char* param2)
{
	 Writeline(conn_s, paramName, strlen(paramName));
	 Readline(conn_s, param1, MAX_LINE_LENGTH-1);
	 Readline(conn_s, param2, MAX_LINE_LENGTH-1);
	 return 1;
}

int closeSocket(void)
{
	if (conn_s >= 0)
		close(conn_s);
	return 1;
}



