/*
 * EventHub.h
 *
 *  Created on: Sep 4, 2011
 *      Author: Pauli Rikula
 */

#ifndef EVENTHUB_H_
#define EVENTHUB_H_

#include <set>

#include <iostream>

//#include <typeinfo>

#include "EventReceiver.h"

#include "Classcomp.h"

//using namespace std;

/*struct typeAndClass{
	type_info aType;
	EventReceiver theClass;
};*/


class EventHub {
public:
	EventHub();
	virtual ~EventHub();
	virtual void sendEvent();
	virtual void addEventReceiver(EventReceiver * r);


private:
	std::set <EventReceiver*, Classcomp> receivers;
};

#endif /* EVENTHUB_H_ */
