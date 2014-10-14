/*
 * dataLogerLogic.c
 *
 *  Created on: 19. mai 2010
 *      Author: Ernad
 */
#include "../utility/paramUtil.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dataLogerLogic.h"

#include "../can/canInterface.h"
#include "../unitGW/utility/gwDataLogerUtility.h"
#include "../unitGW/model/linkedList/deviceLinkedList.h"
#include "../services/sysTime.h"
#include "../unitGW/model/device.h"
#include "../config.h"
#include "../webModule/socketClientPipe.h"
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>

#include <sys/stat.h>

void appendToFile(int nId, int nType, char* date, char* time, unsigned long value, unsigned char msgId);
long getFileSize(char* fileName);
void removeFileIfNotExist(char* fileName);

// USED BY cgiSwitchoverMeasur
char * messageTextTg[] = { "Temperature", "Load Current", "Insulation" };
//char * messageTextSw[] = { "Prioritized supply", "Secondary supply 1", "Secondary supply 2",
//		"IT net" };
char * messageTextSw[] = { "Preferred Supply (V)", "SecSupply 1 (V)", "SecSupply 2 (V)", "IT supply (V)" };

char * messageTextTg_file[] = { "Temperature", "LoadCurrent", "Insulation" };
char * messageTextSw_file[] = { "PreferredSupply", "SecSupply_1", "SecSupply_2", "ITsupply" };

//int logTimer[10];

DEV_MEASSUR_TASK * devMeassuringsTask;
unsigned char percent[] = { 0, 5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60,
		65, 70, 75, 80, 85, 90, 95 };

void dataLogerLogic_checkUnit(int unitIndex);
//int dataLogerLogic_addLogLine(DEV* dev, int i, int j, char* buffer, int devider);

int dataLogerLogic_readFile(int nPos, int nCanAddress);
int dataLogerLogic_updateStructFromBuffer(int nPos, char* bufferRec,
		char* separator);
//int dataLogerLogic_getIndex(int nCanAddress);

void dataLogerLogic_init(void) {
	int i;
	devMeassuringsTask = gwDagaLogerUtility_getStruct();
	//dataLogerFile_initFiles();
	for (i = 0; i < MAX_LOG_TASKS; i++) {
		devMeassuringsTask[i].timer = devMeassuringsTask[i].per * 10;

		if (devMeassuringsTask[i].adr <= 0)
			devMeassuringsTask[i].timer = -1;
	}
}

void dataLogerLogic_onChangedStruct(DEV_MEASSUR_TASK * devMeassuringsTaskNew) {
	int i;

	for (i = 0; i < MAX_LOG_TASKS; i++) {
		if(devMeassuringsTask[i].adr != devMeassuringsTaskNew[i].adr // one existing task is removed
				// !!!! Following line line is new in version 2.08
				|| (devMeassuringsTask[i].msid != devMeassuringsTaskNew[i].msid)){  // or changed
			printf("removed log task?\r\n");
			//if (devMeassuringsTaskNew[i].adr <= 0) {
				printf("delete old log messages\r\n");
				devMeassuringsTask[i].meassuringsList.currentIndex = 0;
				devMeassuringsTask[i].meassuringsList.structSize = 0;
			//}
		}
	}
}

void dataLogerLogic_removeCanID(int id) {
	gwDataLogerUtility_removeCanID(id);
}

int dataLogerLogic_readFile(int nPos, int nCanAddress) {
	int i = 0;
	int maxSize = 500;
	char buff[maxSize];
	char temp[35];
	sprintf(temp, "/gateway/log/logUnit%d", nCanAddress);
	FILE * hFile;
	hFile = fopen(temp, "r");
	if (hFile == NULL) {
		return 0;
	}

	int c;
	while ((c = fgetc(hFile)) != EOF && i < maxSize) {
		buff[i++] = (char) c;
	}
	buff[i] = 0;
	fclose(hFile);

	dataLogerLogic_updateStructFromBuffer(nPos, buff, "\n");

	return 1;
}

