/*
 * MyGpio.h
 *
 *  Created on: Mar 3, 2012
 *      Author: ernad
 */

#ifndef MYGPIO_H_
#define MYGPIO_H_

unsigned GetInput(void);
void setPowerLedOutput(unsigned high);
int OpenSystemController(void);

void setRTS(unsigned high);

#endif /* MYGPIO_H_ */
