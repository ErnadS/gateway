/*
 * myString.c
 *
 *  Created on: 23. apr. 2010
 *      Author: Ernad
 */

#include <string.h>
#include <stdio.h>
int nameStartsWith(char * string1, char* string2) {
	char* str1 = string1;
	char* str2 = string2;

	while (*str1 != 0 && *str2 != 0) {
		if (*str1 != *str2)
			return -1;
		str1++;
		str2++;
	}

	return 0;
}

char myString_convertUTF8toCAN(char* stringIn, char* stringOut, int nMaxCount) {
	char tempString[200];
	int i;
	int newLength = 0;

	strcpy(stringOut, stringIn); // should be changed (don't need tempString)

	if (stringOut == 0) {
		stringOut[0] = '?';
		stringOut[1] = 0;
		return -1;
	}

	unsigned int nLen = strlen(stringOut);
	if (nLen < 1) {
		stringOut[0] = '?';
		stringOut[1] = 0;
		return 1;
	}

	//   Å   Æ    Ø       å    ø     æ
	//  197  146  216    229   248
	////////////////////////////////////////
	// UTF-8 (Norwegian)
	// Å: 0xC385
	// Æ: 0xC386
	// Ø: 0xC398
	// å: 0xC3a5
	// æ: 0xC3a6
	// ø: 0xC3b8
	////////////////////////////////////////
	// Finnish:
	// Node ASCII:
	// Ä = 196, ä = 132, Ö = 153, ö = 148
	// UTF-8:
	// Ä = 0xC384
	// ä = 0xC3A4
	// Ö = 0xC396
	// ö = 0xC3B6

	for (i = 0; i < nLen; i++) {
		if (stringOut[i] == 0xC3) { // UTF-8 (2 chars)
			if (i < nLen - 1) { // last char cannot be first byte of norwegian char
				if (stringOut[i + 1] == 0x85) { // "Å"
					tempString[newLength] = 197;
					newLength++;
				} else if (stringOut[i + 1] == 0x86) { // "Æ"
					tempString[newLength] = 146;
					newLength++;
				} else if (stringOut[i + 1] == 0x98) { // "Ø"
					tempString[newLength] = 216;
					newLength++;
				} else if (stringOut[i + 1] == 0xa5) { // "å"
					tempString[newLength] = 229;
					newLength++;
				} else if (stringOut[i + 1] == 0xa6) { // "æ"
					tempString[newLength] = 145;  // changed in version 2.08
					newLength++;
				} else if (stringOut[i + 1] == 0xb8) { // "ø"
					tempString[newLength] = 248; //178;
					newLength++;
				}
				// Finnish:
				else if (stringOut[i + 1] == 0x84) { // Ä
					tempString[newLength] = 196;
					newLength++;
				}
				else if (stringOut[i + 1] == 0xa4) { // ä
					tempString[newLength] = 132;
					newLength++;
				}
				else if (stringOut[i + 1] == 0x96) { // Ö
					tempString[newLength] = 153;
					newLength++;
				}
				else if (stringOut[i + 1] == 0xb6) { // ö
					tempString[newLength] = 148;
					newLength++;
				}
				i++;
			}
		} else if (!((stringOut[i] >= 'A' && stringOut[i] <= 'Z')
				|| (stringOut[i] >= 'a' && stringOut[i] <= 'z')
				|| (stringOut[i] >= ',' && stringOut[i] <= '9') || stringOut[i] == ':'
				|| stringOut[i] == ' ')) {
			tempString[newLength] = '?';
			newLength++;
		} else {
			tempString[newLength] = stringOut[i];
			newLength++;
		}

	}
	if (newLength == 0) {
		tempString[0] = '?';
		newLength = 1;
	}

	if (newLength > nMaxCount)
		newLength = nMaxCount;

	tempString[newLength] = 0;
	//printf("*** UTF8: %s, TERJE: %.2x %.2x %.2x\n", string1, tempString[0], tempString[1], tempString[2]);
	strcpy(stringOut, tempString);
	return 0; // OK
}

