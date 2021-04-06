/*
 * pcb.cpp
 * sadrzi implementaciju pcb liste
 *  Created on: Apr 9, 2020
 *      Author: OS1
 */

#include "pcb.h"
#include <iostream.h>
#include "thread.h"
#include "qSignal.h"
#include "maskInt.h"
#include "signals.h"
extern volatile unsigned explicitContext;
PCB*PCB::running=0;

PCB::PCB():id(ID++), timerDeblocking(0),
		finished(0),noTimeDispatch(0),timeSlice(defaultTimeSlice),blocked(0),
		destroy(0),signals(new SignalArray()),sentSignals(new QSignal()){
	if(id==0) { //ako je main nit to je samo PCB
		thread=0;
		parent=0;
	}
	else parent=PCB::running->thread;
}

void PCB::wrapper(){
	running->thread->run();
	running->finished=1;
	if(running->parent!=0) {
		running->parent->signal(1);
	}
	running->thread->signal(2);//nit se zavrsila i prosledjuje joj se taj signal
	dispatch();
}

void PCB::destroyThread(){
	delete [] stack;
	stack=0;
	delete sentSignals;
	sentSignals=0;
	delete signals;
	signals=0;
	finished=1;
	asm cli;
}
void PCB::handleSignals(){
	noTimeDispatch=1;
	int signal=sentSignals->deleteFirstNotBlocked();
	while(signal!=16){
		signals->handleSignals((SignalId)signal);
		signal=sentSignals->deleteFirstNotBlocked();
	}
	noTimeDispatch=0;
}

int PCB::isBlockedSignal(SignalId id){
		return signals->isBlocked(id);
}
 PCB::~PCB(){
	 if(signals!=0)delete signals;
	 if(sentSignals!=0) delete sentSignals;
 }
void PCBlist::listElem()const {
	elem*tek = head;
	while (tek != 0) {
		cout << tek->info->getID();
		tek = tek->next;
	}
}

int PCBlist::insertElem(PCB* newpcb) {
	elem*tek = head;
	while (tek != 0) {
		if (tek->info == newpcb) break;
		tek = tek->next;
	}
	if (tek != 0) return -1;
	elem* cur = new elem(newpcb);
	if (head == 0) head = tail = cur;
	else {
		tail->next = cur;
		tail = tail->next;
	}
	return 0;
}

PCB* PCBlist::takeElem(int id) {
	PCB* cur = 0;
	elem*tek = head;
	elem*prev = 0;
	while (tek != 0) {
		if (tek->info->getID() == id) {
			if(prev!=0) prev->next = tek->next;
			else {
				head = tek->next; //azurirali head ako uzimamo prvi
			}
			if (tek == tail) { //ukoliko smo uzeli poslednji
				tail = prev;
			}
			cur = tek->info;
			tek->info = 0;
			delete tek;
			tek = 0;
		}
		else {
			prev = tek;
			tek = tek->next;
		}
	}
	return cur; //vraca null ako nije nasao
}

int PCBlist::deleteElem(int id) {
	PCB* cur = takeElem(id);
	if (cur == 0) return -1;
	delete cur;
	return 0;
}

PCB* PCBlist::getElem(int id) const {
	elem*tek = head;
	while (tek != 0) {
		if (tek->info->getID() == id) return tek->info;
		tek = tek->next;
	}
	return 0;
}

PCBlist::~PCBlist() { //brisanje liste brise i PCB iz nje jer jedino gde se oni cuvaju je u listi
	elem*tek = head;
	while (tek != 0) {
		head = head->next;
		delete tek->info;
		delete tek;
		tek = head;
	}
	tail = 0;
}

int PCB::ID=0;
PCBlist list;




