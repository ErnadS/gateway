/*
 * tg_data.h
 *
 *  Created on: 28. jan. 2010
 *      Author: Ernad
 */

#ifndef TG_DATA_H_
#define TG_DATA_H_

/**
 * The same aliases must be used in javascript POST request
 */
#define TG_LOCATION_ALIAS 						"tg_loc"
#define TG_PASSWORD_ALIAS 						"tg_pas"
#define TG_LANGUAGE_ALIAS 						"tg_lan"
#define TG_CAN_ADDRES_ALIAS 					"tg_canAdr"
#define TG_DISP_CONTR_ALIAS 					"tg_dispC"
#define TG_DISP_TIMEOUT_ALIAS 					"tg_dispT"
#define TG_INS_TRAN_TYPE_ALIAS 					"tg_it_type"
#define TG_POW_ALIAS 							"tg_pow"
#define TG_INPUT_ALIAS 							"tg_inp"
#define TG_OUTPUT_ALIAS 						"tg_out"
#define TG_OUTPUT_CURRENT_ALIAS 				"tg_outI"
#define TG_TEMP_CLASS_ALIAS 					"tg_tempC"

#define TG_ALARM_LEVEL_INSUL_PREALARM_ALIAS 	"tg_ALIP"
#define TG_ALARM_LEVEL_LOAD_PREALARM_ALIAS 		"tg_ALLP"
#define TG_ALARM_LEVEL_TEMP_PREALARM_ALIAS 		"tg_ALTP"

#define TG_ALARM_LEVEL_INSUL_ALARM_ALIAS 		"tg_ALIL"
#define TG_ALARM_LEVEL_LOAD_ALARM_ALIAS 		"tg_ALLL"
#define TG_ALARM_LEVEL_TEMP_ALARM_ALIAS 		"tg_ALTL"

#define TG_ALARM_MSG_INSUL_PREALARM_ALIAS 		"tg_AMIP"
#define TG_ALARM_MSG_LOAD_PREALARM_ALIAS 		"tg_AMLP"
#define TG_ALARM_MSG_TEMP_PREALARM_ALIAS 		"tg_AMTP"

#define TG_ALARM_MSG_INSUL_ALARM_ALIAS 			"tg_AMIL"
#define TG_ALARM_MSG_LOAD_ALARM_ALIAS 			"tg_AMLL"
#define TG_ALARM_MSG_TEMP_ALARM_ALIAS 			"tg_AMTL"

#define TG_EAI_1_ENABLED_ALIAS 					"tg_eai1_e"
#define TG_EAI_1_PULLUPS_ALIAS 					"tg_eai1_p"
#define TG_ALARM_MESSAGE_1_ALIAS 				"tg_al_msg1"

#define TG_EAI_2_ENABLED_ALIAS 					"tg_eai2_e"
#define TG_EAI_2_PULLUPS_ALIAS 					"tg_eai2_p"

#define TG_ALARM_MESSAGE_2_ALIAS 				"tg_al_msg2"
///////////////////////////////////////////
//        TG SETUP
///////////////////////////////////////////
typedef struct {
	char enabled;
	char pullups;
	char message[50];
} externalAlarmInput;

typedef struct {
	int prealarm;
	int alarm;
} alarm_insulation;

typedef struct {
	char prealarmMsg[50];
	char alarmMsg[50];
} alarm_msg_struct;

typedef struct {
	int prealarm;
	int alarm;
} alarm_load;

typedef struct {
	int prealarm;
	int alarm;
} alarm_temperature;

typedef struct {
	alarm_insulation insul;
	alarm_load load;
	alarm_temperature temper;
} alarmLevels;

typedef struct {
	alarm_msg_struct insul;
	alarm_msg_struct load;
	alarm_msg_struct temper;
} alarmMessages;

typedef struct {
	char location[50];
	char password[5];
	char language;
	int canBusAddr;
	char displayContrast;
	int displayTimeout;
	char type[50];
	int power;
	int input;
	int output;
	int outputCurrent;
	char tempClass[50];
	alarmLevels alLevels;
	alarmMessages alMessages;
	externalAlarmInput eai1;
	externalAlarmInput eai2;
} tgSetup;

typedef struct {
	unsigned int ir;
	unsigned int tt;
	unsigned int tc;
} tgMes;
#endif /* TG_DATA_H_ */
