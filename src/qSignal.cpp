/*
 * qSignal.cpp
 *
 *  Created on: Aug 7, 2020
 *      Author: OS1
 */

#include "qSignal.h"
#include "pcb.h"
#include "iostream.h"
void QSignal::insert(SignalId p) {
	if (head == 0)head = tail = new elem(p);
	else {
		tail = tail->next = new elem(p);
	}
}

int QSignal::deleteFirstNotBlocked() {
	elem*cur = head;
	elem*prev = 0;
	while (cur != 0) {
			if (PCB::running->isBlockedSignal(cur->info) == 0) {
			if (prev == 0) head = cur->next;
			else prev->next = cur->next;
			if (cur == tail)tail = prev;
			int id = cur->info;
			delete cur;
			return id;
		}
		prev = cur;
		cur = cur->next;
	}
	return 16;
}

int QSignal::empty() {
	return head == 0;
}

QSignal::~QSignal() {
	while (head != 0) {
		elem*cur = head;
		head = head->next;
		delete cur;
	}
}

void QHandler::insert(SignalHandler i){
	if (head == 0)head = tail = new elem(i);
		else {
			tail = tail->next = new elem(i);
		}
}

SignalHandler QHandler:: deleteFirst(){
	if (head == 0) return 0;
	elem*cur = head;
	head = head->next;
	if (head == 0) tail = 0;
	SignalHandler h = cur->info;
	delete cur;
	return h;
}

int QHandler::empty(){
	return head==0;
}

QHandler::~QHandler(){
	deleteAll();
}

void QHandler::deleteAll(){
	while(deleteFirst()!=0);
}

void QHandler::swapElements(SignalHandler h1, SignalHandler h2){
	elem*h1p=find(h1);
	if(h1p==0)return;
	elem*h2p=find(h2);
	if(h2p==0)return;
	h1p->info=h2;
	h2p->info=h1;
}

QHandler::elem*QHandler::find(SignalHandler h){
	elem*hp=head;
	while(hp!=0&&hp->info!=h)hp=hp->next;
	return hp;
}

void QHandler::callAll(){
		elem*cur=head;
		while(cur!=0){
			cur->info();
			cur=cur->next;
		}
}