int dataLogerLogic_updateStructFromBuffer(int nPos, char* bufferRec,
		char* separator) {
	int i;
	int nLength = 0;
	int buffRecPos = 0;
	char paramName[MAX_PARAM_NAME_LENGTH];
	char paramValue[MAX_PARAM_VALUE_LENGTH];

	devMeassuringsTask[nPos].meassuringsList.structSize = 0;
	devMeassuringsTask[nPos].meassuringsList.currentIndex = -1;

	if (*bufferRec == 0)
		return -1;

	for (i = 0; i < MAX_LOG_LINES;) {
		nLength = paramUtil_getNextParam(&bufferRec[buffRecPos], separator,
				paramName, paramValue);
		if (nLength < 0)
			break; // TODO: return -1 ???

		if (strcmp(paramName, "t") == 0) { // time
			strcpy(
					devMeassuringsTask[nPos].meassuringsList.measStruct[i].measTime,
					paramValue);
			devMeassuringsTask[nPos].meassuringsList.measStruct[i].measTime[19]
					= 0; // TODO: not elegant solution
			devMeassuringsTask[nPos].meassuringsList.structSize++;
			devMeassuringsTask[nPos].meassuringsList.currentIndex++;
			if (devMeassuringsTask[nPos].meassuringsList.currentIndex
					== MAX_LOG_LINES)
				devMeassuringsTask[nPos].meassuringsList.currentIndex = 0;
		} else if (strcmp(paramName, "m") == 0) { // msg id
			devMeassuringsTask[nPos].meassuringsList.measStruct[i].msgId
					= atoi(paramValue);
		} else if (strcmp(paramName, "v") == 0) { // value
			devMeassuringsTask[nPos].meassuringsList.measStruct[i].measValue
					= atof(paramValue);
			i++;
		}

		if (nLength == 0)
			break;
		buffRecPos += nLength;
	}

	return 1;
}

void makeFileName(DEV* dev, int nMsgId, char* fileName) {
	if (dev->type == TG) {
		sprintf(fileName, "TG_%d_%s", dev->adr, messageTextTg_file[nMsgId]);
	} else if (dev->type== SW) {
		sprintf(fileName, "SW_%d_%s", dev->adr, messageTextSw_file[nMsgId]);
	} else if (dev->type == IL) {
		sprintf(fileName, "IL_%d_Channel_%d", dev->adr, (nMsgId + 1));
	}
}



void getFileLineParams(char* line, char* date, char* time, char* value) {
	char *sep = strstr(line, "\t");
	int position =  sep - line;
	line[position] = 0;
	strcpy(date, line);

	sep ++;

	char *sep2 = strstr(sep, "\t");
	position = sep2  - sep;
	sep[position] = 0;
	strcpy(time, sep);

	sep2 ++;
	strcpy(value, sep2);
}

int appenMeasuringFile(DEV * dev, int nMsgId, char bOld, char* bufferXML) {
	char pathName[100];
	char fileName[100];
	FILE * hFile;
	char* buffer = bufferXML;

	char * line = NULL;
	size_t len = 0;
	ssize_t read;

	int nOffset = 0;

	makeFileName(dev, nMsgId, fileName);
	if (bOld)
		sprintf(pathName, "%s%s_old", MEASURE_FOLDER, fileName);
	else
		sprintf(pathName, "%s%s", MEASURE_FOLDER, fileName);

	hFile = fopen(pathName, "r");

	if (hFile == NULL) {
		printf("!!! CANNOT open file: %s\n", pathName);
		return 0;
	}

	nOffset += sprintf(&buffer[nOffset], "<pre style=\"height: 10pc;\">"); //style=\"display:inline-block;white-space: pre-wrap; word-wrap:break-word;\">");
	while ((read = getline(&line, &len, hFile)) != -1) {
			if (read < 5)  // min 5 characters in line (should bi more...)
				continue;
			//getFileLineParams(line, date, time, value);
			nOffset += sprintf(&buffer[nOffset], "%s", line);
		}

	/*while ((read = getline(&line, &len, hFile)) != -1) {
		if (read < 5)  // min 5 characters in line (should bi more...)
			continue;
		getFileLineParams(line, date, time, value);

		//nOffset += sprintf(&buffer[nOffset], "<TR><TD>%s</TD><TD>%s</TD><TD>%s</TD></TR>", date, time, value);
		nOffset += sprintf(&buffer[nOffset], "%s\t\t%s\t\t%s<br>", date, time, value);
	}*/
	nOffset += sprintf(&buffer[nOffset], "</pre>");
	if (line)
	     free(line);

	fclose(hFile);

	return nOffset;
}

