/*
 * userMain.h
 *
 *  Created on: Jun 25, 2020
 *      Author: OS1
 */

#ifndef USERMAIN_H_
#define USERMAIN_H_
#include "pcb.h"
#include "thread.h"
#include "SCHEDULE.H"
#include <iostream.h>
#include "maskInt.h"
#include "timer.h"
extern int counttick;
extern int finishedmain;
extern volatile int schedulerCounter;
int userMain(int argc,char*argv[]);

class my:public Thread{
	 //nit koja se koristi kao nit u pozadini
	//u slucaju da dodje do context switch-a, a ne postoji ni jedna nit osim runnig u scheduleru
public:
	my(){}
	my(unsigned i, unsigned j):Thread(i,j){}
	virtual ~my(){}
protected:
	void run();
};


class UserMain: public Thread{
	//korisnicka nit koja pokrece korisnicku funkciju userMain
public:
	UserMain(int argc1,char*argv1[]);
	int getRet(){
		//povratna vrednost
			return ret;
	}
protected:
	void run();

private:
	int argc;
	char** argv;
	int ret;
	PCB*oldMain;

};

#endif /* USERMAIN_H_ */
