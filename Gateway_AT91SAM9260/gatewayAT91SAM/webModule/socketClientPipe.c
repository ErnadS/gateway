/*
 * tcpPipe.c
 *
 *  Created on: 27. jan. 2010
 *      Author: Ernadf
 */
#include "../utility/ip.h"
#include <string.h>
#include "socketClientPipe.h"

#include <sys/socket.h>       /*  socket definitions        */
#include <sys/types.h>        /*  socket types              */
#include <arpa/inet.h>        /*  inet (3) funtions         */
#include <unistd.h>           /*  misc. UNIX functions      */
#include <errno.h>
#include <signal.h>

#include <stdlib.h>
#include <stdio.h>
//#include <helper.h>
#include "sendRecHelper.h"
#include "../utility/xml.h"
#include "../utility/paramUtil.h"

#include "../unitTG/utility/tgSetupParser.h"
#include "../unitIL/utility/ilSetupParser.h"
#include "../unitSW/utility/swSetupParser.h"
#include "../unitGW/utility/gwDevParParser.h"
#include "../unitGW/utility/gwParConfig/gwParamSaver.h"
#include "../unitGW/utility/gwAlarmLogUtility.h"

#include "../unitGW/utility/gwEmailParser.h"
#include "../unitGW/utility/gwDhcpUtility.h"
#include "../unitGW/utility/dhcpConfig/dhcpConfigFile.h"
#include "../unitGW/utility/gwPassUtility.h"
#include "../services/dhcpLinux.h"

#include "../unitGW/utility/gwDevMonitorUtility.h"
#include "../unitGW/utility/gwDataLogerUtility.h"
#include "../unitGW/utility/gwHelpUtility.h"

#include "../unitAP/ap_data.h"
#include "../unitAP/apUtility.h"

#include "../unitGW/model/gwDevParData.h"
#include "../unitGW/model/gwDHCP.h"
#include "../unitGW/model/gwPass.h"
#include "../unitGW/model/gwDataLoger.h"
#include "../unitTG/model/tg_data.h"
#include "../unitGW/model/gwHelp.h"

#include "../unitGW/utility/emailConfig/emailConfig.h"
#include "../unitGW/utility/devMonitor/devMonitorSaver.h"
#include "../unitGW/utility/dataLoger/dataLogerSaver.h"
#include "../unitGW/utility/urlHelpConfig/helpFile.h"
#include "../services/smtpConfig.h"
#include "../services/myMail.h"
#include "../log/dataLogerLogic.h"

#include "../unitGW/dynamic/gwSetupDeviceMonitor.h"

#include "../services/modBusInterface/modBusInterf.h"

#include "postHeader.h"
#include "../can/canInterface.h"

#include "../services/sysTime.h"
#include "../unitGW/gateway.h"
#include "../services/mail.h"
#include "../config.h"
#include "../loginLogic.h"
#include "../unitGW/dynamic/leftMenu.h"
#include "../unitGW/model/linkedList/deviceLinkedList.h"
#include "../unitGW/utility/gwModBus.h"
#include "../utility/myString.h"

#include "../can/ObjDict.h"
#include <pthread.h> // mutex
extern char softwareVersion[];
pthread_mutex_t changingID_mutex;

pthread_mutex_t webClient_mutex;

ssize_t socketClientPipe_write(int sockd, const void *vptr, size_t nSize);
void socketClientPipe_createErrorMsg(char* buffer, int* nLength, char canResult);
void sigpipe_handler();
char soketOk = 1;

void socketClientPipe_setMutex() {
	pthread_mutex_lock(&changingID_mutex);
}

void socketClientPipe_clearMutex() {
	pthread_mutex_unlock(&changingID_mutex);
}
int socketClientPipe_readAllParams(int conn_s);

void socketClientPipe_handleRequest(int conn_s) {
	soketOk = 1;
	signal(SIGPIPE, sigpipe_handler); // catch socket closing (Ernad)

	socketClientPipe_readAllParams(conn_s);

#ifdef DEBUG_CLIENT
	printf("Closing connection to client\n");
#endif

	if (close(conn_s) < 0) {
		fprintf(stderr, "TCP_SERVER: Error calling close()\n");
	}
}

void sigpipe_handler() {
	printf("SIGPIPE caught\n");
	soketOk = 0;
}

