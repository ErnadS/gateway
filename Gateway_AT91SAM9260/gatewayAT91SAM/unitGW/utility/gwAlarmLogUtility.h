/*
 * gwAlarmLogUtility.h
 *
 *  Created on: 27. mai 2010
 *      Author: Ernad
 */

#ifndef GWALARMLOGUTILITY_H_
#define GWALARMLOGUTILITY_H_

char gwAlarmLogUtility_saveAndSendAlarmMsg(int nUnitAddress, char* unitName, char* alarmMsg);
void gwAlarmLogUtility_deleteAllAlarmMsgs ();
void gwAlarmLogUtility_getAlarmMsgs(char* alarmMsg, int maxSize);
char gwAlarmLogUtility_getCurrentLogFileIndex();
void gwAlarmLogUtility_setCurrentLogFile(char index);
// TODO: wrong place for this function
int removedirectoryrecursively(const char *dirname);

#endif /* GWALARMLOGUTILITY_H_ */
