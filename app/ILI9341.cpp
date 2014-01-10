#include <iostream>
#include <unistd.h>
#include "ILI9341.h"
#include "SimpleGPIO.h"

// unsigned int GPIO_D0=66;
// unsigned int GPIO_D1=67;
// unsigned int GPIO_D2=69;
// unsigned int GPIO_D3=68;
// unsigned int GPIO_D4=45;
// unsigned int GPIO_D5=44;
// unsigned int GPIO_D6=23;
// unsigned int GPIO_D7=26;
// unsigned int GPIO_D8=47;
// unsigned int GPIO_D9=46;
// unsigned int GPIO_D10=27;
// unsigned int GPIO_D11=65;
// unsigned int GPIO_D12=22;
// unsigned int GPIO_D13=61;
// unsigned int GPIO_D14=30;
// unsigned int GPIO_D15=115;
unsigned int GPIO_D0=66;
unsigned int GPIO_D1=67;
unsigned int GPIO_D2=69;
unsigned int GPIO_D3=68;
unsigned int GPIO_D4=45;
unsigned int GPIO_D5=44;
unsigned int GPIO_D6=23;
unsigned int GPIO_D7=26;
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

	LcdReset();
// 	gpio_set_value(GPIO_CS,LOW);

	LcdWriteCommand(0x28);	// Display Off
	
	LcdWriteCommand(0x11);	//exit SLEEP mode
	LcdWriteData(0x0000);


	LcdWriteCommand(0xCB);	//Power Control A
	LcdWriteData(0x39);		//always 0x39
	LcdWriteData(0x2C);		//always 0x2C
	LcdWriteData(0x00);		//always 0x00
	LcdWriteData(0x34);		//Vcore = 1.6V
	LcdWriteData(0x02);		//DDVDH = 5.6V

	LcdWriteCommand(0xCF);	//Power Control B
	LcdWriteData(0x00);		//always 0x00
	LcdWriteData(0x81);		//PCEQ off
	LcdWriteData(0x30);		//ESD protection

	LcdWriteCommand(0xE8);	//Driver timing control A
	LcdWriteData(0x85);		//non‐overlap
	LcdWriteData(0x01);		//EQ timing
	LcdWriteData(0x79);		//Pre‐charge timing

	LcdWriteCommand(0xEA);	//Driver timing control B
	LcdWriteData(0x00);		//Gate driver timing
	LcdWriteData(0x00);		//always 0x00

	LcdWriteCommand(0xED);	//Power‐On sequence control
	LcdWriteData(0x64);		//soft start
	LcdWriteData(0x03);		//power on sequence
	LcdWriteData(0x12);		//power on sequence
	LcdWriteData(0x81);		//DDVDH enhance on

	LcdWriteCommand(0xF7);	//DDVDH enhance on
	LcdWriteData(0x20);		//DDVDH=2xVCI
	
	LcdWriteCommand(0xC0);	//power control 1
	LcdWriteData(0x26);
	LcdWriteData(0x04);		//second parameter for ILI9340 (ignored by ILI9341)

	LcdWriteCommand(0xC1);	//power control 2
	LcdWriteData(0x11);

	LcdWriteCommand(0xC5);	//VCOM control 1
	LcdWriteData(0x35);
	LcdWriteData(0x3E);

	LcdWriteCommand(0xC7);	//VCOM control 2
	LcdWriteData(0xBE);

	LcdWriteCommand(0x36);//memory access control = BGR
	LcdWriteData(0x88);

	LcdWriteCommand(0xB1);	//frame rate control
	LcdWriteData(0x00);
	LcdWriteData(0x10);

	LcdWriteCommand(0xB6);	//display function control
	LcdWriteData(0x0A);
	LcdWriteData(0xA2);

	LcdWriteCommand(0x3A);	//pixel format = 16 bit per pixel
	LcdWriteData(0x55);

	LcdWriteCommand(0xF2);	//3G Gamma control
	LcdWriteData(0x02);		//off

	LcdWriteCommand(0x26);	//Gamma curve 3
	LcdWriteData(0x01);

	LcdWriteCommand(0x2A);	//column address set
	LcdWriteData(0x00);
	LcdWriteData(0x00);		//start 0x0000
	LcdWriteData(0x00);
	LcdWriteData(0xEF);		//end 0x00EF

	LcdWriteCommand(0x2B);	//page address set
	LcdWriteData(0x00);
	LcdWriteData(0x00);		//start 0x0000
	LcdWriteData(0x01);
	LcdWriteData(0x3F);		//end 0x013F

