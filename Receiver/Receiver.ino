#include "X10Receiver.h"
void setup()
{


}

void loop()
{
	X10Receiver(8).Receive();
	delay(2000);
}