int socketClientPipe_readAllParams(int conn_s) {
	char bufferRec[MAX_REQUEST_LENGTH];
	char bufferRemoteIP[40];
	char bufferSend[MAX_RESPONSE_LENGTH];
	int nLength;
	char loginState;
	char tempChar;

	postHeader postHeaderData;

	memset(bufferSend, 0, MAX_RESPONSE_LENGTH);
	memset(bufferRec, 0, MAX_REQUEST_LENGTH);

	// Get Remote IP address
	if (conn_s < 0) {
		printf("Error socketClientPipe_readAllParams\n");
		return -1;
	}
	int nTotLength = Readline(conn_s, bufferRemoteIP, 39);
	bufferRemoteIP[nTotLength] = 0;
#ifdef DEBUG_CLIENT
	printf("Received Cookie: %s\n", bufferRemoteIP);
#endif
	nTotLength = Readline(conn_s, bufferRec, MAX_REQUEST_LENGTH - 1);
	bufferRec[nTotLength] = 0;
	nLength = postHeader_getData(bufferRec, &postHeaderData);

#ifdef DEBUG_CLIENT
	printf("\n************\nREC: %s\n************\n", &bufferRec[0]);
#endif
	//////////////////////////////////////////////////////////
	/// LOG IN
	//////////////////////////////////////////////////////////
	if (strcmp(bufferRemoteIP, "0") == 0) { // Login, Cookie not generated yet
		if (strcmp(postHeaderData.formID, "cgiLog_in") == 0) {
			char paramName[10];
			char paramValue[20];
			char cookie[40];
			int nLoginState;
			paramUtil_getNextParam(&bufferRec[nLength], "&", paramName,
					paramValue);
			nLoginState = loginLogic_checkPassword(paramValue, cookie);

			if (nLoginState > LOGIN_LEVEL_NOT_LOGED) {
				strcpy(bufferSend, cookie);
				nLength = strlen(cookie);
			} else if (nLoginState == LOGIN_LEVEL_NOT_LOGED) {
				printf("Wrong password\n");
				strcpy(bufferSend, "0");
				nLength = 1;
			} else { //LOGIN_TO_BUSY
				printf("!!!To busy, can't login\n");
				strcpy(bufferSend, "1");
				nLength = 1;
			}
		} else {
			printf("Not allowed\n");
			strcpy(bufferSend, "0");
			nLength = 1;
		}

		//fprintf(stderr, bufferSend);
		int nSent;
		if (conn_s >= 0) {
			nSent = socketClientPipe_write(conn_s, bufferSend, nLength);
			printf("Sent %d bytes\n", nSent);
		} else {
			printf("Error sockClientPipe, cannot send to client\n");
		}
		return 1;
	}
	//////////////////////////////////////////////////////////////

	loginState = loginLogic_getLoginState(bufferRemoteIP);

	if (loginState <= LOGIN_LEVEL_NOT_LOGED) {
		printf("user is not logged in. Refused formID: %s\n",
				postHeaderData.formID);
		return -1;
	}

	// gw_dp gw_em tg_s tg_u tg_m
	if (nLength > 0) {
		loginLogic_updateLoginTimer(bufferRemoteIP);
		if (strcmp(postHeaderData.formID, "none") == 0) { // CGI ask about user login status
			bufferSend[0] = loginState; //(char) loginLogic_getLoginState(bufferRemoteIP);
			nLength = 1;
		} else if (strcmp(postHeaderData.formID, "loging") == 0) {
			char paramName[10];
			char paramValue[20];
			int msgId;
			paramUtil_getNextParam(&bufferRec[nLength], "&", paramName,
					paramValue);
			msgId = atoi(paramValue);
			if (msgId >= 0) {
				nLength = dataLogerLogic_getExcel(postHeaderData.canID, msgId,
						bufferSend);
				nLength = strlen(bufferSend);
			} else
				return -1;

		}
		//////////// LEFT MENU ///////////////////////
		else if (strcmp(postHeaderData.formID, "lm") == 0) { // left menu
			nLength = treeNodes_createMenu(bufferSend, loginState - 1);
		}
		//////////// TRAFOGUARD ///////////////////////
		else if (strcmp(postHeaderData.formID, "tg_s") == 0) {
			tgSetup tgSetupData;
			tgSetupParser_getStructFromRequest(&bufferRec[nLength],
					&tgSetupData);


			if (postHeaderData.command == 'w') {
				//////////////////////////////////////////////
				////// CHECK IF CHANGED ID. CANNOT USE EXISTIN ID
				//////////////////////////////////////////////
				if (postHeaderData.canID != tgSetupData.canBusAddr) {
					if (deviceLinkedList_getDevice(
							tgSetupData.canBusAddr) != NULL) {
						strcpy(bufferSend,
								"<err1>Cannot change CAN address, address in use</err1>");
						socketClientPipe_write(conn_s, bufferSend,
								strlen(bufferSend));
						return 1;
					}
				}

				tempChar = canInterface_setTgData(&tgSetupData, postHeaderData.canID);

				if (tempChar == CAN_OK) {
					if (postHeaderData.canID != tgSetupData.canBusAddr) {
						//////////////////////////////////////////////
						////// CHANGED ID !!!
						//////////////////////////////////////////////
						char unitName[60];
						int unitOldCanId = postHeaderData.canID;

						strcpy(unitName, tgSetupData.location);
						// MUTEX ( for thread from main)
						socketClientPipe_setMutex();
						strcpy(bufferSend,
								"<err>System updating, please wait until page refresh</err>");
						nLength =
								strlen(
										"<err>System updating, please wait until page refresh</err>");

						printf("sending back: %d bytes\n", nLength);
						socketClientPipe_write(conn_s, bufferSend, nLength);


						gwDataLogerUtility_changeCanID(unitOldCanId, //postHeaderData.canID,
								tgSetupData.canBusAddr);

						deviceLinkedList_removeDevice(unitOldCanId);//postHeaderData.canID);

						sleep(10); // time for can bus reset

						canInterface_searchDevices();

						gwDevMonitorUtility_updateDevices(); // remove disabled devices which are not found on CAN BUS
						gwSetupDeviceMonitor_createPage();
						devMonitorSaver_saveToFile();

						char tmp1[120];

						sprintf(tmp1, "NCAN bus address %u changed to address %u",
								unitOldCanId /*postHeaderData.canID*/, tgSetupData.canBusAddr);

						gwAlarmLogUtility_saveAndSendAlarmMsg(
								tgSetupData.canBusAddr, unitName, //tgSetupData.location,
								tmp1);

						socketClientPipe_clearMutex();
						return 1;
					}
				} else {
					socketClientPipe_createErrorMsg(bufferSend, &nLength,
							tempChar);
				}
			}

			tempChar = canInterface_getTgData(&tgSetupData,
					postHeaderData.canID);
			if (tempChar == CAN_OK) {
				nLength = tgSetupParser_getXMLfromStruct(&tgSetupData,
						bufferSend);
			} else {
				socketClientPipe_createErrorMsg(bufferSend, &nLength, tempChar);
			}
		} else if (strcmp(postHeaderData.formID, "tg_m") == 0) {
			tgMes tgMesData;
			tempChar = canInterface_getTgMesData(&tgMesData,
					postHeaderData.canID);
			if (tempChar == CAN_OK) {
				nLength = tgMesParser_getXMLfromStruct(&tgMesData, bufferSend);
			} else {
				socketClientPipe_createErrorMsg(bufferSend, &nLength, tempChar);
			}
		}
		/////////////////////////////////////////////////////////////////
		//////////////////    IL    /////////////////////////////////////
		/////////////////////////////////////////////////////////////////
		else if (strcmp(postHeaderData.formID, "il_s") == 0) {
			ilSetup ilSetupData;

			if (postHeaderData.command == 'w') {
				ilSetupParser_getStructFromRequest(&bufferRec[nLength],
						&ilSetupData);

				//////////////////////////////////////////////
				////// CHECK IF CHANGING ID. CANNOT USE EXISTIN ID
				//////////////////////////////////////////////
				if (postHeaderData.canID != ilSetupData.canBusAddr) {
					if (deviceLinkedList_getDevice(
							ilSetupData.canBusAddr) != NULL) {
						strcpy(bufferSend,
								"<err1>Cannot change CAN address, address in use</err1>");
						socketClientPipe_write(conn_s, bufferSend,
								strlen(bufferSend));
						return 1;
					}
				}

				tempChar = canInterface_setIlData(&ilSetupData,
						postHeaderData.canID);
				if (tempChar == CAN_OK) {
					if (postHeaderData.canID != ilSetupData.canBusAddr) {
						//////////////////////////////////////////////
						////// CHANGED ID !!!
						//////////////////////////////////////////////

						// MUTEX
						 socketClientPipe_setMutex();

						strcpy(bufferSend,
								"<err>System updating, please wait until page refresh</err>");
						nLength =
								strlen(
										"<err>System updating, please wait until page refresh</err>");
						printf("sending back: %d bytes: ", nLength);
						socketClientPipe_write(conn_s, bufferSend, nLength);


						gwDataLogerUtility_changeCanID(postHeaderData.canID,
								ilSetupData.canBusAddr);

						deviceLinkedList_removeDevice(postHeaderData.canID);


						sleep(10); // time for can bus reset

						canInterface_searchDevices();

						gwDevMonitorUtility_updateDevices(); // remove disabled devices which are not found on CAN BUS
						gwSetupDeviceMonitor_createPage();
						devMonitorSaver_saveToFile();

						char tmp1[120];

						sprintf(tmp1,
								"NCAN bus address %u changed to address %u",
								postHeaderData.canID, ilSetupData.canBusAddr);
						gwAlarmLogUtility_saveAndSendAlarmMsg(
								ilSetupData.canBusAddr, ilSetupData.location,
								tmp1);
						socketClientPipe_clearMutex();
						return 1;
					}

				} else {
					socketClientPipe_createErrorMsg(bufferSend, &nLength,
							tempChar);
					socketClientPipe_write(conn_s, bufferSend, nLength);
					return 1;
				}
			}

			tempChar = canInterface_getIlData(&ilSetupData,
					postHeaderData.canID);
			if (tempChar == CAN_OK) {
				nLength = ilSetupParser_getXMLfromStruct(&ilSetupData,
						bufferSend);
			} else {
				socketClientPipe_createErrorMsg(bufferSend, &nLength, tempChar);
			}
		} else if (strcmp(postHeaderData.formID, "il_m") == 0) {
			ilMes ilMesData;
			tempChar = canInterface_getIlMesData(&ilMesData,
					postHeaderData.canID);
			if (tempChar == CAN_OK) {
				nLength = ilMesParser_getXMLfromStruct(&ilMesData, bufferSend);
			} else {
				socketClientPipe_createErrorMsg(bufferSend, &nLength, tempChar);
			}
		}
		//////////////////////////////////////////////
		////// SWITCHOVER UNIT
		//////////////////////////////////////////////
		else if (strcmp(postHeaderData.formID, "sw_s") == 0) {
			swSetup swSetupData;
			if (postHeaderData.command == 'w') {
				swSetupParser_getStructFromRequest(&bufferRec[nLength],
						&swSetupData);

				//////////////////////////////////////////////
				////// CHECK IF CHANGING ID. CANNOT USE EXISTIN ID
				//////////////////////////////////////////////
				if (postHeaderData.canID != swSetupData.canBusAddr) {
					if (deviceLinkedList_getDevice(
							swSetupData.canBusAddr) != NULL) {
						strcpy(bufferSend,
								"<err1>Cannot change CAN address, address in use</err1>");
						socketClientPipe_write(conn_s, bufferSend,
								strlen(bufferSend));
						return 1;
					}
				}

				tempChar = canInterface_setSwData(&swSetupData,
						postHeaderData.canID);
				if (tempChar == CAN_OK) {
					if (postHeaderData.canID != swSetupData.canBusAddr) {
						//////////////////////////////////////////////
						////// CHANGED ID !!!
						//////////////////////////////////////////////

						// MUTEX
						socketClientPipe_setMutex();

						strcpy(bufferSend,
								"<err>System updating, please wait until page refresh</err>");
						nLength =
								strlen(
										"<err>System updating, please wait until page refresh</err>");
						printf("sending back: %d bytes: ", nLength);
						socketClientPipe_write(conn_s, bufferSend, nLength);


						gwDataLogerUtility_changeCanID(postHeaderData.canID,
								swSetupData.canBusAddr);

						deviceLinkedList_removeDevice(postHeaderData.canID);

						sleep(10); // time for can bus reset

						canInterface_searchDevices();

						gwDevMonitorUtility_updateDevices(); // remove disabled devices which are not found on CAN BUS
						gwSetupDeviceMonitor_createPage();
						devMonitorSaver_saveToFile();

						char tmp1[120];

						sprintf(tmp1,
								"NCAN bus address %u changed to address %u",
								postHeaderData.canID, swSetupData.canBusAddr);
						gwAlarmLogUtility_saveAndSendAlarmMsg(
								swSetupData.canBusAddr, swSetupData.location,
								tmp1);
						socketClientPipe_clearMutex();
						return 1;
					}

					tempChar = canInterface_getSwData(&swSetupData,
							swSetupData.canBusAddr);
					if (tempChar == CAN_OK) {
						nLength = swSetupParser_getXMLfromStruct(&swSetupData,
								bufferSend);
					} else {
						socketClientPipe_createErrorMsg(bufferSend, &nLength,
								tempChar);
					}
				} else {
					socketClientPipe_createErrorMsg(bufferSend, &nLength,
							tempChar);
					socketClientPipe_write(conn_s, bufferSend, nLength);
					return 1;
				}
			} else {
				tempChar = canInterface_getSwData(&swSetupData,
						postHeaderData.canID);
				if (tempChar == CAN_OK) {
					nLength = swSetupParser_getXMLfromStruct(&swSetupData,
							bufferSend);
				} else {
					socketClientPipe_createErrorMsg(bufferSend, &nLength,
							tempChar);
				}
			}
		} else if (strcmp(postHeaderData.formID, "sw_m") == 0) {
			swMes swMesData;
			tempChar = canInterface_getSwMesData(&swMesData,
					postHeaderData.canID);
			if (tempChar == CAN_OK) {
				nLength = swMesParser_getXMLfromStruct(&swMesData, bufferSend);
			} else {
				socketClientPipe_createErrorMsg(bufferSend, &nLength, tempChar);
			}
		}
		/////////////////////////////////////////////////////////////////
		///////// 			ALARM PANEL  		/////////////////////////
		/////////////////////////////////////////////////////////////////
		else if (strcmp(postHeaderData.formID, "ap_s") == 0) {
			apSetup apSetupData;

			if (postHeaderData.command == 'w') {
				apUtility_getStructFromRequest(&bufferRec[nLength],
						&apSetupData);

				//////////////////////////////////////////////
				////// CHECK IF CHANGING ID. CANNOT USE EXISTIN ID
				//////////////////////////////////////////////
				if (postHeaderData.canID != apSetupData.canBusAddr) {
					if (deviceLinkedList_getDevice(
							apSetupData.canBusAddr) != NULL) { //postHeaderData.canID) != NULL) {
						strcpy(bufferSend,
								"<err1>Cannot change CAN address, address in use</err1>");
						socketClientPipe_write(conn_s, bufferSend,
								strlen(bufferSend));
						return 1;
					}
				}

				tempChar = canInterface_setApData(&apSetupData,
						postHeaderData.canID);
				if (tempChar == CAN_OK
						|| postHeaderData.canID != apSetupData.canBusAddr) { // !!! ""or" added 2014.02.21 (missing answer when changing ID)
					if (postHeaderData.canID != apSetupData.canBusAddr) {
						//////////////////////////////////////////////
						////// CHANGED ID !!!
						//////////////////////////////////////////////

						// MUTEX
						socketClientPipe_setMutex();

						strcpy(bufferSend,
								"<err>System updating, please wait until page refresh</err>");
						nLength =
								strlen(
										"<err>System updating, please wait until page refresh</err>");
						printf("sending back: %d bytes: ", nLength);
						socketClientPipe_write(conn_s, bufferSend, nLength);

						// 2014.02.20: We are not logging for AP or SP devices. Don't needs following line
						//gwDataLogerUtility_changeCanID(postHeaderData.canID,
						//	apSetupData.canBusAddr);

						deviceLinkedList_removeDevice(postHeaderData.canID);

						//sleep(10); // time for can bus reset  // Removed 2014.02.23:
						//timeToStartDevicesRefresh = 12; // refresh devices after 12 sec. //2014.02.23
						//canInterface_searchDevices();  // Removed 2014.02.23:

						gwDevMonitorUtility_updateDevices(); // remove disabled devices which are not found on CAN BUS
						gwSetupDeviceMonitor_createPage();
						devMonitorSaver_saveToFile();

						char tmp1[120];

						sprintf(tmp1,
								"NCAN bus address %u changed to address %u",
								postHeaderData.canID, apSetupData.canBusAddr);
						gwAlarmLogUtility_saveAndSendAlarmMsg(
								apSetupData.canBusAddr, apSetupData.location,
								tmp1);
						socketClientPipe_clearMutex();
						return 1;
					}
				} else {
					socketClientPipe_createErrorMsg(bufferSend, &nLength,
							tempChar);
					socketClientPipe_write(conn_s, bufferSend, nLength);
					return 1;
				}
			}

			tempChar = canInterface_getApData(&apSetupData,
					postHeaderData.canID);
			if (tempChar != CAN_OK) {
				printf("**** ERROR setApData");
				socketClientPipe_createErrorMsg(bufferSend, &nLength, tempChar);
				socketClientPipe_write(conn_s, bufferSend, nLength);
				return 1;
			}
			nLength = apUtility_getXMLfromStruct(&apSetupData, bufferSend);
		} //////////  MOD BUS ////////////////////////////////
		else if (strcmp(postHeaderData.formID, "gw_mb") == 0) {
			GW_MODBUS gwMB;
			gwModBus_clearStruct(&gwMB);
			if (postHeaderData.command == 'w') {
				gwModBus_getStructFromRequest(&bufferRec[nLength], &gwMB, "&");
				gwModBus_saveToFile(gwMB);
				if (gwMB.gw_e == 1) {
					if (modBusInterf_start("/dev/ttyS1", gwMB.br, gwMB.p,
							gwMB.d, gwMB.s, gwMB.add) != 0) {
						gwAlarmLogUtility_saveAndSendAlarmMsg(0, "ModBus",
								"Cannot start ModBus");
						fprintf(stderr,
								"Critical error, cannot start ModBus\n");
					}
				} else {
					modBusInterf_stop();
				}
			}

			gwModBus_readFromFile(&gwMB); // just update unit name
			nLength = gwModBus_getXMLfromStruct(&gwMB, bufferSend);
		} //////////////// DATA PARAMETER (time) /////////////////
		else if (strcmp(postHeaderData.formID, "gw_dp") == 0) {
			GW_TIME* gwTimeData = getTimeStruct();

			if (postHeaderData.command == 'w') {
				gwDevParParser_clearStruct(gwTimeData);

				fprintf(stderr, "W time\n");
				gwDevParParser_getStructFromRequest(&bufferRec[nLength],
						gwTimeData);

				myString_convertUTF8toCAN(gwTimeData->name_UTF8_format, gwTimeData->name_CAN_format, 49);
				strcpy((char*) ObjDict_obj2002, gwTimeData->name_CAN_format);

				gwParamSaver_saveToFile();

				//fprintf(stderr, "gw_dp saved\n");

				if (gwTimeData->gwNewDate.day != -1) {
					sysTime_set(gwTimeData->gwNewDate.year,
							gwTimeData->gwNewDate.month,
							gwTimeData->gwNewDate.day, gwTimeData->gwNewTime.hour,
							gwTimeData->gwNewTime.min, gwTimeData->gwNewTime.sec);
					//fprintf(stderr, "gw_dp get\n");
					sysTime_get(gwTimeData);
				}

				gwParamSaver_readFromFile();// just update unit name
				//getTimeStruct(&gwTimeData); // just update unit name
				//fprintf(stderr, "gw_dp read\n");
				nLength = gwDevParParser__getXMLfromStruct(gwTimeData,
						bufferSend, 'w');
			} else {
				//fprintf(stderr, "timer or read\n");
				sysTime_get(gwTimeData); // update time
				//strcpy(gwTimeData.name_CAN_format, (char*) ObjDict_obj2002);
				//getTimeStruct(&gwTimeData); // just update unit name
				nLength = gwDevParParser__getXMLfromStruct(gwTimeData,
						bufferSend, postHeaderData.command);
			}
			// device info
		} else if (strcmp(postHeaderData.formID, "gw_di") == 0) {
			GW_TIME* gwTimeData = getTimeStruct();

			//gwParamSaver_readFromFile(&gwTimeData);
			sprintf(bufferSend,
					"<loc>%s</loc><canAdr>127</canAdr><vers>%s</vers>",
					gwTimeData->name_UTF8_format, softwareVersion);
			nLength = strlen(bufferSend);
		} else if (strcmp(postHeaderData.formID, "gw_em1") == 0) { // Button save e-mail
			GW_EMAIL gwEmailData;
			//gwEmailUtility_clearStruct(&gwEmailData);

			if (postHeaderData.command == 'w') {
				gwEmailUtility_clearStruct(&gwEmailData);
				gwEmailUtility_updateStructFromBuffer(&bufferRec[nLength],
						&gwEmailData, "&");
				gwEmail_saveStruct(gwEmailData);
				smtpConfig_save(gwEmailData.ssmtp);

				myMail_init(&gwEmailData);

				gwEmail_readStruct(&gwEmailData);
				if (gwEmailData.ssmtp.user[0] != 0) {
					if (strlen(gwEmailData.ssmtp.user) > 0) {
						gwEmailUtility_saveRevaliases(gwEmailData);
					}
				} else {
					//printf("ssmtp user null\n");
				}
				nLength = gwEmail_getXMLfromStruct(&gwEmailData, bufferSend);
			} else {
				gwEmail_readStruct(&gwEmailData);
				nLength = gwEmail_getXMLfromStruct(&gwEmailData, bufferSend);
			}
		} else if (strcmp(postHeaderData.formID, "gw_em2") == 0) { // Button Test e-mail
			char mailErrCode[300];
#ifdef DEBUG_CLIENT
			printf(stderr, "SEND test e-mail\n");
#endif
			/*GW_EMAIL gwEmailData;
			 gwEmailUtility_clearStruct(&gwEmailData);
			 gwEmail_readStruct(&gwEmailData);
			 char dest[300];
			 sprintf(dest, "%s %s %s %s %s", gwEmailData.recip1,
			 gwEmailData.recip2, gwEmailData.recip3, gwEmailData.recip4,
			 gwEmailData.recip5);
			 */

			char tempSubject[200];

			sprintf(tempSubject, "Test e-mail from IMED Secure, IP: %s",
					ip_getOwnIp());
			printf("mail subject: %s\n", tempSubject);
			char eRes = email_sendToAll(tempSubject, EMAIL_TEST_FILE_PATH,
					mailErrCode, 300);

			if (eRes == SMTP_OK) {
				//if (email_it(dest, "test from Gateway", EMAIL_TEST_FILE_PATH) == 0) {
				strcpy(bufferSend, "<err>Success, mail sent</err>");
				nLength = strlen("<err>Success, mail sent</err>");
			} else if (eRes == MAIL_DISABLED) {
				strcpy(bufferSend, "<err>Cannot send e-mail, disabled</err>");
				nLength = strlen("<err>Cannot send e-mail, disabled</err>");
			} else {
				sprintf(bufferSend, "<err>%s</err>", mailErrCode);
				nLength = strlen(bufferSend);
				//strcpy(bufferSend, "<err>Error sending mail</err>");
				//nLength = strlen("<err>Error sending mail</err>");
			}

			socketClientPipe_write(conn_s, bufferSend, nLength);
			return 1;
		} //////////////////////////////////////////////////////////
		  //  		DHCP
		  ///////////////////////////////////////////////////////////
		else if (strcmp(postHeaderData.formID, "gw_sn") == 0) {
			GW_DHCP dhcp;

			gwDhcpUtility_clearStruct(&dhcp);

			if (postHeaderData.command == 'w') {
				gwDhcpUtility_updateStructFromBuffer(&bufferRec[nLength], &dhcp,
						"&");
				gwDhcpUtility_saveStruct(dhcp);
				/////////////////////////////////////////////////////
				// needs for DNS functionality (gateway is DNS)
				gwDhcpUtility_readStruct(&dhcp);
				dhcpConfig_readFromFile(&dhcp); // read DHCP host name

				nLength = gwDhcpUtility_getXMLfromStruct(&dhcp, bufferSend);
			} else if (postHeaderData.command == 'r') {
				gwDhcpUtility_readStruct(&dhcp);
				dhcpConfig_readFromFile(&dhcp); // read DHCP host name
				nLength = gwDhcpUtility_getXMLfromStruct(&dhcp, bufferSend);
			}
		} else if (strcmp(postHeaderData.formID, "gw_ps") == 0) {
			GW_PASS gwPassNew;
			GW_PASS gwPassOld;
			GW_PASS gwPassCorrect;
			gwPassUtility_clearStruct(&gwPassNew);
			gwPassUtility_clearStruct(&gwPassOld);
			gwPassUtility_clearStruct(&gwPassCorrect);

			if (postHeaderData.command == 'w') {
				gwPassUtility_updateStructFromBuffer(&bufferRec[nLength],
						&gwPassOld, &gwPassNew, "&");
				gwPassUtility_readStruct(&gwPassCorrect);

				if (strcmp(gwPassOld.lpass1, gwPassCorrect.lpass1) == 0
						&& strcmp(gwPassOld.apass1, gwPassCorrect.apass1)
								== 0) {
					gwPassUtility_saveStruct(&gwPassNew);

					nLength = gwPassUtility_getXMLfromStruct(&gwPassNew,
							bufferSend);
				} else {
					strcpy(bufferSend, "<ERR>Wrong password</ERR>");
					nLength = strlen("<ERR>Wrong password</ERR>");
				}
			} else if (postHeaderData.command == 'r') {
				//gwPassUtility_readStruct(&gwPass);
				nLength = gwPassUtility_getXMLfromStruct(&gwPassOld,
						bufferSend);
			}
		} else if (strcmp(postHeaderData.formID, "gw_dm") == 0) {
			if (postHeaderData.command == 'w') {
				gwDevMonitorUtility_updateListFromWebBuffer(&bufferRec[nLength],
						"&");
				devMonitorSaver_saveToFile();
				nLength = gwDevMonitorUtility_getXML(bufferSend);
			} else {
				nLength = gwDevMonitorUtility_getXML(bufferSend);
			}
		} else if (strcmp(postHeaderData.formID, "gw_dl") == 0) {  // Get log configuration
			if (postHeaderData.command == 'w') {
				DEV_MEASSUR_TASK dataLogerStruct[MAX_LOG_TASKS];
				gwDataLogerUtility_updateStructFromBuffer(&bufferRec[nLength],
						(DEV_MEASSUR_TASK*) &dataLogerStruct, "&");
				dataLogerLogic_onChangedStruct(dataLogerStruct);
				dataLogerSaver_saveToFile((DEV_MEASSUR_TASK*) &dataLogerStruct);
				gwDataLogerUtility_readStruct(); // read from file
				DEV_MEASSUR_TASK* pdataLogerStruct =
						gwDagaLogerUtility_getStruct();
				nLength = gwDataLogerUtility_getXMLfromStruct(pdataLogerStruct,
						bufferSend);
			} else {
				DEV_MEASSUR_TASK* pdataLogerStruct =
						gwDagaLogerUtility_getStruct();
				nLength = gwDataLogerUtility_getXMLfromStruct(pdataLogerStruct,
						bufferSend);
			}
		} else if (strcmp(postHeaderData.formID, "gw_wc") == 0) {
			strcpy(bufferSend, "<ERR>OK password</ERR>");
			nLength = strlen("<ERR>OK password</ERR>");
		} else if (strcmp(postHeaderData.formID, "gwUrl") == 0) {
			GW_HELP gwHelp;
			if (postHeaderData.command == 'w') {
				gwHelpUtility_updateStructFromBuffer(&bufferRec[nLength],
						(GW_HELP*) &gwHelp, "&");
				helpFile_saveToFile((GW_HELP*) &gwHelp);
				gwHelpUtility_readStruct(&gwHelp); // read from file
				nLength = gwHelpUtility_getXMLfromStruct(&gwHelp, bufferSend);
			} else {
				gwHelpUtility_readStruct(&gwHelp); // read from file
				nLength = gwHelpUtility_getXMLfromStruct(&gwHelp, bufferSend);
			}
		} else if (strcmp(postHeaderData.formID, "help") == 0) {
			GW_HELP gwHelp;
			gwHelpUtility_readStruct(&gwHelp); // read from file
			sprintf(bufferSend,
					"\"%s\" target=\"_blank\">here</a><br/> Get support by e-mail <a href=\"mailto:%s\"",
					gwHelp.u, gwHelp.m);
			nLength = strlen(bufferSend);
		} else if (strcmp(postHeaderData.formID, "gw_am") == 0) {    // GET ALARM FILES
			if (postHeaderData.command == 'd') {
				gwAlarmLogUtility_deleteAllAlarmMsgs();
			}
			gwAlarmLogUtility_getAlarmMsgs(bufferSend, MAX_RESPONSE_LENGTH);

			nLength = strlen(bufferSend);
		}

		//fprintf(stderr, bufferSend);
		int nSent;
		nSent = socketClientPipe_write(conn_s, bufferSend, nLength);
		printf("Sent %d bytes\n", nSent);
		return 1;
	}

	return -1;
}

