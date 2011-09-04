/*
 * EventReceiver.cpp
 *
 *  Created on: Sep 4, 2011
 *      Author: Pauli Rikula
 */

#include "EventReceiver.h"

#include <iostream>

using namespace std;


EventReceiver::EventReceiver(void * r) {
	target = r;
	// TODO Auto-generated constructor stub

}

EventReceiver::~EventReceiver() {
	// TODO Auto-generated destructor stub
}

void EventReceiver::receive() const
{
	//cout << "foo"<<endl;
}
