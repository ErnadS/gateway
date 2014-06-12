
/*
This file is part of the NCANbus, a library implementing an light version of the Canopen standard.

Copyright (c) Elreg as 2010

THIS OBJECT DICTIONARY IS FOR TEST ONLY, MUST BE UPDATED TO HOLD THE DIFFERENT IMED SECURE DEVICES VARIABLES
*/


/*THE OBJECT DICTIONARY MUST BE STORED IN FLASH ONLY, BECAUSE NOT ENOUGH RAM IS AVAILABLE*/



#include "ObjDict.h"


/**************************************************************************/
/* Declaration of the value range types                                   */
/**************************************************************************/

#define valueRange_EMC 0x9F /* Type for index 0x1003 subindex 0x00 (only set of value 0 is possible) */
unsigned long ObjDict_valueRangeTest (unsigned char typeValue, void * value)
{
  switch (typeValue) {
  case valueRange_EMC:
    if (*(unsigned char*)value != (unsigned char)0) return OD_VALUE_RANGE_EXCEEDED;
    break;
  }
  return 0;
}

/**************************************************************************/
/* The node id                                                            */
/**************************************************************************/
/* node_id default value.*/
unsigned char ObjDict_bDeviceNodeId = 0x00;

/**************************************************************************/
/* Array of message processing information */

const unsigned char ObjDict_iam_a_slave = 1;//1=I am a slave 1= I am a master

TIMER_HANDLE ObjDict_heartBeatTimers[10] = {TIMER_NONE, TIMER_NONE, TIMER_NONE, TIMER_NONE, TIMER_NONE, TIMER_NONE, TIMER_NONE, TIMER_NONE, TIMER_NONE, TIMER_NONE,};

/*
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$

OBJECT DICTIONARY

$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
*/

//typedef struct td_subindex
//{
//    unsigned char           bAccessType;
//    unsigned char           bDataType; /* Defines of what datatype the entry is */
//    unsigned long           size;      /* The size (in Byte) of the variable */
//    void*                   pObject;   /* This is the pointer of the Variable */
//} subindex;

/*Device Type. at index 1000*/
#define TRAFOGUARD_DEVICE     0x01000000
#define ISOLOCATOR_DEVICE     0x02000000
#define SWITCHOVERUNIT_DEVICE 0x03000000
#define ALARMPANEL_DEVICE     0x04000000
#define GATEWAY_DEVICE        0x05000000
/* index 0x1000 :   Device Type. */
unsigned long ObjDict_obj1000 = GATEWAY_DEVICE;	/* 0x05000000 */
//unsigned long ObjDict_obj1000 = 0x30191;

subindex ObjDict_Index1000[] = 
{
  { RO, uint32, sizeof (unsigned long), &ObjDict_obj1000 }
};

/* index 0x1001 :   Error Register. */
unsigned char ObjDict_obj1001 = 0x0;	/* 0 */
subindex ObjDict_Index1001[] = 
{
  { RO, uint8, sizeof (unsigned char), (void *)&ObjDict_obj1001 }
};

/* index 0x1003 :   Pre-defined Error Field. */
unsigned char ObjDict_highestSubIndex_obj1003 = 0; /* number of subindex - 1*/
unsigned long ObjDict_obj1003[] = 
{
  0x0	/* 0 */
};
ODCallback_t ObjDict_Index1003_callbacks[] = 
{
  NULL,
  NULL,
};
subindex ObjDict_Index1003[] = 
{
  { RW, valueRange_EMC, sizeof (unsigned char), (void *)&ObjDict_highestSubIndex_obj1003 },
  { RO, uint32, sizeof (unsigned long), (void *)&ObjDict_obj1003[0] }
};

/* index 0x1005 :   SYNC COB ID. */
unsigned long ObjDict_obj1005 = 0x80;	/* 128 */
ODCallback_t ObjDict_Index1005_callbacks[] = 
{
  NULL,
};
subindex ObjDict_Index1005[] = 
{
  { RW, uint32, sizeof (unsigned long), (void *)&ObjDict_obj1005 }
};

/* index 0x1006 :   Communication / Cycle Period */
unsigned long ObjDict_obj1006 = 0x00;   /* 0 */

/* index 0x1008 :   Manufacturer Device Name. */
unsigned char ObjDict_obj1008[] = "GateWay";

subindex ObjDict_Index1008[] = 
{
  { RO, visible_string, sizeof(ObjDict_obj1008), (void *)&ObjDict_obj1008 }
};

