#include "X10Receiver.h"

const int MAX_READINGS = 22;
const int LONG_INTRO_LENGTH = 3300;
bool _readValues[MAX_READINGS];

String _buttonNames [] = { "Mute", "", "Sleep", "", "Disc", "", "", "", "VolumeDown", "VolumeUp", "", "ChannelUp", "ChannelDown", "One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine", "TeleText", "Zero", "RecordProgram", "", "Up", "Go", "Archive", "Left", "OK", "Right", "Back", "PreviousChapter", "Down", "NextChapter", "Rewind", "Play", "FastForward", "Record", "Stop", "Pause", "", "", "", "", "", "Information", "Clear", "Program", "Red", "Green", "Yellow", "Blue", "Enter", "Star", "Hash" };
byte _buttonCodes [] = { 171, 0, 174, 0, 179, 0, 0, 0, 187, 189, 0, 193, 195, 197, 199, 201, 203, 205, 207, 209, 211, 213, 215, 217, 219, 0, 223, 225, 227, 229, 231, 233, 235, 237, 239, 241, 243, 245, 247, 249, 251, 253, 0, 0, 0, 0, 0, 9, 11, 13, 15, 17, 19, 21, 23, 25, 27 };


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
	String buttonName = Decode();
	delay(200);
	return buttonName;
}

byte X10Receiver::GetValueFromReceivedValuesRange(byte firstIndex, byte lastIndex)
{
	byte value = 0;

	for (size_t i = firstIndex; i <= lastIndex; i++)
	{
		value = value | _readValues[i];
		
		if (i < lastIndex)
		{
			value <<= 1;
		}
	}

	return value;
}

void X10Receiver::WaitForLongLow()
{
	unsigned long t2 = 0;
	unsigned long highLength = 0;
	unsigned long lowLength = 0;
	unsigned long previousLowLength = 0;
	 
	while (previousLowLength < LONG_INTRO_LENGTH || highLength < LONG_INTRO_LENGTH)
	{
		previousLowLength = lowLength;
		lowLength = pulseIn(_pinId, LOW, 1000000);
		highLength = (micros() - lowLength) - t2;
		t2 = micros();
	}
}

void X10Receiver::ResetVariables()
{
	_timing = 0;
	_counter = 0;
	for (size_t i = 0; i < MAX_READINGS; i++)
	{
		_readValues[i] = 0;
	}
}

void X10Receiver::ReadLows()
{
	while (_timing < LONG_INTRO_LENGTH && _counter < MAX_READINGS)
	{
		_timing = pulseIn(_pinId, LOW, 1000000);
		_readValues[_counter] = _timing > 750;
		_counter = _counter + 1;
	}
}

String X10Receiver::Decode()
{
	byte buttonIndex = GetValueFromReceivedValuesRange(10, 15);
	byte readButtonCode = GetValueFromReceivedValuesRange(1, 8);

	byte buttonCode = _buttonCodes[buttonIndex];

	if (buttonCode - readButtonCode < 2)
	{
		String buttonName = _buttonNames[buttonIndex];
		if (buttonName != "")
		{
			return buttonName;
		}
	}

	return "Unknown";
}