/*
 This file is part of the NCANbus, a library implementing an light version of the Canopen standard.

 Copyright (c) Elreg as 2010

 AVR implementation of the  NCAN bus timer driver, uses Timer 3 (16 bit)

 */

#include "ncanbus.h"
#include "timer.h"

#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

pthread_mutex_t timer_mutex;

pthread_t thread1;
void * runTimerThread(void *ptr);

void initTimer(void) {
	int ret;
	ret = pthread_create(&thread1, NULL, runTimerThread, (void*) NULL);
	setTimer(0xFFFF);
}
unsigned long last_time_set = 0;
long timerPeriod = -1;

void * runTimerThread(void *ptr) {
	struct timeval currentTime;
	long newTime;
	gettimeofday(&currentTime, NULL);
	last_time_set = (currentTime.tv_sec * 1000000 + currentTime.tv_usec) / 8;

	while (1) {
		//sleep(0);
		usleep(100);

		pthread_mutex_lock(&timer_mutex);
		gettimeofday(&currentTime, NULL);
		newTime = (currentTime.tv_sec * 1000000 + currentTime.tv_usec) / 8;


	//	usleep(100);
		if (timerPeriod > 0) {
			if ((newTime - last_time_set) > timerPeriod) {
				last_time_set = newTime;
				pthread_mutex_unlock(&timer_mutex);
				TimeDispatch();
			} else {
				pthread_mutex_unlock(&timer_mutex);
			}
		} else {
			last_time_set = newTime;
			pthread_mutex_unlock(&timer_mutex);
		}
		//pthread_mutex_unlock(&timer_mutex);
	}
	return NULL;
}

// ******************************************************************************
// Set the timer for the next alarm.
// INPUT	value TIMEVAL (unsigned long)
// RETURNS	void
// ******************************************************************************
void setTimer(TIMEVAL value) {
	//printf("Set Timer: %lu\n", value);
	struct timeval currentTime;
	pthread_mutex_lock(&timer_mutex);
	gettimeofday(&currentTime, NULL);
	last_time_set = (currentTime.tv_sec * 1000000 + currentTime.tv_usec) / 8;

	timerPeriod = (long)value;
	pthread_mutex_unlock(&timer_mutex);
	//TimerAlarm += (int) value; // Add the desired time to timer interrupt time
}

// *****************************************************************************
// Return the elapsed time to tell the Stack how much time is spent since last call.
// INPUT	void
// RETURNS	value TIMEVAL (unsigned long) the elapsed time
// ******************************************************************************
TIMEVAL getElapsedTime(void) {
	struct timeval currentTime;
	long newTime;
	long tempTime;
	pthread_mutex_lock(&timer_mutex);
	gettimeofday(&currentTime, NULL);
	newTime = (currentTime.tv_sec * 1000000 + currentTime.tv_usec) / 8;
	//printf("getElapsedTime %lu\n", (newTime - last_time_set));
	tempTime = newTime - last_time_set;
	pthread_mutex_unlock(&timer_mutex);
	return tempTime;
}

/*

 // Define the timer registers
 #define TimerAlarm        OCR3B
 #define TimerCounter      TCNT3

 // ************************* Modul variables **********************************
 // Store the last timer value to calculate the elapsed time
 static TIMEVAL last_time_set = TIMEVAL_MAX;

 // *****************************************************************************
 Initializes the timer, turn on the interrupt and put the interrupt time to zero
 INPUT	void
 RETURNS	void
 ******************************************************************************
 void initTimer(void)
 {
 TimerAlarm = 0;		// Set it back to the zero
 // Set timer 3 for NCANbus operation tick 8us max, time is 524ms
 TCCR3B = 1 << CS31 | 1 << CS30;       // Timer 3 normal, with CK/64
 TIMSK3 = 1 << OCIE3B;                 // Enable the interrupt
 }

 // ******************************************************************************
 Set the timer for the next alarm.
 INPUT	value TIMEVAL (unsigned long)
 RETURNS	void
 ******************************************************************************
 void setTimer(TIMEVAL value)
 {
 TimerAlarm += (int)value;	// Add the desired time to timer interrupt time
 }

 // *****************************************************************************
 Return the elapsed time to tell the Stack how much time is spent since last call.
 INPUT	void
 RETURNS	value TIMEVAL (unsigned long) the elapsed time
 ******************************************************************************
 TIMEVAL getElapsedTime(void)
 {
 unsigned int timer = TimerCounter;            // Copy the value of the running timer
 if (timer > last_time_set)                    // In case the timer value is higher than the last time.
 return (timer - last_time_set);             // Calculate the time difference
 else if (timer < last_time_set)
 return (last_time_set - timer);             // Calculate the time difference
 else
 return TIMEVAL_MAX;
 }


 #pragma type_attribute = __interrupt
 #pragma vector=TIMER3_COMPB_vect
 // *****************************************************************************
 Interruptserviceroutine Timer 3 Compare B for the CAN timer
 ******************************************************************************
 void TIMER3_COMPB_interrupt(void)
 {
 last_time_set = TimerCounter;
 TimeDispatch();                               // Call the time handler of the stack to adapt the elapsed time
 }
 */