/* index 0x1009 :   Manufacturer Hardware Version. */
unsigned char ObjDict_obj1009[10] = "1.00";
subindex ObjDict_Index1009[] = 
{
  { RO, visible_string, 4, (void*)&ObjDict_obj1009 }
};

/* index 0x100A :   Manufacturer Software Version. */
unsigned char ObjDict_obj100A[10] = "1.00";
subindex ObjDict_Index100A[] = 
{
  { RO, visible_string, 4, (void*)&ObjDict_obj100A }
};


/* index 0x1014 :   Emergency COB ID. */
unsigned long ObjDict_obj1014 = 0x80;	/* 128 */
subindex ObjDict_Index1014[] = 
{
  { RW, uint32, sizeof (unsigned long), (void*)&ObjDict_obj1014 }
};

/* index 0x1016 :   Consumer Heartbeat Time. */
unsigned char ObjDict_highestSubIndex_obj1016 = 0; /* number of subindex - 1*/
unsigned long ObjDict_obj1016[] = {
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	    /*(0x4E20 | (1 << 16)), // 12000 ms + ID= 3
		(0x4E20 | (5 << 16)), // 12000 ms + ID= 3
		(0x4E20 | (12 << 16)), // 12000 ms + ID= 3
		(0x4E20 | (64 << 16)) // 12000 ms + ID= 3*/
		};
subindex ObjDict_Index1016[] = {
		{ RO, uint8,  sizeof(unsigned char), (void*) &ObjDict_highestSubIndex_obj1016 },
		{ RW, uint32, sizeof(unsigned long), (void*) &ObjDict_obj1016[0] },
		{ RW, uint32, sizeof(unsigned long), (void*) &ObjDict_obj1016[1] },
		{ RW, uint32, sizeof(unsigned long), (void*) &ObjDict_obj1016[2] },
		{ RW, uint32, sizeof(unsigned long), (void*) &ObjDict_obj1016[3] },
		{ RW, uint32, sizeof(unsigned long), (void*) &ObjDict_obj1016[4] },
		{ RW, uint32, sizeof(unsigned long), (void*) &ObjDict_obj1016[5] },
		{ RW, uint32, sizeof(unsigned long), (void*) &ObjDict_obj1016[6] },
		{ RW, uint32, sizeof(unsigned long), (void*) &ObjDict_obj1016[7] },
		{ RW, uint32, sizeof(unsigned long), (void*) &ObjDict_obj1016[8] },
		{ RW, uint32, sizeof(unsigned long), (void*) &ObjDict_obj1016[9] },
		{ RW, uint32, sizeof(unsigned long), (void*) &ObjDict_obj1016[10] },
		{ RW, uint32, sizeof(unsigned long), (void*) &ObjDict_obj1016[11] },
		{ RW, uint32, sizeof(unsigned long), (void*) &ObjDict_obj1016[12] },
		{ RW, uint32, sizeof(unsigned long), (void*) &ObjDict_obj1016[13] },
		{ RW, uint32, sizeof(unsigned long), (void*) &ObjDict_obj1016[14] },
		{ RW, uint32, sizeof(unsigned long), (void*) &ObjDict_obj1016[15] },
		{ RW, uint32, sizeof(unsigned long), (void*) &ObjDict_obj1016[16] },
		{ RW, uint32, sizeof(unsigned long), (void*) &ObjDict_obj1016[17] },
		{ RW, uint32, sizeof(unsigned long), (void*) &ObjDict_obj1016[18] },
		{ RW, uint32, sizeof(unsigned long), (void*) &ObjDict_obj1016[19] }
};

/* index 0x1017 :   Producer Heartbeat Time. */
unsigned int ObjDict_obj1017 = 0x2710;	/* 10000 */
ODCallback_t ObjDict_Index1017_callbacks[] = 
{
  NULL,
};
subindex ObjDict_Index1017[] = 
{
  { RW, uint16, sizeof (unsigned int), (void*)&ObjDict_obj1017 }
};

/* index 0x1018 :   Identity. */
unsigned char ObjDict_highestSubIndex_obj1018 = 4; /* number of subindex - 1*/
unsigned long ObjDict_obj1018_Vendor_ID       = 0x0;	/* 0 */
unsigned long ObjDict_obj1018_Product_Code    = 0x0;	/* 0 */
unsigned long ObjDict_obj1018_Revision_Number = 0x0;	/* 0 */
unsigned long ObjDict_obj1018_Serial_Number   = 0x12345678;	/* 0 */
subindex ObjDict_Index1018[] = 
{
  { RO, uint8, sizeof (unsigned char), (void*)&ObjDict_highestSubIndex_obj1018 },
  { RO, uint32, sizeof (unsigned long), (void*)&ObjDict_obj1018_Vendor_ID },
  { RO, uint32, sizeof (unsigned long), (void*)&ObjDict_obj1018_Product_Code },
  { RO, uint32, sizeof (unsigned long), (void*)&ObjDict_obj1018_Revision_Number },
  { RO, uint32, sizeof (unsigned long), (void*)&ObjDict_obj1018_Serial_Number }
};

