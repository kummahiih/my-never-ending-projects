/*
 * EventHub.cpp
 *
 *  Created on: Sep 4, 2011
 *      Author: Pauli Rikula
 */

#include "EventHub.h"

#include <iostream>

using namespace std;

EventHub::EventHub() {
	// TODO Auto-generated constructor stub

};

EventHub::~EventHub() {
	// TODO Auto-generated destructor stub
};


void EventHub::sendEvent(){
	std::set <EventReceiver*, Classcomp>::iterator it;
	//cout<< "set size: "<<receivers.size()<<endl;
	for( it = receivers.begin(); it != receivers.end(); it++ ) {
		//cout<<"calling"<<endl;

		(*it)->receive();
	}

	return;
};

void  EventHub::addEventReceiver(EventReceiver * r){
	//cout<<"inserting"<<endl;
	this->receivers.insert(r);
};
