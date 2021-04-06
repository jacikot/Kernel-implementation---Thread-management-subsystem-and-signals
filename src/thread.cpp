/*
 * thread.cpp
 *
 *  Created on: Apr 9, 2020
 *      Author: OS1
 */

#include "thread.h"
#include "pcb.h"
#include <dos.h>
#include <iostream.h>
#include "SCHEDULE.h"
#include "maskInt.h"
#include "timer.h"
#include "signals.h"
extern int schedulerCounter;
volatile unsigned int inDisp=0;
void dispatch(){
	  // maskira PSWI jer ovakav poziv timera to ne radi
	 //pritom se radi push starih vrednosti PSW jer u slucaju da je kod pozivaoca
	//PSWI bio maskiran to mora tako i da ostane

	lockNested
	explicitContext=1;
	inDisp=1;
	unlockNested
	//timer();
	asm int 8h

}

ID Thread::getId(){
	return myPCB->id;
}

ID Thread::getRunningId(){
	return PCB::running->id;
}

Thread * Thread:: getThreadById(ID id){
	if (id<0)return 0;
	PCB*pcb=list.getElem(id);
	if(pcb==0) return 0;
	return pcb->thread;

}

Thread::~Thread(){
	if(myPCB==0)return;
	waitToComplete();
	//lockNoInt
	if(myPCB->stack!=0) delete[] myPCB->stack;
	list.deleteElem(myPCB->id);
	//unlockNoInt
}

Thread::Thread(StackSize stackSize, Time timeSlice){
	//lockNoInt
	stackSize/=sizeof(unsigned); //jer je stek u bajtovima
	if(stackSize>65535){
		myPCB=0;
		//unlockNoInt
		return;
	}
	unsigned* st=new unsigned[stackSize];
	myPCB=new PCB();
	//PSW
	myPCB->thread=this;
	if(timeSlice==0) myPCB->noTimeDispatch=1;
	st[stackSize-1]=0x200;

#ifndef BCC_BLOCK_IGNORE
	//cs
	st[stackSize-2]=FP_SEG(PCB::wrapper);
	st[stackSize-3]=FP_OFF(PCB::wrapper);
	//vrh steka
	myPCB->sp=FP_OFF(st+stackSize-12);
	myPCB->ss=FP_SEG(st+stackSize-12);
	myPCB->bp=FP_OFF(st+stackSize-12);
#endif
	myPCB->stack=st;
	myPCB->stackSize=stackSize;
	myPCB->timeSlice=timeSlice;
	list.insertElem(myPCB);
	//unlockNoInt
}

void Thread::start(){
if(myPCB==0)return;
Scheduler::put(this->myPCB);
schedulerCounter++;
}

void Thread::waitToComplete(){
	if(this->myPCB==0)return;
	if(this->myPCB->id==1)return; //ako je to dummy thread ne cekati je
	while (!this->myPCB->finished){
		//cout<<PCB::running->id<<" ceka "<<myPCB->id<<endl;
		dispatch();
	}
}

void Thread::blockSignalGlobally(SignalId id){
	if(id>15)return;
	SignalArray::blockGlobally(id);
}

void Thread::unblockSignalGlobally(SignalId id){
	if(id>15)return;
	SignalArray::unblockGlobally(id);
}

void Thread::blockSignal(SignalId id){
	if(id>15)return;
	myPCB->signals->blockSignal(id);
}

void Thread::unblockSignal(SignalId id){
	if(id>15)return;
	myPCB->signals->unblockSignal(id);
}


void Thread::registerHandler(SignalId signal, SignalHandler handler){
	if(signal>15||handler==0)return;
	myPCB->signals->registerHandler(signal,handler);
}
void Thread::unregisterAllHandlers(SignalId id){
	if(id>15)return;
	myPCB->signals->unregisterHandles(id);
}

void Thread::swap(SignalId id, SignalHandler hand1, SignalHandler hand2){
	if(id>15||hand1==0||hand2==0) return;
	myPCB->signals->swap(id,hand1,hand2);
}
void Thread::signal(SignalId id){
	if(id>15)return;
	myPCB->sentSignals->insert(id);
}