/* index 0x1200 :   SERVER SDO Parameter. */
unsigned char ObjDict_highestSubIndex_obj1200 = 2; /* number of subindex - 1*/
unsigned long ObjDict_obj1200_COB_ID_Client_to_Server_Receive_SDO = 0x600;	/* 0x600 + nodeid */
unsigned long ObjDict_obj1200_COB_ID_Server_to_Client_Transmit_SDO = 0x580;	/* 0x580+ nodeid */
subindex ObjDict_Index1200[] = 
{
  { RO, uint8, sizeof (unsigned char), (void*)&ObjDict_highestSubIndex_obj1200 },
  { RO, uint32, sizeof (unsigned long), (void*)&ObjDict_obj1200_COB_ID_Client_to_Server_Receive_SDO },
  { RO, uint32, sizeof (unsigned long), (void*)&ObjDict_obj1200_COB_ID_Server_to_Client_Transmit_SDO }
};

/* index 0x1280 :   CLIENT SDO 1 Parameter. */
unsigned char ObjDict_highestSubIndex_obj1280 = 3; /* number of subindex - 1*/
unsigned long ObjDict_obj1280_COB_ID_Client_to_Server_Transmit_SDO = 0x600;	/* 0x600 + nodeid */
unsigned long ObjDict_obj1280_COB_ID_Server_to_Client_Receive_SDO = 0x580;	/* 0x580+ nodeid */
unsigned char ObjDict_obj1280_Node_ID_of_the_SDO_Server = 0x00;	                /* Default 0x00 */
subindex ObjDict_Index1280[] = 
{
  { RO, uint8, sizeof (unsigned char), (void*)&ObjDict_highestSubIndex_obj1280 },
  { RW, uint32, sizeof (unsigned long), (void*)&ObjDict_obj1280_COB_ID_Client_to_Server_Transmit_SDO },
  { RW, uint32, sizeof (unsigned long), (void*)&ObjDict_obj1280_COB_ID_Server_to_Client_Receive_SDO },
  { RW, uint8, sizeof (unsigned char), (void*)&ObjDict_obj1280_Node_ID_of_the_SDO_Server }
};

