#ifndef SOCKETCLIENT_H_
#define SOCKETCLIENT_H_

#define MAX_LINE_LENGTH           (10000) //(150000)

int connectToServer(void);
int sendCookie(int conn_s, char* psw);
char* getParam(int conn_s, char* paramName, ssize_t *count, size_t maxRecLength);
//ssize_t getParam(char* paramName, char** param, size_t maxRecLength);
int closeSocket(int conn_s);

#endif /*SOCKETCLIENT_H_*/
