#include <iostream>
#include <string>
#include <unistd.h>
#include "lcd2.h"
#include "SimpleGPIO.h"
using namespace std;

unsigned int GPIO_D0=32;
unsigned int GPIO_D1=33;
unsigned int GPIO_D2=34;
unsigned int GPIO_D3=35;
unsigned int GPIO_D4=36;
unsigned int GPIO_D5=37;
unsigned int GPIO_D6=38;
unsigned int GPIO_D7=39;
unsigned int GPIO_CS=48;
unsigned int GPIO_DC=49;
unsigned int GPIO_WR=50;
unsigned int GPIO_RD=51;
unsigned int GPIO_RS=78;

int main(int argc, char *argv[])
{
	cout << "Testing the LCD 2.4'" << endl;

	InitGpio();

	ReleaseGpio();
	
	return 0;
}

void InitGpio()
{
	gpio_export(GPIO_D0);
	gpio_export(GPIO_D1);
	gpio_export(GPIO_D2);
	gpio_export(GPIO_D3);
	gpio_export(GPIO_D4);
	gpio_export(GPIO_D5);
	gpio_export(GPIO_D6);
	gpio_export(GPIO_D7);
	gpio_export(GPIO_CS);
	gpio_export(GPIO_DC);
	gpio_export(GPIO_WR);
	gpio_export(GPIO_RD);
	gpio_export(GPIO_RS);

	gpio_set_dir(GPIO_D0,OUTPUT_PIN);
	gpio_set_dir(GPIO_D1,OUTPUT_PIN);
	gpio_set_dir(GPIO_D2,OUTPUT_PIN);
	gpio_set_dir(GPIO_D3,OUTPUT_PIN);
	gpio_set_dir(GPIO_D4,OUTPUT_PIN);
	gpio_set_dir(GPIO_D5,OUTPUT_PIN);
	gpio_set_dir(GPIO_D6,OUTPUT_PIN);
	gpio_set_dir(GPIO_D7,OUTPUT_PIN);
	gpio_set_dir(GPIO_CS,OUTPUT_PIN);
	gpio_set_dir(GPIO_DC,OUTPUT_PIN);
	gpio_set_dir(GPIO_WR,OUTPUT_PIN);
	gpio_set_dir(GPIO_RD,OUTPUT_PIN);
	gpio_set_dir(GPIO_RS,OUTPUT_PIN);

	gpio_set_value(GPIO_D0,LOW);
	gpio_set_value(GPIO_D1,LOW);
	gpio_set_value(GPIO_D2,LOW);
	gpio_set_value(GPIO_D3,LOW);
	gpio_set_value(GPIO_D4,LOW);
	gpio_set_value(GPIO_D5,LOW);
	gpio_set_value(GPIO_D6,LOW);
	gpio_set_value(GPIO_D7,LOW);
	gpio_set_value(GPIO_CS,HIGH);
	gpio_set_value(GPIO_DC,HIGH);
	gpio_set_value(GPIO_WR,HIGH);
	gpio_set_value(GPIO_RD,HIGH);
	gpio_set_value(GPIO_RS,HIGH);
}

void ReleaseGpio()
{
	gpio_unexport(GPIO_D0);
	gpio_unexport(GPIO_D1);
	gpio_unexport(GPIO_D2);
	gpio_unexport(GPIO_D3);
	gpio_unexport(GPIO_D4);
	gpio_unexport(GPIO_D5);
	gpio_unexport(GPIO_D6);
	gpio_unexport(GPIO_D7);
	gpio_unexport(GPIO_CS);
	gpio_unexport(GPIO_DC);
	gpio_unexport(GPIO_WR);
	gpio_unexport(GPIO_RD);
	gpio_unexport(GPIO_RS);
}
