/*
 * gwConfig.h
 *
 *  Created on: 8. feb. 2010
 *      Author: Ernad
 */

#ifndef CONFIG_H_
#define CONFIG_H_

//////////////////////////////////////////////////
// EXTRA DEBUG MESSAGES:
//////////////////////////////////////////////////
//#define DEBUG_CAN
//#define DEBUG_CAN_TIME
//#define DEBUG_CAN_TIME2

//#define DEBUG_CLIENT
//#define PRINT_REC_PARAMETERS
//////////////////////////////////////////////////


//////////////////////////////////////////////////////
//          MOCK
//////////////////////////////////////////////////////
//#define CAN_BUS_MOCK

//////////////////////////////////////////////////////
//			LINUX PATHS
//////////////////////////////////////////////////////
#define SMTP_FOLDER_PATH      	"/etc/ssmtp"
#define SMTP_CONFIG_PATH      	"/etc/ssmtp/ssmtp.conf"

/////////////////////////////////////////////////////
// Can not send mail if SSMTP user name is not "root".
// "revaliases" file must be changed to get this functionality
// Example:
// root:noratel.imed.secure@elreg.eu:customer-smtp.one.com:2525
#define SMTP_REVALIASES  		"/etc/ssmtp/revaliases"

#define DHCP_LIN_CONFIG_PATH  	"/etc/network/interfaces"

/////////////////////////////////////////////////////
// needs for DNS functionality (gateway is DNS)
#define RESOLV_CONF  			"/etc/resolv.conf"


//////////////////////////////////////////////////////
//			GATEWAY PATHS
//////////////////////////////////////////////////////
#define EMAIL_CONFIG_PATH 		"/gateway/Config/eMail.conf"
#define EMAIL_TEST_FILE_PATH 	"/tmp/testmail"

#define MODBUS_CONFIG_PATH 		"/gateway/Config/ModBus.conf"


#define GW_PARAM_FILE_PATH 		"/gateway/Config/devPar.conf"
#define DHCP_CONFIG_PATH 		"/gateway/Config/dhcp.conf"
#define PASS_FILE_PATH 		    "/gateway/Config/pass"
#define DEV_FILE_PATH 		    "/gateway/Config/dev"
#define DATA_LOGER_CONFIG_PATH  "/gateway/Config/dataLoger.conf"
#define HELP_FILE_PATH  		"/gateway/Config/help.conf"

#define LOGING_FOLDER 			"/gateway/log/"

#define ALARM_FOLDER 				"/gateway/alarm/"
#define ALARM_CURRENT_FILE_INDEX 	"/gateway/alarm/currentFileIndex"
#define ALARM_LOG_PATH_1 			"/gateway/alarm/alarmLogFile1"
#define ALARM_LOG_PATH_2 			"/gateway/alarm/alarmLogFile2"
#define MEASURE_FOLDER 				"/srv/www/logFiles/"

// dynamic
#define TREE_NODES_PATH				"/srv/www/tree_nodes.js"
#define SET_DEV_MON_PATH_1  		"/gateway/dynamic/gw_setup_devicemonitor_1.htm"
#define SET_DEV_MON_PATH_2  		"/gateway/dynamic/gw_setup_devicemonitor_2.htm"
#define SET_DEV_MON_PATH_3  		"/gateway/dynamic/gw_setup_devicemonitor_3.htm"
#define SET_DEV_MON_PATH_COMPLET  	"/srv/www/gw_setup_devicemonitor.htm"


//////////////////////////////////////////////////////
//          COMMON
//////////////////////////////////////////////////////
#define MAX_PARAM_NAME_LENGTH   40
#define MAX_PARAM_VALUE_LENGTH 200

#endif /* CONFIG_H_ */
