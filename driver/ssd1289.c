--- ../setup-scripts/build/tmp-angstrom_v2012_05-eglibc/work/beaglebone-angstrom-linux-gnueabi/linux-ti33x-psp-3.2-r10d+gitre8004dad869568692ca2a45c04e8464ce48cc4d7/git/arch/arm/mach-omap2/board-am335xevm.c	2012-05-17 14:52:55.654937078 +0000
+++ ../setup-scripts-new/build/tmp-angstrom_v2012_05-eglibc/work/beaglebone-angstrom-linux-gnueabi/linux-ti33x-psp-3.2-r10d+gitre8004dad869568692ca2a45c04e8464ce48cc4d7/git/arch/arm/mach-omap2/board-am335xevm.c	2012-05-13 20:40:20.044145349 +0000
@@ -172,6 +172,29 @@
   .type			= "TFC_S9700RTWV35TR_01B",
};

+#if defined(CONFIG_FB_SSD1289)
+#define  LCD_BASE_ADDR					0x4830E000
+static struct resource ssd1289_resources[] = {
+	[0] = {
+		.start = LCD_BASE_ADDR,			//LCD BASE ADDR
+		.end   = LCD_BASE_ADDR+SZ_4K-1,
+		.flags = IORESOURCE_MEM,
+	},
+	[1] = {
+		.start = AM33XX_IRQ_LCD,			//LCD BASE ADDR
+		.end   = AM33XX_IRQ_LCD,
+		.flags = IORESOURCE_IRQ,
+	}
+};
+
+static struct platform_device ssd1289_device = {
+	.name          = "ssd1289",
+	.id            = 0,
+	.num_resources = ARRAY_SIZE(ssd1289_resources),
+	.resource      = ssd1289_resources,
+};
+#endif
+
#include "common.h"

static const struct display_panel bbtoys7_panel = {
@@ -640,6 +663,55 @@
   {NULL, 0},
};

+/* Module pin mux for SSD1289 */
+static struct pinmux_config ssd1289_pin_mux[] = {
+	{"lcd_data0.lcd_data0",		OMAP_MUX_MODE0 | AM33XX_PIN_OUTPUT
+						       | AM33XX_PULL_DISA},
+	{"lcd_data1.lcd_data1",		OMAP_MUX_MODE0 | AM33XX_PIN_OUTPUT
+						       | AM33XX_PULL_DISA},
+	{"lcd_data2.lcd_data2",		OMAP_MUX_MODE0 | AM33XX_PIN_OUTPUT
+						       | AM33XX_PULL_DISA},
+	{"lcd_data3.lcd_data3",		OMAP_MUX_MODE0 | AM33XX_PIN_OUTPUT
+						       | AM33XX_PULL_DISA},
+	{"lcd_data4.lcd_data4",		OMAP_MUX_MODE0 | AM33XX_PIN_OUTPUT
+						       | AM33XX_PULL_DISA},
+	{"lcd_data5.lcd_data5",		OMAP_MUX_MODE0 | AM33XX_PIN_OUTPUT
+						       | AM33XX_PULL_DISA},
+	{"lcd_data6.lcd_data6",		OMAP_MUX_MODE0 | AM33XX_PIN_OUTPUT
+						       | AM33XX_PULL_DISA},
+	{"lcd_data7.lcd_data7",		OMAP_MUX_MODE0 | AM33XX_PIN_OUTPUT
+						       | AM33XX_PULL_DISA},
+	{"lcd_data8.lcd_data8",		OMAP_MUX_MODE0 | AM33XX_PIN_OUTPUT
+						       | AM33XX_PULL_DISA},
+	{"lcd_data9.lcd_data9",		OMAP_MUX_MODE0 | AM33XX_PIN_OUTPUT
+						       | AM33XX_PULL_DISA},
+	{"lcd_data10.lcd_data10",	OMAP_MUX_MODE0 | AM33XX_PIN_OUTPUT
+						       | AM33XX_PULL_DISA},
+	{"lcd_data11.lcd_data11",	OMAP_MUX_MODE0 | AM33XX_PIN_OUTPUT
+						       | AM33XX_PULL_DISA},
+	{"lcd_data12.lcd_data12",	OMAP_MUX_MODE0 | AM33XX_PIN_OUTPUT
+						       | AM33XX_PULL_DISA},
+	{"lcd_data13.lcd_data13",	OMAP_MUX_MODE0 | AM33XX_PIN_OUTPUT
+						       | AM33XX_PULL_DISA},
+	{"lcd_data14.lcd_data14",	OMAP_MUX_MODE0 | AM33XX_PIN_OUTPUT
+						       | AM33XX_PULL_DISA},
+	{"lcd_data15.lcd_data15",	OMAP_MUX_MODE0 | AM33XX_PIN_OUTPUT
+						       | AM33XX_PULL_DISA},
+	{"gpmc_ad8.lcd_data16",		OMAP_MUX_MODE7 | AM33XX_PIN_OUTPUT},
+	{"gpmc_ad9.lcd_data17",		OMAP_MUX_MODE7 | AM33XX_PIN_OUTPUT},
+	{"gpmc_ad10.lcd_data18",	OMAP_MUX_MODE7 | AM33XX_PIN_OUTPUT},
+	{"gpmc_ad11.lcd_data19",	OMAP_MUX_MODE7 | AM33XX_PIN_OUTPUT},
+	{"gpmc_ad12.lcd_data20",	OMAP_MUX_MODE7 | AM33XX_PIN_OUTPUT},
+	{"gpmc_ad13.lcd_data21",	OMAP_MUX_MODE7 | AM33XX_PIN_OUTPUT},
+	{"gpmc_ad14.lcd_data22",	OMAP_MUX_MODE7 | AM33XX_PIN_OUTPUT},
+	{"gpmc_ad15.lcd_data23",	OMAP_MUX_MODE7 | AM33XX_PIN_OUTPUT},
+	{"lcd_vsync.lcd_vsync",		OMAP_MUX_MODE0 | AM33XX_PIN_OUTPUT},
+	{"lcd_hsync.lcd_hsync",		OMAP_MUX_MODE0 | AM33XX_PIN_OUTPUT},
+	{"lcd_pclk.lcd_pclk",		OMAP_MUX_MODE0 | AM33XX_PIN_OUTPUT},
+	{"lcd_ac_bias_en.lcd_ac_bias_en", OMAP_MUX_MODE0 | AM33XX_PIN_OUTPUT},
+	{NULL, 0},
+};
+
static struct pinmux_config w1_gpio_pin_mux[] = {
   {"gpmc_ad3.gpio1_3",	OMAP_MUX_MODE7 | AM33XX_PIN_INPUT_PULLUP},
   {NULL, 0},
@@ -1527,6 +1599,31 @@
   return;
}

+#if defined(CONFIG_FB_SSD1289)
+static void __init bbone_add_device_ssd1289(int evm_id, int profile)
+{
+	pr_info("Initializing SSD1289 LCD driver...\n");
+	setup_pin_mux(ssd1289_pin_mux);
+	if (conf_disp_pll(300000000)) {
+		pr_info("Failed configure display PLL, not attempting to"
+				"register LCDC\n");
+		return;
+	}
+	if (am33xx_register_lcdc_lidd_dma(&ssd1289_device))
+		pr_info("Failed to register SSD1289 LCD device\n");
+
+	pr_info("Initialized SSD1289 LCD driver.\n");
+
+	pr_info("Registering PWM backlight for directly-connected LIDD LCD\n");
+	enable_ehrpwm1(0,0);
+	beaglebone_tsadcpins_free = 0;
+}
+#else
+static void __init bbone_add_device_ssd1289(int evm_id, int profile) {
+	pr_info("Not initializing SSD1289 LCD driver (disabled in kernel config).\n");
+}
+#endif /* CONFIG_SSD1289 */
+
#define BEAGLEBONEDVI_PDn  GPIO_TO_PIN(1, 7)

