/*
 * gwAlarmLogUtility.c
 *
 *  Created on: 27. mai 2010
 *      Author: Ernad
 */
#include "../../utility/ip.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<sys/stat.h>
#include "gwAlarmLogUtility.h"
#include "../../config.h"
#include "../model/device.h"
#include "../model/linkedList/deviceLinkedList.h"
#include "../../log/dataLogerLogic.h"
#include "../../utility/myString.h"
#include "../../services/sysTime.h"
#include <sys/types.h>
#include <dirent.h>
#include "../../services/mail.h"
#include "../../services/myMail.h"

#define MAX_ALARM_FILE_SIZE 200

int gwAlarmLogUtility_getLogLines(char* alarmMsgBuff, int nLines,
		int nFileIndex, int nLineIndex);
int fineNextFreeIndex(char * fileNameStart);
int gwAlarmLogUtility_addFileToXML(char* alarmMsgBuff, int nIndex,
		const char* fileName);

// int removedirectoryrecursively(const char *dirname);

/*
 char gwAlarmLogUtility_saveAlarmMsg(char* alarmMsg) {
 char fileIndex = gwAlarmLogUtility_getCurrentLogFileIndex();
 FILE * hFile;
 if (fileIndex == '1')
 hFile = fopen(ALARM_LOG_PATH_1, "a"); // append, create file if not exist
 else
 hFile = fopen(ALARM_LOG_PATH_2, "a"); // append, create file if not exist

 fputs(alarmMsg, hFile);
 fputs("\n", hFile);
 fseek(hFile, 0, SEEK_END);
 int size = ftell(hFile);

 fclose(hFile);

 if (size > MAX_ALARM_FILE_SIZE) {
 if (fileIndex == '1')
 gwAlarmLogUtility_setCurrentLogFile('2');
 else
 gwAlarmLogUtility_setCurrentLogFile('1');
 }

 return 1;
 }
 */



char gwAlarmLogUtility_saveAndSendAlarmMsg(int nUnitAddress, char* unitName, char* alarmMsg) {
	char alarmPath[60];
	char temp[100];

	char fileNameStart[30];
	DEV* dev;
	char time[9];
	char date[11];

	char time2[9];
	char date2[11];


	sysTime_toString3(date, time);
	//sysTime_toString2(date, time);
	sysTime_toString(date2, time2);

	FILE * hFile;
	sprintf(fileNameStart, "a_%s_%s", date, time);
	int nInd = fineNextFreeIndex(fileNameStart);
	sprintf(alarmPath, "%s%s_%d", ALARM_FOLDER, fileNameStart, nInd);
	printf("Creating new alarm file: %s, unitName: %s\n", alarmPath, unitName);
	hFile = fopen(alarmPath, "w"); // CREATE NEW FILE

	if (hFile == NULL) {
		printf("Error creating file\n");
		return -1; //ERROR !!!
	}

	sprintf(temp, "%s\n", date2);
	fputs(temp, hFile);

	sprintf(temp, "%s\n", time2);
	fputs(temp, hFile);

	dev = deviceLinkedList_getDevice(nUnitAddress);
	if (dev == NULL) {
		sprintf(temp, "Unknown addr. %d\n", nUnitAddress);
		fputs(temp, hFile);
		fputs(unitName, hFile);
		fputs("\n", hFile);
	} else {
		sprintf(temp, "%s addr. %d\n", devName[dev->type], dev->adr);
		fputs(temp, hFile);
		fputs(unitName, hFile);
		fputs("\n", hFile);
	}

	sprintf(temp, "%s\n", alarmMsg);
	fputs(temp, hFile);

	fclose(hFile);

	char tempSubject[100];
	char mailErrCode[300];
	int nMailResult;
	sprintf(tempSubject, "Message from IMED Secure, IP: %s", ip_getOwnIp());

	nMailResult = email_sendToAll(tempSubject, alarmPath, mailErrCode, 300);
	if (nMailResult != SMTP_OK) {
		if (nMailResult == MAIL_DISABLED)
			printf("Cannot send mail, disabled\n");
		else
			printf("Err sending mail: %s\n", mailErrCode);
	} else {
		printf("E-mail sent\n");
	}


	return 0;
}


void gwAlarmLogUtility_deleteAllAlarmMsgs () {
	removedirectoryrecursively(ALARM_FOLDER);
}


