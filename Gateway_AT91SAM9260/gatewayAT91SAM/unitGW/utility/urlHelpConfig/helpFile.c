/*
 * helpFile.c
 *
 *  Created on: 26. mai 2010
 *      Author: Ernad
 */

#include <stdio.h>
#include <string.h>
#include<sys/stat.h>

#include "helpFile.h"
#include "../gwHelpUtility.h"
#include "../../../config.h"


char helpFile_init(void) {
	FILE * hFile;
	GW_HELP tempHelp;
	hFile = fopen(HELP_FILE_PATH, "r");
	if (hFile == NULL) { // FILE NOT EXIST, CREATE
		strcpy(tempHelp.u, "http://www.imedsecure.com");
		strcpy(tempHelp.m, "imed@imedsecure.com");
		helpFile_saveToFile(&tempHelp);
		//hFile = fopen(HELP_FILE_PATH, "w"); // create file if not exist
		return 0;
	}

	fclose(hFile);
	return 1;
}

char helpFile_readFromFile(GW_HELP * pgwHelp) {
	int i = 0;
	int maxSize = 500;
	char buff[maxSize];

	pgwHelp->u[0] = '?';
	pgwHelp->u[1] = 0;

	pgwHelp->m[0] = '?';
	pgwHelp->m[1] = 0;

	FILE * hFile;
	hFile = fopen(HELP_FILE_PATH, "r");
	if (hFile == NULL) {
		return 0;
	}

	int c;
	while ((c = fgetc(hFile)) != EOF && i < maxSize) {
		buff[i++] = (char) c;
	}
	buff[i] = 0;


	gwHelpUtility_updateStructFromBuffer(buff, pgwHelp, "\n");

	fclose(hFile);
	return 1;
}

char helpFile_saveToFile(GW_HELP * pgwHelp) {

	FILE * hFile;
	hFile = fopen(HELP_FILE_PATH, "w");
	if (hFile == NULL) {
		return 0;
	}

	fputs("u=", hFile);
	fputs(pgwHelp->u, hFile);
	fputs("\n", hFile);

	fputs("m=", hFile);
	fputs(pgwHelp->m, hFile);
	fputs("\n", hFile);

	fclose(hFile);

	return 1;
}