int dataLogerLogic_getExcel(int nCanAddress, int nMsgId, char* bufferXML) {
	//int i;
	//int j;
	//int devider = 1;
	char * buffer = bufferXML;
	DEV * dev = deviceLinkedList_getDevice(nCanAddress);
	int nOffset = 0;
	//DEV_MEASSUR_LIST devMeasList;
	//char temp[200];
	char msgTypeText[30];

	//int lineLen;

	if (dev->type == TG) {
			sprintf(msgTypeText, "%s", messageTextTg_file[nMsgId]);
	} else if (dev->type== SW) {
			sprintf(msgTypeText, "%s", messageTextSw_file[nMsgId]);
	} else if (dev->type == IL) {
			sprintf(msgTypeText, "Channel_%d", (nMsgId + 1));
	} else { // never?
		strcpy(msgTypeText, "Error type");
	}

	nOffset += sprintf(&buffer[nOffset], "<TABLE><TR><TD><b>UNIT:</b></TD><TD>%s</TD></TR><TR><TD><b>ADDRESS:</b></TD><TD>%d</TD></TR><TR><TD><b>Msg type:</b></TD><TD>%s</TD></TR></TABLE>",
					devName[dev->type], dev->adr, msgTypeText);


//	nOffset += sprintf(&buffer[nOffset], "<TR><TD width=\"100px\"><b>Date</b></TD><TD width=\"100px\"><b>Time</b></TD><TD></TD><TD><b>Value</b></TD></TR>");

	nOffset += sprintf(&buffer[nOffset], "<pre>Date\t\tTime\t\tValue</pre>");

	nOffset += appenMeasuringFile(dev, nMsgId, 1, &buffer[nOffset]);
	nOffset += appenMeasuringFile(dev, nMsgId, 0, &buffer[nOffset]);
/*
	strcpy(&buffer[nOffset], "</TABLE>");
	nOffset += 8;*/
	return nOffset;
}
/*
int dataLogerLogic_addLogLine(DEV* dev, int i, int j, char* buffer, int devider) {
	int nOffset = 0;
	char temp[100];
	char temp2[11];
	// ADD TIME
	memcpy(temp2,
			&devMeassuringsTask[i].meassuringsList.measStruct[j].measTime[9],
			10);
	temp2[10] = 0;

	sprintf(temp, "<TR><TD>%s</TD>", temp2);
	nOffset = strlen(temp);
	// ADD DATE
	memcpy(temp2, devMeassuringsTask[i].meassuringsList.measStruct[j].measTime,
			8);
	temp2[8] = 0;

	sprintf(&temp[nOffset], "<TD>%s</TD><TD></TD>", temp2);
	strcpy(buffer, temp); //measLog[i].measStruct[j].measTime);
	nOffset = strlen(temp); // old: 19

	// ADD MEASSURMENT
	//int ind = devMeassuringsTask[i].meassuringsList.measStruct[j].msgId;

	if(devider == 1) {
		sprintf(temp, "<TD>%lu</TD></TR>", devMeassuringsTask[i].meassuringsList.measStruct[j].measValue);
	} else if(devider == 10) {
		sprintf(temp, "<TD>%.1f</TD></TR>", (float)devMeassuringsTask[i].meassuringsList.measStruct[j].measValue / devider);
	} else if(devider == 100) {
		sprintf(temp, "<TD>%.2f</TD></TR>", (float)devMeassuringsTask[i].meassuringsList.measStruct[j].measValue / devider);
	} else if(devider == 1000) {
		sprintf(temp, "<TD>%.3f</TD></TR>", (float)devMeassuringsTask[i].meassuringsList.measStruct[j].measValue / devider);
	}
	strcpy(&buffer[nOffset], temp);
	nOffset += strlen(temp);

	return nOffset;
}*/