///* index 0x1400 :   Receive PDO 1 Parameter. */
//unsigned char ObjDict_highestSubIndex_obj1400 = 5; /* number of subindex - 1*/
//unsigned long ObjDict_obj1400_COB_ID_used_by_PDO = 0x200;	/* 512 */
//unsigned char ObjDict_obj1400_Transmission_Type = 0x1;	/* 1 */
//unsigned int ObjDict_obj1400_Inhibit_Time = 0x0;	/* 0 */
//unsigned char ObjDict_obj1400_Compatibility_Entry = 0x0;	/* 0 */
//unsigned int ObjDict_obj1400_Event_Timer = 0x0;	/* 0 */
//subindex const __flash ObjDict_Index1400[] = 
//{
//  { RO, uint8, sizeof (unsigned char), (void*)&ObjDict_highestSubIndex_obj1400 },
//  { RW, uint32, sizeof (unsigned long), (void*)&ObjDict_obj1400_COB_ID_used_by_PDO },
//  { RW, uint8, sizeof (unsigned char), (void*)&ObjDict_obj1400_Transmission_Type },
//  { RW, uint16, sizeof (unsigned int), (void*)&ObjDict_obj1400_Inhibit_Time },
//  { RW, uint8, sizeof (unsigned char), (void*)&ObjDict_obj1400_Compatibility_Entry },
//  { RW, uint16, sizeof (unsigned int), (void*)&ObjDict_obj1400_Event_Timer }
//};
//
///* index 0x1600 :   Receive PDO 1 Mapping. */
//unsigned char ObjDict_highestSubIndex_obj1600 = 1; /* number of subindex - 1*/
//unsigned long ObjDict_obj1600[] = 
//{
//  0x62000108	/* 1644167432 */
//};
//subindex const __flash ObjDict_Index1600[] = 
//{
//  { RW, uint8, sizeof (unsigned char), (void*)&ObjDict_highestSubIndex_obj1600 },
//  { RW, uint32, sizeof (unsigned long), (void*)&ObjDict_obj1600[0] }
//};
//
///* index 0x1800 :   Transmit PDO 1 Parameter. */
//unsigned char ObjDict_highestSubIndex_obj1800 = 5; /* number of subindex - 1*/
//unsigned long ObjDict_obj1800_COB_ID_used_by_PDO = 0x180;	/* 384 */
//unsigned char ObjDict_obj1800_Transmission_Type = 0xFF;	/* 255 */
//unsigned int ObjDict_obj1800_Inhibit_Time = 0x0;	/* 0 */
//unsigned char ObjDict_obj1800_Compatibility_Entry = 0x0;	/* 0 */
//unsigned int ObjDict_obj1800_Event_Timer = 0x0;	/* 0 */
//ODCallback_t ObjDict_Index1800_callbacks[] = 
//{
//  NULL,
//  NULL,
//  NULL,
//  NULL,
//  NULL,
//  NULL,
//};
//subindex const __flash ObjDict_Index1800[] = 
//{
//  { RO, uint8, sizeof (unsigned char), (void*)&ObjDict_highestSubIndex_obj1800 },
//  { RW, uint32, sizeof (unsigned long), (void*)&ObjDict_obj1800_COB_ID_used_by_PDO },
//  { RW, uint8, sizeof (unsigned char), (void*)&ObjDict_obj1800_Transmission_Type },
//  { RW, uint16, sizeof (unsigned int), (void*)&ObjDict_obj1800_Inhibit_Time },
//  { RW, uint8, sizeof (unsigned char), (void*)&ObjDict_obj1800_Compatibility_Entry },
//  { RW, uint16, sizeof (unsigned int), (void*)&ObjDict_obj1800_Event_Timer }
//};
//
///* index 0x1A00 :   Transmit PDO 1 Mapping. */
//unsigned char ObjDict_highestSubIndex_obj1A00 = 1; /* number of subindex - 1*/
//unsigned long ObjDict_obj1A00[] = 
//{
//  0x60000108	/* 1610613000 */
//};
//subindex const __flash ObjDict_Index1A00[] = 
//{
//  { RW, uint8, sizeof (unsigned char), (void*)&ObjDict_highestSubIndex_obj1A00 },
//  { RW, uint32, sizeof (unsigned long), (void*)&ObjDict_obj1A00[0] }
//};

/* index 0x2000 :   Node id */
//unsigned char ObjDict_highestSubIndex_obj2000 = 1; /* number of subindex - 1*/
unsigned char ObjDict_obj2000[] = 
{
  0x00	/*    */
};
subindex ObjDict_Index2000[] = 
{
  //{ RO, uint8, sizeof (unsigned char), (void*)&ObjDict_highestSubIndex_obj2000 }, //, NULL  Ernad???
  { RW, uint8, sizeof (unsigned char), (void*)&ObjDict_obj2000[0] }
};

/* index 0x2001 :   Baud rate */
unsigned char ObjDict_highestSubIndex_obj2001 = 1; /* number of subindex - 1*/
unsigned int ObjDict_obj2001[] = 
{
  0x00	/*    */
};
subindex ObjDict_Index2001[] = 
{
  { RO, uint8, sizeof (unsigned char), (void*)&ObjDict_highestSubIndex_obj2001},
  { RW, uint32, sizeof (unsigned char), (void*)&ObjDict_obj2001[0] }
};

/* index 0x2002 :   Node name / location */
unsigned char ObjDict_obj2002[24] = "GateWay in room 2";
subindex ObjDict_Index2002[] = 
{
  { RW, visible_string, sizeof(ObjDict_obj2002), (void*)&ObjDict_obj2002 }
};

/* index 0x3000 :   TCP/IP Network address */
unsigned char ObjDict_obj2003[] = "000.000.000.000";
subindex ObjDict_Index2003[] = 
{
  { RO, visible_string, sizeof(ObjDict_obj2003), (void*)&ObjDict_obj2003 }
};

