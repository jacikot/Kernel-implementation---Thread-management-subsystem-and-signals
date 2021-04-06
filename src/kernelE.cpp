/*
 * kernelE.cpp
 *
 *  Created on: Jul 30, 2020
 *      Author: OS1
 */
#include "kernelE.h"
#include "ivt.h"
#include "pcb.h"
#include "schedule.h"
extern int inSem;
KernelEv::KernelEv(unsigned char e):blocked(0),val(0){
	entry=IVTEntry::getEntry(e);
	entry->setEventObj(this);
	owner=PCB::running;
}

void KernelEv::wait(){
	PCB::running->noTimeDispatch=1;
	if(PCB::running!=owner)return;
	if(val==1)val=0;
	else{
		val=0;
		owner->blocked=1;
		blocked=owner;
		inSem=1;
		dispatch();
		owner->blocked=0;

	}
	PCB::running->noTimeDispatch=0;
}

void KernelEv::signal(){
	PCB::running->noTimeDispatch=1;
	if(blocked==0) val=1;
	else{
		val=0;
		blocked=0;
		Scheduler::put(owner);
	}
	PCB::running->noTimeDispatch=0;
}

KernelEv::~KernelEv(){
	PCB::running->noTimeDispatch=1;
	if(blocked!=0){
		Scheduler::put(owner); //ako je neka nit bila blokirana bice odblokirana
	}
	entry->deleteEventObj();
	PCB::running->noTimeDispatch=0;

}
