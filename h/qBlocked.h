/*
 * qBlocked.h
 *
 *  Created on: Jun 26, 2020
 *      Author: OS1
 */

#ifndef QBLOCKED_H_
#define QBLOCKED_H_
#include "pcb.h"
#include <iostream.h>
class QBlocked
{
	struct elem {
		PCB*info;
		int time;
		int infinite;
		elem*next;
		elem(PCB*p, int t = 0, int inf=1, elem*n = 0) :info(p), next(n), time(t), infinite(inf) {
		}
	};
public:
	void insert(PCB*thread, int time, int infinite);

	PCB* deleteFirst();

	PCB* deleteFirstFinished();
	int empty();
	void reduceTime();
	QBlocked():head(0),tail(0){}
	~QBlocked();
private:
	elem*head,*tail;
};



#endif /* QBLOCKED_H_ */
