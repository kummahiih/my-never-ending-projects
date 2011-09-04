//============================================================================
// Name        : signnalsreinvented.cpp
// Author      : Pauli Rikula
// Version     :
// Copyright   : MIT License
// Description : See ../doc/readme.txt
//============================================================================

#include <iostream>

#include "EventHub.h"
#include "EventReceiver.h"

using namespace std;

class DummySignalSource
{
public:
	DummySignalSource(){};
	virtual ~DummySignalSource(){};
	virtual void exampleSignalEmitingFnc(){
		cout << "This should send a signal" << endl;
	};
};


class DummySignalSink
{
public:
	DummySignalSink(){}

	virtual ~DummySignalSink(){};
	void exampleSignalreceivingFnc1(){
		cout << "muutos1" << endl;
	}
	void exampleSignalreceivingFnc2(){
		cout << "muutos2" << endl;
	}
};


class DummySignalSourceWrapper: public DummySignalSource, public EventHub
{
public:
	void exampleSignalEmitingFnc(){
		cout << "old functionality" << endl;
		DummySignalSource::exampleSignalEmitingFnc();

		cout << "This glue emits the signal" << endl;
		sendEvent();
		cout << "Signal emit done" << endl;

		};
};





class DummySignalSinkGlue1: public EventReceiver
{
public:
	DummySignalSinkGlue1(DummySignalSink *r ):EventReceiver((void *)r){};
	virtual void receive()const{
		((DummySignalSink*)target)->exampleSignalreceivingFnc1();
		return;
	};
};

class DummySignalSinkGlue2: public EventReceiver
{
public:
	DummySignalSinkGlue2(DummySignalSink *r ):EventReceiver((void *)r){};

	virtual void receive()const{
		((DummySignalSink*)target)->exampleSignalreceivingFnc2();
	};
};





int main() {
	DummySignalSourceWrapper source;
	DummySignalSink sink;
	DummySignalSinkGlue1 event1(&sink);
	DummySignalSinkGlue2 event2(&sink);

	cout << "addEventReceiver event 1" << endl;
	source.addEventReceiver(&event1);
	cout << "addEventReceiver event 2" << endl;

	source.addEventReceiver(&event2);
	cout << "now emit:" << endl;

	source.exampleSignalEmitingFnc();

	return 0;
}
