/*
 * signal.h
 *
 *  Created on: Aug 7, 2020
 *      Author: OS1
 */

#ifndef SIGNALS_H_
#define SIGNALS_H_
#include "qSignal.h"
typedef void (*SignalHandler)();
typedef unsigned SignalId;

void default0();
class SignalArray{
public:
	SignalArray();
	static void blockGlobally(SignalId i);
	static void unblockGlobally(SignalId i);
	void blockSignal(SignalId i);
	void unblockSignal(SignalId i);
	void registerHandler(SignalId i,SignalHandler h);
	void unregisterHandles(SignalId i);
	void swap(SignalId id, SignalHandler hand1, SignalHandler hand2);
	int isBlocked(SignalId id);
	int handleSignals(SignalId id);
private:
	static unsigned blockedGlobally[16];
	unsigned blocked[16];
	QHandler handlers[16];
};


#endif /* SIGNALS_H_ */
