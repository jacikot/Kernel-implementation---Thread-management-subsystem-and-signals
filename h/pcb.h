/*
 * pcb.h
 *	Sadrzi klasu PCB zajedno sa listom u kojoj se cuvaju PCB
 *  Created on: Apr 9, 2020
 *      Author: OS1
 */

#ifndef PCB_H_
#define PCB_H_

#include "thread.h"
class QSignal;
class SignalArray;
class PCB{
private:
	//registers
	unsigned int sp;
	unsigned int ss;
	unsigned int bp;
	//identification
	static int ID;
	int id;
	//memory
	unsigned long stackSize;
	unsigned* stack;
	//time
	unsigned int timeSlice;
	unsigned int finished;
	unsigned int noTimeDispatch;
	//blocking
	unsigned int blocked;
	unsigned int timerDeblocking;
	//thread
	Thread* thread;
	static PCB* running;
	Thread*parent;
	//signals
	SignalArray* signals;
	QSignal* sentSignals;
	unsigned int destroy;
public:
	~PCB();
	int getID() const{
		return id;
	}
	void destroyThread();
	Thread* getThread(){
		return thread;
	}
	int isBlockedSignal(SignalId id);
	void handleSignals();
	unsigned int isInfinite(){
		return noTimeDispatch;
	}
	static void wrapper(); //fja koja poziva odgovarajucu run metodu polimorfno
	friend class QSignal;
	friend class PCBList;
	friend class Thread;
	friend class UserMain;
	friend class KernelSem;
	friend class Semaphore;
	friend class KernelEv;
	friend void interrupt timer(...);
	friend void inic();
	friend void default0();
	PCB();

};



class PCBlist {
	struct elem {
		PCB*info;
		elem*next;
		elem(PCB*p, elem*n = 0) :info(p), next(n) {}
	};

public:
	PCBlist() :head(0), tail(0) {}
	int insertElem(PCB*newPCB);
	PCB* takeElem(int id);
	int deleteElem(int id);
	PCB* getElem(int id) const;
	~PCBlist();
	void listElem()const; //za proveru
private:
	elem*head, *tail;


};


extern PCBlist list;



#endif /* PCB_H_ */
