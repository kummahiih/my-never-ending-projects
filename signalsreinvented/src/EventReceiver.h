/*
 * EventReceiver.h
 *
 *  Created on: Sep 4, 2011
 *      Author: Pauli Rikula
 */

#ifndef EVENTRECEIVER_H_
#define EVENTRECEIVER_H_


class EventReceiver
{
public:
	EventReceiver(void * r);
	virtual ~EventReceiver();
	virtual void receive()const;
	void * target; //**** friends
};

#endif /* EVENTRECEIVER_H_ */