void dataLogerLogic_checkTimers(void) {
	int i;
	for (i = 0; i < 4; i++) {
		if (devMeassuringsTask[i].timer > 0)
			devMeassuringsTask[i].timer -= 1; // minus 1 sec

		if (devMeassuringsTask[i].timer == 0) {
			devMeassuringsTask[i].timer = (devMeassuringsTask[i].per + 1) * 10;
			dataLogerLogic_checkUnit(i);
		}
	}
}

void dataLogerLogic_checkUnit(int unitIndex) {
	socketClientPipe_setMutex();
	int unitAddress = devMeassuringsTask[unitIndex].adr;

	if (unitAddress <= 0) {
		socketClientPipe_clearMutex();
		return;
	}

	DEV* dev = deviceLinkedList_getDevice(unitAddress);

	if (dev == NULL) {
		socketClientPipe_clearMutex();
		printf("ERROR: Cannot log for device: %d. Not found\n", unitAddress);
		return;
	}

	if (dev->enab != 1) {
		socketClientPipe_clearMutex();
		printf("ERROR: Cannot log for device: %d. Not enabled\n", unitAddress);
		return;
	}

	unsigned long fCanValue = canInterface_getValue(dev, devMeassuringsTask[unitIndex].msid);
//printf("REMOVE THIS: Got msg id: %d for device %d, value: %ul\n", devMeassuringsTask[unitIndex].msid, dev->adr, fCanValue);
	socketClientPipe_clearMutex();

	if (fCanValue == ERROR_CAN) {
		printf("ERRPR: receviced ERROR_CAN for device: %d\n", unitAddress);
		//TODO: Log ERROR ???
		return;
	}

	int nPreviousIndex =
			devMeassuringsTask[unitIndex].meassuringsList.currentIndex - 1;
	if (nPreviousIndex < 0)
		nPreviousIndex = MAX_LOG_LINES - 1;

	unsigned long lastVal;
	if (devMeassuringsTask[unitIndex].meassuringsList.structSize <= 0) {
		lastVal = -1000;
		printf("** structSize <= 0\n");
	}
	else {
		lastVal = devMeassuringsTask[unitIndex].meassuringsList.measStruct[nPreviousIndex].measValue;
	}

	// Changed in ver. 2.07
	//if (fCanValue== 0 &&  lastVal == 0)
	if (fCanValue== 0 &&  lastVal == 0 && (percent[devMeassuringsTask[unitIndex].chan] != 0))
			return;

	if ((fCanValue*100 >= lastVal * (100 + (unsigned long) percent[devMeassuringsTask[unitIndex].chan]))
			|| (fCanValue*100 <= lastVal * (100 - (unsigned long) percent[devMeassuringsTask[unitIndex].chan]))) {
		char time[9];
		char date[11];
		sysTime_toString(date, time);

		// set value
		devMeassuringsTask[unitIndex].meassuringsList.measStruct[devMeassuringsTask[unitIndex].meassuringsList.currentIndex].measValue
				= fCanValue;
		// set time
		strcpy(
				devMeassuringsTask[unitIndex].meassuringsList.measStruct[devMeassuringsTask[unitIndex].meassuringsList.currentIndex].measTime,
				time);
		devMeassuringsTask[unitIndex].meassuringsList.measStruct[devMeassuringsTask[unitIndex].meassuringsList.currentIndex].measTime[8]
				= ' ';
		strcpy(
				&devMeassuringsTask[unitIndex].meassuringsList.measStruct[devMeassuringsTask[unitIndex].meassuringsList.currentIndex].measTime[9],
				date);
		// set msg ID
		devMeassuringsTask[unitIndex].meassuringsList.measStruct[devMeassuringsTask[unitIndex].meassuringsList.currentIndex].msgId
				= devMeassuringsTask[unitIndex].msid;

		devMeassuringsTask[unitIndex].meassuringsList.currentIndex++;
		if (devMeassuringsTask[unitIndex].meassuringsList.currentIndex
				== MAX_LOG_LINES)
			devMeassuringsTask[unitIndex].meassuringsList.currentIndex = 0;

		if (devMeassuringsTask[unitIndex].meassuringsList.structSize
				< MAX_LOG_LINES)
			devMeassuringsTask[unitIndex].meassuringsList.structSize++;

		appendToFile(dev->adr, dev->type, date, time, fCanValue, devMeassuringsTask[unitIndex].msid);

	}
}

