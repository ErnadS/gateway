#include <arpa/inet.h>        /*  inet (3) funtions         */
#include <unistd.h>           /*  misc. UNIX functions      */
#include <netdb.h> /* gethostname, et al */
#include <sys/param.h> /* MAXHOSTNAMELEN */
#include "utility/MyGpio.h"
#include <sys/stat.h>
#include "services/watchDogInterface.h"

#include "webModule/socketServer.h"


#include <stdio.h>
#include <fcntl.h>
//#include "pipeHandler.h"

#include "can/canInterface.h"
#include <pthread.h>

#include "services/mail.h"
#include "unitGW/utility/emailConfig/emailConfig.h"
#include "unitGW/utility/gwEmailParser.h"
#include "unitGW/utility/dhcpConfig/dhcpConfigFile.h"
#include "unitGW/utility/passFile/passFile.h"
#include "unitGW/utility/urlHelpConfig/helpFile.h"
#include "unitGW/utility/gwDhcpUtility.h"

// DYNAMIC PAGE
#include "unitGW/dynamic/gwSetupDeviceMonitor.h"
//#include "unitGW/dynamic/treeNodes.h"

#include "unitGW/model/device.h"
#include "unitGW/gateway.h"
#include "unitGW/utility/devMonitor/devMonitorSaver.h"
#include "unitGW/utility/dataLoger/dataLogerSaver.h"
#include "unitGW/utility/gwDataLogerUtility.h"
#include "unitGW/utility/gwDevMonitorUtility.h"
#include "unitGW/utility/gwModBus.h"
#include "services/modBusInterface/modBusInterf.h"

#include "unitGW/model/linkedList/deviceLinkedList.h"
#include "loginLogic.h"
#include "log/dataLogerLogic.h"
#include <string.h>
#include "unitGW/utility/gwAlarmLogUtility.h"
#include "unitGW/utility/gwParConfig/gwParamSaver.h"
// CAN BUS
#include "can/can_AVR.h"
#include "can/ncanbus.h"
#include "can/sdo.h"
#include "can/data.h"
#include "can/ObjDict.h"

#include "utility/ip.h"

#include "loginLogic.h"
#include "can/lifegrd.h"
#include <syslog.h>

#include "services/modBusInterface/modbus.h"
#include "services/myMail.h"
#include "services/UART/serial.h"
#include "services/modBusInterface/modbus-private.h"

#include <time.h>  // eno test
#include <sys/time.h>

#include "utility/myString.h"
#include "utility/gpioPowerLed.h"
#include "Alarm.h"
#include "services/myMail.h"
#include "log/dataLogerLogic.h"


/////////////////////////////////////////////////////////////////////
char softwareVersion[] = {"R.2.11"};

////////////////////////////////////////////////////
// RELEASE NOTES:
// version 1.10 was used by Noratel x.x.2010 - 24.sep.2011
//
// version 1.11 from 24.sep.2011
// Changed myMail.c. Widows SMTP server expect "\r\n" after commands
//
// version 2.00 from 12.mars.2012
// Created new kernel (with gpio) and RFS. Backup of Virtual Linux and XP
// Norwegian characters, factory restet button, version and bug fixing

/////////////////////////////////////////////////////////////////////
// Revision history:
/////////////////////////////////////////////////////////////////////

//#############################################
// R.2.01:
//#############################################
// 1. changed Linux kernel (GPIO support)
// 2. new RootFileSystem
// 3. factory reset button (if pushed > 3sec => IP address and password to default)
// 4. max 4 logging at time (data logger setup)
// 5. Norwegian characters
// 6. support for Google Chrome and Firefox

//#############################################
// R.2.02:
//#############################################
// 1. Alarm messages bug and Chrome and Firefox support
// ("gw_alarmMsgs.htm")
// 2. Changed factory reset functionality (delete alarm
// messages)
// 3. Power-LED blinking when factory reset

//#############################################
// R.2.03:
//#############################################
// Thttpd was logging to much.
// Changed thttpd.c (new line):
// setlogmask(LOG_MASK(LOG_WARNING));

//#############################################
// R.2.05:  22.10.2012 (endringer ble gjort 20 dager tidligere men i dag
// endret versjon.
//
// Endret CAN bus driver pga bedre håndtering av error og mer logging i driver
// Endret cgiLog project for å sende html og ikke Excel
// Endret denne pga. version nummer.
// Noe mer?
//#############################################

