/*
This file is part of the NCAN bus, a library implementing CanOpen Light Stack.

Copyright (c) Elreg as 2010


*/

#ifndef __timer_h__
#define __timer_h__

#include "timerscfg.h"
#include "applicfg.h"

#define TIMER_HANDLE signed short

#include "data.h"

/* --------- types and constants definitions --------- */
#define TIMER_FREE 0
#define TIMER_ARMED 1
#define TIMER_TRIG 2
#define TIMER_TRIG_PERIOD 3

#define TIMER_NONE -1

typedef void (*TimerCallback_t)(CO_Data* d, unsigned long id);

struct struct_s_timer_entry {
	unsigned char state;
	CO_Data* d;
	TimerCallback_t callback; /* The callback func. */
	unsigned long id; 
	TIMEVAL val;
	TIMEVAL interval; /* Period */
};

typedef struct struct_s_timer_entry s_timer_entry;

/* ---------  prototypes --------- */
/*#define SetAlarm(d, id, callback, value, period) printf("%s, %d, SetAlarm(%s, %s, %s, %s, %s)\n",__FILE__, __LINE__, #d, #id, #callback, #value, #period); _SetAlarm(d, id, callback, value, period)*/

/**
 * Set an alarm to execute a callback function when expired.
 * *d Pointer to a CAN object data structure
 *  id The alarm Id
 *  callback A callback function
 *  value Call the callback function at current time + value
 *  period Call periodically the callback function
 * @return handle The timer handle
 */
TIMER_HANDLE SetAlarm(CO_Data* d, unsigned long id, TimerCallback_t callback, TIMEVAL value, TIMEVAL period);

/**
 *  Delete an alarm before expiring.
 *  handle A timer handle
 *  Returns The timer handle
 */
TIMER_HANDLE DelAlarm(TIMER_HANDLE handle);

void TimeDispatch(void);

/**
 *  Set a timerfor a given time.
 *  value The time value.
 */
void setTimer(TIMEVAL value);

/**
 *  Get the time elapsed since latest timer occurence.
 *  Returns time elapsed since latest timer occurence
 */
TIMEVAL getElapsedTime(void);

#endif /* #define __timer_h__ */


