/*
 * semaphor.cpp
 *
 *  Created on: Jun 26, 2020
 *      Author: OS1
 */


#include "semaphor.h"
#include<iostream.h>
#include "kernelS.h"
#include "maskInt.h"
Semaphore::Semaphore(int init){
	myImpl=new KernelSem(init);
}

Semaphore::~Semaphore (){
	delete myImpl;
}

volatile unsigned int inSem=0;
int Semaphore::wait(Time maxTime){
	return myImpl->wait(maxTime);
}

int Semaphore::signal(int n){
	return myImpl->signal(n);
}

int Semaphore::val() const{
	return myImpl->getValue();
}
