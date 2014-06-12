/*
This file is part of the NCAN bus, a library implementing CanOpen Light Stack.

Copyright (C): Elreg A.S, Terje Sparengen


*/

/** 
 * @brief Send the consise dcf to node corresping to nodeId
 * @param *d Pointer on a CAN object data structure
 * @param nodeId Id of the slave node
 * @return
 */
unsigned char send_consise_dcf(CO_Data* d, unsigned char nodeId);