static void dvi_init(int evm_id, int profile)
@@ -2588,6 +2685,7 @@
   {i2c2_init,	DEV_ON_BASEBOARD, PROFILE_NONE},
   {mmc0_init,	DEV_ON_BASEBOARD, PROFILE_NONE},
   {boneleds_init,	DEV_ON_BASEBOARD, PROFILE_ALL},
+	{bbone_add_device_ssd1289, DEV_ON_BASEBOARD, PROFILE_NONE},
   {NULL, 0, 0},
};

@@ -2601,6 +2699,7 @@
   {mmc0_init,	DEV_ON_BASEBOARD, PROFILE_NONE},
   {boneleds_init,	DEV_ON_BASEBOARD, PROFILE_ALL},
   {bonew1_gpio_init, DEV_ON_BASEBOARD, PROFILE_ALL},
+	{bbone_add_device_ssd1289, DEV_ON_BASEBOARD, PROFILE_NONE},
   {NULL, 0, 0},
};

@@ -2688,6 +2787,7 @@
static void setup_beaglebone(void)
{
   pr_info("The board is a AM335x Beaglebone.\n");
+	pr_info("This is a Cemetech Red Edition prototype.\nAll rights reserved (c) 2012 Cemetech");
   gp_evm_revision = GP_EVM_ACTUALLY_BEAGLEBONE;

   /* Beagle Bone has Micro-SD slot which doesn't have Write Protect pin */
--- ../setup-scripts/build/tmp-angstrom_v2012_05-eglibc/work/beaglebone-angstrom-linux-gnueabi/linux-ti33x-psp-3.2-r10d+gitre8004dad869568692ca2a45c04e8464ce48cc4d7/git/arch/arm/mach-omap2/clock33xx_data.c	2012-05-17 14:50:11.117320612 +0000
+++ ../setup-scripts-new/build/tmp-angstrom_v2012_05-eglibc/work/beaglebone-angstrom-linux-gnueabi/linux-ti33x-psp-3.2-r10d+gitre8004dad869568692ca2a45c04e8464ce48cc4d7/git/arch/arm/mach-omap2/clock33xx_data.c	2012-05-07 21:42:22.934784607 +0000
@@ -2093,7 +2093,8 @@
   CLK(NULL,	"l4wkup_ick",		&l4wkup_ick,	CK_AM33XX),
   CLK(NULL,	"l4fw_ick",		&l4fw_ick,	CK_AM33XX),
   CLK(NULL,	"l4ls_ick",		&l4ls_ick,	CK_AM33XX),
-	CLK("da8xx_lcdc.0",	NULL,		&lcdc_fck,	CK_AM33XX),
+	CLK("ssd1289.0",	NULL,		&lcdc_fck,	CK_AM33XX),
+//	CLK("da8xx_lcdc.0",	NULL,		&lcdc_fck,	CK_AM33XX),
   CLK(NULL,	"mailbox0_fck",		&mailbox0_fck,	CK_AM33XX),
   CLK(NULL,	"mcasp1_ick",		&mcasp0_ick,	CK_AM33XX),
   CLK("davinci-mcasp.0",	NULL,		&mcasp0_fck,	CK_AM33XX),
@@ -2131,7 +2132,8 @@
   CLK(NULL,	"gpt5_fck",		&timer5_fck,	CK_AM33XX),
   CLK(NULL,	"gpt6_fck",		&timer6_fck,	CK_AM33XX),
   CLK(NULL,	"gpt7_fck",		&timer7_fck,	CK_AM33XX),
-	CLK("da8xx_lcdc.0",	"lcdc_ick",	&lcdc_ick,	CK_AM33XX),
+	CLK("ssd1289.0",	"lcdc_ick",	&lcdc_ick,	CK_AM33XX),
+//	CLK("da8xx_lcdc.0",	"lcdc_ick",	&lcdc_ick,	CK_AM33XX),
   CLK(NULL,	"tpcc_ick",		&tpcc_ick,	CK_AM33XX),
   CLK(NULL,	"tptc0_ick",		&tptc0_ick,	CK_AM33XX),
   CLK(NULL,	"tptc1_ick",		&tptc1_ick,	CK_AM33XX),
--- ../setup-scripts/build/tmp-angstrom_v2012_05-eglibc/work/beaglebone-angstrom-linux-gnueabi/linux-ti33x-psp-3.2-r10d+gitre8004dad869568692ca2a45c04e8464ce48cc4d7/git/arch/arm/mach-omap2/devices.c	2012-05-17 14:50:11.161314627 +0000
+++ ../setup-scripts-new/build/tmp-angstrom_v2012_05-eglibc/work/beaglebone-angstrom-linux-gnueabi/linux-ti33x-psp-3.2-r10d+gitre8004dad869568692ca2a45c04e8464ce48cc4d7/git/arch/arm/mach-omap2/devices.c	2012-05-13 20:36:34.514787166 +0000
@@ -148,6 +148,30 @@
};
#endif