void socketClientPipe_createErrorMsg(char* buffer, int* nLength, char canResult) {
	if (canResult == CAN_BUSY) {
		strcpy(buffer, "<err>NCANBUS busy. Please try again later</err>");
		*nLength = strlen("<err>NCANBUS busy. Please try again later</err>");
	} else {
		strcpy(buffer, "<err>NCANBus error</err>");
		*nLength = strlen("<err>NCANBus error</err>");
	}
	*nLength = strlen(buffer);
}

ssize_t socketClientPipe_write(int sockd, const void *vptr, size_t nSize) {
	ssize_t nwritten;
	ssize_t nWrittenTot = 0;
	const void * buffer = vptr;

/*
 * char length[2];
	length[0] = nSize / 256;
	length[1] = nSize % 256;*/

	if (sockd < 0 || soketOk == 0) {
		printf("!!! Cannot write to socket. Closed\n");
		return 0;
	}

	char length[3];
	size_t nTempSize = nSize;

	length[0] = nTempSize / (256*256);
	nTempSize = nTempSize - length[0]*(256*256);
	length[1] = nTempSize / 256;
	length[2] = nTempSize - length[1] * 256;

	write(sockd, length, 3); // old: 2

	while (nSize > 0) {
		if ((nwritten = write(sockd, buffer, nSize)) <= 0) {
			if (errno == EINTR)
				nwritten = 0;
			else
				return -1;
		}
		nWrittenTot += nwritten;
		nSize -= nwritten;
		buffer += nwritten;
	}

	return nWrittenTot;
}

