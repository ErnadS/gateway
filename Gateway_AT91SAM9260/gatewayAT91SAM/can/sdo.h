/*
This file is part of the NCAN bus, a library implementing CanOpen Light Stack.

Copyright (c) Elreg as 2010

SDOs provide the access to entries in the NCANbus Object Dictionary.
An SDO is made up of at least two CAN messages with different identifiers.
SDO's are always confirmed point-to-point communications services.

*/

#ifndef __sdo_h__
#define __sdo_h__

struct struct_s_transfer;

#include "timer.h"

/* Block mode : Data consumer receive step
 * - set to RXSTEP_STARTED when client receive initiate upload response
 * - set to RXSTEP_END when last segment of a block received
 */
typedef enum {RXSTEP_INIT, RXSTEP_STARTED, RXSTEP_END } rxStep_t;

typedef void (*SDOCallback_t)(CO_Data* d, unsigned char nodeId);

///* The Transfer structure
//Used to store the different segments of
// - a SDO received before writing in the dictionary
// - the reading of the dictionary to put on a SDO to transmit
//*/
//struct struct_s_transfer {
//  unsigned char           CliServNbr; /**< The index of the SDO client / server in our OD minus 0x1280 / 0x1200 */
//
//  unsigned char           whoami;     /**< Takes the values SDO_CLIENT or SDO_SERVER */
//  unsigned char           state;      /**< state of the transmission : Takes the values SDO_... */
//  unsigned char           toggle;
//  unsigned long          abortCode;  /**< Sent or received */
//  /**< index and subindex of the dictionary where to store */
//  /**< (for a received SDO) or to read (for a transmit SDO) */
//  unsigned int          index;
//  unsigned char           subIndex;
//  unsigned long          count;      /**< Number of data received or to be sent. */
//  unsigned long          offset;     /**< stack pointer of data[]
//                              * Used only to tranfer part of a line to or from a SDO.
//                              * offset is always pointing on the next free cell of data[].
//                              * WARNING s_transfer.data is subject to ENDIANISATION
//                              * (with respect to CANOPEN_BIG_ENDIAN)
//                              */
//  unsigned char           data [SDO_MAX_LENGTH_TRANSFERT];
//#ifdef SDO_DYNAMIC_BUFFER_ALLOCATION
//  unsigned char           *dynamicData;
//  unsigned long          dynamicDataSize;
//#endif //SDO_DYNAMIC_BUFFER_ALLOCATION
//
//  unsigned char           peerCRCsupport;    /**< True if peer supports CRC */
//  unsigned char           blksize;           /**< Number of segments per block with 0 < blksize < 128 */
//  unsigned char           ackseq;            /**< sequence number of last segment that was received successfully */
//  unsigned long           objsize;           /**< Size in bytes of the object provided by data producer */
//  unsigned char           lastblockoffset;   /**< Value of offset before last block */
//  unsigned char           seqno;             /**< Last sequence number received OK or transmitted */
//  unsigned char           endfield;          /**< nbr of bytes in last segment of last block that do not contain data */
//  rxStep_t       rxstep;            /**< data consumer receive step - set to true when last segment of a block received */
//  unsigned char           tmpData[8];        /**< temporary segment storage */
//
//  unsigned char           dataType;   /**< Defined in objdictdef.h Value is visible_string
//                              * if it is a string, any other value if it is not a string,
//                              * like 0. In fact, it is used only if client.
//                              */
//  TIMER_HANDLE   timer;      /**< Time counter to implement a timeout in milliseconds.
//                              * It is automatically incremented whenever
//                              * the line state is in SDO_DOWNLOAD_IN_PROGRESS or
//                              * SDO_UPLOAD_IN_PROGRESS, and reseted to 0
//                              * when the response SDO have been received.
//                              */
//  SDOCallback_t Callback;   /**< The user callback func to be called at SDO transaction end */
//};

struct struct_s_transfer {
  unsigned char           nodeId;     /**<own ID if server, or node ID of the server if client */