//#############################################
// R.2.06:  23.10.2012
//
// "Time Date" endret til "Date Time" i log tabel
//#############################################

//#############################################
// R.2.07:  05.01.2013
//
// 1. gatewayDataLoger.h endret til 1000 linjer:
//#define MAX_LOG_LINES  1000 //200
// 2. dataLogerLogic.c :
// ny metode: "dataLogerLogic_onChangedStruct()" for å slette gamle
// meldinger hvis en log-task er fjernet
// 3. cgiSwitchover:
// changed "mainSwitchover.c" one line (missing header)
// 4. changed gwSetupDataloger.html
//#############################################


//#############################################
// R.2.08:  10.02.2013
//
// 1. added "æ" (was replaced with "Æ" in previous versions)
// 2. myMail_sendMail (sprintf(tempBuf, "Content-Type: text/plain; charset=UTF-8)
// 3. dataLogerLogic_onChangedStruct()  // new line when changed loging task

//#############################################


//#############################################
// R.2.09:  03.04.2013
//
// missing "unlock" in canInterface line 576
//#############################################

//#############################################
// R.2.10:  20.02.2014
// 1. error if changing CAN ID. Here is changed (in all "socketClientPipe.c"):
// if( deviceLinkedList_getDevice(apSetupData.canBusAddr) != NULL) { //postHeaderData.canID) != NULL) {
// 2. Changed tree_ap_setup.js // alarmPanel <->AlarmPanelSlave
// 3. Changed cgiAlarmPanel // alarmPanel <->AlarmPanelSlave
// 4. fixed problem with norwegian chars (saving to alarm -file and e-mail)
// 5. Time (each second) stopped if not answer in 10 sec
//#############################################

//#############################################
// R.2.11:  13.04.2014
// Support for Finnish language
///////////////////////////////////////////////

void * startServer(void *ptr);
//void * canThreadTask(void *ptr);
void * runTick(void *ptr);

//void CheckReadSDO(CO_Data* d, unsigned char nodeid);