char* logFilePrefix[] = {"TG", "IL", "SW"};
//char  logFilesDir[] = "/srv/www/logFiles/";

void appendToFile(int nId, int nType, char* date, char* time,
		unsigned long value, unsigned char nMsgId) {
	FILE *fp;
	char filePath[100];
	char msgTypeText[100];
	char fileRenamed = 0;
	float devider = 1.f;

	if (nType < 1 || nType > 3)
		return;


	//////////////////////////////////////////////////////
	if (nType == TG) {
		strcpy(msgTypeText, messageTextTg_file[nMsgId]);
		if (nMsgId == 0) 		// temperature
			devider = 10.f;
		else if (nMsgId == 1) 	// load
			devider = 100.f;
	} else if (nType== SW) {
		strcpy(msgTypeText, messageTextSw_file[nMsgId]);
	} else if (nType == IL) {
		devider = 1000.f;
		sprintf(msgTypeText, "Channel_%d", (nMsgId + 1));
	}
	//////////////////////////////////////////////////////

	sprintf(filePath, "%s%s_%d_%s", MEASURE_FOLDER, logFilePrefix[nType - 1], nId, msgTypeText);

	long fileSize = getFileSize(filePath);

	///////////////////////////////////////////////////////////////////////////////////
	// TO BIG FILE, rename to "old" and open new
	///////////////////////////////////////////////////////////////////////////////////
	if (fileSize > 30000 && fileSize > 0) {  // MAX_FILE_SIZE = 30000bytes => "...old" is 30000 + size of the new file = max 60000. It's 60000/25 = 2400 lines
		char newPath[100];
		sprintf(newPath, "%sold_%s_%d_%s", MEASURE_FOLDER, logFilePrefix[nType - 1], nId, msgTypeText);
		int ret = rename(filePath, newPath);

		if (ret != 0) {
			printf("Cannot rename file %s\n", filePath);
			return;
		}
		else
			fileRenamed = 1;
	}

	fp = fopen(filePath, "a");
	if (fp == NULL) {
		printf("I couldn't open file %s\n", filePath);
		return;
	}


	fprintf(fp, "%s\t%s\t%.1f\r\n", date, time, value/devider);
	fclose(fp);
}



long getFileSize(char* fileName) {
	struct stat st;

	// stat() returns -1 on error. Skipping check in this example
	if(stat(fileName, &st) == 0) {
		//printf("File size %lu\n", st.st_size);
		return st.st_size;
	}
	else
		return 0;
}


void removeUnusedMeasureFiles(void) {
	DIR *dir;
	struct dirent *entry;

	dir = opendir(MEASURE_FOLDER);
	if (dir == NULL) {
	    printf("Error removeUnusedMeasureFiles()");
	    return;
	}
	printf("REMOVE ALL UNUSED FILES IN: %s\n", MEASURE_FOLDER);

	while ((entry = readdir(dir)) != NULL) {
		if (strcmp(entry->d_name, ".") && strcmp(entry->d_name, "..")) { // if not = "." and not = ".."
			removeFileIfNotExist(entry->d_name);
	    }
	}

	closedir(dir);
}

// private function
void removeFileIfNotExist(char* fileName) {
	int i;
	char taskFileName [100];
	char tempOld [100];
	DEV* dev;
	printf ("Remove if not exist: %s\n", fileName);
	for (i = 0; i < MAX_LOG_TASKS; i++) {
		if (devMeassuringsTask[i].adr > 0 && devMeassuringsTask[i].adr <= 127) { //
			dev = deviceLinkedList_getDevice(devMeassuringsTask[i].adr);
			if (dev != NULL) {
				makeFileName(dev, devMeassuringsTask[i].msid, taskFileName);
				sprintf(tempOld, "%s_old", taskFileName);
				if ((strcmp(taskFileName, fileName) == 0) || (strcmp(tempOld, fileName) == 0)) {
					printf("Do not delete file\n");
					return; // found task which is source for this log file. Do not delete log
				}
			}
		}
	}

	char fullFilePath[100];
	sprintf(fullFilePath, "%s%s", MEASURE_FOLDER, fileName);
	unlink (fullFilePath);
	printf ("REMOVED: %s\n", fullFilePath);
}
