/*
 * kernelSem.cpp
 *
 *  Created on: Jun 26, 2020
 *      Author: OS1
 */

#include "kernelS.h"
#include "SCHEDULE.H"
#include "maskInt.h"
#include <iostream.h>
SEMlist KernelSem::sl;
int KernelSem::ID=0;
extern int counttick;
KernelSem::KernelSem(int v):val(v),id(ID++){
	sl.insertElem(this);
}
KernelSem::~KernelSem(){
	PCB::running->noTimeDispatch=1;
	sl.takeElem(id);
	if(val<0){
		signal(-val);
	}
	PCB::running->noTimeDispatch=0;
}
int SEMlist::insertElem(KernelSem* newpcb) {
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
	s++;
	return 0;
}

KernelSem* SEMlist::takeElem(int id) {
	KernelSem* cur = 0;
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
			s--;
		}
		else {
			prev = tek;
			tek = tek->next;
		}
	}
	return cur; //vraca null ako nije nasao
}

int SEMlist::deleteElem(int id) {
	KernelSem* cur = takeElem(id);
	if (cur == 0) return -1;
	delete cur;
	return 0;
}

KernelSem* SEMlist::getElem(int id) const {
	elem*tek = head;
	while (tek != 0) {
		if (tek->info->getID() == id) return tek->info;
		tek = tek->next;
	}
	return 0;
}

SEMlist::~SEMlist() { //brisanje liste brise i PCB iz nje jer jedino gde se oni cuvaju je u listi
	elem*tek = head;
	while (tek != 0) {
		head = head->next;
		delete tek->info;
		delete tek;
		tek = head;
	}
	tail = 0;
}


void KernelSem::block(int time, int infinite){
	PCB::running->blocked=1;
	blocked.insert(PCB::running,time,infinite);
	inSem=1;
	dispatch();
	PCB::running->blocked=0;
}

void KernelSem::deblock(){
	PCB* cur=blocked.deleteFirst();
	if(cur!=0){
		Scheduler::put(cur);
	}
}

int KernelSem::wait(Time maxTime){
	PCB::running->noTimeDispatch=1;
		decVal();
		int time=maxTime,inf=0;
		if(maxTime==0){
			time=0;
			inf=1;
		}
		int ret;
		if(val<0) {
			block(time,inf);
		}
		if(PCB::running->timerDeblocking==1) {
			PCB::running->timerDeblocking=0;
			ret=0;
		}
		else ret=1;
		PCB::running->noTimeDispatch=0;
		return ret; //doradi ovo
}

int KernelSem::signal(int n){
	PCB::running->noTimeDispatch=1;
		int ret=0;
		if(n<0){
			PCB::running->noTimeDispatch=0;
			return n;
		}
		if(n==0){
			if(val<0) deblock();
			addVal(1);
			PCB::running->noTimeDispatch=0;
			return 0;
		}
		if(n>0){
			int nArgument=n;
			if(val<0){
				if((-val)<n) n=-val;
				for(int i=0;i<n;i++){
					ret++;
					deblock();
				}
			}
			addVal(nArgument);
		}
		PCB::running->noTimeDispatch=0;
		return ret;
}

void KernelSem::time(){
	int s=sl.size();
	for(int i=0;i<s;i++){
		KernelSem*ks=sl.getElem(i);
		ks->blocked.reduceTime();
		PCB*cur;
		while((cur=ks->blocked.deleteFirstFinished())!=0){
			cur->timerDeblocking=1;
			ks->addVal(1);
			Scheduler::put(cur);
		}
	}
}
