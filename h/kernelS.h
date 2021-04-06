/*
 * kernelSem.h
 *
 *  Created on: Jun 26, 2020
 *      Author: OS1
 */

#ifndef KERNELS_H_
#define KERNELS_H_

#include "maskint.h"
#include "qBlocked.h"
typedef unsigned int Time;
class SEMlist;

class KernelSem{
public:
	KernelSem(int v);
	~KernelSem();
	int getValue(){
		return val;
	}
	void decVal(){
		val--;
	}
	void addVal(int i){
		val+=i;
	}
	void block(int time=0, int infinite=1);
	void deblock();
	int wait(Time maxTime);
	int signal(int n);
	static void time();
	int getID(){
		return id;
	}

private:
	int val;
	int id;
	static int ID;
	QBlocked blocked;
	static SEMlist sl;
};

extern volatile unsigned int inSem;
class SEMlist {
	struct elem {
		KernelSem*info;
		elem*next;
		elem(KernelSem*p, elem*n = 0) :info(p), next(n) {}
	};

public:
	SEMlist() :head(0), tail(0),s(0) {}
	int insertElem(KernelSem*newPCB);
	KernelSem* takeElem(int id);
	int deleteElem(int id);
	KernelSem* getElem(int id) const;
	int size(){
		return s;
	}
	~SEMlist();
private:
	elem*head, *tail;
	int s;


};


#endif /* KERNELS_H_ */
