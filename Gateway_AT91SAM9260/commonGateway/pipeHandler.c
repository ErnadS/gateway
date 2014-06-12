#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/time.h>
#include <string.h>


#include <sys/ioctl.h>
#include <sys/vfs.h>


#define MAX_LINE 80

FILE * readingPipeHandler;
FILE * writingPipeHandler;

int pipeHandler_openReadingPipe(char* pipeName) {
	//readingPipeHandler = mkfifo(pipeName, 0666);//, 0);
	
	/* Create the FIFO if it does not exist */
	        umask(0);
	        mknod(pipeName, S_IFIFO|0666, 0);

	        
	/*if (readingPipeHandler == 0) {
		fprintf(stderr, "[mkfifo: OK]\n");
	} else {
		fprintf(stderr, "mkfifo IN failed\n");
		return 0;
	}*/

	readingPipeHandler = fopen(pipeName, "r");//O_RDONLY);
	if (readingPipeHandler < 0) {
		fprintf(stderr, "Open pipe IN failed\n");
		return -1;
	}
	
	return 1;
}

void pipeHandler_closeReadingPipe() {
	fclose(readingPipeHandler);
}

int pipeHandler_openWritingPipe(char* pipeName) {
	writingPipeHandler = fopen(pipeName, "w");//O_WRONLY);
	if (writingPipeHandler < 0) {
		fprintf(stderr, "Open pipe OUT failed\n");
		return -1;
	}
	return 1;
}

void pipeHandler_closeWritingPipe() {
	fclose(writingPipeHandler);
}

int pipeHandler_getParam(char* paramName, char* param) {
	//int nMsgLength;

	//fgets(&nMsgLength, sizeof(int), readingPipeHandler);
	fgets(paramName, 80, readingPipeHandler);
	//paramName[nMsgLength] = 0;
	//fgets(&nMsgLength, sizeof(int), readingPipeHandler);
	fgets(param, 80, readingPipeHandler);
	//param[nMsgLength] = 0;

	return 1;
}

/////////////////////////////////////////////////////////////////
// 
/////////////////////////////////////////////////////////////////
int pipeHandler_sendParam(char* paramName, char* param) {
	int nParmNameLength = strlen(paramName) + 1;
	int nParamLength = strlen(param);

	//write(writingPipeHandler, &nParmNameLength, sizeof(int));
	//fputs(paramName, writingPipeHandler);
	fwrite(paramName, 1, nParmNameLength, writingPipeHandler);
	//write(writingPipeHandler, &nParamLength, sizeof(int));
	//fputs(param, writingPipeHandler);
	fwrite(param, 1, nParamLength, writingPipeHandler);

	return 1;
}

