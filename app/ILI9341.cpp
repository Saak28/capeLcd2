#include <iostream>
#include <unistd.h>
#include "ILI9341.h"
#include "SimpleGPIO.h"

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
unsigned int GPIO_RS=60;

enum
{
	MemoryAccessControlNormalOrder,
	MemoryAccessControlReverseOrder
} MemoryAccessControlRefreshOrder;

enum
{
	MemoryAccessControlColorOrderBGR,
	MemoryAccessControlColorOrderRGB
} MemoryAccessControlColorOrder;

enum
{
	ColumnAddressSet = 0x2a,
	PageAddressSet = 0x2b,
	MemoryWrite = 0x2c,
	MemoryAccessControl = 0x36,
	WriteDisplayBrightness = 0x51
} ILI9341Register;

static lcdProperties_t lcdProperties = {240,320,true,true,true};
static lcdOrientation_t lcdOrientation=LCD_ORIENTATION_PORTRAIT;

static unsigned char lcdPortraitConfig = 0;
static unsigned char lcdLandscapeConfig = 0;

void LcdInit()
{
	lcdPortraitConfig=LcdBuildMemoryAccessControlConfig(
		MemoryAccessControlNormalOrder, // rowAddressOrder
		MemoryAccessControlReverseOrder, // columnAddressOrder
		MemoryAccessControlNormalOrder, // rowColumnExchange
		MemoryAccessControlNormalOrder, // verticalRefreshOrder
		MemoryAccessControlColorOrderRGB, // colorOrder
		MemoryAccessControlNormalOrder); // horizontalRefreshOrder

	lcdLandscapeConfig=LcdBuildMemoryAccessControlConfig(
		MemoryAccessControlNormalOrder, // rowAddressOrder
		MemoryAccessControlNormalOrder, // columnAddressOrder
		MemoryAccessControlReverseOrder, // rowColumnExchange
		MemoryAccessControlReverseOrder, // verticalRefreshOrder
		MemoryAccessControlColorOrderRGB, // colorOrder
		MemoryAccessControlReverseOrder); // horizontalRefreshOrder

	usleep(120000);
	LcdReset();
	
	gpio_set_value(GPIO_CS,LOW);

	LcdWriteCommand(0x01);
	usleep(10000);
	LcdWriteCommand(0x28);

	LcdWriteCommand(0xcf);
	LcdWriteData(0x00);
	LcdWriteData(0x83);
	LcdWriteData(0x30);

	LcdWriteCommand(0xed);
	LcdWriteData(0x64);
	LcdWriteData(0x03);
	LcdWriteData(0x12);
	LcdWriteData(0x81);

	LcdWriteCommand(0xe8);
	LcdWriteData(0x85);
	LcdWriteData(0x01);
	LcdWriteData(0x79);

	LcdWriteCommand(0xcb);
	LcdWriteData(0x39);
	LcdWriteData(0x2c);
	LcdWriteData(0x00);
	LcdWriteData(0x34);
	LcdWriteData(0x02);

	LcdWriteCommand(0xf7);
	LcdWriteData(0x20);

	LcdWriteCommand(0xea);
	LcdWriteData(0x00);
	LcdWriteData(0x00);

	LcdWriteCommand(0xc0);
	LcdWriteData(0x26);

	LcdWriteCommand(0xc1);
	LcdWriteData(0x11);

	LcdWriteCommand(0xc5);
	LcdWriteData(0x35);
	LcdWriteData(0x3e);

	LcdWriteCommand(0xc7);
	LcdWriteData(0xbe);

	LcdWriteCommand(MemoryAccessControl);
	LcdWriteData(lcdPortraitConfig);

	LcdWriteCommand(0x3a);
	LcdWriteData(0x55);

	LcdWriteCommand(0xb1);
	LcdWriteData(0x00);
	LcdWriteData(0x1B);

	LcdWriteCommand(0xf2);
	LcdWriteData(0x08);

	LcdWriteCommand(0x26);
	LcdWriteData(0x01);

	LcdWriteCommand(0xe0);
	LcdWriteData(0x1f);
	LcdWriteData(0x1a);
	LcdWriteData(0x18);
	LcdWriteData(0x0a);
	LcdWriteData(0x0f);
	LcdWriteData(0x06);
	LcdWriteData(0x45);
	LcdWriteData(0x87);
	LcdWriteData(0x32);
	LcdWriteData(0x0a);
	LcdWriteData(0x07);
	LcdWriteData(0x02);
	LcdWriteData(0x07);
	LcdWriteData(0x05);
	LcdWriteData(0x00);

	LcdWriteCommand(0xe1);
	LcdWriteData(0x00);
	LcdWriteData(0x25);
	LcdWriteData(0x27);
	LcdWriteData(0x05);
	LcdWriteData(0x10);
	LcdWriteData(0x09);
	LcdWriteData(0x3a);
	LcdWriteData(0x78);
	LcdWriteData(0x4d);
	LcdWriteData(0x05);
	LcdWriteData(0x18);
	LcdWriteData(0x0d);
	LcdWriteData(0x38);
	LcdWriteData(0x3a);
	LcdWriteData(0x1f);

	LcdWriteCommand(0x2a);
	LcdWriteData(0x00);
	LcdWriteData(0x00);
	LcdWriteData(0x00);
	LcdWriteData(0xEF);

	LcdWriteCommand(0x2b);
	LcdWriteData(0x00);
	LcdWriteData(0x00);
	LcdWriteData(0x01);
	LcdWriteData(0x3F);

	LcdWriteCommand(0xb7);
	LcdWriteData(0x07);

	LcdWriteCommand(0xb6);
	LcdWriteData(0x0a);
	LcdWriteData(0x82);
	LcdWriteData(0x27);
	LcdWriteData(0x00);

	LcdWriteCommand(0x11);
	usleep(100000);
	LcdWriteCommand(0x29);
	usleep(100000);
	LcdWriteCommand(MemoryWrite);
}

