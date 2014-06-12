/*
 * apUtility.h
 *
 *  Created on: 25. aug. 2010
 *      Author: Ernad
 */

#ifndef APUTILITY_H_
#define APUTILITY_H_

int apUtility_getStructFromRequest(char* bufferRec, apSetup* apSetupData);
int apUtility_getXMLfromStruct(apSetup* apSetupData, char* bufferXML);

#endif /* APUTILITY_H_ */
