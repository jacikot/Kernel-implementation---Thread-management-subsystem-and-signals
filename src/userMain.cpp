/*
 * userMain.cpp
 *
 *  Created on: Jun 25, 2020
 *      Author: OS1
 */

#include "userMain.h"
#include "semaphor.h"
#include "maskInt.h"


void my::run(){
		while(!finishedmain){
			//lockNoInt
			//cout<<"neka druga";
			//lockNoInt
			dispatch();
		}
}

UserMain::UserMain(int argc1,char*argv1[]):argc(argc1),argv(argv1),ret(0),Thread(8192){
		//cuva staru sistemsku nit koja ne treba da se izvrsava dok je u toku korisnicki program
		//pcb te niti formiran je u inic delu i on ne treba da bude u scheduleru vec ga cuva userMain
		oldMain=PCB::running;
}

void UserMain::run(){
		//funkciju userMain definise korisnik
		ret=userMain(argc,argv);
		cout<<"userMain finished"<<endl;
		//nakon zavrsenog usermaina postavicemo nit maina u scheduler i finishedminom signaliciramo
		//pomocnoj niti da se vise ne izvrsava
		schedulerCounter++;
		Scheduler::put(oldMain);
		finishedmain=1;
		//dispatch();
}

