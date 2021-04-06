/*
 * inic.cpp
 *
 *  Created on: Apr 12, 2020
 *      Author: OS1
 */
#include "timer.h"
#include "pcb.h"
#include"thread.h"
#include "maskInt.h"

unsigned oldTimerOFF, oldTimerSEG;

// postavlja novu prekidnu rutinu
void inic(){
	asm{
		cli
		push es
		push ax

		mov ax,0
		mov es,ax

		// pamti staru rutinu
		mov ax, word ptr es:0022h
		mov word ptr oldTimerSEG, ax
		mov ax, word ptr es:0020h
		mov word ptr oldTimerOFF, ax

		//postavlja novu rutinu
		mov word ptr es:0022h, seg timer
		mov word ptr es:0020h, offset timer

		//postavlja staru rutinu na ulaz 60h
		mov ax, oldTimerSEG
		mov word ptr es:0182h, ax
		mov ax, oldTimerOFF
		mov word ptr es:0180h, ax

		pop ax
		pop es
		sti
	}
	//fja main
	//sve ce se restaurirati context switchom
	lock
	PCB::running=new PCB();
	unlock
}

// vraca staru prekidnu rutinu
void restore(){
	asm {
		cli
		push es
		push ax

		mov ax,0
		mov es,ax

		//restaurira staru prekidnu rutinu, na 60h ostaje isto ona
		mov ax, word ptr oldTimerSEG
		mov word ptr es:0022h, ax
		mov ax, word ptr oldTimerOFF
		mov word ptr es:0020h, ax

		pop ax
		pop es
		sti
	}
}