// 	LcdWriteCommand(0xe0);
// 	LcdWriteData(0x1f);
// 	LcdWriteData(0x1a);
// 	LcdWriteData(0x18);
// 	LcdWriteData(0x0a);
// 	LcdWriteData(0x0f);
// 	LcdWriteData(0x06);
// 	LcdWriteData(0x45);
// 	LcdWriteData(0x87);
// 	LcdWriteData(0x32);
// 	LcdWriteData(0x0a);
// 	LcdWriteData(0x07);
// 	LcdWriteData(0x02);
// 	LcdWriteData(0x07);
// 	LcdWriteData(0x05);
// 	LcdWriteData(0x00);
// 
// 	LcdWriteCommand(0xe1);
// 	LcdWriteData(0x00);
// 	LcdWriteData(0x25);
// 	LcdWriteData(0x27);
// 	LcdWriteData(0x05);
// 	LcdWriteData(0x10);
// 	LcdWriteData(0x09);
// 	LcdWriteData(0x3a);
// 	LcdWriteData(0x78);
// 	LcdWriteData(0x4d);
// 	LcdWriteData(0x05);
// 	LcdWriteData(0x18);
// 	LcdWriteData(0x0d);
// 	LcdWriteData(0x38);
// 	LcdWriteData(0x3a);
// 	LcdWriteData(0x1f);
// 
// 	LcdWriteCommand(0xb7);
// 	LcdWriteData(0x07);
// 
// 	LcdWriteCommand(0x11);
// 	usleep(100000);
// 	LcdWriteCommand(0x29);
// 	usleep(100000);
// 	LcdWriteCommand(MemoryWrite);
}

void LcdReset()
{
	gpio_set_value(GPIO_RD,HIGH);
	gpio_set_value(GPIO_WR,HIGH);
	gpio_set_value(GPIO_CS,HIGH);

	gpio_set_value(GPIO_RS,LOW);
	usleep(120000);
	gpio_set_value(GPIO_RS,HIGH);
	usleep(120000);
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
	gpio_set_value(GPIO_CS,LOW);
// 	GPIO_Write(data);
	if(data&0x0100)	gpio_set_value(GPIO_D0,HIGH);
	else					gpio_set_value(GPIO_D0,LOW);

	if(data&0x0200)	gpio_set_value(GPIO_D1,HIGH);
	else					gpio_set_value(GPIO_D1,LOW);

	if(data&0x0400)	gpio_set_value(GPIO_D2,HIGH);
	else					gpio_set_value(GPIO_D2,LOW);

	if(data&0x0800)	gpio_set_value(GPIO_D3,HIGH);
	else					gpio_set_value(GPIO_D3,LOW);

	if(data&0x1000)	gpio_set_value(GPIO_D4,HIGH);
	else					gpio_set_value(GPIO_D4,LOW);

	if(data&0x2000)	gpio_set_value(GPIO_D5,HIGH);
	else					gpio_set_value(GPIO_D5,LOW);

	if(data&0x4000)	gpio_set_value(GPIO_D6,HIGH);
	else					gpio_set_value(GPIO_D6,LOW);

	if(data&0x8000)	gpio_set_value(GPIO_D7,HIGH);
	else					gpio_set_value(GPIO_D7,LOW);

  // Strobe the WRITE pin
	gpio_set_value(GPIO_WR,LOW);
	gpio_set_value(GPIO_WR,HIGH);

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

  // Strobe the WRITE pin
	gpio_set_value(GPIO_WR,LOW);
	gpio_set_value(GPIO_WR,HIGH);
	
	gpio_set_value(GPIO_CS,HIGH);
}