  unsigned char           whoami;     /**< Takes the values SDO_CLIENT or SDO_SERVER */
  unsigned char           state;      /**< state of the transmission : Takes the values SDO_... */
  unsigned char           toggle;
  unsigned long          abortCode;  /**< Sent or received */
                                      /**< index and subindex of the dictionary where to store */
                                      /**< (for a received SDO) or to read (for a transmit SDO) */
  unsigned int          index;
  unsigned char           subIndex;
  unsigned long          count;      /**< Number of data received or to be sent. */
  unsigned long          offset;     /**< stack pointer of data[]
                              * Used only to tranfer part of a line to or from a SDO.
                              * offset is always pointing on the next free cell of data[].
                              * WARNING s_transfer.data is subject to ENDIANISATION
                              * (with respect to CANOPEN_BIG_ENDIAN)
                              */
  unsigned char           data [SDO_MAX_LENGTH_TRANSFERT];
  unsigned char           dataType;   /**< Defined in objdictdef.h Value is visible_string
                              * if it is a string, any other value if it is not a string,
                              * like 0. In fact, it is used only if client.
                              */
  TIMER_HANDLE   timer;      /**< Time counter to implement a timeout in milliseconds.
                              * It is automatically incremented whenever
                              * the line state is in SDO_DOWNLOAD_IN_PROGRESS or
                              * SDO_UPLOAD_IN_PROGRESS, and reseted to 0
                              * when the response SDO have been received.
                              */
  SDOCallback_t Callback;   /**< The callback func to be called at SDO transaction end */

  unsigned char         SDOflag; //This flag is set when we received a SDO not for us
};
typedef struct struct_s_transfer s_transfer;


#include "data.h"


struct BODY{
    unsigned char data[8]; /**< The 8 bytes data of the SDO */
};

/* The SDO structure ...*/
struct struct_s_SDO {
  unsigned char nodeId;		/**< In any case, Node ID of the server (case sender or receiver).*/
  struct BODY body;
};


typedef struct struct_s_SDO s_SDO;


extern inline unsigned char _readNetworkDict (CO_Data* d, unsigned char nodeId, unsigned int index, unsigned char subIndex,unsigned char dataType, SDOCallback_t Callback);
/**
 * Reset of a SDO exchange on timeout.
 * Send a SDO abort.
 * *d Pointer on a CAN object data structure
 * id
 */
void SDOTimeoutAlarm(CO_Data* d, unsigned long id);

/**
 *  Reset all SDO buffers.
 *  *d Pointer on a CAN object data structure
 */
void resetSDO (CO_Data* d);


/**
 *  Copy the data received from the SDO line transfert to the object dictionary.
 *  *d Pointer on a CAN object data structure
 *  line SDO line
 *  Returns: SDO error code if error. Else, returns 0.
 */
unsigned long SDOlineToObjdict (CO_Data* d, unsigned char line);

/**
 *  Copy the data from the object dictionary to the SDO line for a network transfert.
 *  *d Pointer on a CAN object data structure
 *  line SDO line
 *  Returns: SDO error code if error. Else, returns 0.
 */
unsigned long objdictToSDOline (CO_Data* d, unsigned char line);

/**
 *  Copy data from an existant line in the argument "* data"
 *  d Pointer on a CAN object data structure
 *  line SDO line
 *  nbBytes
 *  *data Pointer on the data
 *  Returns: 0xFF if error. Else, returns 0.
 */
unsigned char lineToSDO (CO_Data* d, unsigned char line, unsigned long nbBytes, unsigned char * data);

/**
 *  Add data to an existant line
 *  d Pointer on a CAN object data structure
 *  line SDO line
 *  nbBytes
 *  *data Pointer on the data
 *  Returns: 0xFF if error. Else, returns 0.
 */
unsigned char SDOtoLine (CO_Data* d, unsigned char line, unsigned long nbBytes, unsigned char * data);

/**
 *  Called when an internal SDO abort occurs.
 * Release the line * Only if server *
 * If client, the line must be released manually in the core application.
 * The reason of that is to permit the program to read the transfers structure before its reset,
 * because many informations are stored on it : index, subindex, data received or trasmited, ...
 * In all cases, sends a SDO abort.
 *  *d Pointer on a CAN object data structure
 *  nodeId
 *  whoami
 *  index
 *  subIndex
 *  abortCode
 * Returns: 0
 */
unsigned char failedSDO (CO_Data* d, unsigned char nodeId, unsigned char whoami, unsigned int index, unsigned char subIndex, unsigned long abortCode);

/**
 *  Reset an unused line.
 *  *d Pointer on a CAN object data structure
 *  line SDO line
 */
void resetSDOline (CO_Data* d, unsigned char line);

/**
 *  Initialize some fields of the structure.
 *  *d Pointer on a CAN object data structure
 *  line
 *  nodeId
 *  index
 *  subIndex
 *  state
 * Returns: 0
 */
unsigned char initSDOline (CO_Data* d, unsigned char line, unsigned char nodeId, unsigned int index, unsigned char subIndex, unsigned char state);

