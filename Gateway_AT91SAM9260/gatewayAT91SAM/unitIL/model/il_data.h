/*
 * il_data.h
 *
 *  Created on: 4. mai 2010
 *      Author: Ernad
 */

#ifndef IL_DATA_H_
#define IL_DATA_H_

///////////////////////////////////////////
//        IL SETUP
///////////////////////////////////////////

typedef struct {
	char location[50];
	char password[5];
	char language;
	char canBusAddr;
	char displayContrast;
	int displayTimeout;
	char En[16];
	int AL[16];
	char AT[16][50];
} ilSetup;

typedef struct {
	int ch1;
	int ch2;
	int ch3;
	int ch4;
	int ch5;
	int ch6;
	int ch7;
	int ch8;
	int ch9;
	int ch10;
	int ch11;
	int ch12;
	int ch13;
	int ch14;
	int ch15;
	int ch16;
} ilMes;

#endif /* IL_DATA_H_ */