void GPIO_Write(unsigned int data)
{
// 	if(data&0x0001)	gpio_set_value(GPIO_D0,HIGH);
// 	else					gpio_set_value(GPIO_D0,LOW);
// 
// 	if(data&0x0002)	gpio_set_value(GPIO_D1,HIGH);
// 	else					gpio_set_value(GPIO_D1,LOW);
// 
// 	if(data&0x0004)	gpio_set_value(GPIO_D2,HIGH);
// 	else					gpio_set_value(GPIO_D2,LOW);
// 
// 	if(data&0x0008)	gpio_set_value(GPIO_D3,HIGH);
// 	else					gpio_set_value(GPIO_D3,LOW);
// 
// 	if(data&0x0010)	gpio_set_value(GPIO_D4,HIGH);
// 	else					gpio_set_value(GPIO_D4,LOW);
// 
// 	if(data&0x0020)	gpio_set_value(GPIO_D5,HIGH);
// 	else					gpio_set_value(GPIO_D5,LOW);
// 
// 	if(data&0x0040)	gpio_set_value(GPIO_D6,HIGH);
// 	else					gpio_set_value(GPIO_D6,LOW);
// 
// 	if(data&0x0080)	gpio_set_value(GPIO_D7,HIGH);
// 	else					gpio_set_value(GPIO_D7,LOW);
// 
// 	if(data&0x0100)	gpio_set_value(GPIO_D8,HIGH);
// 	else					gpio_set_value(GPIO_D8,LOW);
// 
// 	if(data&0x0200)	gpio_set_value(GPIO_D9,HIGH);
// 	else					gpio_set_value(GPIO_D9,LOW);
// 
// 	if(data&0x0400)	gpio_set_value(GPIO_D10,HIGH);
// 	else					gpio_set_value(GPIO_D10,LOW);
// 
// 	if(data&0x0800)	gpio_set_value(GPIO_D11,HIGH);
// 	else					gpio_set_value(GPIO_D11,LOW);
// 
// 	if(data&0x1000)	gpio_set_value(GPIO_D12,HIGH);
// 	else					gpio_set_value(GPIO_D12,LOW);
// 
// 	if(data&0x2000)	gpio_set_value(GPIO_D13,HIGH);
// 	else					gpio_set_value(GPIO_D13,LOW);
// 
// 	if(data&0x4000)	gpio_set_value(GPIO_D14,HIGH);
// 	else					gpio_set_value(GPIO_D14,LOW);
// 
// 	if(data&0x8000)	gpio_set_value(GPIO_D15,HIGH);
// 	else					gpio_set_value(GPIO_D15,LOW);
	
// 	if(data&0x0001)	gpio_set_value(GPIO_D8,HIGH);
// 	else					gpio_set_value(GPIO_D8,LOW);
// 
// 	if(data&0x0002)	gpio_set_value(GPIO_D9,HIGH);
// 	else					gpio_set_value(GPIO_D9,LOW);
// 
// 	if(data&0x0004)	gpio_set_value(GPIO_D10,HIGH);
// 	else					gpio_set_value(GPIO_D10,LOW);
// 
// 	if(data&0x0008)	gpio_set_value(GPIO_D11,HIGH);
// 	else					gpio_set_value(GPIO_D11,LOW);
// 
// 	if(data&0x0010)	gpio_set_value(GPIO_D12,HIGH);
// 	else					gpio_set_value(GPIO_D12,LOW);
// 
// 	if(data&0x0020)	gpio_set_value(GPIO_D13,HIGH);
// 	else					gpio_set_value(GPIO_D13,LOW);
// 
// 	if(data&0x0040)	gpio_set_value(GPIO_D14,HIGH);
// 	else					gpio_set_value(GPIO_D14,LOW);
// 
// 	if(data&0x0080)	gpio_set_value(GPIO_D15,HIGH);
// 	else					gpio_set_value(GPIO_D15,LOW);
// 
// 	if(data&0x0100)	gpio_set_value(GPIO_D0,HIGH);
// 	else					gpio_set_value(GPIO_D0,LOW);
// 
// 	if(data&0x0200)	gpio_set_value(GPIO_D1,HIGH);
// 	else					gpio_set_value(GPIO_D1,LOW);
// 
// 	if(data&0x0400)	gpio_set_value(GPIO_D2,HIGH);
// 	else					gpio_set_value(GPIO_D2,LOW);
// 
// 	if(data&0x0800)	gpio_set_value(GPIO_D3,HIGH);
// 	else					gpio_set_value(GPIO_D3,LOW);
// 
// 	if(data&0x1000)	gpio_set_value(GPIO_D4,HIGH);
// 	else					gpio_set_value(GPIO_D4,LOW);
// 
// 	if(data&0x2000)	gpio_set_value(GPIO_D5,HIGH);
// 	else					gpio_set_value(GPIO_D5,LOW);
// 
// 	if(data&0x4000)	gpio_set_value(GPIO_D6,HIGH);
// 	else					gpio_set_value(GPIO_D6,LOW);
// 
// 	if(data&0x8000)	gpio_set_value(GPIO_D7,HIGH);
// 	else					gpio_set_value(GPIO_D7,LOW);
}

