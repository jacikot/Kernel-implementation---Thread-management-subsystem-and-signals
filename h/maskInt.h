/*
 * maskInt.h
 *
 *  Created on: Apr 12, 2020
 *      Author: OS1
 */

#ifndef MASKINT_H_
#define MASKINT_H_
//sa maskiranjem prekida
#include "timer.h"
#define lock asm cli
#define unlock asm sti

extern volatile unsigned int lockflag;


//bez maskiranja prekida
#define lockNoInt lockflag=1;
#define unlockNoInt {\
	if(lockflag>0) lockflag=0;\
	if(explicitContext&&lockflag==0) dispatch();\
}

#define lockNoInt2 lockflag++;
#define unlockNoInt2 {\
	if(lockflag>0) lockflag--;\
}
//za ugnezdene kriticne sekcije
#define lockNested asm{\
	pushf;\
	cli;\
}
#define unlockNested asm popf




#endif /* MASKINT_H_ */