char myString_convertCANtoUTF8(char* string1) {
	char tempString[200];
	int i;
	int newLength = 0;

	if (string1 == 0) {
		string1[0] = '?';
		string1[1] = 0;
		return -1;
	}

	unsigned int nLen = strlen(string1);
	if (nLen < 1) {
		string1[0] = '?';
		string1[1] = 0;
		return 1;
	}
	//   Å   Æ    Ø       å    ø     æ
	//  197  146  216    229   248  !!! æ not implemented, change with 146 (Æ)
		// UTF-8:
		// Å: 0xC385
		// Æ: 0xC386
		// Ø: 0xC398
		// å: 0xC3a5
		// æ: 0xC3a6
		// ø: 0xC3b8
	////////////////////////////////////////
		// Finnish:
		// Node ASCII:
		// Ä = 196, ä = 132, Ö = 153, ö = 148
		// UTF-8:
		// Ä = 0xC384
		// ä = 0xC3A4
		// Ö = 0xC396
		// ö = 0xC3B6

	for (i = 0; i < nLen; i++) {
		if (string1[i] == 197) { 		// Å
			tempString[newLength++] = 0xC3;
			tempString[newLength++] = 0x85;
		} else if (string1[i] == 146) { // "Æ"
			tempString[newLength++] = 0xC3;
			tempString[newLength++] = 0x86;
		} else if (string1[i] == 216) { // Ø
			tempString[newLength++] = 0xC3;
			tempString[newLength++] = 0x98;
		} else if (string1[i] == 229) { // å
			tempString[newLength++] = 0xC3;
			tempString[newLength++] = 0xa5;
		} else if (string1[i] == 248) { //ø
			tempString[newLength++] = 0xC3;
			tempString[newLength++] = 0xb8;
		} else if (string1[i] == 145) { //æ  // added in version 2.08
			tempString[newLength++] = 0xC3;
			tempString[newLength++] = 0xa6;
		}
		// Finnish:
		else if (string1[i] == 196) {   // Ä
			tempString[newLength++] = 0xC3;
			tempString[newLength++] = 0x84;
		} else if (string1[i] == 132) { // ä
			tempString[newLength++] = 0xC3;
			tempString[newLength++] = 0xa4;
		} else if (string1[i] == 153) { // Ö
			tempString[newLength++] = 0xC3;
			tempString[newLength++] = 0x96;
		} else if (string1[i] == 148) { // ö
			tempString[newLength++] = 0xC3;
			tempString[newLength++] = 0xb6;
		}

		else if (!((string1[i] >= 'A' && string1[i] <= 'Z')
				|| (string1[i] >= 'a' && string1[i] <= 'z')
				|| (string1[i] >= ',' && string1[i] <= '9') || string1[i] == ':'
				|| string1[i] == ' ')) {
			tempString[newLength] = '?';
			newLength++;
		} else {
			tempString[newLength++] = string1[i];
		}
	}


	tempString[newLength] = 0;
	strcpy(string1, tempString);
	return 0; // OK
}
/*
char myString_validateString(char* string1) {
	int i;
	if (string1 == 0) {
		string1[0] = '?';
		string1[1] = 0;
		return -1;
	}
	printf("string before val. %s\n", string1);
	unsigned int nLen = strlen(string1);
	if (nLen < 1) {
		string1[0] = '?';
		string1[1] = 0;
		return 1;
	}
	if (nLen > 23)
		nLen = 23;

	for (i = 0; i < nLen; i++) {
		if (!((string1[i] >= 'A' && string1[i] <= 'Z')
				|| (string1[i] >= 'a' && string1[i] <= 'z')
				|| (string1[i] >= ',' && string1[i] <= '9') || string1[i] == ':'
				|| string1[i] == ' '))
			//|| string1[i] == '�' || string1[i] == '�' || string1[i] == '�'
			//		|| string1[i] == '�' || string1[i] == '�' || string1[i] == '�'))
			string1[i] = '?';

	}
	string1[nLen] = 0;
	return 0; // OK
}
char myString_validateString2(char* string1) {
	int i;
	if (string1 == 0) {
		string1[0] = '?';
		string1[1] = 0;
		return -1;
	}
	printf("string before val. %s\n", string1);
	unsigned int nLen = strlen(string1);
	if (nLen < 1) {
		string1[0] = '?';
		string1[1] = 0;
		return 1;
	}
	if (nLen > 59)
		nLen = 59;

	for (i = 0; i < nLen; i++) {
		if (!((string1[i] >= 'A' && string1[i] <= 'Z')
				|| (string1[i] >= 'a' && string1[i] <= 'z')
				|| (string1[i] >= ',' && string1[i] <= '9') || string1[i] == ':'
				|| string1[i] == ' '))
			//|| string1[i] == '�' || string1[i] == '�' || string1[i] == '�'
			//		|| string1[i] == '�' || string1[i] == '�' || string1[i] == '�'))
			string1[i] = '?';

	}
	string1[nLen] = 0;
	return 0; // OK
}*/
