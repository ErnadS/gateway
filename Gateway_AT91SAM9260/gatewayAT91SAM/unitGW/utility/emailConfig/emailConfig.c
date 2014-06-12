/*
 * emailFile.c
 *
 *  Created on: 7. apr. 2010
 *      Author: Ernad
 */
#include <stdio.h>
#include<sys/stat.h>

#include "emailConfig.h"
#include "../gwEmailParser.h"
#include "../../../config.h"

//char fileName[] = "/gateway/Config/eMail.conf";

char emailConfig_init(void) {
	FILE * hFile;
	hFile = fopen(EMAIL_CONFIG_PATH, "r");
	if (hFile == NULL) { // FILE NOT EXIST, CREATE
		struct stat st;
		if (stat("/gateway", &st) != 0) {
			mkdir("/gateway", 0777);
			mkdir("/gateway/Config", 0777);
		} else if (stat("/gateway/Config", &st) != 0) {
			mkdir("/gateway/Config", 0777);
		}

		hFile = fopen(EMAIL_CONFIG_PATH, "w"); // create file if not exist

		emailConfig_saveToFile("0", "", "", "", "", "", "");
	}

	if (hFile == NULL) {
		return 0;
	}

	fclose(hFile);
	return 1;
}

char emailConfig_readFromFile(GW_EMAIL * peMailStruct) {
	int i = 0;
	int maxSize = 500;
	char buff[maxSize];

	FILE * hFile;
	hFile = fopen(EMAIL_CONFIG_PATH, "r");
	if (hFile == NULL) {
		return 0;
	}

	int c;
	while ((c = fgetc(hFile)) != EOF && i < maxSize) {
		buff[i++] = (char) c;
	}
	buff[i] = 0;

	gwEmailUtility_updateStructFromBuffer(buff, peMailStruct, "\n");

	fclose(hFile);
	return 1;
}

char emailConfig_saveToFile(char* enabled, char* usName, char* recip1, char* recip2, char* recip3, char* recip4, char* recip5) {
	//char chTemp[5];

	FILE * hFile;
	hFile = fopen(EMAIL_CONFIG_PATH, "w");
	if (hFile == NULL) {
		return 0;
	}

	fputs("gw_enab=", hFile);
	//sprintf(chTemp, "%d", enabled); // write asci "enabled".
	fputs(enabled, hFile);
	fputs("\n", hFile);

	fputs("usName=", hFile);
	fputs(usName, hFile);
	fputs("\n", hFile);

	fputs("r1=", hFile);
	fputs(recip1, hFile);
	fputs("\n", hFile);

	fputs("r2=", hFile);
	fputs(recip2, hFile);
	fputs("\n", hFile);

	fputs("r3=", hFile);
	fputs(recip3, hFile);
	fputs("\n", hFile);

	fputs("r4=", hFile);
	fputs(recip4, hFile);
	fputs("\n", hFile);

	fputs("r5=", hFile);
	fputs(recip5, hFile);
	fputs("\n", hFile);

	fclose(hFile);

	return 1;
}
/*
char emailFile_writeStruct(GW_EMAIL eMailStruct) {
	char chTemp[5];

	FILE * hFile;
	hFile = fopen(EMAIL_CONFIG_PATH, "w");
	if (hFile == NULL) {
		return 0;
	}
	//gw_enab smtp port usr pass send r1 r2 r3 r4 r5
	fputs("gw_enab=", hFile);
	sprintf(chTemp, "%d", eMailStruct.enabled); // write asci "enabled".
	fputs(chTemp, hFile);
	fputs("&", hFile);

	fputs("smtp=", hFile);
	fputs(eMailStruct.smtp, hFile);
	fputs("&", hFile);

	fputs("port=", hFile);
	sprintf(chTemp, "%d", eMailStruct.port); // write asci "port no".
	fputs(chTemp, hFile);
	fputs("&", hFile);

	fputs("usr=", hFile);
	fputs(eMailStruct.user, hFile);
	fputs("&", hFile);

	fputs("pass=", hFile);
	fputs(eMailStruct.pass, hFile);
	fputs("&", hFile);

	fputs("send=", hFile);
	fputs(eMailStruct.sender, hFile);
	fputs("&", hFile);

	fputs("r1=", hFile);
	fputs(eMailStruct.recip1, hFile);
	fputs("&", hFile);

	fputs("r2=", hFile);
	fputs(eMailStruct.recip2, hFile);
	fputs("&", hFile);

	fputs("r3=", hFile);
	fputs(eMailStruct.recip3, hFile);
	fputs("&", hFile);

	fputs("r4=", hFile);
	fputs(eMailStruct.recip4, hFile);
	fputs("&", hFile);

	fputs("r5=", hFile);
	fputs(eMailStruct.recip5, hFile);

	fclose(hFile);

	return 1;
}*/
