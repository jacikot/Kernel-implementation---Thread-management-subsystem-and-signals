/*
 * event.cpp
 *
 *  Created on: Jul 30, 2020
 *      Author: OS1
 */

#include "event.h"
#include "kernelE.h"


Event::Event(IVTNo n):myImpl(new KernelEv(n)){}

Event::~Event(){
	delete myImpl;
}

void Event::wait(){
	myImpl->wait();
}

void Event::signal(){
	myImpl->signal();
}
