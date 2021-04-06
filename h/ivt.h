/*
 * ivt.h
 *
 *  Created on: Jul 30, 2020
 *      Author: OS1
 */

#ifndef IVT_H_
#define IVT_H_
typedef void interrupt (*pInt) (...);
void interrupt timer(...);
extern int explicitContext;
class KernelEv;
class IVTEntry{
public:
	//IVTEntry(){}
	IVTEntry(unsigned char c,pInt newR);
	~IVTEntry();
	static IVTEntry* getEntry(unsigned char c);
	void signal();
	void setEventObj(KernelEv*e);
	pInt getOldR(){
		return oldR;
	}
	void deleteEventObj(){
		sem=0;
	}
private:
	static IVTEntry* entries [256];
	unsigned char entryNo;
	KernelEv*sem;
	pInt oldR;
};

#define PREPAREENTRY(entryNo,callOldR)\
	void interrupt routine##entryNo(...);\
	IVTEntry ivtEntry##entryNo((unsigned char)entryNo,routine##entryNo);\
	void interrupt routine##entryNo(...){\
		ivtEntry##entryNo.signal();\
		if(callOldR!=0){\
			(ivtEntry##entryNo.getOldR())();\
		}\
		explicitContext=1;\
		timer();\
	}\


//svaka od tih prekidnih rutina  izvrsi signal za dogadjaj na koji se odnosi
//ukoliko je postavljeno da se poziva stara prekidna rutina pozove se
//izvrsi se context switch na kraju postavljanjem explicitContext kako se ne bi vrsilo dekrementiranje brojaca timera
//pozove se prekidna rutina timer (moze kao funkcija jer su prekidi svakako zabranjeni ulaskom u ovu prkidnu rutinu)
//ako se nalazimo u nekom kodu kernela koji je zasticen context switch se nece izvrsiti


#endif /* IVT_H_ */
