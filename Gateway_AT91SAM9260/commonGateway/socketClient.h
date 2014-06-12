#ifndef SOCKETCLIENT_H_
#define SOCKETCLIENT_H_

#define MAX_LINE_LENGTH           (150000)

int connectToServer(void);
int sendCookie(char* psw);
int getParam(char* paramName, char* param, size_t maxRecLength);
int getParams(char* paramName, char* param1, char* param2);
int closeSocket(void);

#endif /*SOCKETCLIENT_H_*/
