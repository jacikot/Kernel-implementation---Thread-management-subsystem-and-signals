/*
 * qBlocked.cpp
 *
 *  Created on: Jun 26, 2020
 *      Author: OS1
 */

#include "qBlocked.h"
#include "maskInt.h"
#include <iostream.h>
QBlocked::~QBlocked()
{
	while(head!=0) {
		elem*cur = head;
		head = head->next;
		delete cur;
	}
	tail = 0;
}
int QBlocked::empty(){
	if(head==0) return 0;
	else return 1;
}
void QBlocked::insert(PCB*thread, int time, int infinite) {
	if (infinite != 0) {
		time = 0; infinite = 1;
	}

	elem*cur = head;
	if (cur == 0) {
		head = tail = new elem(thread, time, infinite);
		return;
	}
	if (infinite == 1) {
		tail = tail->next = new elem(thread);
	}
	else {
		if ((cur->infinite == 1) || (cur->time > time)) {
			head = new elem(thread, time, infinite, head);
			return;
		}
		while ((cur->next != 0) && (cur->next->infinite == 0) && (cur->next->time <= time))cur = cur->next;
		if (cur->next == 0) tail = tail->next = new elem(thread, time, infinite);
		else cur->next = new elem(thread, time, infinite, cur->next);
	}
}
PCB* QBlocked::deleteFirst() {
	if (head == 0) return 0;
	elem*cur = head;
	head = head->next;
	if (head == 0) tail = 0;
	PCB*thread = cur->info;
	delete cur;
	return thread;
}

PCB* QBlocked::deleteFirstFinished() {
	if (head == 0) return 0;
	if ((head->infinite == 1) || (head->time != 0)) return 0;
	return deleteFirst();
}
void QBlocked::reduceTime() {

	for (elem*cur = head; (cur != 0) && (cur->infinite == 0); cur = cur->next) {
		cur->time--;
	}
}

