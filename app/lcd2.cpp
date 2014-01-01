#include <iostream>
#include <string>
#include <unistd.h>
using namespace std;

unsigned int LEDGPIO = 60;   // GPIO1_28 = (1x32) + 28 = 60
unsigned int ButtonGPIO = 15;   // GPIO0_15 = (0x32) + 15 = 15

int main(int argc, char *argv[])
{
	cout << "Testing the LCD 2.4'" << endl;

	return 0;
}
