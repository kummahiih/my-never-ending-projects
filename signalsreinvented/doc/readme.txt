== What ==

EventHub, Eventreceiver and Clascomp classes make it possible to make object's function to raise events which is
handled then by other object's function.

== How to use ==

See signnalsreinvented.cpp:
  * DummySignalSource named class, which object should emit an event, when ever exampleSignalEmitingFnc is called.
        * see DummySignalSourceWrapper
  * DummySignalSink object, which exampleSignalreceivingFnc1 fnc you should call when ever an event is emited
        * see DummySignalSinkGlue1
  * DummySignalSink object, which exampleSignalreceivingFnc2 fnc you should call when ever an event is emited
        * see DummySignalSinkGlue2

For connecting individual objects see signnalsreinvented.cpp main fnc:
 * replace the type of source to the type of it's wrapper class
    * DummySignalSource -> DummySignalSourceWrapper
    * add events for the object(s) to the DummySignalSourceWrapper
       * here those are DummySignalSinkGlue1 event1(&sink) and DummySignalSinkGlue2 event2(&sink)
       * because only the pointers are stored to the DummySignalSourceWrapper, please make sure that the event classes last as long as the 
         event raising object. here those are destroyed as soon as we leave the main fnc.


== How to build in Linux ==

$ cd src
$ autoscan
$ mv configure.scan configure.ac
$ autoconf
$ autoheader
$ ./configure
$ make signnalsreinvented

$ ./signnalsreinvented

OR

$ cd src
$ g++ EventReceiver.cpp Classcomp.cpp EventHub.cpp signnalsreinvented.cpp -o signnalsreinvented
$ ./signnalsreinvented


== how does the output look like? ==

$ ./signnalsreinvented
addEventReceiver event 1
addEventReceiver event 2
now emit:
old functionality
This should send a signal
This glue emits the signal
muutos1
muutos2
Signal emit done
