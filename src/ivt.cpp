/*
 * ivt.cpp
 *
 *  Created on: Jul 30, 2020
 *      Author: OS1
 */




#include "ivt.h"
#include <dos.h>
#include "kernelE.h"
IVTEntry* IVTEntry::entries[256];
IVTEntry::IVTEntry(unsigned char c,pInt newR):entryNo(c),sem(0){
	entries[c]=this;
#ifndef BCC_BLOCK_IGNORE
	oldR=getvect(c);
	setvect(c,newR);
#endif

}

IVTEntry::~IVTEntry(){
#ifndef BCC_BLOCK_IGNORE
	setvect(entryNo,oldR);
#endif
	entries[entryNo]=0;
	sem=0;
}

IVTEntry* IVTEntry::getEntry(unsigned char c){
	return entries[c];
}

void IVTEntry::signal(){
	if(sem!=0)sem->signal();
}

void IVTEntry::setEventObj(KernelEv*e){
		sem=e;
}
