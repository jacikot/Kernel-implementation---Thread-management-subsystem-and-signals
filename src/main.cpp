/*
 * main.cpp
 *
 *  Created on: Apr 9, 2020
 *      Author: OS1
 */
#include "pcb.h"
#include "thread.h"
#include "SCHEDULE.H"
#include <iostream.h>
#include "maskInt.h"
#include "timer.h"
#include "userMain.h"
int counttick=0;
int finishedmain=0;
volatile int schedulerCounter=0;
my* dummy;
//nit koja se izvrsava ako u scheduleru nema nijedna
//ona se ne stavlja u scheduler vec ako get vrati 0, uzimamo prvu nit iz liste tj njen pcb i izvrsavamo ga do dispatcha
//dispatch ce staviti u listu
int main(int argc, char*argv[]){
	cout<<"Main entered"<<endl;
	inic();
	lockNoInt
	dummy=new my(1024,0);
	//pokretanje pomocne niti koja ne radi nista ali uvek tece u pozadini ako ne postoji nijedna druga
	//dokle god se usermain ne zavrsi
	UserMain* um=new UserMain(argc,argv);
	um->start();
	unlockNoInt
	dispatch();
	int ret=um->getRet();
	restore();
	delete um;
	delete dummy;
	cout<<"kraj"<<endl;
	return ret;
}