unsigned int GPIO_Read()
{
	unsigned int data=0;
	unsigned int val;
	
	gpio_set_value(GPIO_DC,HIGH);
	gpio_set_value(GPIO_CS,LOW);
	gpio_set_value(GPIO_RD,LOW);
	
// 	gpio_get_value(GPIO_D0,&val);		if(val)	data|=0x0001;
// 	gpio_get_value(GPIO_D1,&val);		if(val)	data|=0x0002;
// 	gpio_get_value(GPIO_D2,&val);		if(val)	data|=0x0004;
// 	gpio_get_value(GPIO_D3,&val);		if(val)	data|=0x0008;
// 	gpio_get_value(GPIO_D4,&val);		if(val)	data|=0x0010;
// 	gpio_get_value(GPIO_D5,&val);		if(val)	data|=0x0020;
// 	gpio_get_value(GPIO_D6,&val);		if(val)	data|=0x0040;
// 	gpio_get_value(GPIO_D7,&val);		if(val)	data|=0x0080;
// 	gpio_get_value(GPIO_D8,&val);		if(val)	data|=0x0100;
// 	gpio_get_value(GPIO_D9,&val);		if(val)	data|=0x0200;
// 	gpio_get_value(GPIO_D10,&val);	if(val)	data|=0x0400;
// 	gpio_get_value(GPIO_D11,&val);	if(val)	data|=0x0800;
// 	gpio_get_value(GPIO_D12,&val);	if(val)	data|=0x1000;
// 	gpio_get_value(GPIO_D13,&val);	if(val)	data|=0x2000;
// 	gpio_get_value(GPIO_D14,&val);	if(val)	data|=0x4000;
// 	gpio_get_value(GPIO_D15,&val);	if(val)	data|=0x8000;

	gpio_get_value(GPIO_D0,&val);	if(val)	data|=0x01;
	gpio_get_value(GPIO_D1,&val);	if(val)	data|=0x02;
	gpio_get_value(GPIO_D2,&val);	if(val)	data|=0x04;
	gpio_get_value(GPIO_D3,&val);	if(val)	data|=0x08;
	gpio_get_value(GPIO_D4,&val);	if(val)	data|=0x10;
	gpio_get_value(GPIO_D5,&val);	if(val)	data|=0x20;
	gpio_get_value(GPIO_D6,&val);	if(val)	data|=0x40;
	gpio_get_value(GPIO_D7,&val);	if(val)	data|=0x80;
	
	gpio_set_value(GPIO_RD,HIGH);
	gpio_set_value(GPIO_CS,HIGH);
	
	return data;
}

void LcdSetOrientation(lcdOrientation_t value)
{
	lcdOrientation = value;
	LcdWriteCommand(MemoryAccessControl);
	if (lcdOrientation == LCD_ORIENTATION_PORTRAIT)
	{
		LcdWriteData(lcdPortraitConfig);
		lcdProperties.width = 240;
		lcdProperties.height = 320;
	}
	else
	{
		LcdWriteData(lcdLandscapeConfig);
		lcdProperties.width = 320;
		lcdProperties.height = 240;
	}
	LcdWriteCommand(MemoryWrite);
	LcdSetWindow(0, lcdProperties.width-1, 0, lcdProperties.height-1);
}

lcdOrientation_t LcdGetOrientation()
{
	return lcdOrientation;
}

unsigned int LcdGetWidth(void)
{
	return lcdProperties.width;
}

unsigned int LcdGetHeight(void)
{
	return lcdProperties.height;
}

void LcdSetWindow(unsigned short x0, unsigned short y0, unsigned short x1, unsigned short y1)
{
	LcdWriteCommand(ColumnAddressSet);
	LcdWriteData((x0 >> 8) & 0xFF);
	LcdWriteData(x0 & 0xFF);
	LcdWriteData((y0 >> 8) & 0xFF);
	LcdWriteData(y0 & 0xFF);
	LcdWriteCommand(PageAddressSet);
	LcdWriteData((x1 >> 8) & 0xFF);
	LcdWriteData(x1 & 0xFF);
	LcdWriteData((y1 >> 8) & 0xFF);
	LcdWriteData(y1 & 0xFF);
	LcdWriteCommand(MemoryWrite);
}

