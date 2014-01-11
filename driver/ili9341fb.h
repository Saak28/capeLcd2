/*
 * linux/include/video/ili9341fb.h -- FB driver for ST7735 LCD controller
 *
 * Copyright (C) 2012, Matt Porter
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License. See the file COPYING in the main directory of this archive for
 * more details.
 */

#define DRVNAME										"ili9341fb"
#define WIDTH										240
#define HEIGHT										320
#define BPP											16
#define MAX_PALETTE									16

/* Supported display modules */
#define LCD_2_4										0	/* Saak LCD 2.4" */

/* Init script function */
struct ili9341_function
{
	u16 cmd;
	u16 data;
};

struct ili9341fb_par
{
	struct spi_device *spi;
	struct fb_info *info;
	struct mutex io_lock;
	int xoff;
	int yoff;
	int rst;
	int dc;
	u16 *ssbuf;
	u8 *buf;
};

int gpio_rst;
int gpio_dc;

/////////////////////////////////////////////////////////////////////////////////////
// ILI9341 COMMAND SET
/////////////////////////////////////////////////////////////////////////////////////
#define CMD_NOP								0x00	// No Operation
#define CMD_SOFTWARE_RESET					0x01	// Software Reset
#define CMD_READ_DISPLAY_ID_INFO			0x04	// Read Display Identification Information
#define CMD_READ_DISPLAY_STATUS				0x09	// Read Display Status
#define CMD_READ_DISPLAY_POWER_MODE			0x0A	// Read Display Power Mode
#define CMD_READ_DISPLAY_MADCTL				0x0B	// ReadDisplayMADCTL
#define CMD_READ_DISPLAY_PIXEL_FORMAT		0x0C	// Read Display Pixel Format
#define CMD_READ_DISPLAY_IMAGE_FORMAT		0x0D	// Read Display Image Format
#define CMD_READ_DISPLAY_SIGNAL_MODE		0x0E	// Read Display Signal Mode
#define CMD_READ_DISPLAY_SELF_DIAG_RESULT	0x0F	// Read Display Self-Diagnostic Result
#define CMD_ENTER_SLEEP_MODE				0x10	// Enter Sleep Mode
#define CMD_SLEEP_OUT						0x11	// Sleep OUT
#define CMD_PARTIAL_MODE_ON					0x12	// Partial Mode ON
#define CMD_NORMAL_DISPLAY_MODE_ON			0x13	// Normal Display Mode ON
#define CMD_DISPLAY_INVERSION_OFF			0x20	// Display Inversion OFF
#define CMD_DISPLAY_INVERSION_ON			0x21	// Display Inversion ON
#define CMD_GAMMA_SET						0x26	// Gamma Set
#define CMD_DISPLAY_OFF						0x28	// Display OFF
#define CMD_DISPLAY_ON						0x29	// Display ON
#define CMD_COLUMN_ADDRESS_SET				0x2A	// Column Address Set
#define CMD_PAGE_ADDRESS_SET				0x2B	// Page Address Set
#define CMD_MEMORY_WRITE					0x2C	// Memory Write
#define CMD_COLOR_SET						0x2D	// Color SET
#define CMD_MEMORY_READ						0x2E	// Memory Read
#define CMD_PARTIAL_AREA					0x30	// Partial Area
#define CMD_VERTICAL_SCROLLING_DEF			0x33	// Vertical Scrolling Definition
#define CMD_REARING_EFFECT_LINE_OFF			0x34	// Tearing Effect Line OFF
#define CMD_REARING_EFFECT_LINE_ON			0x35	// Tearing Effect Line ON
#define CMD_MEMORY_ACCESS_CONTROL			0x36	// Memory Access Control
#define CMD_VERTICAL_SCROLLING_START_ADDR	0x37	// Vertical Scrolling Start Address
#define CMD_IDLE_MODE_OFF					0x38	// Idle Mode OFF
#define CMD_IDLE_MODE_ON					0x39	// Idle Mode ON
#define CMD_PIXEL_FORMAT_SET				0x3A	// Pixel Format Set
#define CMD_WRITE_MEMORY_CONTINUE			0x3C	// Write Memory Continue
#define CMD_READ_MEMORY_CONTINUE			0x3E	// Read Memory Continue
#define CMD_SET_TEAR_SCANLINE				0x44	// Set Tear Scanline
#define CMD_GET_SCANLINE					0x45	// Get Scanline
#define CMD_WRITE_DISPLAY_BRIGHTNESS		0x51	// Write Display Brightness
#define CMD_READ_DISPLAY_BRIGHTNESS			0x52	// Read Display Brightness
#define CMD_WRITE_CTRL_DISPLAY				0x53	// Write CTRL Display
#define CMD_READ_CTRL_DISPLAY				0x54	// Read CTRL Display
#define CMD_WRITE_CONTENT_ADAPT_BRIGHT_CTL	0x55	//  Write Content Adaptive Brightness Control
#define CMD_READ_CONTENT_ADAPT_BRIGHT_CTL	0x56	//  Read Content Adaptive Brightness Control
#define CMD_WRITE_CABC_MIN_BRIGHT			0x5E	// Write CABC Minimum Brightness
#define CMD_READ_CABC_MIN_BRIGHT			0x5F	// Read CABC Minimum Brightness
#define CMD_READ_ID1						0xDA	// Read ID1
#define CMD_READ_ID2						0xDB	// Read ID2
#define CMD_READ_ID3						0xDC	// Read ID3
//  Extended Command Set
#define CMD_RGB_INTERFACE_SIGNAL_CONTROL	0xB0	// RGB Interface Signal Control
#define CMD_FRAME_CONTROL_NORMAL_MODE		0xB1	// Frame Control (In Normal Mode)
#define CMD_FRAME_CONTROL_IDLE_MODE			0xB2	// Frame Control (In Idle Mode)
#define CMD_FRAME_CONTROL_PARTIAL_MODE		0xB3	// Frame Control (In Partial Mode)
#define CMD_DISPLAY_INVERSION_CONTROL		0xB4	// Display Inversion Control
#define CMD_BLANKING_PORCH_CONTROL			0xB5	// Blanking Porch Control
#define CMD_DISPLAY_FUNCTION_CONTROL		0xB6	// Display Function Control
#define CMD_ENTRY_MODE_SET					0xB7	// Entry Mode Set
#define CMD_BACKLIGHT_CONTROL_1				0xB8	// Backlight Control 1
#define CMD_BACKLIGHT_CONTROL_2				0xB9	// Backlight Control 2
#define CMD_BACKLIGHT_CONTROL_3				0xBA	// Backlight Control 3
#define CMD_BACKLIGHT_CONTROL_4				0xBB	// Backlight Control 4
#define CMD_BACKLIGHT_CONTROL_5				0xBC	// Backlight Control 5
#define CMD_BACKLIGHT_CONTROL_7				0xBE	// Backlight Control 7
#define CMD_BACKLIGHT_CONTROL_8				0xBF	// Backlight Control 8
#define CMD_POWER_CONTROL_1					0xC0	// Power Control 1
#define CMD_POWER_CONTROL_2					0xC1	// Power Control 2
#define CMD_VCOM_CONTROL_1					0xC5	// VCOM Control 1
#define CMD_VCOM_CONTROL_2					0xC7	// VCOM Control 2
#define CMD_POWER_CONTROL_A					0xCB	// Power control A
#define CMD_POWER_CONTROL_B					0xCF	// Power control B
#define CMD_NV_MEMORY_WRITE					0xD0	// NV Memory Write
#define CMD_NV_MEMORY_PROTECTION_KEY		0xD1	// NV Memory Protection Key
#define CMD_NV_MEMORY_STATUS_READ			0xD2	// NV Memory Status Read
#define CMD_READ_ID4						0xD3	// Read ID4
#define CMD_POSITIVE_GAMMA_CORRECTION		0xE0	// Positive Gamma Correction
#define CMD_NEGATIVE_GAMMA_CORRECTION		0xE1	// Negative Gamma Correction
#define CMD_DIGITAL_GAMMA_CONTROL_1			0xE2	// Digital Gamma Control 1
#define CMD_DIGITAL_GAMMA_CONTROL_2			0xE3	// Digital Gamma Control 2
#define CMD_DRIVER_TIMING_CONTROL_A			0xE8	// Driver timing control A
#define CMD_DRIVER_TIMING_CONTROL_B			0xEA	// Driver timing control B
#define CMD_POWER_ON_SEQUENCE_CONTROL		0xED	// Power on sequence control
#define CMD_ENABLE_3G						0xF2	// Enable 3G
#define CMD_INTERFACE_CONTROL				0xF6	// Interface Control
#define CMD_PUMP_RATION_CONTROL				0xF7	// Pump ratio control


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
