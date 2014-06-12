#ifndef PIPEHANDLER_H_
#define PIPEHANDLER_H_

int pipeHandler_openReadingPipe(char* pipeName);
void pipeHandler_closeReadingPipe();

int pipeHandler_openWritingPipe(char* pipeName);
void pipeHandler_closeWritingPipe();

int pipeHandler_getParam(char* paramName, char* param);
int pipeHandler_sendParam(char* paramName, char* param);

#endif /*PIPEHANDLER_H_*/
