/*
 * swData.h
 *
 *  Created on: 6. mai 2010
 *      Author: Ernad
 */

#ifndef SWDATA_H_
#define SWDATA_H_

///////////////////////////////////////////
//        SW SETUP
///////////////////////////////////////////

typedef struct {
	char location[50];
	char password[5];
	char language;
	int canBusAddr;
	char displayContrast;
	int displayTimeout;

	char uv;
	char ov;

	int k1On;
	int k1Of;
	int pOn1;
	int pOf1;
	char ocOn1;
	char ocOf1;

	int k2On;
	int k2Of;
	int pOn2;
	int pOf2;
	char ocOn2;
	char ocOf2;

	int k3On;
	int k3Of;
	int pOn3;
	int pOf3;
	char ocOn3;
	char ocOf3;

	unsigned int rd;
	unsigned int ofP;
	unsigned int rt;

	char f1[50];
	char f2[50];
	char f3[50];
	char fd[50];

	char f1On[50];
	char f1Of[50];
	char f2On[50];
	char f2Of[50];
	char f3On[50];
	char f3Of[50];
} swSetup;

typedef struct {
	unsigned int su1;
	unsigned int su2;
	unsigned int su3;
	unsigned int dv;
}swMes;

#endif /* SWDATA_H_ */