/**
 *  Search for an unused line in the transfers array
 *  to store a new SDO.
 *  ie a line which value of the field "state" is "SDO_RESET"
 *  An unused line have the field "state" at the value SDO_RESET
 *  *d Pointer on a CAN object data structure
 *  whoami Create the line for a SDO_SERVER or SDO_CLIENT.
 *  *line Pointer on a SDO line
 *  Returns: 0xFF if all the lines are on use. Else, return 0.
 */
unsigned char getSDOfreeLine (CO_Data* d, unsigned char whoami, unsigned char *line);

/**
 *  Search for the line, in the transfers array, which contains the
 *  beginning of the reception of a fragmented SDO
 *  *d Pointer on a CAN object data structure
 *  nodeId correspond to the message node-id
 *  whoami takes 2 values : look for a line opened as SDO_CLIENT or SDO_SERVER
 *  *line Pointer on a SDO line
 *  Returns: 0xFF if error.  Else, return 0
 */
unsigned char getSDOlineOnUse (CO_Data* d, unsigned char nodeId, unsigned char whoami, unsigned char *line);

/**
 *  Close a transmission.
 *  *d Pointer on a CAN object data structure
 *  nodeId Node id of the server if both server or client
 *  whoami Line opened as SDO_CLIENT or SDO_SERVER

*/
unsigned char closeSDOtransfer (CO_Data* d, unsigned char nodeId, unsigned char whoami);
/*
 * Search for the line, in the transfers array, which contains the
 * beginning of the reception of a fragmented SDO
 *
 * Because getSDOlineOnUse() does not return any line in state \c SDO_ABORTED_INTERNAL,
 * this funtion is used to return them, too.
 *
 * @param *d Pointer on a CAN object data structure
 * @param nodeId correspond to the message node-id
 * @param whoami takes 2 values : look for a line opened as SDO_CLIENT or SDO_SERVER
 * @param *line Pointer on a SDO line
 * @return 0xFF if error.  Else, return 0
 */
unsigned char getSDOlineToClose (CO_Data* d, unsigned char nodeId, unsigned char whoami, unsigned char *line);
/**
 *  Bytes in the line structure which must be transmited (or received)
 *  *d Pointer on a CAN object data structure
 *  line SDO line
 *  *nbBytes Pointer on nbBytes
 *  Returns: 0.
 */
unsigned char getSDOlineRestBytes (CO_Data* d, unsigned char line, unsigned long * nbBytes);

/**
 *  Store in the line structure the nb of bytes which must be transmited (or received)
 *  *d Pointer on a CAN object data structure
 *  line SDO line
 *  nbBytes
 *  Returns: 0 if success, 0xFF if error.
 */
unsigned char setSDOlineRestBytes (CO_Data* d, unsigned char line, unsigned long nbBytes);

/**
 *  Transmit a SDO frame on the bus bus_id
 *  *d Pointer on a CAN object data structure
 *  whoami Takes 2 values : SDO_CLIENT or SDO_SERVER
 *  sdo SDO Structure which contains the sdo to transmit
 *  Returns: canSend(bus_id,&m) or 0xFF if error.
 */
unsigned char sendSDO (CO_Data* d, unsigned char whoami, s_SDO sdo);

/**
 *  Transmit a SDO error to the client. The reasons may be :
 *  Read/Write to a undefined object
 *  Read/Write to a undefined subindex
 *  Read/write a not valid length object
 *  Write a read only object
 *  *d Pointer on a CAN object data structure
 *  whoami takes 2 values : SDO_CLIENT or SDO_SERVER
 *  nodeId
 *  index
 *  subIndex
 *  abortCode
 *  Returns: 0
 */
unsigned char sendSDOabort (CO_Data* d, unsigned char whoami, unsigned char nodeId, unsigned int index, unsigned char subIndex, unsigned long abortCode);

/**
 *  Treat a SDO frame reception
 *  call the function sendSDO
 *  *d Pointer on a CAN object data structure
 *  *m Pointer on a CAN message structure
 *  Returns: code :
 * 		   - 0xFF if error
 *         - 0x80 if transfert aborted by the server
 *         - 0x0  ok
 */
unsigned char proceedSDO (CO_Data* d, Message *m);

