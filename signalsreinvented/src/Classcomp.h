/*
 * Classcomp.h
 *
 *  Created on: Sep 4, 2011
 *      Author: prikula
 */

#ifndef CLASSCOMP_H_
#define CLASSCOMP_H_

#include "EventReceiver.h"

#include <iostream>

using namespace std;

#include <typeinfo>
#include <string.h>


class Classcomp {
public:
  bool operator() (EventReceiver *lhs, EventReceiver *rhs)
  {
	  if( lhs->target == rhs->target){
		  //cout<<"same target"<< endl;
		  //cout<<typeid(*lhs).name()<< " vs " << typeid(*rhs).name() << endl;

		  if( strcmp(typeid(*lhs).name(), typeid(*rhs).name()) < 0)
		  			  return true;
		  return false;
	  }
	  if( lhs->target < rhs->target)
		  return true;
	  return false;
  }
};

#endif /* CLASSCOMP_H_ */
