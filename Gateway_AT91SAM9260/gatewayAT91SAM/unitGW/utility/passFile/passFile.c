/*
 * passFile.c
 *
 *  Created on: 21. apr. 2010
 *      Author: Ernad
 */

#include <stdio.h>
#include<sys/stat.h>

#include "passFile.h"
#include "../gwPassUtility.h"
#include "../../../config.h"


char passFile_init(void) {
	FILE * hFile;
	hFile = fopen(PASS_FILE_PATH, "r");
	if (hFile == NULL) { // FILE NOT EXIST, CREATE
		struct stat st;
		if (stat("/gateway", &st) != 0) {
			mkdir("/gateway", 0777);
			mkdir("/gateway/Config", 0777);
		} else if (stat("/gateway/Config", &st) != 0) {
			mkdir("/gateway/Config", 0777);
		}

		hFile = fopen(PASS_FILE_PATH, "w"); // create file if not exist

		GW_PASS passStruct;
		gwPassUtility_clearStruct(&passStruct);
		gwPassUtility_setDefault(&passStruct);
		passFile_saveToFile(&passStruct);
	}

	if (hFile == NULL) {
		return 0;
	}

	fclose(hFile);
	return 1;
}

char passFile_readFromFile(GW_PASS * ppassStruct) {
	int i = 0;
	int maxSize = 500;
	char buff[maxSize];

	FILE * hFile;
	hFile = fopen(PASS_FILE_PATH, "r");
	if (hFile == NULL) {
		return 0;
	}

	int c;
	while ((c = fgetc(hFile)) != EOF && i < maxSize) {
		buff[i++] = (char) c;
	}
	buff[i] = 0;

	gwPassUtility_updateStructFromBuffer(buff, ppassStruct, NULL, "\n");

	fclose(hFile);
	return 1;
}

char passFile_saveToFile(GW_PASS * passStruct) {
	//char chTemp[5];

	FILE * hFile;
	hFile = fopen(PASS_FILE_PATH, "w");
	if (hFile == NULL) {
		return 0;
	}

	fputs("lpass1=", hFile);
	//sprintf(chTemp, "%d", enabled); // write asci "enabled".
	fputs(passStruct->lpass1, hFile);
	fputs("\n", hFile);

/*	fputs("lpass2=", hFile);
	fputs(passStruct.lpass2, hFile);
	fputs("\n", hFile);
*/
	fputs("apass1=", hFile);
	fputs(passStruct->apass1, hFile);
	fputs("\n", hFile);

/*	fputs("apass2=", hFile);
	fputs(passStruct.apass2, hFile);
	fputs("\n", hFile);
*/

	fclose(hFile);

	return 1;
}