int main(void) {
	int nButtonCounter = 0;

	pthread_t thread1;
	pthread_t thread2;
	pthread_t threadCAN;
	pthread_t threadAlarmHandler;
	pthread_t threadMail;

	int ret;
	int i = 0;
	char *message1 = "Thread 1";
	char* heardBeatTemp;
	char* mainHeardBeat;
	char defaultGateway[20];
	char pingCommand[40];

	unsigned int nMeasFilesDeleteCount = 0;

	char softVerString[100];

	sprintf(softVerString, "********* version %s ************", softwareVersion);


	openlog("GW", LOG_PID, LOG_USER);
	setlogmask(LOG_MASK(LOG_ERR));


	syslog(LOG_ERR, "****************************************");
	syslog(LOG_ERR, "********  GATEWAY START  ***************");
	syslog(LOG_ERR, softVerString);


	printf("********  GATEWAY START  ***************\n");
	printf(softVerString);

	ip_updateIpInfo();
	syslog(LOG_ERR, "**MY IP is: %s\n", ip_getOwnIp());
	printf("**MY IP is: %s\n", ip_getOwnIp());
	/////////////////////////////////////////////////
	// calculate default gateway
	strcpy(defaultGateway, ip_getOwnIp());
	char* tempChar = strstr(defaultGateway, ".");
	tempChar = strstr(++tempChar, ".");
	tempChar = strstr(++tempChar, ".");
	tempChar++;
	*tempChar = '1';
	tempChar++;
	*tempChar = 0;

	sprintf(pingCommand, "ping -c 1 %s", defaultGateway);
	/////////////////////////////////////////////////
	// MAIL TEST:
/*	char mailErrCode[300];
	int nMailResult = email_sendToAll("test", "/gateway/alarm/a_2013_02_08_11_52_46_0", mailErrCode, 300);
		if (nMailResult != SMTP_OK) {
			if (nMailResult == MAIL_DISABLED)
				printf("Cannot send mail, disabled\n");
			else
				printf("Err sending mail: %s\n", mailErrCode);
		} else {
			printf("E-mail sent\n");
		}

*/
	FILE * hFile;
	hFile = fopen(MEASURE_FOLDER, "r");
	if (hFile == NULL) { // FILE NOT EXIST, CREATE
		//struct stat st;
		//if (stat("/gateway", &st) != 0) {
			mkdir(MEASURE_FOLDER, 0777);
		//}
	}
	if (hFile != NULL) {
		fclose(hFile);
	}

	//////////////////////////////////////////////////

	strcpy((char*) ObjDict_obj2003, ip_getOwnIp());
	//watchDogInterface_start(); // TODO: will be used

	gwParamSaver_init();

	canInit(); // !!! Starter driver. Must be called before thread start
	canInterface_init();
	initTimer();
	ret = pthread_create(&threadCAN, NULL, canInterface_canThreadTask,
			(void*) NULL);

	gateway_init();
	emailConfig_init();
	GW_EMAIL gwMail;
	gwEmail_readStruct(&gwMail);
	myMail_init(&gwMail);

	dhcpConfig_init();
	passFile_init();
	loginLogic_init();
	helpFile_init();

	////////////////////////////////////////////////
	/// MOD BUS MASTER SIMULATOR
	////////////////////////////////////////////////
	/*modbus_t* mb;
	int rc;
	//mb = modbus_new_rtu("/dev/ttyS1", 9600, 'N', 8, 1, 7);
	printf("ENO TEST\n");
	mb = modbus_new_rtu("/dev/ttyS1", 9600, 'N', 8, 1, 7);
	mb->debug = 1;
	rc = modbus_connect(mb);
	if (rc != 0) {
		printf("MODBUS error opening ttyS1\n");
	}
	unsigned char tab_reg[10];
	modbus_set_slave(mb, 2);
	while (1) {
		sleep(1);
	rc = modbus_read_bits(mb, 0, 1, tab_reg);
	if (rc <= 0) {
		printf("MODBUS read failed\n");
	} else {
		if (rc == 1) {
			if (tab_reg[0] == 0) {
				printf("MODBUS: NOT ALARM\n");
			} else {
				unsigned char newValue = 0;
				printf("!!!! MODBUS: ALARM\n");
				rc = modbus_write_bits(mb, 0, 1, &newValue);
				if (rc == 1)
					printf("!!!! MODBUS SENT CLEAR ALARM\n");
				else
					printf("!!!! MODBUS SENT CLEAR ALARM but received: %u bytes\n", rc);
			}
		}
	}
	}
*/
	////////////////////////////////////////////////

	////////////////////////////////////////////////
	// MODBUS INIT
	////////////////////////////////////////////////
	GW_MODBUS gwModBus;
	gwModBus_init();
	gwModBus_clearStruct(&gwModBus);
	gwModBus_readFromFile(&gwModBus);
	if (gwModBus.gw_e == 1) {
		//"/dev/ttyS1", 19200, 'N', 8, 1
		if (modBusInterf_start("/dev/ttyS1", gwModBus.br, gwModBus.p,
				gwModBus.d, gwModBus.s, gwModBus.add) != 0) {
			gwAlarmLogUtility_saveAndSendAlarmMsg(i, "ModBus", "Cannot start ModBus");
			syslog(LOG_ERR, "Critical error, cannot start ModBus\n");
		}
	} else {
		syslog(LOG_ERR, "ModBus is not activated\n");
	}
	///////////////////////////////////////////////

	devMonitorSaver_updateDevicesFromFile();

	canInterface_searchDevices();
	gwDevMonitorUtility_updateDevices(); // remove disabled devices which are not found on CAN BUS
	gwSetupDeviceMonitor_createPage();

	dataLogerSaver_init();
	gwDataLogerUtility_readStruct(); // read from file
	dataLogerLogic_init();

	///////////////////////////////////////////////
	int nSize = deviceLinkedList_getSize();
	printf("Found %u devices\n", nSize);

	DEV * dev;

	for (i = 0; i < nSize; i++) {
		dev = deviceLinkedList_getElementAt(i);
		if (dev != NULL && dev->type != AP) {
			canInterface_checkOldAlarm(dev->adr);
		}
	}
	/////////////////////////////////////////////////

	ret = pthread_create(&thread1, NULL, startServer, (void*) message1);
	ret = pthread_create(&thread2, NULL, runTick, (void*) NULL);
	ret = pthread_create(&threadAlarmHandler, NULL, runAlarmHandler, (void*) NULL);
	ret = pthread_create(&threadMail, NULL, runMailDispatcher, (void*) NULL);

	//int alarmToRead = 0;
	char alarmUnitName[30];

	mainHeardBeat = canInterface_getHeardBeatPointer();

	for (i = 0; i < 126; i++) {
		mainHeardBeat[i] = 0;
	}

	int nPingCount = 0;

	OpenSystemController();

	int  nTest= 0;
unsigned char result ;
char answ[60];

	while (1) {
/*
		if (nTest == 10) {
			canInterface_onReceivedAlarm(20, 0xF301, 0, 0x3000, 5);
		}
		else if (nTest == 20) {

			canInterface_onReceivedAlarm(20, 0, 0, 0xF301, 2);
		}
		else if (nTest == 23)
			canInterface_onReceivedAlarm(1, 0, 0, 0xfe, 0);

		nTest++;*/

		//watchDogInterface_keepAlive(); // TODO: will be used

		///////////////////////////////////////
		// if user is logging data
		dataLogerLogic_checkTimers();
		///////////////////////////////////////

		heardBeatTemp = lifegrd_getErrorArray();

		for (i = 0; i < 126; i++) {
			if (mainHeardBeat[i] != heardBeatTemp[i]) {
				if (heardBeatTemp[i] == 1) {
					deviceLinkedList_setFound(i, 0);
					gwAlarmLogUtility_saveAndSendAlarmMsg(i, "Unknown",
							"NCAN bus offline");
					modBusInterface_notifyAlarm();
				} else { //cleared error
					deviceLinkedList_setFound(i, 1);
					strcpy(alarmUnitName, "Unknown Name"); // if cannot get unit name
					canInterface_getString(alarmUnitName, i, 0x2002, 0);
					gwAlarmLogUtility_saveAndSendAlarmMsg(i, alarmUnitName,
							"NCAN bus connected");
				}
				mainHeardBeat[i] = heardBeatTemp[i];
			}
		}

/*
		if (timeToStartDevicesRefresh == 0) { // Counter is "0" => search devices
			timeToStartDevicesRefresh = -1;
			canInterface_refreshDevices();
		}*/

		/////////////////////////////////////////////////
		// Ping router each 60 sec to keep address
		nPingCount++;
		if (nPingCount % 60 == 0) {
			system(pingCommand);
			nPingCount = 0;
		}

		/////////////////////////////////////////////////

		/////////////////////////////////////////////////

		if (nMeasFilesDeleteCount == 0) {  // overflow after 65535 sec => 18 hours
			removeUnusedMeasureFiles();
			nMeasFilesDeleteCount = 0;
		}

		nMeasFilesDeleteCount++;
		/////////////////////////////////////////////////

		// Check reset button
		int nGpio = GetInput();
		if (nGpio == 0) {
			nButtonCounter ++;
			if (nButtonCounter >= 3) {
				if( remove( PASS_FILE_PATH ) != 0 )
					printf( "Error deleting password file" );
				nButtonCounter = 0;
				passFile_init();

				GW_DHCP pdhcpStruct;
				gwDhcpUtility_clearStruct(&pdhcpStruct);
				gwDhcpUtility_saveStruct(pdhcpStruct);
				gwAlarmLogUtility_deleteAllAlarmMsgs();
				blinkPowerLed();
			}

		} else {
			nButtonCounter = 0;
		}
		/////////////////////////////////////////////////

		sleep(1);
	}

	return 0;
}

void * startServer(void *ptr) {
	char *message;
	message = (char *) ptr;
	syslog(LOG_ERR, "TCP_SERVER THREAD START\n");
	serverSocket_start();
	return NULL;
}

void * runTick(void *ptr) {
	while(1) {
		if (timeToStartDevicesRefresh > 0) { // decrease counter. Cannot be done by main thread because of time used for sending of mail
			timeToStartDevicesRefresh --;
			if (timeToStartDevicesRefresh == 0) { // Counter is "0" => search devices
				timeToStartDevicesRefresh = -1;
				canInterface_refreshDevices();
			}
		}
		sleep(1);
	}
	return NULL;
}

