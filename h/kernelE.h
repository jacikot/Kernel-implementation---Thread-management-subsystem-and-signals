/*
 * kernelE.h
 *
 *  Created on: Jul 30, 2020
 *      Author: OS1
 */

#ifndef BCC_KERNELE_H_
#define BCC_KERNELE_H_
class IVTEntry;
class PCB;
class KernelEv{
public:
	KernelEv(unsigned char e);
	~KernelEv();
	void wait();
	void signal();
	int getVal(){
		return val;
	}
private:
	PCB* blocked;
	int val;
	IVTEntry*entry; //potencijalno moze da bude null ako ulaz nije pripremljen
	PCB*owner;
};



#endif /* BCC_KERNELE_H_ */