int removedirectoryrecursively(const char *dirname)
{
    DIR *dir;
    struct dirent *entry;
    char path[PATH_MAX];

    if (path == NULL) {
        fprintf(stderr, "Out of memory error\n");
        return 0;
    }
    dir = opendir(dirname);
    if (dir == NULL) {
        perror("Error opendir()");
        return 0;
    }
    while ((entry = readdir(dir)) != NULL) {
            if (strcmp(entry->d_name, ".") && strcmp(entry->d_name, "..")) {
                snprintf(path, (size_t) PATH_MAX, "%s%s", dirname, entry->d_name);
                if (entry->d_type == DT_DIR) {
                    removedirectoryrecursively(path);
                }

                /*
                 * Here, the actual deletion must be done.  Because this is
                 * quite a dangerous thing to do, and this program is not very
                 * well tested, we are just printing as if we are deleting.
                 */
                printf("Deleting: %s\n", path);
                remove (path);
                /*
                 * When you are finished testing this and feel you are ready to do the real
                 * deleting, use this: remove*STUB*(path);
                 * (see "man 3 remove")
                 * Please note that I DONT TAKE RESPONSIBILITY for data you delete with this!
                 */
            }

        }
        closedir(dir);
        return 1;
}



int fineNextFreeIndex(char * fileNameStart) {
	DIR *dp;
	struct dirent *ep;
	int nCurrentIndex = 0;
	int nFileCount = 0;
	char firsteFile[200];

	dp = opendir(ALARM_FOLDER);
	if (dp != NULL) {
		while ((ep = readdir(dp)) > 0) {
			if (nFileCount == 2) { // firste file is ".", second is ".."
				sprintf(firsteFile, "%s%s", ALARM_FOLDER, ep->d_name);
			}

			if (nameStartsWith(ep->d_name, fileNameStart) == 0) {
				nCurrentIndex = atoi(&(ep->d_name[21])) + 1;
			}
			nFileCount ++;
		}
		(void) closedir(dp);

		if (nFileCount >= MAX_ALARM_FILE_SIZE) {
			printf("deleting: %s\n", firsteFile);
			remove(firsteFile); // delete oldest file
		}

	} else
		printf("Couldn't open the directory: %s\n", ALARM_FOLDER);

	return nCurrentIndex;
}

void gwAlarmLogUtility_getAlarmMsgs(char* alarmMsgBuff, int maxSize) {
	//DIR *dp;
	//struct dirent *ep;
	int nBufferPos = 0;
	char filePath[50];
	int nIndex = 0;
	memset(alarmMsgBuff, 0, maxSize); // needs for null-termination

	struct dirent **namelist;
	int n;

	n = scandir(ALARM_FOLDER, &namelist, 0, alphasort); //"."
	if (n < 0)
	   perror("scandir");
	else {
	   while (n-- && (maxSize > nBufferPos + 200)) {
	      //printf("File: %s\n", namelist[n]->d_name);
	      if (nameStartsWith(namelist[n]->d_name, "a_") == 0) {
	    	  sprintf(filePath, "%s%s", ALARM_FOLDER, namelist[n]->d_name);
	    	  nBufferPos += gwAlarmLogUtility_addFileToXML(&alarmMsgBuff[nBufferPos], nIndex, filePath);
	    	  nIndex++;
	      }
	      free(namelist[n]);
	   }
	   free(namelist);
	}
}

int gwAlarmLogUtility_addFileToXML(char* alarmMsgBuff, int nIndex,
		const char* fileName) {
	char line[200];
	int nLineIndex = 0;
	int nMsgBufPos = 0;
	int nLineBufPos = 0;

	//alarmMsgBuff[0] = 0;

	FILE * hFile;
	hFile = fopen(fileName, "r");
	if (hFile == NULL) {
		return 0;
	}

	int c;
	while ((c = fgetc(hFile)) != EOF) {
		if ((char) c != '\n') {
			line[nLineBufPos] = c; // add char to line
			nLineBufPos++;
		} else {
			line[nLineBufPos] = 0; // null terminate line
			nLineBufPos = 0;
			nMsgBufPos = strlen(alarmMsgBuff); // alarmMsgBuff contains all "0" and we can take "strlen"

			if (nLineIndex == 0) { // date
				sprintf(&alarmMsgBuff[nMsgBufPos], "<d%d>%s</d%d>", nIndex,
						line, nIndex);
				nLineIndex++;
				nLineBufPos = 0;
			} else if (nLineIndex == 1) { // time
				sprintf(&alarmMsgBuff[nMsgBufPos], "<t%d>%s</t%d>", nIndex,
						line, nIndex);
				nLineIndex++;
				nLineBufPos = 0;
			} else if (nLineIndex == 2) { // unit name
				sprintf(&alarmMsgBuff[nMsgBufPos], "<n%d>%s</n%d>", nIndex,
						line, nIndex);
				nLineIndex++;
				nLineBufPos = 0;
			} else if (nLineIndex == 3) { // msg type
				sprintf(&alarmMsgBuff[nMsgBufPos], "<i%d>%s</i%d>", nIndex,
						line, nIndex);
				nLineIndex++;
				nLineBufPos = 0;
			} else if (nLineIndex == 4) { //alarm msg
				sprintf(&alarmMsgBuff[nMsgBufPos], "<m%d>%s</m%d>", nIndex,
						line, nIndex);
				break;
			}
		}
	}

	fclose(hFile);
	return (strlen(alarmMsgBuff));
}