const indextable ObjDict_objdict[] =
{
  { (subindex *)ObjDict_Index1000,sizeof(ObjDict_Index1000)/sizeof(ObjDict_Index1000[0]), 0x1000},
  { (subindex *)ObjDict_Index1001,sizeof(ObjDict_Index1001)/sizeof(ObjDict_Index1001[0]), 0x1001},
  { (subindex *)ObjDict_Index1003,sizeof(ObjDict_Index1003)/sizeof(ObjDict_Index1003[0]), 0x1003},
  { (subindex *)ObjDict_Index1005,sizeof(ObjDict_Index1005)/sizeof(ObjDict_Index1005[0]), 0x1005},
  { (subindex *)ObjDict_Index1008,sizeof(ObjDict_Index1008)/sizeof(ObjDict_Index1008[0]), 0x1008},
  { (subindex *)ObjDict_Index1009,sizeof(ObjDict_Index1009)/sizeof(ObjDict_Index1009[0]), 0x1009},
  { (subindex *)ObjDict_Index100A,sizeof(ObjDict_Index100A)/sizeof(ObjDict_Index100A[0]), 0x100A},
  { (subindex *)ObjDict_Index1014,sizeof(ObjDict_Index1014)/sizeof(ObjDict_Index1014[0]), 0x1014},
  { (subindex *)ObjDict_Index1016,sizeof(ObjDict_Index1016)/sizeof(ObjDict_Index1016[0]), 0x1016},
  { (subindex *)ObjDict_Index1017,sizeof(ObjDict_Index1017)/sizeof(ObjDict_Index1017[0]), 0x1017},
  { (subindex *)ObjDict_Index1018,sizeof(ObjDict_Index1018)/sizeof(ObjDict_Index1018[0]), 0x1018},
  { (subindex *)ObjDict_Index1200,sizeof(ObjDict_Index1200)/sizeof(ObjDict_Index1200[0]), 0x1200},
  { (subindex *)ObjDict_Index1280,sizeof(ObjDict_Index1280)/sizeof(ObjDict_Index1280[0]), 0x1280},
  { (subindex *)ObjDict_Index2000,sizeof(ObjDict_Index2000)/sizeof(ObjDict_Index2000[0]), 0x2000},
  { (subindex *)ObjDict_Index2001,sizeof(ObjDict_Index2001)/sizeof(ObjDict_Index2001[0]), 0x2001},
  { (subindex *)ObjDict_Index2002,sizeof(ObjDict_Index2002)/sizeof(ObjDict_Index2002[0]), 0x2002},
  { (subindex *)ObjDict_Index2003,sizeof(ObjDict_Index2003)/sizeof(ObjDict_Index2003[0]), 0x2003}
};



const indextable * ObjDict_scanIndexOD (unsigned int wIndex, unsigned long * errorCode, ODCallback_t **callbacks)
{
  int i;
  *callbacks = NULL;
  //ObjDict_Index1000[0].bAccessType =  RO; //, uint32, sizeof (unsigned long), &ObjDict_obj1000 };

  switch(wIndex){
  case 0x1000: i = 0;break;
  case 0x1001: i = 1;break;
  case 0x1003: i = 2;*callbacks = ObjDict_Index1003_callbacks; break;
  case 0x1005: i = 3;*callbacks = ObjDict_Index1005_callbacks; break;
  case 0x1008: i = 4;break;
  case 0x1009: i = 5;break;
  case 0x100A: i = 6;break;
  case 0x1014: i = 7;break;
  case 0x1016: i = 8;break;
  case 0x1017: i = 9;*callbacks = ObjDict_Index1017_callbacks; break;
  case 0x1018: i = 10;break;
  case 0x1200: i = 11;break;
  case 0x1280: i = 12;break;
  case 0x2000: i = 13;break;
  case 0x2001: i = 14;break;
  case 0x2002: i = 15;break;
  case 0x2003: i = 16;break;
  
  default:
    *errorCode = OD_NO_SUCH_OBJECT;
    return NULL;
  }
  *errorCode = OD_SUCCESSFUL;
  return &ObjDict_objdict[i];
}

/* 
* To count at which received SYNC a PDO must be sent.
* Even if no pdoTransmit are defined, at least one entry is computed
* for compilations issues.
*/
s_PDO_status ObjDict_PDO_status[1] = {s_PDO_status_Initializer};

quick_index ObjDict_firstIndex = {
  11, /* SDO_SVR */
  12, /* SDO_CLT */
  0, /* PDO_RCV */
  0, /* PDO_RCV_MAP */
  0, /* PDO_TRS */
  0 /* PDO_TRS_MAP */
};

quick_index ObjDict_lastIndex = {
  11, /* SDO_SVR */
  12, /* SDO_CLT */
  0, /* PDO_RCV */
  0, /* PDO_RCV_MAP */
  0, /* PDO_TRS */
  0 /* PDO_TRS_MAP */
};

unsigned int ObjDict_ObjdictSize = sizeof(ObjDict_objdict)/sizeof(ObjDict_objdict[0]); 

CO_Data ObjDict_Data = NCAN_NODE_DATA_INITIALIZER(ObjDict);