void LcdTest()
{
//	LcdSetOrientation(LCD_ORIENTATION_PORTRAIT);
	LcdWriteCommand(MemoryWrite);

	for (int y=0;y<32;y++)
	{
		for (int x=0;x<24;x++)
		{
			LcdWriteData(0x03);
// 			if (y > 279) LcdWriteData(COLOR_WHITE);
// 			else if (y > 239) LcdWriteData(COLOR_BLUE);
// 			else if (y > 199) LcdWriteData(COLOR_GREEN);
// 			else if (y > 159) LcdWriteData(COLOR_CYAN);
// 			else if (y > 119) LcdWriteData(COLOR_RED);
// 			else if (y > 79) LcdWriteData(COLOR_MAGENTA);
// 			else if (y > 39) LcdWriteData(COLOR_YELLOW);
// 			else LcdWriteData(COLOR_BLACK);
		}
	}
}

unsigned long GetLcdInformation()
{
	unsigned long data=0;
	
//	LcdWriteCommand(0x01);	// Software Reset
	usleep(100000);
	LcdWriteCommand(0x04);	// Read display identification information 
//	LcdWriteCommand(0x09);	// Read Display Status
	gpio_set_dir(GPIO_D0,INPUT_PIN);
	gpio_set_dir(GPIO_D1,INPUT_PIN);
	gpio_set_dir(GPIO_D2,INPUT_PIN);
	gpio_set_dir(GPIO_D3,INPUT_PIN);
	gpio_set_dir(GPIO_D4,INPUT_PIN);
	gpio_set_dir(GPIO_D5,INPUT_PIN);
	gpio_set_dir(GPIO_D6,INPUT_PIN);
	gpio_set_dir(GPIO_D7,INPUT_PIN);
// 	gpio_set_dir(GPIO_D8,INPUT_PIN);
// 	gpio_set_dir(GPIO_D9,INPUT_PIN);
// 	gpio_set_dir(GPIO_D10,INPUT_PIN);
// 	gpio_set_dir(GPIO_D11,INPUT_PIN);
// 	gpio_set_dir(GPIO_D12,INPUT_PIN);
// 	gpio_set_dir(GPIO_D13,INPUT_PIN);
// 	gpio_set_dir(GPIO_D14,INPUT_PIN);
// 	gpio_set_dir(GPIO_D15,INPUT_PIN);
	
	data|=GPIO_Read();
	data<<=8;
	data|=GPIO_Read();
	data<<=8;
	data|=GPIO_Read();
	data<<=8;
	data|=GPIO_Read();
	
	gpio_set_dir(GPIO_D0,OUTPUT_PIN);
	gpio_set_dir(GPIO_D1,OUTPUT_PIN);
	gpio_set_dir(GPIO_D2,OUTPUT_PIN);
	gpio_set_dir(GPIO_D3,OUTPUT_PIN);
	gpio_set_dir(GPIO_D4,OUTPUT_PIN);
	gpio_set_dir(GPIO_D5,OUTPUT_PIN);
	gpio_set_dir(GPIO_D6,OUTPUT_PIN);
	gpio_set_dir(GPIO_D7,OUTPUT_PIN);
// 	gpio_set_dir(GPIO_D8,OUTPUT_PIN);
// 	gpio_set_dir(GPIO_D9,OUTPUT_PIN);
// 	gpio_set_dir(GPIO_D10,OUTPUT_PIN);
// 	gpio_set_dir(GPIO_D11,OUTPUT_PIN);
// 	gpio_set_dir(GPIO_D12,OUTPUT_PIN);
// 	gpio_set_dir(GPIO_D13,OUTPUT_PIN);
// 	gpio_set_dir(GPIO_D14,OUTPUT_PIN);
// 	gpio_set_dir(GPIO_D15,OUTPUT_PIN);

	gpio_set_value(GPIO_D0,LOW);
	gpio_set_value(GPIO_D1,LOW);
	gpio_set_value(GPIO_D2,LOW);
	gpio_set_value(GPIO_D3,LOW);
	gpio_set_value(GPIO_D4,LOW);
	gpio_set_value(GPIO_D5,LOW);
	gpio_set_value(GPIO_D6,LOW);
	gpio_set_value(GPIO_D7,LOW);
// 	gpio_set_value(GPIO_D8,LOW);
// 	gpio_set_value(GPIO_D9,LOW);
// 	gpio_set_value(GPIO_D10,LOW);
// 	gpio_set_value(GPIO_D11,LOW);
// 	gpio_set_value(GPIO_D12,LOW);
// 	gpio_set_value(GPIO_D13,LOW);
// 	gpio_set_value(GPIO_D14,LOW);
// 	gpio_set_value(GPIO_D15,LOW);
	
	return data;
}

