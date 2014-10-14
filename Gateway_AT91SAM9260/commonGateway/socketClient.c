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

#include <errno.h>
#include <signal.h>

#define SERVER_PORT   2004

int soketOk = 1;

void sigpipe_handler() {
	printf("SIGPIPE caught\n");
	soketOk = 0;
}
 
 
int connectToServer(void) {
	int       conn_s;                			/*  connection socket         */ // ES: 2014.07.05 moved inside in case of 2 sockets in the same time
    struct    sockaddr_in servaddr;  		/*  socket address structure  */
    char     *szAddress;             		/*  Holds remote IP address   */

	szAddress = "127.0.0.1";
	//szAddress = "192.168.1.222";
	
    if ( (conn_s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1 ) {
    	return -1;
    }
    soketOk = 1;

    signal(SIGPIPE, sigpipe_handler);

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family      = AF_INET;
    servaddr.sin_port        = htons(SERVER_PORT);
    
    if ( inet_aton(szAddress, &servaddr.sin_addr) <= 0 ) {
    	return -3;
    }

    if ( connect(conn_s, (struct sockaddr *) &servaddr, sizeof(servaddr) ) < 0 ) {
    	return -4;
    }

    return conn_s;
}

int sendCookie(int conn_s, char* psw)
{
	if (soketOk != 1)
		return 0;
	Writeline(conn_s, psw, strlen(psw));
	return 1;
}

char* getParam(int conn_s, char* paramName, ssize_t *count, size_t maxRecLength)
{
	if (soketOk != 1)
			return 0;
	Writeline(conn_s, paramName, strlen(paramName));
	if (soketOk != 1)
			return 0;
	return Readline(conn_s, count, maxRecLength);
}

int closeSocket(int conn_s)
{
	if (conn_s >= 0)
		close(conn_s);
	return 1;
}