/**
 * @ingroup sdo
 *  Used to send a SDO request frame to write the data at the index and subIndex indicated
 *  *d Pointer to a CAN object data structure
 *  nodeId Node Id of the slave
 *  index At index indicated
 *  subIndex At subIndex indicated
 *  count number of bytes to write in the dictionnary.
 *  dataType (defined in objdictdef.h) : put "visible_string" for strings, 0 for integers or reals or other value.
 *  *data Pointer to data
 * Returns:
 * - 0 is returned upon success.
 * - 0xFE is returned when no sdo client to communicate with node.
 * - 0xFF is returned when error occurs.
 */
unsigned char writeNetworkDict (CO_Data* d, unsigned char nodeId, unsigned int index,
		       unsigned char subIndex, unsigned long count, unsigned char dataType, void *data);

/**
 *  Used to send a SDO request frame to write in a distant node dictionnary.
 *  The function Callback which must be defined in the user code is called at the
 *  end of the exchange. (on succes or abort).
 *
 *  *d Pointer to a CAN object data structure
 *  nodeId Node Id of the slave
 *  index At index indicated
 *  subIndex At subIndex indicated
 *  count number of bytes to write in the dictionnary.
 *  dataType (defined in objdictdef.h) : put "visible_string" for strings, 0 for integers or reals or other value.
 *  *data Pointer to data
 *  Callback Callback function
 * Returns:
 * - 0 is returned upon success.
 * - 0xFE is returned when no sdo client to communicate with node.
 * - 0xFF is returned when error occurs.
 */
unsigned char writeNetworkDictCallBack (CO_Data* d, unsigned char nodeId, unsigned int index,
		       unsigned char subIndex, unsigned long count, unsigned char dataType, void *data, SDOCallback_t Callback);

/**
 *  Used to send a SDO request frame to write in a distant node dictionnary.
 *  The function Callback which must be defined in the user code is called at the
 *  end of the exchange. (on succes or abort). First free SDO client parameter is
 *  automatically initialized for specific node if not already defined.
 *
 *  *d Pointer to a CAN object data structure
 *  nodeId Node Id of the slave
 *  index At index indicated
 *  subIndex At subIndex indicated
 *  count number of bytes to write in the dictionnary.
 *  dataType (defined in objdictdef.h) : put "visible_string" for strings, 0 for integers or reals or other value.
 *  *data Pointer to data
 *  Callback Callback function
 *  endianize When not 0, data is endianized into network byte order
 *                  when 0, data is not endianized and copied in machine native
 *                  endianness
 * Returns:
 * - 0 is returned upon success.
 * - 0xFF is returned when error occurs.
 */
unsigned char writeNetworkDictCallBackAI (CO_Data* d, unsigned char nodeId, unsigned int index,
		       unsigned char subIndex, unsigned long count, unsigned char dataType, void *data, SDOCallback_t Callback, unsigned char endianize);

/**
 *  Used to send a SDO request frame to read.
 *  *d Pointer to a CAN object data structure
 *  nodeId Node Id of the slave
 *  index At index indicated
 *  subIndex At subIndex indicated
 *  dataType (defined in objdictdef.h) : put "visible_string" for strings, 0 for integers or reals or other value.
 * Returns:
 * - 0 is returned upon success.
 * - 0xFE is returned when no sdo client to communicate with node.
 * - 0xFF is returned when error occurs.
 */
unsigned char readNetworkDict (CO_Data* d, unsigned char nodeId, unsigned int index, unsigned char subIndex, unsigned char dataType);

/**
 *  Used to send a SDO request frame to read in a distant node dictionnary.
 *  The function Callback which must be defined in the user code is called at the
 *  end of the exchange. (on succes or abort).
 *  *d Pointer on a CAN object data structure
 *  nodeId Node Id of the slave
 *  index At index indicated
 *  subIndex At subIndex indicated
 *  dataType (defined in objdictdef.h) : put "visible_string" for strings, 0 for integers or reals or other value.
 *  Callback Callback function
 *  Returns:
 * - 0 is returned upon success.
 * - 0xFE is returned when no sdo client to communicate with node.
 * - 0xFF is returned when error occurs.
 */
unsigned char readNetworkDictCallback (CO_Data* d, unsigned char nodeId, unsigned int index, unsigned char subIndex, unsigned char dataType, SDOCallback_t Callback);

/**
 *  Used to send a SDO request frame to read in a distant node dictionnary.
 *  The function Callback which must be defined in the user code is called at the
 *  end of the exchange. (on succes or abort). First free SDO client parameter is
 *  automatically initialized for specific node if not already defined.
 *  *d Pointer on a CAN object data structure
 *  nodeId Node Id of the slave
 *  index At index indicated
 *  subIndex At subIndex indicated
 *  dataType (defined in objdictdef.h) : put "visible_string" for strings, 0 for integers or reals or other value.
 *  Callback Callback function
 *  Returns:
 * - 0 is returned upon success.
 * - 0xFF is returned when error occurs.
 */
