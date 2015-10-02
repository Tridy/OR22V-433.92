// X10Receiver.h

#ifndef _X10RECEIVER_h
#define _X10RECEIVER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


#endif

class X10Receiver
{
private:
	byte _pinId;
	unsigned long _timing;
	int _counter;
	void WaitForLongLow();
	void ResetVariables();
	void ReadLows();
public:
	X10Receiver(byte pinId);
	String Receive();
};