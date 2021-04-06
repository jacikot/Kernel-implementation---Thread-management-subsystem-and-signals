/*
 * signal.cpp
 *
 *  Created on: Aug 7, 2020
 *      Author: OS1
 */
#include "signals.h"
#include "pcb.h"
#include <iostream.h>
unsigned SignalArray::blockedGlobally[16]={0};


int SignalArray::handleSignals(SignalId id){
	if(blocked[id]==1||blockedGlobally[id]==1) return 1;
	if(!handlers[id].empty()){
		handlers[id].callAll();
	}
	return 0;
}

void default0(){
	PCB::running->destroy=1;
	dispatch();
}

SignalArray::SignalArray():blocked(),handlers(){
		handlers[0].insert(default0);
}

void SignalArray::blockGlobally(SignalId i){
		blockedGlobally[i]=1;
}

void SignalArray::unblockGlobally(SignalId i){
		blockedGlobally[i]=0;
}

void SignalArray::blockSignal(SignalId i){
		blocked[i]=1;
}

void SignalArray::unblockSignal(SignalId i){
	blocked[i]=0;
}


void SignalArray::registerHandler(SignalId i,SignalHandler h){
		handlers[i].insert(h);
}

void SignalArray::unregisterHandles(SignalId i){
		handlers[i].deleteAll();
}

void SignalArray::swap(SignalId id, SignalHandler hand1, SignalHandler hand2){
		handlers[id].swapElements(hand1,hand2);
}

int SignalArray::isBlocked(SignalId id){
		return blocked[id]||blockedGlobally[id];
}