unsigned char readNetworkDictCallbackAI (CO_Data* d, unsigned char nodeId, unsigned int index, unsigned char subIndex, unsigned char dataType, SDOCallback_t Callback);



/**
 *  Used to send a SDO request frame to read in a distant node dictionnary.
 *  The function Callback which must be defined in the user code is called at the
 *  end of the exchange. (on succes or abort). First SDO client parameter at object index 0x1280 is
 *  automatically initialized for specific node.
 *  *d Pointer on a CAN object data structure
 *  nodeId Node Id of the slave
 *  index At index indicated
 *  subIndex At subIndex indicated
 *  dataType (defined in objdictdef.h) : put "visible_string" for strings, 0 for integers or reals or other value.
 *  Callback Callback function
 *  Returns:
 * - 0 is returned upon success.
 * - 0xFF is returned when error occurs.
 */
unsigned char readNetworkDictCallbackCC (CO_Data* d, unsigned char nodeId, unsigned int index, unsigned char subIndex, unsigned char dataType, SDOCallback_t Callback);

/**
 *  Use this function after calling readNetworkDict to get the result.
 *
 *  *d Pointer to a CAN object data structure
 *  nodeId Node Id of the slave
 *  *data Pointer to the datas
 *  *size Pointer to the size
 *  *abortCode Pointer to the abortcode. (0 = not available. Else : SDO abort code. (received if return SDO_ABORTED_RCV)
 *
 *
 * Returns:
 *           - SDO_FINISHED             // datas are available
 *           - SDO_ABORTED_RCV          // Transfert failed (abort SDO received)
 *           - SDO_ABORTED_INTERNAL     // Transfert failed (internal abort)
 *           - SDO_UPLOAD_IN_PROGRESS   // Datas are not yet available
 *           - SDO_DOWNLOAD_IN_PROGRESS // Download is in progress
 *
 * Example :
 *
 * unsigned long data;
 * unsigned char size;
 * readNetworkDict(0, 0x05, 0x1016, 1, 0) // get the data index 1016 subindex 1 of node 5
 * while (getReadResultNetworkDict (0, 0x05, &data, &size) == SDO_UPLOAD_IN_PROGRESS);
 *
*/
unsigned char getReadResultNetworkDict (CO_Data* d, unsigned char nodeId, void* data, unsigned long *size, unsigned long * abortCode);

/**
 *  Use this function after calling writeNetworkDict function to get the result of the write.
 *  It is mandatory to call this function because it is releasing the line used for the transfer.
 *  *d Pointer to a CAN object data structure
 *  nodeId Node Id of the slave
 *  *abortCode Pointer to the abortcode
 * - 0 = not available.
 * - SDO abort code (received if return SDO_ABORTED_RCV)
 *
 * Returns: :
 *           - SDO_FINISHED             // datas are available
 *           - SDO_ABORTED_RCV          // Transfert failed (abort SDO received)
 *           - SDO_ABORTED_INTERNAL     // Transfert failed (Internal abort)
 *           - SDO_DOWNLOAD_IN_PROGRESS // Datas are not yet available
 *           - SDO_UPLOAD_IN_PROGRESS   // Upload in progress
 *
 * Example :
 *
 * unsigned long data = 0x50;
 * unsigned char size;
 * unsigned long abortCode;
 * writeNetworkDict(0, 0x05, 0x1016, 1, size, &data) // write the data index 1016 subindex 1 of node 5
 * while (getWriteResultNetworkDict (0, 0x05, &abortCode) == SDO_DOWNLOAD_IN_PROGRESS);
 *
*/
unsigned char getWriteResultNetworkDict (CO_Data* d, unsigned char nodeId, unsigned long * abortCode);




/*!    Edited by T.S   12.02.2014
**
**
** d pointer to node data
**
**
**
** Returns: sdo state
**/
unsigned char getSDOlineState (CO_Data* d);

#endif


/*!    Edited by T.S   12.02.2014
**
**
** d pointer to node data
**
**
**
** Returns: sdo activty
**/
unsigned char getSDO_Received (CO_Data* d);

/*!    Edited by T.S   12.02.2014
**
**
** d pointer to node data
**
**
**
** Returns: Clear sdo received flag
**/
void resetSDO_flag (CO_Data* d);
