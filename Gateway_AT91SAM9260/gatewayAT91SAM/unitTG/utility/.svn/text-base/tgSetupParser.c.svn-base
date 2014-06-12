/*
 * tg_web.c
 *
 *  Created on: 28. jan. 2010
 *      Author: Ernad
 */
#include "tgSetupParser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../utility/xml.h"
#include "../../utility/paramUtil.h"
#include "../../config.h"

int tgSetupParser_getStructFromRequest(char* bufferRec, tgSetup* tgSetupData) {
	int nLength = 0;
	int buffRecPos = 0;
	char paramName[MAX_PARAM_NAME_LENGTH];
	char paramValue[MAX_PARAM_VALUE_LENGTH];

	while (1) {
		nLength = paramUtil_getNextParam(&bufferRec[buffRecPos], "&",
				paramName, paramValue);
		if (nLength < 0)
			break; // TODO: return -1 ???


		if (strcmp(paramName, TG_LOCATION_ALIAS) == 0) {
			memcpy(&(tgSetupData->location), paramValue,
					sizeof(tgSetupData->location));
		} else if (strcmp(paramName, TG_PASSWORD_ALIAS) == 0) {
			memcpy(&(tgSetupData->password), paramValue,
					sizeof(tgSetupData->password));
		} else if (strcmp(paramName, TG_LANGUAGE_ALIAS) == 0) {
			tgSetupData->language = atoi(paramValue);
		} else if (strcmp(paramName, TG_CAN_ADDRES_ALIAS) == 0) {
			tgSetupData->canBusAddr = atoi(paramValue);
		}

		else if (strcmp(paramName, TG_DISP_CONTR_ALIAS) == 0) {
			tgSetupData->displayContrast = atoi(paramValue);
		} else if (strcmp(paramName, TG_DISP_TIMEOUT_ALIAS) == 0) {
			tgSetupData->displayTimeout = atoi(paramValue);
		} else if (strcmp(paramName, TG_INS_TRAN_TYPE_ALIAS) == 0) {
			memcpy(&(tgSetupData->type), paramValue, sizeof(tgSetupData->type));
		} else if (strcmp(paramName, TG_POW_ALIAS) == 0) {
			tgSetupData->power = atoi(paramValue);
		} else if (strcmp(paramName, TG_INPUT_ALIAS) == 0) {
			tgSetupData->input = atoi(paramValue);
		} else if (strcmp(paramName, TG_OUTPUT_ALIAS) == 0) {
			tgSetupData->output = atoi(paramValue);
		} else if (strcmp(paramName, TG_OUTPUT_CURRENT_ALIAS) == 0) {
			tgSetupData->outputCurrent = atoi(paramValue);
		} else if (strcmp(paramName, TG_TEMP_CLASS_ALIAS) == 0) {
			memcpy(&(tgSetupData->tempClass), paramValue,
					sizeof(tgSetupData->tempClass));
		}

		else if (strcmp(paramName, TG_ALARM_LEVEL_INSUL_ALARM_ALIAS) == 0) {
			tgSetupData->alLevels.insul.alarm = atoi(paramValue);
		} else if (strcmp(paramName, TG_ALARM_LEVEL_INSUL_PREALARM_ALIAS) == 0) {
			tgSetupData->alLevels.insul.prealarm = atoi(paramValue);
		} else if (strcmp(paramName, TG_ALARM_LEVEL_LOAD_ALARM_ALIAS) == 0) {
			tgSetupData->alLevels.load.alarm = atoi(paramValue);
		} else if (strcmp(paramName, TG_ALARM_LEVEL_LOAD_PREALARM_ALIAS) == 0) {
			tgSetupData->alLevels.load.prealarm = atoi(paramValue);
		} else if (strcmp(paramName, TG_ALARM_LEVEL_TEMP_ALARM_ALIAS) == 0) {
			tgSetupData->alLevels.temper.alarm = atoi(paramValue);
		} else if (strcmp(paramName, TG_ALARM_LEVEL_TEMP_PREALARM_ALIAS) == 0) {
			tgSetupData->alLevels.temper.prealarm = atoi(paramValue);
		}

		else if (strcmp(paramName, TG_ALARM_MSG_INSUL_ALARM_ALIAS) == 0) {
			strcpy(tgSetupData->alMessages.insul.alarmMsg, paramValue);
		} else if (strcmp(paramName, TG_ALARM_MSG_INSUL_PREALARM_ALIAS) == 0) {
			strcpy(tgSetupData->alMessages.insul.prealarmMsg, paramValue);
		} else if (strcmp(paramName, TG_ALARM_MSG_LOAD_ALARM_ALIAS) == 0) {
			strcpy(tgSetupData->alMessages.load.alarmMsg, paramValue);
		} else if (strcmp(paramName, TG_ALARM_MSG_LOAD_PREALARM_ALIAS) == 0) {
			strcpy(tgSetupData->alMessages.load.prealarmMsg, paramValue);
		} else if (strcmp(paramName, TG_ALARM_MSG_TEMP_ALARM_ALIAS) == 0) {
			strcpy(tgSetupData->alMessages.temper.alarmMsg, paramValue);
		} else if (strcmp(paramName, TG_ALARM_MSG_TEMP_PREALARM_ALIAS) == 0) {
			strcpy(tgSetupData->alMessages.temper.prealarmMsg, paramValue);
		}

		else if (strcmp(paramName, TG_EAI_1_ENABLED_ALIAS) == 0) {
			tgSetupData->eai1.enabled = atoi(paramValue);
		} else if (strcmp(paramName, TG_EAI_1_PULLUPS_ALIAS) == 0) {
			tgSetupData->eai1.pullups = atoi(paramValue);
		} else if (strcmp(paramName, TG_ALARM_MESSAGE_1_ALIAS) == 0) {
			memcpy(&(tgSetupData->eai1.message), paramValue,
					sizeof(tgSetupData->eai1.message));
		} else if (strcmp(paramName, TG_EAI_2_ENABLED_ALIAS) == 0) {
			tgSetupData->eai2.enabled = atoi(paramValue);
		} else if (strcmp(paramName, TG_EAI_2_PULLUPS_ALIAS) == 0) {
			tgSetupData->eai2.pullups = atoi(paramValue);
		} else if (strcmp(paramName, TG_ALARM_MESSAGE_2_ALIAS) == 0) {
			memcpy(&(tgSetupData->eai2.message), paramValue,
					sizeof(tgSetupData->eai2.message));
		}

		if (nLength == 0)
			break;
		buffRecPos += nLength;
	}

	return 1;
}
// TODO: change "reboot" to reload or refresh
int tgSetupParser_getXMLfromStruct(tgSetup* tgSetupData, char* bufferXML) {
	int nLength = 0;
	char chTemp[MAX_PARAM_VALUE_LENGTH];

	sprintf(chTemp, "%s", tgSetupData->location);
	nLength += xml_addTag(&bufferXML[nLength], TG_LOCATION_ALIAS, chTemp);

	sprintf(chTemp, "%s", tgSetupData->password);
	nLength += xml_addTag(&bufferXML[nLength], TG_PASSWORD_ALIAS, chTemp);

	sprintf(chTemp, "%d", tgSetupData->language);
	nLength += xml_addTag(&bufferXML[nLength], TG_LANGUAGE_ALIAS, chTemp);

	sprintf(chTemp, "%d", tgSetupData->canBusAddr);
	nLength += xml_addTag(&bufferXML[nLength], TG_CAN_ADDRES_ALIAS, chTemp);

	sprintf(chTemp, "%d", tgSetupData->displayContrast);
	nLength += xml_addTag(&bufferXML[nLength], TG_DISP_CONTR_ALIAS, chTemp);

	sprintf(chTemp, "%d", tgSetupData->displayTimeout);
	nLength += xml_addTag(&bufferXML[nLength], TG_DISP_TIMEOUT_ALIAS, chTemp);

	sprintf(chTemp, "%s", tgSetupData->type);
	nLength += xml_addTag(&bufferXML[nLength], TG_INS_TRAN_TYPE_ALIAS, chTemp);

	sprintf(chTemp, "%d", tgSetupData->power);
	nLength += xml_addTag(&bufferXML[nLength], TG_POW_ALIAS, chTemp);

	sprintf(chTemp, "%d", tgSetupData->input);
	nLength += xml_addTag(&bufferXML[nLength], TG_INPUT_ALIAS, chTemp);

	sprintf(chTemp, "%d", tgSetupData->output);
	nLength += xml_addTag(&bufferXML[nLength], TG_OUTPUT_ALIAS, chTemp);

	sprintf(chTemp, "%d", tgSetupData->outputCurrent);
	nLength += xml_addTag(&bufferXML[nLength], TG_OUTPUT_CURRENT_ALIAS, chTemp);

	sprintf(chTemp, "%s", tgSetupData->tempClass);
	nLength += xml_addTag(&bufferXML[nLength], TG_TEMP_CLASS_ALIAS, chTemp);

	////////////////////////////////////////////////////////
	sprintf(chTemp, "%d", tgSetupData->alLevels.insul.alarm);
	nLength += xml_addTag(&bufferXML[nLength],
			TG_ALARM_LEVEL_INSUL_ALARM_ALIAS, chTemp);

	sprintf(chTemp, "%d", tgSetupData->alLevels.insul.prealarm);
	nLength += xml_addTag(&bufferXML[nLength],
			TG_ALARM_LEVEL_INSUL_PREALARM_ALIAS, chTemp);

	sprintf(chTemp, "%d", tgSetupData->alLevels.load.alarm);
	nLength += xml_addTag(&bufferXML[nLength], TG_ALARM_LEVEL_LOAD_ALARM_ALIAS,
			chTemp);

	sprintf(chTemp, "%d", tgSetupData->alLevels.load.prealarm);
	nLength += xml_addTag(&bufferXML[nLength],
			TG_ALARM_LEVEL_LOAD_PREALARM_ALIAS, chTemp);

	sprintf(chTemp, "%d", tgSetupData->alLevels.temper.alarm);
	nLength += xml_addTag(&bufferXML[nLength], TG_ALARM_LEVEL_TEMP_ALARM_ALIAS,
			chTemp);

	sprintf(chTemp, "%d", tgSetupData->alLevels.temper.prealarm);
	nLength += xml_addTag(&bufferXML[nLength],
			TG_ALARM_LEVEL_TEMP_PREALARM_ALIAS, chTemp);

	sprintf(chTemp, "%s", tgSetupData->alMessages.insul.alarmMsg);
	nLength += xml_addTag(&bufferXML[nLength], TG_ALARM_MSG_INSUL_ALARM_ALIAS,
			chTemp);

	sprintf(chTemp, "%s", tgSetupData->alMessages.insul.prealarmMsg);
	nLength += xml_addTag(&bufferXML[nLength],
			TG_ALARM_MSG_INSUL_PREALARM_ALIAS, chTemp);

	sprintf(chTemp, "%s", tgSetupData->alMessages.load.alarmMsg);
	nLength += xml_addTag(&bufferXML[nLength], TG_ALARM_MSG_LOAD_ALARM_ALIAS,
			chTemp);

	sprintf(chTemp, "%s", tgSetupData->alMessages.load.prealarmMsg);
	nLength += xml_addTag(&bufferXML[nLength],
			TG_ALARM_MSG_LOAD_PREALARM_ALIAS, chTemp);

	sprintf(chTemp, "%s", tgSetupData->alMessages.temper.alarmMsg);
	nLength += xml_addTag(&bufferXML[nLength], TG_ALARM_MSG_TEMP_ALARM_ALIAS,
			chTemp);

	sprintf(chTemp, "%s", tgSetupData->alMessages.temper.prealarmMsg);
	nLength += xml_addTag(&bufferXML[nLength],
			TG_ALARM_MSG_TEMP_PREALARM_ALIAS, chTemp);

	sprintf(chTemp, "%d", tgSetupData->eai1.enabled);
	nLength += xml_addTag(&bufferXML[nLength], TG_EAI_1_ENABLED_ALIAS, chTemp);
	sprintf(chTemp, "%d", tgSetupData->eai1.pullups);
	nLength += xml_addTag(&bufferXML[nLength], TG_EAI_1_PULLUPS_ALIAS, chTemp);
	sprintf(chTemp, "%s", tgSetupData->eai1.message);
	nLength
			+= xml_addTag(&bufferXML[nLength], TG_ALARM_MESSAGE_1_ALIAS, chTemp);

	sprintf(chTemp, "%d", tgSetupData->eai2.enabled);
	nLength += xml_addTag(&bufferXML[nLength], TG_EAI_2_ENABLED_ALIAS, chTemp);
	sprintf(chTemp, "%d", tgSetupData->eai2.pullups);
	nLength += xml_addTag(&bufferXML[nLength], TG_EAI_2_PULLUPS_ALIAS, chTemp);
	sprintf(chTemp, "%s", tgSetupData->eai2.message);
	nLength
			+= xml_addTag(&bufferXML[nLength], TG_ALARM_MESSAGE_2_ALIAS, chTemp);

	return nLength;
}

int tgMesParser_getXMLfromStruct(tgMes* tgMesData, char* bufferXML) {
	int nLength = 0;
	char chTemp[MAX_PARAM_VALUE_LENGTH];

	sprintf(chTemp, "%u", tgMesData->ir);
	nLength += xml_addTag(&bufferXML[nLength], "ir", chTemp);

	sprintf(chTemp, "%u", tgMesData->tt);
	nLength += xml_addTag(&bufferXML[nLength], "tt", chTemp);

	sprintf(chTemp, "%u", tgMesData->tc);
	nLength += xml_addTag(&bufferXML[nLength], "tc", chTemp);

	return nLength;
}
