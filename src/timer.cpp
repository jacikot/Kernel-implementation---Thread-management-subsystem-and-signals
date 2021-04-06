/*
 * timer.cpp
 *
 *  Created on: Apr 12, 2020
 *      Author: OS1
 */

#include "thread.h"
#include "pcb.h"
#include "SCHEDULE.H"
#include "maskInt.h"
#include "timer.h"
#include <iostream.h>
#include "kernelS.h"
#include "userMain.h"

volatile int counter=defaultTimeSlice;
volatile unsigned int lockflag=0;
volatile unsigned int tss,tsp,tbp;
volatile PCB*pom;
volatile unsigned explicitContext=0;
extern volatile unsigned int inDisp;
extern volatile int schedulerCounter;
extern my*dummy;
volatile PCB* toDestroy=0;
void interrupt timer(...){
	if( !explicitContext && !PCB::running->isInfinite() ){
		counter--;
	}
	if(!explicitContext){
		 //i ako je zasticena sekcija tajmer i dalje otkucava i poziva se stara prekidna rutina
		//i obavestava se korisnik o proteklom vremenu
		asm int 60h
		tick();
		KernelSem::time();
	}
	if((counter==0 && !PCB::running->isInfinite())||explicitContext){
		if((!lockflag&&!PCB::running->noTimeDispatch)||inDisp){
			//contextswitch
			explicitContext=0;
			inSem=0;
			inDisp=0;
			asm{
				mov tss, ss
				mov tsp, sp
				mov tbp, bp
			}
			PCB::running->ss=tss;
			PCB::running->sp=tsp;
			PCB::running->bp=tbp;

			//potencijalno korisnik bi mogao staticki da alocira thread, onda bi se ona napravila
			//pre ovih nasih niti, menjaj ovo ako se tako nesto desi
			if(PCB::running->destroy){
				//ako je poslat signal0 uradjen je dispatch sa flagom destroy
				//kada sacuvamo kontekst tekuce niti necemo je odmah unistiti
				//to je i dalje running thread i unistavanjem njenog steka necemo moci da nastavimo ovu prekidnu rutinu
				//zato prvo uradimo context switch do kraja i onda iz konteksta nove niti unistimo prethodnu
				schedulerCounter--;
				toDestroy=PCB::running;

			}
			else {
				if(!PCB::running->finished&&(PCB::running->getID()!=0)&&(PCB::running->getID()!=1)&&(PCB::running->blocked==0))
					Scheduler::put(PCB::running);
				else schedulerCounter--;
			}
			pom=Scheduler::get();
			if(pom==0) PCB::running=list.getElem(1);
			else PCB::running=(PCB*)pom;

			tbp=PCB::running->bp;
			tsp=PCB::running->sp;
			tss=PCB::running->ss;

			asm{
				mov bp, tbp
				mov sp, tsp
				mov ss, tss
			}
			counter= PCB::running->timeSlice;
			if(toDestroy!=0){
				//ovde unistavamo staru nit za koju je pokrenut signal0
				toDestroy->destroyThread();
				toDestroy=0;
			}
			PCB::running->handleSignals();
		}
		else {
			explicitContext=1;
		}
	}


}