+int __init am33xx_register_lcdc_lidd_dma(struct platform_device *pdata)
+{
+	int id = 0;
+	struct platform_device *pdev;
+	struct omap_hwmod *oh;
+	char *oh_name = "lcdc";
+	char *dev_name = "ssd1289";
+
+	oh = omap_hwmod_lookup(oh_name);
+	if (!oh) {
+		pr_err("SSD1289 register: Could not look up LCD%d hwmod\n", id);
+		return -ENODEV;
+	}
+
+	pdev = omap_device_build(dev_name, id, oh, pdata,
+			sizeof(struct platform_device), NULL, 0, 0);
+	if (IS_ERR(pdev)) {
+		WARN(1, "SSD1289 register: Can't build omap_device for %s:%s.\n",
+			dev_name, oh->name);
+		return PTR_ERR(pdev);
+	}
+	return 0;
+}
+
int __init am33xx_register_lcdc(struct da8xx_lcdc_platform_data *pdata)
{
   int id = 0;
--- ../setup-scripts/build/tmp-angstrom_v2012_05-eglibc/work/beaglebone-angstrom-linux-gnueabi/linux-ti33x-psp-3.2-r10d+gitre8004dad869568692ca2a45c04e8464ce48cc4d7/git/drivers/video/Kconfig	2012-05-17 14:52:54.099148728 +0000
+++ ../setup-scripts-new/build/tmp-angstrom_v2012_05-eglibc/work/beaglebone-angstrom-linux-gnueabi/linux-ti33x-psp-3.2-r10d+gitre8004dad869568692ca2a45c04e8464ce48cc4d7/git/drivers/video/Kconfig	2012-05-12 06:32:00.134225626 +0000
@@ -2431,6 +2431,18 @@
   help
	 Framebuffer support for the ST7735 display controller in SPI mode.

+config FB_SSD1289
+	tristate "Solomon Systech SSD1289 controller support"
+	depends on FB
+	select FB_CFB_FILLRECT
+	select FB_CFB_COPYAREA
+	select FB_CFB_IMAGEBLIT
+	select FB_SYS_FOPS
+	select FB_DEFERRED_IO
+	help
+	  This is a framebuffer device for the Solomon Systek SSD1289
+	  controller.
+
source "drivers/video/omap/Kconfig"
source "drivers/video/omap2/Kconfig"

--- ../setup-scripts/build/tmp-angstrom_v2012_05-eglibc/work/beaglebone-angstrom-linux-gnueabi/linux-ti33x-psp-3.2-r10d+gitre8004dad869568692ca2a45c04e8464ce48cc4d7/git/drivers/video/Makefile	2012-05-17 14:52:54.099148728 +0000
+++ ../setup-scripts-new/build/tmp-angstrom_v2012_05-eglibc/work/beaglebone-angstrom-linux-gnueabi/linux-ti33x-psp-3.2-r10d+gitre8004dad869568692ca2a45c04e8464ce48cc4d7/git/drivers/video/Makefile	2012-05-06 06:27:21.457153706 +0000
@@ -144,6 +144,7 @@
obj-$(CONFIG_FB_JZ4740)		  += jz4740_fb.o
obj-$(CONFIG_FB_PUV3_UNIGFX)      += fb-puv3.o
obj-$(CONFIG_FB_ST7735)		  += st7735fb.o
+obj-$(CONFIG_FB_SSD1289)	  += ssd1289.o

# Platform or fallback drivers go here
obj-$(CONFIG_FB_UVESA)            += uvesafb.o
--- ../setup-scripts/build/tmp-angstrom_v2012_05-eglibc/work/beaglebone-angstrom-linux-gnueabi/linux-ti33x-psp-3.2-r10d+gitre8004dad869568692ca2a45c04e8464ce48cc4d7/git/drivers/video/ssd1289.c	1970-01-01 00:00:00.000000000 +0000
+++ ../setup-scripts-new/build/tmp-angstrom_v2012_05-eglibc/work/beaglebone-angstrom-linux-gnueabi/linux-ti33x-psp-3.2-r10d+gitre8004dad869568692ca2a45c04e8464ce48cc4d7/git/drivers/video/ssd1289.c	2012-05-15 22:48:57.797674581 +0000
@@ -0,0 +1,851 @@
+/*
+ * SSD1289 Framebuffer
+ * Heavily modified for the AM335X Cortex MPU and Linux 3.x+
+ *
+ * Copyright (c) 2009 Jean-Christian de Rivaz
+ * Copyright (c) 2012 Christopher Mitchell
+ *
+ * This file is subject to the terms and conditions of the GNU General Public
+ * License.  See the file "COPYING" in the main directory of this archive
+ * for more details.
+ *
+ * The Solomon Systech SSD1289 chip drive TFT screen up to 240x320. This
+ * driver expect the SSD1286 to be connected to a 16 bits local bus and
+ * to be set in the 16 bits parallel interface mode. To use it you must
+ * define in your board file a struct platform_device with a name set to
+ * "ssd1289" and a struct resource array with two IORESOURCE_MEM: the first
+ * for the control register; the second for the data register.
+ *
+ * As of heavy modifications, takes only one resource. Also assumes that in the
+ * board file that you set necessary GPIO pin mutices. Does not currently implement
+ * proper suspend/resume behavior for full sleep (and for setting power states of
+ * the AM335X LCDC module).
+ */
+
+//#define DEBUG
+
+#include <linux/kernel.h>
+#include <linux/device.h>
+#include <linux/module.h>
+#include <linux/platform_device.h>
+#include <linux/mm.h>
+#include <linux/clk.h>
+#include <linux/vmalloc.h>
+#include <linux/console.h>
+#include <linux/fb.h>
+#include <asm/io.h>
+#include <linux/dma-mapping.h>
+#include <linux/pm_runtime.h>
+#include <linux/delay.h>
+
+#ifdef DEBUG
+//#define dev_err(dvc, str, args...) { printk(KERN_EMERG str, ##args); }
+#define dev_dbg(dvc, str, args...) { printk(KERN_EMERG str, ##args); }
+/* Convert GPIO signal to GPIO pin number */
+#define GPIO_TO_PIN(bank, gpio) (32 * (bank) + (gpio))
+#include <asm/gpio.h>
+#endif
+
+#define LCD_PID						0x00
+#define LCD_CTRL					0x04
+#define LCD_STAT_REG				0x08
+#define LCD_LIDD_CTRL				0x0C
+#define LCD_CS0_CONF				0x10
+#define LCD_LIDD_CS0_ADDR			0x14
+#define LCD_LIDD_CS0_DATA			0x18
+#define LCD_DMA_CTRL_REG			0x40
+#define LCD_DMA_FRM_BUF_BASE_ADDR_0_REG	0x44
+#define LCD_DMA_FRM_BUF_CEILING_ADDR_0_REG	0x48
+#define LCD_DMA_FRM_BUF_BASE_ADDR_1_REG	0x4C
+#define LCD_DMA_FRM_BUF_CEILING_ADDR_1_REG	0x50
+#define  LCD_RAW_STAT_REG			0x58
+#define  LCD_MASKED_STAT_REG			0x5c
+#define  LCD_INT_ENABLE_SET_REG			0x60
+#define  LCD_INT_ENABLE_CLR_REG			0x64
+#define  LCD_END_OF_INT_IND_REG			0x68
+#define LCD_CLKC_ENABLE				0x6C
+
+#define LCD_LIDD_TYPE_8080		BIT(0) | BIT(1)
+
+#define LCD_V2_DMA_CLK_EN		BIT(2)
+#define LCD_V2_LIDD_CLK_EN		BIT(1)
+#define LCD_V2_CORE_CLK_EN		BIT(0)
+
+/* LCD Control Register */
+#define LCD_CLK_DIVISOR(x)		((x) << 8)
+#define LCD_RASTER_MODE			0x01
+
+#define  LCD_CLK_ENABLE_REG          0x6c
+#define  LCD_CLK_RESET_REG           0x70
+#define  LCD_CLK_MAIN_RESET          BIT(3)
+
+/* DMA Control Registers */
+#define LCD_DMA_BURST_SIZE(x)		((x) << 4)
+#define LCD_DMA_BURST_1			0x0
+#define LCD_DMA_BURST_2			0x1
+#define LCD_DMA_BURST_4			0x2
+#define LCD_DMA_BURST_8			0x3
+#define LCD_DMA_BURST_16		0x4
+
+#define LCD_V2_END_OF_FRAME0_INT_ENA	BIT(8)
+#define LCD_V2_END_OF_FRAME1_INT_ENA	BIT(9)
+#define LCD_V2_UNDERFLOW_INT_ENA		BIT(5)
+#define LCD_V2_DONE_INT_ENA					BIT(0)
+
+#define LCD_DUAL_FRAME_BUFFER_ENABLE	BIT(0)
+
+/* Interrupt Registers available only in Version 2 */
+#define  LCD_RAW_STAT_REG			0x58
+#define  LCD_MASKED_STAT_REG			0x5c
+#define  LCD_INT_ENABLE_SET_REG			0x60
+#define  LCD_INT_ENABLE_CLR_REG			0x64
+#define  LCD_END_OF_INT_IND_REG			0x68
+
+/* LCD DMA Status Register */
+#define LCD_END_OF_FRAME1		BIT(9)
+#define LCD_END_OF_FRAME0		BIT(8)
+#define LCD_PL_LOAD_DONE		BIT(6)
+#define LCD_FIFO_UNDERFLOW		BIT(5)
+#define LCD_SYNC_LOST			BIT(2)
+
+#define SSD1289_REG_OSCILLATION      0x00
+#define SSD1289_REG_DEV_CODE_READ    0x00
+#define SSD1289_REG_DRIVER_OUT_CTRL  0x01
+#define SSD1289_REG_LCD_DRIVE_AC     0x02
+#define SSD1289_REG_POWER_CTRL_1     0x03
+#define SSD1289_REG_DISPLAY_CTRL     0x07
+#define SSD1289_REG_FRAME_CYCLE      0x0b
+#define SSD1289_REG_POWER_CTRL_2     0x0c
+#define SSD1289_REG_POWER_CTRL_3     0x0d
+#define SSD1289_REG_POWER_CTRL_4     0x0e
+#define SSD1289_REG_GATE_SCAN_START  0x0f
+#define SSD1289_REG_SLEEP_MODE       0x10
+#define SSD1289_REG_ENTRY_MODE       0x11
+#define SSD1289_REG_POWER_CTRL_5     0x1e
+#define SSD1289_REG_GDDRAM_DATA      0x22
+#define SSD1289_REG_WR_DATA_MASK_1   0x23
+#define SSD1289_REG_WR_DATA_MASK_2   0x24
+#define SSD1289_REG_FRAME_FREQUENCY  0x25
+#define SSD1289_REG_GAMMA_CTRL_1     0x30
+#define SSD1289_REG_GAMME_CTRL_2     0x31
+#define SSD1289_REG_GAMMA_CTRL_3     0x32
+#define SSD1289_REG_GAMMA_CTRL_4     0x33
+#define SSD1289_REG_GAMMA_CTRL_5     0x34
+#define SSD1289_REG_GAMMA_CTRL_6     0x35
+#define SSD1289_REG_GAMMA_CTRL_7     0x36
+#define SSD1289_REG_GAMMA_CTRL_8     0x37
+#define SSD1289_REG_GAMMA_CTRL_9     0x3a
+#define SSD1289_REG_GAMMA_CTRL_10    0x3b
+#define SSD1289_REG_V_SCROLL_CTRL_1  0x41
+#define SSD1289_REG_V_SCROLL_CTRL_2  0x42
+#define SSD1289_REG_H_RAM_ADR_POS    0x44
+#define SSD1289_REG_V_RAM_ADR_START  0x45
+#define SSD1289_REG_V_RAM_ADR_END    0x46
+#define SSD1289_REG_FIRST_WIN_START  0x48
+#define SSD1289_REG_FIRST_WIN_END    0x49
+#define SSD1289_REG_SECND_WIN_START  0x4a
+#define SSD1289_REG_SECND_WIN_END    0x4b
+#define SSD1289_REG_GDDRAM_X_ADDR    0x4e
+#define SSD1289_REG_GDDRAM_Y_ADDR    0x4f
+
+#define LCD_NUM_BUFFERS	1		// was 2
+#define DRIVER_NAME "ssd1289"
+
+struct ssd1289 {
+	struct device *dev;
+	struct fb_info *info;
+	unsigned int pseudo_palette[16];
+
+	//DMA/Memory things
+	int 				irq;		//irq resource number
+	dma_addr_t			vram_phys;	//should be same as dma_start
+	unsigned long		vram_size;
+	void				*vram_virt;
+	unsigned int		dma_start;	//physical addresses
+	unsigned int		dma_end;
+	int 				blank;		//?
+	int 				suspending;	//whether a suspend is in progress
+
+	//these two are sort-of AM33xx-dependent
+	resource_size_t lcd_regs;		//Base virtual address for LCDC MMIO
+	struct clk *lcdc_clk;			//Power enable for the LCDC
+};
+
+// Function Prototypes
+static inline unsigned int lcdc_read(struct ssd1289 *item, unsigned int addr);
+static inline void lcdc_write(struct ssd1289 *item, unsigned int val, unsigned int addr);
+static inline void ssd1289_reg_set(struct ssd1289 *item, unsigned char reg, unsigned short value);
+static inline unsigned int ssd1289_reg_get(struct ssd1289 *item, unsigned char reg);
+static void __init ssd1289_setup(struct ssd1289 *item);
+static int __init ssd1289_video_alloc(struct ssd1289 *item);
+static void ssd1289_video_free(struct ssd1289 *item);
+static int ssd1289_setcolreg(unsigned regno, unsigned red, unsigned green,
+                             unsigned blue, unsigned transp, struct fb_info *info);
+static int lcd_cfg_dma(ffstruct ssd1289 *item, int burst_size,  int fifo_th);
+static irqreturn_t ssd1289_irq_handler(int irq, void *arg);
+static void ssd1289_dma_setstatus(struct ssd1289 *item, int doenable);
+static int fb_ioctl(struct fb_info *info, unsigned int cmd, unsigned long arg);
+static int __init ssd1289_probe(struct platform_device *dev);
+static int __init ssd1289_init(void);
+
+#ifdef CONFIG_PM
+static int ssd1289_suspend (struct platform_device *dev, pm_message_t state);
+static int ssd1289_resume (struct platform_device *dev, pm_message_t state);
+static void lcd_context_save(struct ssd1289* item);
+static void lcd_context_restore(struct ssd1289* item);
+#endif
+
+static inline unsigned int lcdc_read(struct ssd1289 *item, unsigned int addr)
+{
+	//dev_dbg(item->dev, "%s: Read at offset 0x%p, addr 0x%p\n", __func__, (void *)addr, (void*)(item->lcd_regs + (addr)));
+	return (unsigned int)__raw_readl(item->lcd_regs + (addr));
+}
+
+static inline void lcdc_write(struct ssd1289 *item, unsigned int val,
+                   unsigned int addr)
+{
+	//dev_dbg(item->dev, "%s: Write %04x at offset 0x%p, addr 0x%p\n", __func__, val, (void *)addr, (void*)(item->lcd_regs + (addr)));
+	__raw_writel(val, item->lcd_regs + (addr));
+}
+
+static inline void ssd1289_reg_set(struct ssd1289 *item, unsigned char reg,
+				   unsigned short value)
+{
+	lcdc_write(item,0x000000FF&(unsigned int)reg, LCD_LIDD_CS0_ADDR);
+	lcdc_write(item,(unsigned int)value, LCD_LIDD_CS0_DATA);
+}
+
+static inline unsigned int ssd1289_reg_get(struct ssd1289 *item, unsigned char reg)
+{
+	lcdc_write(item,0x000000FF&(unsigned int)reg, LCD_LIDD_CS0_ADDR);
+	return lcdc_read(item,LCD_LIDD_CS0_ADDR);
+}
+
+static void __init ssd1289_setup(struct ssd1289 *item)
+{
+
+	dev_dbg(item->dev, "%s: item=0x%p\n", __func__, (void *)item);
+
+	// OSCEN=1
+	ssd1289_reg_set(item, SSD1289_REG_OSCILLATION, 0x0001);
+	// DCT=b1010=fosc/4 BT=b001=VGH:+6,VGL:-4
+	// DC=b1010=fosc/4 AP=b010=small to medium
+	ssd1289_reg_set(item, SSD1289_REG_POWER_CTRL_1, 0xa2a4);
+	// VRC=b100:5.5V
+	ssd1289_reg_set(item, SSD1289_REG_POWER_CTRL_2, 0x0004);
+	// VRH=b1000:Vref*2.165
+	ssd1289_reg_set(item, SSD1289_REG_POWER_CTRL_3, 0x0308);
+	// VCOMG=1 VDV=b1000:VLCD63*1.05
+	ssd1289_reg_set(item, SSD1289_REG_POWER_CTRL_4, 0x3000);
+	// nOTP=1 VCM=0x2a:VLCD63*0.77
+	ssd1289_reg_set(item, SSD1289_REG_POWER_CTRL_5, 0x00ea);
+	// RL=0 REV=1 CAD=0 BGR=1 SM=0 TB=1 MUX=0x13f=319
+	ssd1289_reg_set(item, SSD1289_REG_DRIVER_OUT_CTRL, 0x2b3f);
+	// FLD=0 ENWS=0 D/C=1 EOR=1 WSMD=0 NW=0x00=0
+	ssd1289_reg_set(item, SSD1289_REG_LCD_DRIVE_AC, 0x0600);
+	// SLP=0
+	ssd1289_reg_set(item, SSD1289_REG_SLEEP_MODE, 0x0000);
+	// VSMode=0 DFM=3:65k TRAMS=0 OEDef=0 WMode=0 Dmode=0
+	// TY=0 ID=3 AM=0 LG=0
+	ssd1289_reg_set(item, SSD1289_REG_ENTRY_MODE, 0x6030);
+	// PT=0 VLE=1 SPT=0 GON=1 DTE=1 CM=0 D=3
+	ssd1289_reg_set(item, SSD1289_REG_DISPLAY_CTRL, 0x0233);
+	// NO=0 SDT=0 EQ=0 DIV=0 SDIV=1 SRTN=1 RTN=9:25 clock
+	// ssd1289_reg_set(item, SSD1289_REG_FRAME_CYCLE, 0x0039);
+	// NO=0 SDT=0 EQ=0 DIV=0 SDIV=0 SRTN=0 RTN=9:25 clock
+	ssd1289_reg_set(item, SSD1289_REG_FRAME_CYCLE, 0x0009);
+	// SCN=0
+	ssd1289_reg_set(item, SSD1289_REG_GATE_SCAN_START, 0x0000);
+
+	// PKP1=7 PKP0=7
+	ssd1289_reg_set(item, SSD1289_REG_GAMMA_CTRL_1, 0x0707);
+	// PKP3=2 PKP2=4
+	ssd1289_reg_set(item, SSD1289_REG_GAMME_CTRL_2, 0x0204);
+	// PKP5=2 PKP4=2
+	ssd1289_reg_set(item, SSD1289_REG_GAMMA_CTRL_3, 0x0204);
+	// PRP1=5 PRP0=2
+	ssd1289_reg_set(item, SSD1289_REG_GAMMA_CTRL_4, 0x0502);
+	// PKN1=5 PKN0=7
+	ssd1289_reg_set(item, SSD1289_REG_GAMMA_CTRL_5, 0x0507);
+	// PKN3=2 PNN2=4
+	ssd1289_reg_set(item, SSD1289_REG_GAMMA_CTRL_6, 0x0204);
+	// PKN5=2 PKN4=4
+	ssd1289_reg_set(item, SSD1289_REG_GAMMA_CTRL_7, 0x0204);
+	// PRN1=5 PRN0=2
+	ssd1289_reg_set(item, SSD1289_REG_GAMMA_CTRL_8, 0x0502);
+	// VRP1=3 VRP0=2
+	ssd1289_reg_set(item, SSD1289_REG_GAMMA_CTRL_9, 0x0302);
+	// VRN1=3 VRN0=2
+	ssd1289_reg_set(item, SSD1289_REG_GAMMA_CTRL_10, 0x0302);
+
+	// WMR=0 WMG=0
+	ssd1289_reg_set(item, SSD1289_REG_WR_DATA_MASK_1, 0x0000);
+	// WMB=0
+	ssd1289_reg_set(item, SSD1289_REG_WR_DATA_MASK_2, 0x0000);
+	// OSC=b1010:548k
+	ssd1289_reg_set(item, SSD1289_REG_FRAME_FREQUENCY, 0xa000);
+	// SS1=0
+	ssd1289_reg_set(item, SSD1289_REG_FIRST_WIN_START, 0x0000);
+	// SE1=319
+	ssd1289_reg_set(item, SSD1289_REG_FIRST_WIN_END,
+			(item->info->var.yres - 1));
+	// SS2=0
+	ssd1289_reg_set(item, SSD1289_REG_SECND_WIN_START, 0x0000);
+	// SE2=0
+	ssd1289_reg_set(item, SSD1289_REG_SECND_WIN_END, 0x0000);
+	// VL1=0
+	ssd1289_reg_set(item, SSD1289_REG_V_SCROLL_CTRL_1, 0x0000);
+	// VL2=0
+	ssd1289_reg_set(item, SSD1289_REG_V_SCROLL_CTRL_2, 0x0000);
+	// HEA=0xef=239 HSA=0
+	ssd1289_reg_set(item, SSD1289_REG_H_RAM_ADR_POS,
+			(item->info->var.xres - 1) << 8);
+	// VSA=0
+	ssd1289_reg_set(item, SSD1289_REG_V_RAM_ADR_START, 0x0000);
+	// VEA=0x13f=319
+	ssd1289_reg_set(item, SSD1289_REG_V_RAM_ADR_END,
+			(item->info->var.yres - 1));
+}
+
+static int __init ssd1289_video_alloc(struct ssd1289 *item)
+{
+	unsigned int frame_size;
+	int pages_count;
+
+	dev_dbg(item->dev, "%s: item=0x%p\n", __func__, (void *)item);
+
+	// Calculate raw framebuffer size
+	frame_size = item->info->fix.line_length * item->info->var.yres;
+	dev_dbg(item->dev, "%s: item=0x%p frame_size=%u\n",
+		__func__, (void *)item, frame_size);
+
+	// Figure out how many full pages we need
+	pages_count = frame_size / PAGE_SIZE;
+	if ((pages_count * PAGE_SIZE) < frame_size) {
+		pages_count++;
+	}
+	dev_dbg(item->dev, "%s: item=0x%p pages_count=%u per each of %d buffer(s)\n",
+		__func__, (void *)item, pages_count, LCD_NUM_BUFFERS);
+
+	// Reserve DMA-able RAM, set up fix.
+	item->vram_size = pages_count * PAGE_SIZE * LCD_NUM_BUFFERS;
+	item->vram_virt = dma_alloc_coherent(NULL,
+							item->vram_size,
+							(resource_size_t *) &item->vram_phys,
+							GFP_KERNEL | GFP_DMA);
+	if (!item->vram_virt) {
+		dev_err(item->dev, "%s: unable to vmalloc\n", __func__);
+		return -ENOMEM;
+	}
+	memset(item->vram_virt, 0, item->vram_size);
+	item->info->fix.smem_start = item->vram_virt;
+	item->info->fix.smem_len = item->vram_size;
+
+	item->info->screen_base = (char __iomem *)item->vram_virt;
+	item->info->screen_size = (unsigned int)item->vram_size;
+
+	item->dma_start = item->vram_phys;
+	item->dma_end   = item->dma_start + (item->info->var.yres * item->info->fix.line_length) - 1;
+	dev_dbg(item->dev, "%s: DMA set from 0x%p to 0x%p, %d bytes\n",__func__,item->dma_start,item->dma_end,item->vram_size);
+
+	return 0;
+}
+
+static void ssd1289_video_free(struct ssd1289 *item) {
+	dev_dbg(item->dev, "%s: item=0x%p\n", __func__, (void *)item);
+
+	kfree((void *)item->info->fix.smem_start);
+}
+
+static int ssd1289_setcolreg(unsigned regno, unsigned red, unsigned green,
+			      unsigned blue, unsigned transp,
+			      struct fb_info *info)
+{
+	struct ssd1289 *par = info->par;
+
+	if (regno >= 16)
+		return 1;
+
+	if (info->fix.visual == FB_VISUAL_DIRECTCOLOR)
+		return 1;
+
+	if ((info->var.bits_per_pixel == 16) && regno < 16) {
+		red >>= (16 - info->var.red.length);
+		red <<= info->var.red.offset;
+
+		green >>= (16 - info->var.green.length);
+		green <<= info->var.green.offset;
+
+		blue >>= (16 - info->var.blue.length);
+		blue <<= info->var.blue.offset;
+
+		par->pseudo_palette[regno] = red | green | blue;
+	}
+
+	return 0;
+}
+
+/* Configure the Burst Size and fifo threhold of DMA */
+static int lcd_cfg_dma(struct ssd1289 *item, int burst_size,  int fifo_th)
+{
+	u32 reg;
+
+	reg = (lcdc_read(item, LCD_DMA_CTRL_REG) & 0x00000005);	// | LCD_DUAL_FRAME_BUFFER_ENABLE; not for LIDD DMA??
+	switch (burst_size) {
+	case 1:
+		reg |= LCD_DMA_BURST_SIZE(LCD_DMA_BURST_1);
+		break;
+	case 2:
+		reg |= LCD_DMA_BURST_SIZE(LCD_DMA_BURST_2);
+		break;
+	case 4:
+		reg |= LCD_DMA_BURST_SIZE(LCD_DMA_BURST_4);
+		break;
+	case 8:
+		reg |= LCD_DMA_BURST_SIZE(LCD_DMA_BURST_8);
+		break;
+	case 16:
+		reg |= LCD_DMA_BURST_SIZE(LCD_DMA_BURST_16);
+		break;
+	default:
+		return -EINVAL;
+	}
+
+	reg |= (fifo_th << 8);
+	reg |= BIT(2);					// EOF_INTEN
+
+	lcdc_write(item, reg, LCD_DMA_CTRL_REG);
+
+	return 0;
+}
+
+/* IRQ handler for version 2 of LCDC */
+static irqreturn_t ssd1289_irq_handler(int irq, void *arg)
+{
+	struct ssd1289 *item = (struct ssd1289 *)arg;
+	u32 stat = lcdc_read(item,LCD_MASKED_STAT_REG);
+	u32 reg_int;
+
+	if ((stat & LCD_SYNC_LOST) || (stat & LCD_FIFO_UNDERFLOW)) {
+		printk(KERN_ERR "LCDC sync lost or underflow error occured\nNot sure what to do...\n");
+		lcdc_write(item, stat, LCD_MASKED_STAT_REG);
+	} else {
+		ssd1289_dma_setstatus(item, 0);	//disable DMA
+
+		lcdc_write(item, stat, LCD_MASKED_STAT_REG);
+
+		ssd1289_reg_set(item, SSD1289_REG_GDDRAM_X_ADDR, 0);
+		ssd1289_reg_set(item, SSD1289_REG_GDDRAM_Y_ADDR, 0);
+		lcdc_write(item,SSD1289_REG_GDDRAM_DATA, LCD_LIDD_CS0_ADDR);	//set up for data before DMA begins
+
+		if (!item->suspending) {				//Don't re-enable DMA if we want to suspend or disable the driver
+			ssd1289_dma_setstatus(item, 1);	//enable DMA
+		} else {
+			item->suspending = 0;
+		}
+	}
+	//lcdc_write(item, 0, LCD_END_OF_INT_IND_REG);		//not a thing...?
+	return IRQ_HANDLED;
+}
+
+static void ssd1289_dma_setstatus(struct ssd1289 *item, int doenable) {
+	//enable DMA
+	unsigned int cfg;
+
+	cfg = lcdc_read(item,LCD_LIDD_CTRL);
+	cfg = (cfg & ~BIT(8)) | ((doenable&1) << 8);	//enable or disable DMA
+	lcdc_write(item,cfg,LCD_LIDD_CTRL);
+}
+
+static int fb_ioctl(struct fb_info *info, unsigned int cmd, unsigned long arg) {
+	//struct lcd_sync_arg sync_arg;
+
+	switch (cmd) {
+		case FBIOPUT_VSCREENINFO:
+			return 0;
+		default:
+			return -EINVAL;
+	}
+}
+
+static struct fb_ops ssd1289_fbops = {
+	.owner        = THIS_MODULE,
+	.fb_setcolreg = ssd1289_setcolreg,
+	.fb_fillrect = cfb_fillrect,
+	.fb_copyarea = cfb_copyarea,
+	.fb_imageblit = cfb_imageblit,
+	.fb_ioctl = fb_ioctl,
+//	.fb_blank = sys_blank,
+};
+
+static struct fb_fix_screeninfo ssd1289_fix __initdata = {
+	.id          = DRIVER_NAME,
+	.type        = FB_TYPE_PACKED_PIXELS,
+	.visual      = FB_VISUAL_TRUECOLOR,
+	.accel       = FB_ACCEL_NONE,
+	.line_length = 240 * 2,
+};
+
+static struct fb_var_screeninfo ssd1289_var __initdata = {
+	.xres		= 240,
+	.yres		= 320,
+	.xres_virtual	= 240,
+	.yres_virtual	= 320,
+	.bits_per_pixel	= 16,
+	.red		= {6, 5, 0},
+	.green		= {11, 5, 0},
+	.blue		= {0, 6, 0},
+	.activate	= FB_ACTIVATE_FORCE,	//FB_ACTIVATE_NOW,
+	.height		= 320,
+	.width		= 240,
+	.vmode		= FB_VMODE_NONINTERLACED,
+};
+
+static int __init ssd1289_probe(struct platform_device *dev)
+{
+	int ret = 0;
+	struct ssd1289 *item;
+	struct resource *res_res;
+	unsigned int cfg;
+	unsigned int lcdc_regs_size;
+	struct resource *res_req;
+	unsigned int signature;
+	struct fb_info *info;
+	u32 reg_int;
+
+	dev_dbg(&dev->dev, "%s\n", __func__);
+
+	item = kzalloc(sizeof(struct ssd1289), GFP_KERNEL);
+	if (!item) {
+		dev_err(&dev->dev,
+			"%s: unable to kzalloc for ssd1289\n", __func__);
+		ret = -ENOMEM;
+		goto out;
+	}
+	item->dev = &dev->dev;
+
+	res_res = platform_get_resource(dev, IORESOURCE_MEM, 0);
+	if (!res_res) {
+		dev_err(&dev->dev,
+			"%s: unable to platform_get_resource for ctrl_res\n",
+			__func__);
+		ret = -ENOENT;
+		goto out_item;
+	}
+	lcdc_regs_size = resource_size(res_res);
+	res_req = request_mem_region(res_res->start, lcdc_regs_size,
+				      dev->name);
+	if (!res_req) {
+		dev_err(&dev->dev,
+			"%s: unable to request_mem_region for res_req\n",
+			__func__);
+		ret = -EIO;
+		goto out_item;
+	}
+	dev_dbg(&dev->dev, "%s: Raw lcd_regs=%p\n",
+		__func__, res_req->start);
+	item->lcd_regs = ioremap(res_req->start, lcdc_regs_size);
+	if (!item->lcd_regs) {
+		ret = -EINVAL;
+		dev_err(&dev->dev,
+			"%s: unable to ioremap for ctrl_io\n", __func__);
+		goto out_request_mem;
+	}
+
+	dev_dbg(&dev->dev, "%s: Remapped lcd_regs=%p\n",
+		__func__, item->lcd_regs);
+
+	//Enable device clock
+	item->lcdc_clk = clk_get(&dev->dev, NULL);
+	if (IS_ERR(item->lcdc_clk)) {
+		dev_err(&dev->dev, "Can not get device clock\n");
+		ret = -ENODEV;
+		goto out_ioremap;
+	}
+
+	pm_runtime_irq_safe(&dev->dev);
+	pm_runtime_enable(&dev->dev);
+	pm_runtime_get_sync(&dev->dev);
+
+	ret = clk_enable(item->lcdc_clk);
+	if (ret) {
+		dev_err(&dev->dev, "Can not enable device clock\n");
+		goto err_clk_get;
+	}
+
+	//AM335X LCD Controller Check
+	signature = lcdc_read(item,LCD_PID);
+	dev_dbg(&dev->dev, "%s: controller signature=0x%08x\n", __func__, signature);
+	if (signature != 0x4F200800 && signature != 0x4F201000) {
+		ret = -ENODEV;
+		dev_err(&dev->dev,
+			"%s: unknown LCDC v2 controller signature 0x%08x\n", __func__, signature);
+		goto out_clk_enable;
+	}
+
+	//Setting up AM335X LCDC Controller
+	cfg = LCD_V2_DMA_CLK_EN | LCD_V2_LIDD_CLK_EN | LCD_V2_CORE_CLK_EN; // Turn on LIDD clock and DMA clock, core clock doesn't help DMA :( ?
+	lcdc_write(item,cfg,LCD_CLKC_ENABLE);
+	cfg = 0 | LCD_CLK_DIVISOR(4);						// LCDC Mode = LIDD
+														// for 500MHz -> 250MHz, try to get to 50MHz
+	lcdc_write(item,cfg,LCD_CTRL);
+	lcdc_write(item,LCD_LIDD_TYPE_8080, LCD_LIDD_CTRL);			// 8080 display, DMA (NOT YET) enabled
+
+	//Clocking: 00000000011001000000011110001100
+	//          _____------____-----______----__
+	//			W_SU W_STR W_H R_SU R_STR R_H TA
+	//			0    3     2   0    30    3   0
+	cfg = (0<<0) | (3<<2) | (30<<6) | (0<<12) | (2<<17) | (3<<21) | (0<<27); // Clocking
+	lcdc_write(item,cfg,LCD_CS0_CONF);
+	dev_dbg(&dev->dev, "%s: Initialized LCDC controller\n",__func__);
+
+	//SSD1289 LCD Driver Check
+	ssd1289_reg_set(item, SSD1289_REG_OSCILLATION, 0x0001);
+	signature = ssd1289_reg_get(item,SSD1289_REG_DEV_CODE_READ);
+	dev_dbg(&dev->dev, "%s: driver signature=0x%04x\n", __func__, (unsigned short)signature);
+	if ((unsigned short)signature != 0x8989) {
+		dev_warn(&dev->dev,
+			"%s: unknown driver signature 0x%04x (reg_get failure?)\n", __func__, (unsigned short)signature);
+	}
+
+	dev_dbg(&dev->dev, "item=0x%p lcd_regs=0x%p\n", (void *)item,
+		 (void *)res_res->start);
+
+	info = framebuffer_alloc(sizeof(struct ssd1289), &dev->dev);
+	if (!info) {
+		ret = -ENOMEM;
+		dev_err(&dev->dev,
+			"%s: unable to framebuffer_alloc\n", __func__);
+		goto out_clk_enable;
+	}
+	item->info = info;
+	info->par = item;
+	info->fbops = &ssd1289_fbops;
+
+	// Palette setup
+	item->pseudo_palette[0] = 0;
+	item->pseudo_palette[1] = item->pseudo_palette[7] = item->pseudo_palette[15] = 0x0000ffff;
+	info->pseudo_palette = item->pseudo_palette;
+
+	item->irq = platform_get_irq(dev, 0);
+	if (item->irq < 0) {
+		ret = -ENOENT;
+		goto out_info;
+	}
+
+	info->flags = FBINFO_FLAG_DEFAULT;
+	info->fix = ssd1289_fix;
+	info->var = ssd1289_var;
+	fb_set_var(info,&ssd1289_var);
+	dev_set_drvdata(dev, info);
+
+	ret = ssd1289_video_alloc(item);
+	if (ret) {
+		ret = -ENOMEM;
+		dev_err(&dev->dev,
+			"%s: unable to ssd1289_video_alloc\n", __func__);
+		goto out_info;
+	}
+	dev_dbg(&dev->dev, "Initialized video memory at 0x%p-0x%p (%d bytes)\n",info->screen_base,info->screen_base + info->screen_size, info->screen_size);
+
+	// Do SSD1289 setup
+	ssd1289_setup(item);
+	dev_dbg(&dev->dev, "Performed setup\n");
+
+	// Set up all kinds of fun DMA
+	lcdc_write(item,0, LCD_DMA_CTRL_REG);					// Start out with a blank slate
+	lcd_cfg_dma(item, 16, 0);								// DMA burst and FIFO threshold
+	reg_int = LCD_V2_UNDERFLOW_INT_ENA | LCD_SYNC_LOST | LCD_V2_DONE_INT_ENA;
+	lcdc_write(item,reg_int, LCD_INT_ENABLE_SET_REG);
+
+	lcdc_write(item,item->dma_start, LCD_DMA_FRM_BUF_BASE_ADDR_0_REG);
+	lcdc_write(item,item->dma_end, LCD_DMA_FRM_BUF_CEILING_ADDR_0_REG);
+	lcdc_write(item,item->dma_start, LCD_DMA_FRM_BUF_BASE_ADDR_1_REG);
+	lcdc_write(item,item->dma_end, LCD_DMA_FRM_BUF_CEILING_ADDR_1_REG);
+
+	info->fbops = &ssd1289_fbops;
+	ret = register_framebuffer(info);
+	if (ret < 0) {
+		ret = -EIO;
+		dev_err(&dev->dev,
+			"%s: unable to register_frambuffer\n", __func__);
+		goto out_pages;
+	}
+	dev_dbg(&dev->dev, "Registered framebuffer.\n");
+
+	// Set up LCD coordinates as necessary
+	ssd1289_reg_set(item, SSD1289_REG_GDDRAM_X_ADDR, 0);
+	ssd1289_reg_set(item, SSD1289_REG_GDDRAM_Y_ADDR, 0);
+	lcdc_write(item,SSD1289_REG_GDDRAM_DATA, LCD_LIDD_CS0_ADDR);	//set up for data before DMA begins
+
+	// Try to get IRQ for DMA
+	ret = request_irq(item->irq, ssd1289_irq_handler, 0,
+			DRIVER_NAME, item);
+	if (ret) {
+		ret = -EIO;
+		goto out_framebuffer;
+	}
+
+	ssd1289_dma_setstatus(item, 1);	//enable DMA
+	return 0;
+
+out_framebuffer:
+	unregister_framebuffer(info);
+out_pages:
+	ssd1289_video_free(item);
+out_info:
+	framebuffer_release(info);
+out_clk_enable:
+	clk_disable(item->lcdc_clk);
+err_clk_get:
+	pm_runtime_put_sync(&dev->dev);
+	pm_runtime_disable(&dev->dev);
+	clk_put(item->lcdc_clk);
+out_ioremap:
+	iounmap((void __iomem *)item->lcd_regs);
+out_request_mem:
+	release_mem_region(res_res->start, lcdc_regs_size);
+out_item:
+	kfree(item);
+out:
+	printk(KERN_EMERG "failed to probe/init ssd1289 driver\n");
+	dev_err(&dev->dev,"failed to probe/init ssd1289 driver\n");
+	return ret;
+}
+
+//Thanks to the da8xx driver for inspiration on suspend/resume
+#ifdef CONFIG_PM
+
+struct lcdc_context {
+	u32 clk_enable;
+	u32 ctrl;
+	u32 dma_ctrl;
+	u32 int_enable_set;
+	u32 dma_frm_buf_base_addr_0;
+	u32 dma_frm_buf_ceiling_addr_0;
+	u32 dma_frm_buf_base_addr_1;
+	u32 dma_frm_buf_ceiling_addr_1;
+} reg_context;
+
+static int ssd1289_suspend (struct platform_device *dev, pm_message_t state) {
+
+	int i = 5000;
+	u32 stat;
+	struct ssd1289* item = (struct ssd1289*)dev->dev.platform_data;
+
+	console_lock();
+	fb_set_suspend(item->info, 1);
+	item->suspending = 1;
+	do {
+		mdelay(1);
+	} while (item->suspending && (i--));
+
+	if (item->suspending) {
+		dev_err(&dev->dev,"Failed to suspend %s driver\n",DRIVER_NAME);
+		return 1;
+	}
+	stat = lcdc_read(item,LCD_STAT_REG);
+	lcdc_write(item,stat,LCD_MASKED_STAT_REG);
+
+	// PT=0 VLE=1 SPT=0 GON=1 DTE=1 CM=0 D=0 (Turn off the display)
+	ssd1289_reg_set(item, SSD1289_REG_DISPLAY_CTRL, 0x0230);
+
+	lcd_context_save(item);
+	pm_runtime_put(&dev->dev);
+	console_unlock();
+
+	return 0;
+}
+
+static int ssd1289_resume (struct platform_device *dev, pm_message_t state) {
+	struct ssd1289* item = (struct ssd1289*)dev->dev.platform_data;
+
+	console_lock();
+	pm_runtime_get_sync(&dev->dev);
+	msleep(1);
+
+	lcd_context_restore(item);
+
+	// Do SSD1289 setup
+	ssd1289_setup(item);
+
+	// Set up LCD coordinates as necessary
+	ssd1289_reg_set(item, SSD1289_REG_GDDRAM_X_ADDR, 0);
+	ssd1289_reg_set(item, SSD1289_REG_GDDRAM_Y_ADDR, 0);
+	lcdc_write(item,SSD1289_REG_GDDRAM_DATA, LCD_LIDD_CS0_ADDR);	//set up for data before DMA begins
+	fb_set_suspend(item->info, 0);
+	ssd1289_dma_setstatus(item, 1);	//enable DMA
+	console_unlock();
+
+	dev_dbg(&dev->dev,"Resumed.");
+	return 0;
+}
+
+static void lcd_context_save(struct ssd1289* item)
+{
+	reg_context.clk_enable = lcdc_read(item,LCD_CLK_ENABLE_REG);
+	reg_context.ctrl = lcdc_read(item,LCD_CTRL);
+	reg_context.dma_ctrl = lcdc_read(item,LCD_DMA_CTRL_REG);
+	reg_context.int_enable_set = lcdc_read(item,LCD_INT_ENABLE_SET_REG);
+	reg_context.dma_frm_buf_base_addr_0 =
+		lcdc_read(item,LCD_DMA_FRM_BUF_BASE_ADDR_0_REG);
+	reg_context.dma_frm_buf_ceiling_addr_0 =
+		lcdc_read(item,LCD_DMA_FRM_BUF_CEILING_ADDR_0_REG);
+	reg_context.dma_frm_buf_base_addr_1 =
+		lcdc_read(item,LCD_DMA_FRM_BUF_BASE_ADDR_1_REG);
+	reg_context.dma_frm_buf_ceiling_addr_1 =
+		lcdc_read(item,LCD_DMA_FRM_BUF_CEILING_ADDR_1_REG);
+	return;
+}
+
+static void lcd_context_restore(struct ssd1289* item)
+{
+	lcdc_write(item,reg_context.clk_enable, LCD_CLK_ENABLE_REG);
+	lcdc_write(item,reg_context.ctrl, LCD_CTRL);
+	lcdc_write(item,reg_context.dma_ctrl, LCD_DMA_CTRL_REG);
+	lcdc_write(item,reg_context.int_enable_set, LCD_INT_ENABLE_SET_REG);
+	lcdc_write(item,reg_context.dma_frm_buf_base_addr_0,
+			LCD_DMA_FRM_BUF_BASE_ADDR_0_REG);
+	lcdc_write(item,reg_context.dma_frm_buf_ceiling_addr_0,
+			LCD_DMA_FRM_BUF_CEILING_ADDR_0_REG);
+	lcdc_write(item,reg_context.dma_frm_buf_base_addr_1,
+			LCD_DMA_FRM_BUF_BASE_ADDR_1_REG);
+	lcdc_write(item,reg_context.dma_frm_buf_ceiling_addr_1,
+			LCD_DMA_FRM_BUF_CEILING_ADDR_1_REG);
+	return;
+}
+
+#else
+#define fb_suspend NULL
+#define fb_resume NULL
+#endif							//#ifdef CONFIG_PM
+
+static struct platform_driver ssd1289_driver = {
+	.probe = ssd1289_probe,
+	.suspend = ssd1289_suspend,
+	.resume = ssd1289_resume,
+	.driver = {
+		   .name = DRIVER_NAME,
+		   .owner = THIS_MODULE,
+		   },
+};
+
+static int __init ssd1289_init(void)
+{
+	int ret = 0;
+
+	pr_debug("%s\n", __func__);
+
+	ret = platform_driver_register(&ssd1289_driver);
+	if (ret) {
+		pr_err("%s: unable to platform_driver_register\n", __func__);
+	}
+
+	return ret;
+}
+
+module_init(ssd1289_init);
+
+MODULE_DESCRIPTION("SSD1289 LCD Driver for AM335x");
+MODULE_AUTHOR("Heavily rewritten by Christopher Mitchell <linux@cemetech.net>; original by Jean-Christian de Rivaz <jc@eclis.ch>");
+MODULE_LICENSE("GPL");
\ No newline at end of file
--- build/tmp-angstrom_v2012_05-eglibc/work/beaglebone-angstrom-linux-gnueabi/linux-ti33x-psp-3.2-r10d+gitre8004dad869568692ca2a45c04e8464ce48cc4d7/git/arch/arm/plat-omap/include/plat/lcdc.h	2012-05-17 14:50:11.529264572 +0000
+++ ../setup-scripts-new/build/tmp-angstrom_v2012_05-eglibc/work/beaglebone-angstrom-linux-gnueabi/linux-ti33x-psp-3.2-r10d+gitre8004dad869568692ca2a45c04e8464ce48cc4d7/git/arch/arm/plat-omap/include/plat/lcdc.h	2012-05-13 20:51:26.961534900 +0000
@@ -16,6 +16,8 @@
#ifndef __OMAP2_LCDC_H
#define __OMAP2_LCDC_H

+struct platform_device *am33xx_register_lcdc_lidd_dma(
+		struct platform_device *pdata);
struct platform_device *am33xx_register_lcdc(
	   struct da8xx_lcdc_platform_data *pdata);
#endif
