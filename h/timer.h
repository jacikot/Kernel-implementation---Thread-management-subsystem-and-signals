/*
 * timer.h
 *
 *  Created on: Apr 12, 2020
 *      Author: OS1
 */

#ifndef TIMER_H_
#define TIMER_H_
void tick();
extern volatile unsigned explicitContext;
void interrupt timer(...);
extern volatile unsigned int inSem;

void inic();
void restore();

#endif /* TIMER_H_ */
