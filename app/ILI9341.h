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
unsigned int		GPIO_Read(void);
void					LcdTest(void);
void					LcdSetOrientation(lcdOrientation_t value);
lcdOrientation_t	LcdGetOrientation(void);
unsigned int		LcdGetWidth(void);
unsigned int		LcdGetHeight(void);
void					LcdSetWindow(unsigned short x0, unsigned short y0, unsigned short x1, unsigned short y1);
unsigned long		GetLcdInformation(void);

// void              lcdTest(void);
// unsigned int          lcdGetPixel(unsigned int x, unsigned int y);
// void              lcdFillRGB(unsigned int color);
// void              lcdDrawPixel(unsigned int x, unsigned int y, unsigned int color);
// void              lcdDrawPixels(unsigned int x, unsigned int y, unsigned int *data, uint32_t len);
// void              lcdDrawHLine(unsigned int x0, unsigned int x1, unsigned int y, unsigned int color);
// void              lcdDrawVLine(unsigned int x, unsigned int y0, unsigned int y1, unsigned int color);
// void              lcdScroll(int16_t pixels, unsigned int fillColor);
// unsigned int          lcdGetWidth(void);
// unsigned int          lcdGetHeight(void);
// void              lcdSetOrientation(lcdOrientation_t orientation);
// unsigned int          lcdGetControllerID(void);
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
extern unsigned int GPIO_D8;
extern unsigned int GPIO_D9;
extern unsigned int GPIO_D10;
extern unsigned int GPIO_D11;
extern unsigned int GPIO_D12;
extern unsigned int GPIO_D13;
extern unsigned int GPIO_D14;
extern unsigned int GPIO_D15;
extern unsigned int GPIO_CS;
extern unsigned int GPIO_DC;
extern unsigned int GPIO_WR;
extern unsigned int GPIO_RD;
extern unsigned int GPIO_RS;

// Basic Color definitions
#define	COLOR_BLACK                         (unsigned int)(0x0000)
#define	COLOR_BLUE                          (unsigned int)(0x001F)
#define	COLOR_RED                           (unsigned int)(0xF800)
#define	COLOR_GREEN                         (unsigned int)(0x07E0)
#define COLOR_CYAN                          (unsigned int)(0x07FF)
#define COLOR_MAGENTA                       (unsigned int)(0xF81F)
#define COLOR_YELLOW                        (unsigned int)(0xFFE0)
#define COLOR_WHITE                         (unsigned int)(0xFFFF)

// Grayscale Values
#define COLOR_GRAY_15                       (unsigned int)(0x0861)    //  15  15  15
#define COLOR_GRAY_30                       (unsigned int)(0x18E3)    //  30  30  30
#define COLOR_GRAY_50                       (unsigned int)(0x3186)    //  50  50  50
#define COLOR_GRAY_80                       (unsigned int)(0x528A)    //  80  80  80
#define COLOR_GRAY_128                      (unsigned int)(0x8410)    // 128 128 128
#define COLOR_GRAY_200                      (unsigned int)(0xCE59)    // 200 200 200
#define COLOR_GRAY_225                      (unsigned int)(0xE71C)    // 225 225 225

// Color Palettes
#define COLOR_THEME_LIMEGREEN_BASE          (unsigned int)(0xD7F0)    // 211 255 130
#define COLOR_THEME_LIMEGREEN_DARKER        (unsigned int)(0x8DE8)    // 137 188  69
#define COLOR_THEME_LIMEGREEN_LIGHTER       (unsigned int)(0xEFF9)    // 238 255 207
#define COLOR_THEME_LIMEGREEN_SHADOW        (unsigned int)(0x73EC)    // 119 127 103
#define COLOR_THEME_LIMEGREEN_ACCENT        (unsigned int)(0xAE6D)    // 169 204 104

#define COLOR_THEME_VIOLET_BASE             (unsigned int)(0x8AEF)    // 143  94 124
#define COLOR_THEME_VIOLET_DARKER           (unsigned int)(0x4187)    //  66  49  59
#define COLOR_THEME_VIOLET_LIGHTER          (unsigned int)(0xC475)    // 194 142 174
#define COLOR_THEME_VIOLET_SHADOW           (unsigned int)(0x40E6)    //  66  29  52
#define COLOR_THEME_VIOLET_ACCENT           (unsigned int)(0xC992)    // 204  50 144

#define COLOR_THEME_EARTHY_BASE             (unsigned int)(0x6269)    //  97  79  73
#define COLOR_THEME_EARTHY_DARKER           (unsigned int)(0x3103)    //  48  35  31
#define COLOR_THEME_EARTHY_LIGHTER          (unsigned int)(0x8C30)    // 140 135 129
#define COLOR_THEME_EARTHY_SHADOW           (unsigned int)(0xAB29)    // 173 102  79
#define COLOR_THEME_EARTHY_ACCENT           (unsigned int)(0xFE77)    // 250 204 188

#define COLOR_THEME_SKYBLUE_BASE            (unsigned int)(0x95BF)    // 150 180 255
#define COLOR_THEME_SKYBLUE_DARKER          (unsigned int)(0x73B0)    // 113 118 131
#define COLOR_THEME_SKYBLUE_LIGHTER         (unsigned int)(0xE75F)    // 227 235 255
#define COLOR_THEME_SKYBLUE_SHADOW          (unsigned int)(0x4ACF)    //  75  90 127
#define COLOR_THEME_SKYBLUE_ACCENT          (unsigned int)(0xB5F9)    // 182 188 204

// Using these values allows you to update the entire UI color scheme in one location
#define COLOR_THEME_DEFAULT_BASE            COLOR_THEME_LIMEGREEN_BASE
#define COLOR_THEME_DEFAULT_DARKER          COLOR_THEME_LIMEGREEN_DARKER
#define COLOR_THEME_DEFAULT_LIGHTER         COLOR_THEME_LIMEGREEN_LIGHTER
#define COLOR_THEME_DEFAULT_SHADOW          COLOR_THEME_LIMEGREEN_SHADOW
#define COLOR_THEME_DEFAULT_ACCENT          COLOR_THEME_LIMEGREEN_ACCENT

#endif