void LcdReset()
{
	gpio_set_value(GPIO_RD,HIGH);
	gpio_set_value(GPIO_WR,HIGH);
	gpio_set_value(GPIO_CS,HIGH);

	gpio_set_value(GPIO_RS,HIGH);
	usleep(100000);
	gpio_set_value(GPIO_RS,LOW);
	usleep(100000);
	gpio_set_value(GPIO_RS,HIGH);
	usleep(100000);
}

unsigned char LcdBuildMemoryAccessControlConfig(
	bool rowAddressOrder,
	bool columnAddressOrder,
	bool rowColumnExchange,
	bool verticalRefreshOrder,
	bool colorOrder,
	bool horizontalRefreshOrder)
{
	unsigned char value = 0;

	if(horizontalRefreshOrder) value |= 0x0004;
	if(colorOrder) value |= 0x0008;
	if(verticalRefreshOrder) value |= 0x0010;
	if(rowColumnExchange) value |= 0x0020;
	if(columnAddressOrder) value |= 0x0040;
	if(rowAddressOrder) value |= 0x0080;
	return value;
}

// Write an 8 bit command to the IC driver
void LcdWriteCommand(unsigned char command)
{
	gpio_set_value(GPIO_DC,LOW);
	LcdWrite(command);
}

// Write an 16 bit data word to the IC driver
void LcdWriteData(unsigned short data)
{
	gpio_set_value(GPIO_DC,HIGH);
	LcdWrite(data);
}

// Places a 16 bit word on the port connected to the IC driver and pulses the write pin
void LcdWrite(unsigned short data)
{
//	gpio_set_value(GPIO_WR,HIGH);

	// Write bit 0-13 (low 13 bits) to the data port
	GPIO_Write(data);

  // Strobe the WRITE pin
	gpio_set_value(GPIO_WR,LOW);
	gpio_set_value(GPIO_WR,HIGH);
}

void GPIO_Write(unsigned int data)
{
	if(data&0x0001)	gpio_set_value(GPIO_D0,HIGH);
	else					gpio_set_value(GPIO_D0,LOW);

	if(data&0x0002)	gpio_set_value(GPIO_D1,HIGH);
	else					gpio_set_value(GPIO_D1,LOW);

	if(data&0x0004)	gpio_set_value(GPIO_D2,HIGH);
	else					gpio_set_value(GPIO_D2,LOW);

	if(data&0x0008)	gpio_set_value(GPIO_D3,HIGH);
	else					gpio_set_value(GPIO_D3,LOW);

	if(data&0x0010)	gpio_set_value(GPIO_D4,HIGH);
	else					gpio_set_value(GPIO_D4,LOW);

	if(data&0x0020)	gpio_set_value(GPIO_D5,HIGH);
	else					gpio_set_value(GPIO_D5,LOW);

	if(data&0x0040)	gpio_set_value(GPIO_D6,HIGH);
	else					gpio_set_value(GPIO_D6,LOW);

	if(data&0x0080)	gpio_set_value(GPIO_D7,HIGH);
	else					gpio_set_value(GPIO_D7,LOW);
}

void LcdTest()
{
	LcdSetOrientation(LCD_ORIENTATION_PORTRAIT);

	for (int y = 0; y < 320; y++)
	{
		for (int x = 0; x < 240; x++)
		{
			if (y > 279) LcdWriteData(COLOR_WHITE);
			else if (y > 239) LcdWriteData(COLOR_BLUE);
			else if (y > 199) LcdWriteData(COLOR_GREEN);
			else if (y > 159) LcdWriteData(COLOR_CYAN);
			else if (y > 119) LcdWriteData(COLOR_RED);
			else if (y > 79) LcdWriteData(COLOR_MAGENTA);
			else if (y > 39) LcdWriteData(COLOR_YELLOW);
			else LcdWriteData(COLOR_BLACK);
		}
	}
}
