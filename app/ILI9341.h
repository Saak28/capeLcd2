#ifndef __ILI9341_H__
#define __ILI9341_H__

typedef enum 
{
	LCD_ORIENTATION_PORTRAIT=0,
	LCD_ORIENTATION_LANDSCAPE=1
} lcdOrientation_t;

// This struct is used to indicate the capabilities of different LCDs
typedef struct
{
	unsigned int width;         // LCD width in pixels (default orientation)
	unsigned int height;        // LCD height in pixels (default orientation)
	bool touchscreen;   // Whether the LCD has a touch screen
	bool orientation;   // Whether the LCD orientation can be modified
	bool hwscrolling;   // Whether the LCD support HW scrolling
} lcdProperties_t;

void              LcdInit(void);
void              LcdReset(void);
unsigned char     LcdBuildMemoryAccessControlConfig(
										bool rowAddressOrder,
										bool columnAddressOrder,
										bool rowColumnExchange,
										bool verticalRefreshOrder,
										bool colorOrder,
										bool horizontalRefreshOrder);
void              LcdWriteCommand(unsigned char command);
void              LcdWriteData(unsigned short data);
void              LcdWrite(unsigned short data);
void					GPIO_Write(unsigned int data);
void					LcdTest(void);

// void              lcdTest(void);
// uint16_t          lcdGetPixel(uint16_t x, uint16_t y);
// void              lcdFillRGB(uint16_t color);
// void              lcdDrawPixel(uint16_t x, uint16_t y, uint16_t color);
// void              lcdDrawPixels(uint16_t x, uint16_t y, uint16_t *data, uint32_t len);
// void              lcdDrawHLine(uint16_t x0, uint16_t x1, uint16_t y, uint16_t color);
// void              lcdDrawVLine(uint16_t x, uint16_t y0, uint16_t y1, uint16_t color);
// void              lcdScroll(int16_t pixels, uint16_t fillColor);
// uint16_t          lcdGetWidth(void);
// uint16_t          lcdGetHeight(void);
// void              lcdSetOrientation(lcdOrientation_t orientation);
// uint16_t          lcdGetControllerID(void);
// lcdOrientation_t  lcdGetOrientation(void);
// lcdProperties_t   lcdGetProperties(void);
// void              lcdSetCursor(unsigned short x, unsigned short y);
// void              lcdSetWindow(unsigned short x0, unsigned short y0, unsigned short x1, unsigned short y1);
// void              lcdHome(void);
// 
// void              lcdSetWindow(unsigned short x0, unsigned short y0, unsigned short x1, unsigned short y1);

extern unsigned int GPIO_D0;
extern unsigned int GPIO_D1;
extern unsigned int GPIO_D2;
extern unsigned int GPIO_D3;
extern unsigned int GPIO_D4;
extern unsigned int GPIO_D5;
extern unsigned int GPIO_D6;
extern unsigned int GPIO_D7;
extern unsigned int GPIO_CS;
extern unsigned int GPIO_DC;
extern unsigned int GPIO_WR;
extern unsigned int GPIO_RD;
extern unsigned int GPIO_RS;

#endif