char gwAlarmLogUtility_getCurrentLogFileIndex() {
	FILE * hFile;

	hFile = fopen(ALARM_CURRENT_FILE_INDEX, "r");

	if (hFile == NULL) {
		fopen(ALARM_CURRENT_FILE_INDEX, "w"); // create file
		fputs("1", hFile);
		fclose(hFile);
		return 1;
	}
	int c = fgetc(hFile);
	fclose(hFile);

	if (c != EOF) {
		if (c == '1') {
			return '1';
		} else {
			return '2';
		}
	}

	gwAlarmLogUtility_setCurrentLogFile('1');
	return '1';
}

void gwAlarmLogUtility_setCurrentLogFile(char index) {
	FILE * hFile;
	hFile = fopen(ALARM_CURRENT_FILE_INDEX, "w"); // create file
	char nIndex[2];
	nIndex[0] = index;
	nIndex[1] = 0;
	fputs(nIndex, hFile);
	fclose(hFile);

	// delete old file
	if (index == '1')
		hFile = fopen(ALARM_LOG_PATH_1, "w");
	else
		hFile = fopen(ALARM_LOG_PATH_2, "w");

	fclose(hFile);
}
/*
 void gwAlarmLogUtility_getAlarmMsgs(char* alarmMsgBuff, int maxSize) {
 char fileIndex = gwAlarmLogUtility_getCurrentLogFileIndex();

 int nLinesCount = gwAlarmLogUtility_getLogLines(alarmMsgBuff,
 MAX_ALARM_FILE_SIZE, fileIndex, 0);

 if (fileIndex == '1')
 fileIndex = '2';
 else
 fileIndex = '1';

 if (nLinesCount < MAX_ALARM_FILE_SIZE) {
 gwAlarmLogUtility_getLogLines(alarmMsgBuff, MAX_ALARM_FILE_SIZE
 - nLinesCount, fileIndex, nLinesCount + 1);
 }
 }*/

int gwAlarmLogUtility_getLogLines(char* alarmMsgBuff, int nMaxLines,
		int nFileIndex, int nLineIndex) {
	int nLinesCount = 0;
	//char nFileIndex = gwAlarmLogUtility_getCurrentLogFileIndex();
	FILE * hFile;
	char temp[20];
	char bStartingNewLine = 1;

	if (nFileIndex == '1')
		hFile = fopen(ALARM_LOG_PATH_1, "r");
	else
		hFile = fopen(ALARM_LOG_PATH_2, "r");

	if (hFile == NULL) {
		fclose(hFile);
		return 0;
	}

	int c;
	int i = 0;

	while ((c = fgetc(hFile)) != EOF) {
		if (c != '\n') {
			if (bStartingNewLine == 1) {
				nLinesCount++;
				bStartingNewLine = 0;
				sprintf(temp, "</r%d>", nLineIndex);
				strcpy(alarmMsgBuff, temp);
				i += strlen(temp);
			}
			alarmMsgBuff[i++] = (char) c;
		} else { // end of line
			sprintf(temp, "</r%d>", nLineIndex);
			nLineIndex++;
			strcpy(&alarmMsgBuff[i], temp);
			i += strlen(temp);
			bStartingNewLine = 1;
			if (nLinesCount >= nMaxLines) {
				fclose(hFile);
				return nLinesCount;
			}
		}
	}
	alarmMsgBuff[i] = 0;
	return nLinesCount;
}
