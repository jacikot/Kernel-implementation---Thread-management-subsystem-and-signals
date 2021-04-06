/*
 * qSignal.h
 *
 *  Created on: Aug 7, 2020
 *      Author: OS1
 */

#ifndef QSIGNAL_H_
#define QSIGNAL_H_
typedef unsigned SignalId;
typedef void (*SignalHandler)();
class QSignal {
	struct elem {
		SignalId info;
		elem*next;
		elem(SignalId p, elem*n = 0) :info(p), next(n) {
		}
	};
public:
	void insert(SignalId p);
	int deleteFirstNotBlocked();
	int empty();
	QSignal() :head(0), tail(0) {}
	~QSignal();
private:
	elem*head, *tail;
};

class QHandler{
	struct elem {
			SignalHandler info;
			elem*next;
			elem(SignalHandler h, elem*n = 0) :info(h), next(n) {}
	};
public:
	void insert(SignalHandler i);
	SignalHandler deleteFirst();
	int empty();
	QHandler():head(0),tail(0){}
	~QHandler();
	void swapElements(SignalHandler h1, SignalHandler h2);
	void deleteAll();
	void callAll();
private:

	elem*find(SignalHandler h);
	elem*head,*tail;
};


#endif /* QSIGNAL_H_ */
