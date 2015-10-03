#include "X10Receiver.h"
void setup()
{


}

void loop()
{
	String buttonName = X10Receiver(8).Receive();
	Serial.println(buttonName);
	delay(500);
}
