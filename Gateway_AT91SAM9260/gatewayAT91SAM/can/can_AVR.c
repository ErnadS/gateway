/*
This file is part of the NCANbus, a library implementing an light version of the Canopen standard.

Copyright (c) Elreg as 2010


*/

#define DEBUG_WAR_DISPLAY_ON  //Enable debug warning messages to display
#define DEBUG_ERR_DISPLAY_ON  //Enable debug error messages to display

#include "can_AVR.h"
#include "ncanbus.h"
#include "canDriverInterface.h"


volatile unsigned char msg_received = 0;
/******************************************************************************
Initialize the hardware to receive CAN messages and start the timer for the
NCANbus stack.
INPUT	bitrate		bitrate in kilobit
OUTPUT	1 if successful	
******************************************************************************/
unsigned char canInit()
{
	return canDriverInterface_init();
}

/******************************************************************************
The driver send a CAN message passed from the NCANbus stack
Message *m pointer to message to send
RETURNS	1 if  hardware -> CAN frame
******************************************************************************/
unsigned char canSend(Message *m)
{
	return canDriverInterface_sendMessage(m->cob_id, (char*)(m->data), (char)m->len);
}

/******************************************************************************
The driver pass a received CAN message to the stack
INPUT	Message *m pointer to received CAN message
RETURNS	1 if a message received
******************************************************************************/
unsigned char canReceive(Message *m)
{
	return canDriverInterface_readMessage(m);
}


//******************************************************************************/
//unsigned char canChangeBaudRate_driver( CAN_HANDLE fd, char* baud)
//{
//
//	return 0;
//}


/******************************************************************************
CAN Interrupt handler
******************************************************************************/
/*
#pragma type_attribute = __interrupt
#pragma vector=CANIT_vect
void CANIT_interrupt(void)
{
  unsigned char saved_page = CANPAGE;
  unsigned char i;
  
  if (CANGIT & (1 << CANIT))	// is it a messagebox interrupt?
  {
    if ((CANSIT1 & TX_INT_MSK) == 0)	// is it a Rx interrupt?
    {
      for (i = 0; (i < NB_RX_MOB) && (CANGIT & (1 << CANIT)); i++)	// Search the first MOb received
      {
        Can_set_mob(i);			// Change to MOb
        if (CANSTMOB & MOB_RX_COMPLETED)// receive ok
        {
          Can_clear_status_mob();	// Clear status register
	  Can_mob_abort();		// and disable the MOb = received
	  msg_received++;
        }
        else if (CANSTMOB & ~MOB_RX_COMPLETED)	// error
        {
          Can_clear_status_mob();	// Clear status register
	  Can_config_rx_buffer();	// and reconfigure as receive buffer
        }
      }
    }
    else				// It is a Tx interrupt	 
    {
      for (i = NB_RX_MOB; i < NB_MOB; i++)// Search the first MOb transmitted
      {
        Can_set_mob(i);			// change to MOb
        if (CANSTMOB)			// transmission ok or error?
        {
          Can_clear_status_mob();	// clear status register
	  CANCDMOB = 0;			// disable the MOb
	  break;
        }
      }
    }
  }
  
  CANPAGE = saved_page;
  
  // Bus Off Interrupt Flag
  if (CANGIT & (1 << BOFFIT))     // Finaly clear the interrupt status register
  {
    CANGIT |= (1 << BOFFIT);      // Clear the interrupt flag
  }
  else
    CANGIT |= (1 << BXOK) | (1 << SERG) | (1 << CERG) | (1 << FERG) | (1 << AERG);// Clear other interrupts
}

/// *****************************************************************************
// CAN Timer Interrupt
// ******************************************************************************
#pragma type_attribute = __interrupt
#pragma vector=OVRIT_vect
void OVRIT_interrupt(void)
{
  CANGIT |= (1 << OVRTIM);//Just clear the interrupt flag.......for now
}
*/
