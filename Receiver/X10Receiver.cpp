#include "X10Receiver.h"

const int MAX_READINGS = 22;
bool _readValues[MAX_READINGS];

X10Receiver::X10Receiver(byte pinId)
{
	_pinId = pinId;
}

String X10Receiver::Receive()
{
	ResetVariables();
	pinMode(_pinId, INPUT);
	WaitForLongLow();
	ReadLows();

	Serial.println("");

	for (size_t i = 0; i < MAX_READINGS; i++)
	{
		Serial.print(String(_readValues[i]));
	}

	Serial.println("");

	return "GOT";
}

void X10Receiver::WaitForLongLow()
{
	//unsigned long t1 = 0;
	unsigned long t2 = 0;

	unsigned long highLength = 0;
	unsigned long lowLength = 0;
	unsigned long previousLowLength = 0;
	unsigned long now = micros();
	 
	while (previousLowLength < 3000 || highLength < 3000)
	{
		previousLowLength = lowLength;
		lowLength = pulseIn(_pinId, LOW, 1000000);
		now = micros();
		highLength = (now - lowLength) - t2;
		t2 = now;
	}
}

void X10Receiver::ResetVariables()
{
	_timing = 0;
	_counter = 0;
}

void X10Receiver::ReadLows()
{
	while (_timing < 3000 && _counter < MAX_READINGS)
	{
		_timing = pulseIn(_pinId, LOW, 1000000);
		_readValues[_counter] = _timing > 750;
		_counter = _counter + 1;
	}
}