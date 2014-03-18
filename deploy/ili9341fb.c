/*
 * linux/drivers/video/ili9341fb.c -- FB driver for ILI9341 LCD controller
 * Layout is based on ILI9341fb.c from James Simmons and Geert Uytterhoeven.
 * by Saak Dertadian
 * Copyright (C) 2014, Saak Dertadian <s.dertadian@gmail.com>
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License. See the file COPYING in the main directory of this archive for
 * more details.
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/string.h>
#include <linux/mm.h>
#include <linux/vmalloc.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/fb.h>
#include <linux/gpio.h>
#include <linux/spi/spi.h>
#include <linux/delay.h>
#include <linux/err.h>
#include <linux/uaccess.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/of_gpio.h>
#include <linux/pinctrl/consumer.h>
#include <linux/platform_device.h>
#include <linux/clk.h>
#include <linux/of_address.h>
#include <linux/pm_runtime.h>
#include <linux/cpufreq.h>
#include <linux/dma-mapping.h>
#include <linux/interrupt.h>
#include <linux/fb.h>

#include "ili9341fb.h"

static inline unsigned int lcdc_read(struct ili9341fb_par *item,unsigned int addr)
{
	//	printk(KERN_EMERG "%s ====> 0x0%08X\n",__func__,item->lcd_regs+addr);
	return (unsigned int)__raw_readl((void *)(item->lcd_regs+addr));
	//	return ioread32(item->lcd_regs+(addr));
}

static inline void lcdc_write(struct ili9341fb_par *item,unsigned int val,unsigned int addr)
{
	//dev_dbg(item->dev, "%s: Write %04x at offset 0x%p, addr 0x%p\n", __func__, val, (void *)addr, (void*)(item->lcd_regs + (addr)));
	__raw_writel(val,(void*)(item->lcd_regs+addr));
}

static inline void ili9341fb_reg_set(struct ili9341fb_par *item,unsigned char reg,unsigned short value)
{
	lcdc_write(item,0x000000FF&(unsigned int)reg,LCD_LIDD_CS0_ADDR);
	lcdc_write(item,(unsigned int)value,LCD_LIDD_CS0_DATA);
}

static inline unsigned int ili9341fb_reg_get(struct ili9341fb_par *item, unsigned char reg)
{
	lcdc_write(item,0x000000FF&(unsigned int)reg,LCD_LIDD_CS0_ADDR);
	return lcdc_read(item,LCD_LIDD_CS0_DATA);
}

static void ili9341_write_data(struct ili9341fb_par *par,u8 data)
{
	lcdc_write(par,(u8)data,LCD_LIDD_CS0_DATA);
}

static void ili9341_write_data16(struct ili9341fb_par *par,u16 data)
{
	lcdc_write(par,(u16)data,LCD_LIDD_CS0_DATA);
}

//static int ili9341_write_data_buf(struct ili9341fb_par *par,u8 *txbuf, int size)
//{
////	printk(KERN_EMERG "==> %02X %02X %02X %02X %02X %02X %02X %02X\n",txbuf[0],txbuf[1],txbuf[2],txbuf[3],txbuf[4],txbuf[5],txbuf[6],txbuf[7]);
////	gpio_set_value(par->dc,1);	// Set data mode
////	return spi_write(par->spi,txbuf,size);	// Write entire buffer
//	return 0;
//}

static void ili9341_write_cmd(struct ili9341fb_par *par,u8 data)
{
	lcdc_write(par,0x000000FF&(unsigned int)data,LCD_LIDD_CS0_ADDR);
}

static void ili9341_set_addr_win(struct ili9341fb_par *par,int xs,int ys,int xe,int ye)
{
	ili9341_write_cmd(par,CMD_COLUMN_ADDRESS_SET);
	ili9341_write_data(par,(xs >> 8) & 0xFF);
	ili9341_write_data(par,xs & 0xFF);
	ili9341_write_data(par,(ys >> 8) & 0xFF);
	ili9341_write_data(par,ys & 0xFF);
	ili9341_write_cmd(par,CMD_PAGE_ADDRESS_SET);
	ili9341_write_data(par,(xe >> 8) & 0xFF);
	ili9341_write_data(par,xe & 0xFF);
	ili9341_write_data(par,(ye >> 8) & 0xFF);
	ili9341_write_data(par,ye & 0xFF);
	ili9341_write_cmd(par,CMD_MEMORY_WRITE);
}

static void ili9341_splashScreen(struct ili9341fb_par *par,unsigned char fillData)
{
	int x,y;

	ili9341_set_addr_win(par,0,WIDTH-1,0,HEIGHT-1);

	ili9341_write_cmd(par,CMD_MEMORY_WRITE);
	for(y=0;y<320;y++)
	{
		for(x=0;x<240;x++)
		{
			if (y > 279) ili9341_write_data16(par,COLOR_WHITE);
			else if (y > 239) ili9341_write_data16(par,COLOR_BLUE);
			else if (y > 199) ili9341_write_data16(par,COLOR_GREEN);
			else if (y > 159) ili9341_write_data16(par,COLOR_CYAN);
			else if (y > 119) ili9341_write_data16(par,COLOR_RED);
			else if (y > 79) ili9341_write_data16(par,COLOR_MAGENTA);
			else if (y > 39) ili9341_write_data16(par,COLOR_YELLOW);
			else ili9341_write_data16(par,COLOR_BLACK);
		}
	}

//	ili9341_write_cmd(par,CMD_WRITE_CTRL_DISPLAY);					// Write CTRL Display
//	ili9341_write_data(par,0x00);
//	mdelay(1000);
//	ili9341_write_cmd(par,CMD_WRITE_CTRL_DISPLAY);					// Write CTRL Display
//	ili9341_write_data(par,0x08);
//	mdelay(1000);
}

//static void ili9341Cls(struct ili9341fb_par *par,unsigned char fillData)
//{
//	int x,y;

//	ili9341_set_addr_win(par,0,WIDTH-1,0,HEIGHT-1);

//	ili9341_write_cmd(par,CMD_MEMORY_WRITE);
//	for(y=0;y<320;y++)
//		for(x=0;x<240;x++)
//			ili9341_write_data16(par,COLOR_BLACK);
//}

static void ili9341_display_reset(struct ili9341fb_par *par)
{
	gpio_set_value(par->lcd_rs,0);
	udelay(15);
	gpio_set_value(par->lcd_rs,1);
	mdelay(120);
}

static void ili9341_display_init(struct ili9341fb_par *par)
{
	ili9341_write_cmd(par,CMD_DISPLAY_OFF);							// Display Off

	ili9341_write_cmd(par,CMD_SLEEP_OUT);							// exit SLEEP mode
	ili9341_write_data(par,0x0000);


	ili9341_write_cmd(par,CMD_POWER_CONTROL_A);						// Power Control A
	ili9341_write_data(par,0x39);									// always 0x39
	ili9341_write_data(par,0x2C);									// always 0x2C
	ili9341_write_data(par,0x00);									// always 0x00
	ili9341_write_data(par,0x34);									// Vcore = 1.6V
	ili9341_write_data(par,0x02);									// DDVDH = 5.6V

	ili9341_write_cmd(par,CMD_POWER_CONTROL_B);						// Power Control B
	ili9341_write_data(par,0x00);									// always 0x00
	ili9341_write_data(par,0x81);									// PCEQ off
	ili9341_write_data(par,0x30);									// ESD protection

	ili9341_write_cmd(par,CMD_DRIVER_TIMING_CONTROL_A);				// Driver timing control A
	ili9341_write_data(par,0x85);									// non‐overlap
	ili9341_write_data(par,0x01);									// EQ timing
	ili9341_write_data(par,0x79);									// Pre‐charge timing

	ili9341_write_cmd(par,CMD_DRIVER_TIMING_CONTROL_B);				// Driver timing control B
	ili9341_write_data(par,0x00);									// Gate driver timing
	ili9341_write_data(par,0x00);									// always 0x00

	ili9341_write_cmd(par,CMD_POWER_ON_SEQUENCE_CONTROL);			// Power‐On sequence control
	ili9341_write_data(par,0x64);									// soft start
	ili9341_write_data(par,0x03);									// power on sequence
	ili9341_write_data(par,0x12);									// power on sequence
	ili9341_write_data(par,0x81);									// DDVDH enhance on

	ili9341_write_cmd(par,CMD_PUMP_RATION_CONTROL);					// DDVDH enhance on
	ili9341_write_data(par,0x20);									// DDVDH=2xVCI

	ili9341_write_cmd(par,CMD_POWER_CONTROL_1);						// power control 1
	ili9341_write_data(par,0x26);
	ili9341_write_data(par,0x04);									// second parameter for ILI9340 (ignored by ILI9341)

	ili9341_write_cmd(par,CMD_POWER_CONTROL_2);						// power control 2
	ili9341_write_data(par,0x11);

	ili9341_write_cmd(par,CMD_VCOM_CONTROL_1);						// VCOM control 1
	ili9341_write_data(par,0x35);
	ili9341_write_data(par,0x3E);

	ili9341_write_cmd(par,CMD_VCOM_CONTROL_2);						// VCOM control 2
	ili9341_write_data(par,0xBE);

	ili9341_write_cmd(par,CMD_MEMORY_ACCESS_CONTROL);				// memory access control = BGR
	//	ili9341_write_data(par,0x88);
	ili9341_write_data(par,0xC8);

	ili9341_write_cmd(par,CMD_FRAME_CONTROL_NORMAL_MODE);			// frame rate control
	ili9341_write_data(par,0x00);
	ili9341_write_data(par,0x10);

	ili9341_write_cmd(par,CMD_DISPLAY_FUNCTION_CONTROL);			// display function control
	ili9341_write_data(par,0x0A);
	ili9341_write_data(par,0xA2);

	ili9341_write_cmd(par,CMD_PIXEL_FORMAT_SET);					// pixel format = 16 bit per pixel
	ili9341_write_data(par,0x55);

	ili9341_write_cmd(par,CMD_ENABLE_3G);							// 3G Gamma control
	ili9341_write_data(par,0x02);									// OFF

	ili9341_write_cmd(par,CMD_GAMMA_SET);							// Gamma curve 3
	ili9341_write_data(par,0x01);

//	ili9341_write_cmd(par,CMD_WRITE_CTRL_DISPLAY);					// Write CTRL Display
//	ili9341_write_data(par,0x00);

	ili9341_write_cmd(par,CMD_COLUMN_ADDRESS_SET);					// column address set
	ili9341_write_data(par,0x00);
	ili9341_write_data(par,0x00);									// start 0x0000
	ili9341_write_data(par,0x00);
	ili9341_write_data(par,0xEF);									// end 0x00EF

	ili9341_write_cmd(par,CMD_PAGE_ADDRESS_SET);					// page address set
	ili9341_write_data(par,0x00);
	ili9341_write_data(par,0x00);									// start 0x0000
	ili9341_write_data(par,0x01);
	ili9341_write_data(par,0x3F);									// end 0x013F

	ili9341_write_cmd(par,CMD_WRITE_CTRL_DISPLAY);					// Write CTRL Display
	ili9341_write_data(par,0x08);

	ili9341_write_cmd(par,CMD_SLEEP_OUT);
	mdelay(100);
	ili9341_write_cmd(par,CMD_DISPLAY_ON);
	mdelay(100);
	//	ili9341_write_cmd(par,CMD_MEMORY_WRITE);
}

//static void ili9341fb_update_display(struct ili9341fb_par *par)
//{
//	//	int ret=0;
//	//	u16 *vmem;
//	//	int i;

//	////	printk(KERN_EMERG "----> ili9341fb_update_display <----\n");
//	//	u16 *vmem16=(u16 *)par->info->screen_base;
//	//	vmem=par->ssbuf;
//	//	for (i=0;i<WIDTH*HEIGHT*BPP/8/2;i++)
//	//	{
//	////		vmem[i]=(vmem16[i]&0xFFFF);

//	//		vmem[i]=0x0000;

//	//		if(vmem16[i]&0x0F00)	vmem[i]|=(vmem16[i]&0x0F00)<<5;
//	//		if(vmem16[i]&0xF000)	vmem[i]|=(vmem16[i]&0xF000)>>3;
//	//		if(vmem16[i]&0x000F)	vmem[i]|=(vmem16[i]&0x000F)<<5;
//	//		if(vmem16[i]&0x00F0)	vmem[i]|=(vmem16[i]&0x00F0)>>3;
//	//	}

//	//	mutex_lock(&(par->io_lock));

//	//	/* Set row/column data window */
//	//	ili9341_set_addr_win(par,0,0,WIDTH-1,HEIGHT-1);

//	//	/* Internal RAM write command */
//	//	ili9341_write_cmd(par,CMD_WRITE_RAM_CMD);

//	//	/* Blast framebuffer to ILI9341 internal display RAM */
//	//	ret=ili9341_write_data_buf(par,u8 *)vmem,WIDTH*HEIGHT*BPP/8);
//	//	if(ret<0)
//	//		pr_err("%s: spi_write failed to update display buffer\n",par->info->fix.id);

//	//	mutex_unlock(&(par->io_lock));
//}

int RequestGpio(int gpio,int mode, char* str)
{
	int ret=gpio_request_one(gpio,mode,str);
	if(ret<0)
		pr_err("failed to claim %s pin\n",str);
	return ret;
}

//static void ili9341fb_deferred_io(struct fb_info *info,struct list_head *pagelist)
//{
//	//	printk(KERN_EMERG "----> ili9341fb_deferred_io <----\n");
//	ili9341fb_update_display(info->par);
//}

//static void ili9341fb_update_display_deferred(struct fb_info *info)
//{
//	struct fb_deferred_io *fbdefio=info->fbdefio;
//	//	printk(KERN_EMERG "----> ili9341fb_update_display_deferred <----\n");
//	schedule_delayed_work(&info->deferred_work,fbdefio->delay);
//}

//static void ili9341fb_fillrect(struct fb_info *info,const struct fb_fillrect *rect)
//{
//	//	printk(KERN_INFO "----> ili9341fb_fillrect <----\n");
//	sys_fillrect(info,rect);
//	ili9341fb_update_display_deferred(info);
//}

//static void ili9341fb_copyarea(struct fb_info *info,const struct fb_copyarea *area)
//{
//	//	printk(KERN_INFO "----> ili9341fb_copyarea <----\n");
//	sys_copyarea(info, area);
//	ili9341fb_update_display_deferred(info);
//}

//static void ili9341fb_imageblit(struct fb_info *info,const struct fb_image *image)
//{
//	//	printk(KERN_INFO "----> ili9341fb_imageblit <----\n");
//	sys_imageblit(info,image);
//	ili9341fb_update_display_deferred(info);
//}

static int ili9341fb_setcolreg(unsigned regno,unsigned red,unsigned green,unsigned blue,unsigned transp,struct fb_info *info)
{
	struct ili9341fb_par *par=info->par;

	if(regno>=16)
		return 1;

	if(info->fix.visual==FB_VISUAL_DIRECTCOLOR)
		return 1;

	if((info->var.bits_per_pixel==16) && regno<16)
	{
		red>>=(16-info->var.red.length);
		red<<=info->var.red.offset;

		green>>=(16-info->var.green.length);
		green<<=info->var.green.offset;

		blue>>=(16-info->var.blue.length);
		blue<<=info->var.blue.offset;

		par->pseudo_palette[regno]=red | green | blue;
	}
	return 0;
}

int LoadGpioFromDT(struct device_node *np,char *name)
{
	int gpio=of_get_named_gpio(np,name,0);
	if(gpio<0)
	{
		printk(KERN_EMERG "failed to find ILI9341-%s!\n",name);
		return -EINVAL;
	}
	//	else
	//		printk(KERN_EMERG "ILI9341-%s=%d\n",name,gpio);
	return  1;
}

static const struct of_device_id ili9341fb_dt_ids[] = {
	{ .compatible = "Saak,lcd-2.4", .data = (void *) LCD_2_4, },
	{ },
};
MODULE_DEVICE_TABLE(of,ili9341fb_dt_ids);

static int cpufreq_transition(struct notifier_block *nb,unsigned long val,void *data)
{
	struct ili9341fb_par *priv=container_of(nb,struct ili9341fb_par,freq_transition);
	if(val==CPUFREQ_POSTCHANGE)
	{
		if(priv->lcd_fck_rate != clk_get_rate(priv->lcdc_clk))
		{
			priv->lcd_fck_rate = clk_get_rate(priv->lcdc_clk);
			//			tilcdc_crtc_update_clk(priv->crtc);
		}
	}

	return 0;
}

static int fb_ioctl(struct fb_info *info,unsigned int cmd,unsigned long arg)
{
//	struct fb_cmap cmap_from;
//	struct fb_cmap_user cmap;
//	void __user *argp = (void __user *)arg;
//	struct lcd_sync_arg sync_arg;
	long ret = 0;

	printk(KERN_EMERG "%s: ---> %d\n", __func__,cmd);
	switch (cmd)
	{
//	case FBIOPUT_VSCREENINFO:
//		return 0;
//	case FBIOGET_CONTRAST:
//	case FBIOPUT_CONTRAST:
//	case FBIGET_BRIGHTNESS:
//	case FBIPUT_BRIGHTNESS:
//	case FBIGET_COLOR:
//	case FBIPUT_COLOR:
//		return -ENOTTY;
//	case FBCMD_GET_CURRENTPAR:
//		if (copy_to_user((void *)arg, (void *)&current_par,sizeof(struct atafb_par)))
//			return -EFAULT;
//		return 0;
//	case FBIPUT_HSYNC:
//		if (copy_from_user(&sync_arg, (char *)arg,
//				sizeof(struct lcd_sync_arg)))
//			return -EFAULT;
//		lcd_cfg_horizontal_sync(sync_arg.back_porch,
//					sync_arg.pulse_width,
//					sync_arg.front_porch);
//		break;
//	case FBIPUT_VSYNC:
//		if (copy_from_user(&sync_arg, (char *)arg,
//				sizeof(struct lcd_sync_arg)))
//			return -EFAULT;
//		lcd_cfg_vertical_sync(sync_arg.back_porch,
//					sync_arg.pulse_width,
//					sync_arg.front_porch);
//		break;
//	case FBIO_WAITFORVSYNC:
//		return fb_wait_for_vsync(info);
//	case FBIOGETCMAP:
//		if(copy_from_user(&cmap, argp, sizeof(cmap)))
//			return -EFAULT;
//		if (!lock_fb_info(info))
//			return -ENODEV;
//		cmap_from=info->cmap;
//		unlock_fb_info(info);
//		ret=fb_cmap_to_user(&cmap_from,&cmap);
//		break;
	default:
		printk(KERN_INFO "%s: %d\n", __func__,cmd);
		return -EINVAL;
	}
	return ret;
}

static int __init ili9341fb_video_alloc(struct ili9341fb_par *item)
{
	unsigned int frame_size;
	int pages_count;

	dev_dbg(item->dev,"%s: item=0x%p\n",__func__,(void *)item);

	// Calculate raw framebuffer size
	frame_size=item->info->fix.line_length*item->info->var.yres;
	dev_dbg(item->dev,"%s: item=0x%p frame_size=%u\n",__func__,(void *)item,frame_size);

	// Figure out how many full pages we need
	pages_count=frame_size/PAGE_SIZE;
	if((pages_count*PAGE_SIZE)<frame_size)
		pages_count++;
	dev_dbg(item->dev, "%s: item=0x%p pages_count=%u per each of %d buffer(s)\n",__func__,(void *)item,pages_count,LCD_NUM_BUFFERS);
	dev_dbg(item->dev,"%s: PAGE_SIZE=%d\n", __func__,PAGE_SIZE);

	// Reserve DMA-able RAM, set up fix.
	item->vram_size=pages_count*PAGE_SIZE*LCD_NUM_BUFFERS;
	item->vram_virt=dma_alloc_coherent(NULL,item->vram_size,(resource_size_t *)&item->vram_phys,GFP_KERNEL | GFP_DMA);
	if(!item->vram_virt)
	{
		dev_err(item->dev, "%s: unable to vmalloc\n",__func__);
		return -ENOMEM;
	}
	memset(item->vram_virt,0,item->vram_size);
//	item->info->fix.smem_start=(int)item->vram_virt;
	item->info->fix.smem_start=(int)item->vram_phys;
	item->info->fix.smem_len=item->vram_size;

	item->info->screen_base=(char __iomem *)item->vram_virt;
	item->info->screen_size=(unsigned int)item->vram_size;

	item->dma_start=item->vram_phys;
	item->dma_end=item->dma_start+(item->info->var.yres*item->info->fix.line_length)-1;
	dev_dbg(item->dev,"%s: DMA set from 0x%p to 0x%p, %d bytes\n",__func__,(void*)item->dma_start,(void*)item->dma_end,(int)item->vram_size);

	return 0;
}

/* Configure the Burst Size and fifo threhold of DMA */
static int lcd_cfg_dma(struct ili9341fb_par *item,int burst_size,int fifo_th)
{
	u32 reg;

	reg=(lcdc_read(item,LCD_DMA_CTRL_REG)&0x00000005);	// | LCD_DUAL_FRAME_BUFFER_ENABLE; not for LIDD DMA??
	switch(burst_size)
	{
	case 1:
		reg |= LCD_DMA_BURST_SIZE(LCD_DMA_BURST_1);
		break;
	case 2:
		reg |= LCD_DMA_BURST_SIZE(LCD_DMA_BURST_2);
		break;
	case 4:
		reg |= LCD_DMA_BURST_SIZE(LCD_DMA_BURST_4);
		break;
	case 8:
		reg |= LCD_DMA_BURST_SIZE(LCD_DMA_BURST_8);
		break;
	case 16:
		reg |= LCD_DMA_BURST_SIZE(LCD_DMA_BURST_16);
		break;
	default:
		return -EINVAL;
	}
	reg|=(fifo_th<<8);
	reg|=BIT(2);					// EOF_INTEN
	lcdc_write(item,reg,LCD_DMA_CTRL_REG);
	return 0;
}

static void ili9341fb_dma_setstatus(struct ili9341fb_par *item,int doenable)
{
	//enable DMA
	unsigned int cfg;

	cfg=lcdc_read(item,LCD_LIDD_CTRL);
	cfg=(cfg & ~BIT(8)) | ((doenable&1) << 8);	//enable or disable DMA
	lcdc_write(item,cfg,LCD_LIDD_CTRL);
}

/* IRQ handler for version 2 of LCDC */
static irqreturn_t ili9341fb_irq_handler(int irq, void *arg)
{
	struct ili9341fb_par *item =(struct ili9341fb_par *)arg;
	u32 stat=lcdc_read(item,LCD_MASKED_STAT_REG);

	if((stat & LCD_SYNC_LOST) || (stat & LCD_FIFO_UNDERFLOW))
	{
		printk(KERN_ERR "LCDC sync lost or underflow error occured\nNot sure what to do...\n");
		lcdc_write(item,stat,LCD_MASKED_STAT_REG);
	}
	else
	{
		ili9341fb_dma_setstatus(item,0);	//disable DMA
		lcdc_write(item,stat,LCD_MASKED_STAT_REG);

		ili9341_set_addr_win(item,0,WIDTH-1,0,HEIGHT-1);
		ili9341_write_cmd(item,CMD_MEMORY_WRITE);	//set up for data before DMA begins

		if(!item->suspending)
		{
			//Don't re-enable DMA if we want to suspend or disable the driver
			ili9341fb_dma_setstatus(item, 1);	//enable DMA
		}
		else
			item->suspending = 0;
	}
	//lcdc_write(item, 0, LCD_END_OF_INT_IND_REG);		//not a thing...?
	return IRQ_HANDLED;
}

static int ili9341fb_probe(struct platform_device *pdev)
{
	int ret=0;
	struct ili9341fb_par *item;
	struct resource res;
	unsigned int lcdc_regs_size;
	unsigned int signature;
	unsigned int cfg;
	unsigned int rc;
	struct fb_info *info;
	const struct of_device_id *match;
	char *state_name;
	struct pinctrl_state *state;
	u32 reg_int;
	//	unsigned int i,d,d2,tab[16];

	printk(KERN_EMERG "%s ====> 0 <====\n", __func__);

	item=kzalloc(sizeof(struct ili9341fb_par),GFP_KERNEL);
	if(!item)
	{
		dev_err(&pdev->dev,"%s: unable to kzalloc for ili9341\n", __func__);
		ret=-ENOMEM;
		goto out;
	}
	item->dev=&pdev->dev;
	dev_set_drvdata(&pdev->dev,item);

	match=of_match_device(ili9341fb_dt_ids,&pdev->dev);
	if(!match)
		return -EINVAL;

	rc=of_address_to_resource(pdev->dev.of_node,0,&res);
	if(rc)
	{
		dev_err(&pdev->dev,"%s: unable to get address of resource.\n",__func__);
		ret=-ENOENT;
		goto out_item;
	}

	lcdc_regs_size=resource_size(&res);
	if(!request_mem_region(res.start,lcdc_regs_size,pdev->name))
	{
		dev_err(&pdev->dev,"%s: unable to request me  region.\n",__func__);
		ret=-ENOENT;
		goto out_item;
	}
	item->lcdc_regs_size=lcdc_regs_size;
	item->res=res;

	item->lcd_regs=(resource_size_t)ioremap(res.start,lcdc_regs_size);
	if(!item->lcd_regs)
	{
		ret=-EINVAL;
		dev_err(&pdev->dev,"%s: unable to ioremap for ctrl_io\n", __func__);
		goto out_request_mem;
	}
	printk(KERN_EMERG "%s: Remapped lcd_regs=%X\n",__func__,(int)item->lcd_regs);

	// Enable device clock fck
	item->lcdc_clk=clk_get(&pdev->dev,"fck");
	if(IS_ERR(item->lcdc_clk))
	{
		dev_err(&pdev->dev,"Can not get device clock: %d\n",(int)PTR_ERR(item->lcdc_clk));
		ret=-ENODEV;
		goto out_ioremap;
	}
	if(clk_enable(item->lcdc_clk))
	{
		dev_err(&pdev->dev, "Can not enable device clock\n");
		goto err_clk_get;
	}

	// Enable device clock dpll_disp_ck
	item->disp_clk=clk_get(&pdev->dev,"dpll_disp_ck");
	if(IS_ERR(item->disp_clk))
	{
		dev_err(&pdev->dev,"Can not get display clock: %d\n",(int)PTR_ERR(item->disp_clk));
		ret=-ENODEV;
		goto out_ioremap;
	}
	if(clk_enable(item->disp_clk))
	{
		dev_err(&pdev->dev, "Can not enable disp_clk clock\n");
		goto err_clk_get;
	}

	item->lcd_fck_rate=clk_get_rate(item->lcdc_clk);
	item->freq_transition.notifier_call=cpufreq_transition;
	ret=cpufreq_register_notifier(&item->freq_transition,
								  CPUFREQ_TRANSITION_NOTIFIER);
	if(ret)
	{
		dev_err(&pdev->dev, "failed to register cpufreq notifier\n");
		goto err_clk_get;
	}

	pm_runtime_irq_safe(&pdev->dev);
	pm_runtime_enable(&pdev->dev);
	pm_runtime_get_sync(&pdev->dev);

	// AM335X LCD Controller Check
	signature=lcdc_read(item,LCD_PID);
	printk(KERN_EMERG "%s: controller signature=0x%08x\n", __func__,signature);
	if(signature!=0x4F200800 && signature!=0x4F201000)
	{
		ret=-ENODEV;
		dev_err(&pdev->dev,"%s: unknown LCDC v2 controller signature 0x%08x\n",__func__,signature);
		goto out_clk_enable;
	}

	// //////////////////////////////////////////////////////////
	// Init PINMUX
	// //////////////////////////////////////////////////////////
	state_name=kmalloc(strlen(PINCTRL_STATE_DEFAULT)+1,GFP_KERNEL);
	if(state_name==NULL)
	{
		dev_err(&pdev->dev,"%s: Failed to allocate state name\n",__func__);
		goto out_clk_enable;
	}
	item->selected_state_name=state_name;
	strcpy(item->selected_state_name,PINCTRL_STATE_DEFAULT);

	item->pinctrl=devm_pinctrl_get(&pdev->dev);
	if(IS_ERR(item->pinctrl))
	{
		dev_err(&pdev->dev,"%s: Failed to get pinctrl\n",__func__);
		goto out_clk_enable;
	}
	state=pinctrl_lookup_state(item->pinctrl,item->selected_state_name);
	if(!IS_ERR(state))
	{
		ret=pinctrl_select_state(item->pinctrl,state);
		if(ret!=0)
		{
			dev_err(&pdev->dev,"%s: Failed to select default state\n",__func__);
			goto out_clk_enable;
		}
	}
	dev_warn(&pdev->dev,"%s: Init PINMUX done\n",__func__);

	// //////////////////////////////////////////////////////////
	// Init GPIO
	// //////////////////////////////////////////////////////////
	item->lcd_rs=of_get_named_gpio(pdev->dev.of_node, "lcd24-rs", 0);
	if(item->lcd_rs<0)
	{
		printk(KERN_EMERG "%s: failed to find ILI9341-rs node!\n",__func__);
		goto err_clk_get;
	}
	ret=gpio_request_one(item->lcd_rs,GPIOF_OUT_INIT_HIGH,"ILI9341 Reset Pin");
	if(ret<0)
	{
		dev_err(&pdev->dev,"%s: failed to claim reset pin\n",__func__);
		goto err_clk_get;
	}
	dev_err(&pdev->dev,"%s: RS=%d\n",__func__,item->lcd_rs);
	//	gpio_set_value(item->lcd_rs,0);
	//	udelay(120);
	//	gpio_set_value(item->lcd_rs,1);

	// //////////////////////////////////////////////////////////
	// Setting up AM335X LCDC Controller
	// //////////////////////////////////////////////////////////

	/* DMA has to be disabled */
	lcdc_write(item,0,LCD_DMA_CTRL_REG);
	lcdc_write(item,0,LCD_RASTER_CTRL_REG);

	lcdc_write(item,0,LCD_INT_ENABLE_SET_REG);
	/* Write 1 to reset */
	lcdc_write(item,LCD_CLK_MAIN_RESET,LCD_CLK_RESET_REG);
	lcdc_write(item,0,LCD_CLK_RESET_REG);


	cfg=LCD_V2_DMA_CLK_EN | LCD_V2_LIDD_CLK_EN | LCD_V2_CORE_CLK_EN;	// Turn on LIDD clock and DMA clock, core clock doesn't help DMA :( ?
	lcdc_write(item,cfg,LCD_CLKC_ENABLE);
	//	dev_warn(&pdev->dev,"%s: LCD_CLKC_ENABLE=0x%08X\n", __func__,(unsigned short)lcdc_read(item,LCD_CLKC_ENABLE));

	//	cfg=0 | LCD_CLK_DIVISOR(1);											// LCDC Mode = LIDD
	cfg=0;											// LCDC Mode = LIDD
	lcdc_write(item,cfg,LCD_CTRL);
	//	dev_warn(&pdev->dev,"%s: LCD_CTRL=0x%08X\n", __func__,(unsigned short)lcdc_read(item,LCD_CTRL));
	lcdc_write(item,LCD_LIDD_TYPE_8080,LCD_LIDD_CTRL);					// 8080 display, DMA (NOT YET) enabled
	//	dev_warn(&pdev->dev,"%s: LCD_LIDD_CTRL=0x%08X\n", __func__,(unsigned short)lcdc_read(item,LCD_LIDD_CTRL));

	// Clocking: 00000000011001000000011110001100
	//          _____------____-----______----__
	//			W_SU W_STR W_H R_SU R_STR R_H TA
	//			0    3     2   0    30    3   0
	//	cfg=(0<<0) | (3<<2) | (30<<6) | (0<<12) | (2<<17) | (3<<21) | (0<<27); // Clocking
	cfg=(0<<0) | (0x01<<2) | (0x01<<6) | (0x01<<12) | (1<<17) | (1<<21) | (1<<27); // Clocking 250ns
//	cfg=(1<<0) | (0x02<<2) | (0x02<<6) | (0x02<<12) | (2<<17) | (2<<21) | (2<<27); // Clocking 416ns
//	cfg=(3<<0) | (0x08<<2) | (0x08<<6) | (0x08<<12) | (8<<17) | (8<<21) | (8<<27); // Clocking 1.26us
	lcdc_write(item,cfg,LCD_CS0_CONF);
	//	dev_warn(&pdev->dev,"%s: LCD_CS0_CONF=0x%08X\n", __func__,(unsigned short)lcdc_read(item,LCD_CS0_CONF));
	printk(KERN_EMERG "%s: Initialized LCDC controller\n",__func__);

	// //////////////////////////////////////////////////////////
	// INIT FRAME BUFFER
	// //////////////////////////////////////////////////////////
	info=framebuffer_alloc(sizeof(struct ili9341fb_par),&pdev->dev);
	if(!info)
	{
		ret=-ENOMEM;
		dev_err(&pdev->dev,"%s: unable to framebuffer_alloc\n", __func__);
		goto out_clk_enable;
	}
	item->info=info;
	info->par=item;
	info->fbops=&ili9341fb_fbops;

	// Palette setup
	item->pseudo_palette[0]=0;
	item->pseudo_palette[1]=item->pseudo_palette[7]=item->pseudo_palette[15]=0x0000FFFF;
	info->pseudo_palette=item->pseudo_palette;

	item->irq=platform_get_irq(pdev,0);
	if(item->irq<0)
	{
		ret=-ENOENT;
		goto out_info;
	}

	info->flags=FBINFO_FLAG_DEFAULT;
	info->fix=ili9341fb_fix;
	info->var=ili9341fb_var;
	fb_set_var(info,&ili9341fb_var);
	dev_set_drvdata(&pdev->dev,info);

	ret=ili9341fb_video_alloc(item);
	if (ret)
	{
		ret=-ENOMEM;
		dev_err(&pdev->dev,"%s: unable to ssd1289_video_alloc\n",__func__);
		goto out_info;
	}
	dev_dbg(&pdev->dev, "Initialized video memory at 0x%p-0x%p (%d bytes)\n",info->screen_base,info->screen_base+info->screen_size,(int)info->screen_size);

	// //////////////////////////////////////////////////////////
	// INIT LCD CONTROLLER
	// //////////////////////////////////////////////////////////
	ili9341_display_reset(item);

//	unsigned int tab[16];
//	lcdc_write(item,CMD_READ_DISPLAY_ID_INFO,LCD_LIDD_CS0_ADDR);
//	tab[0]=lcdc_read(item,LCD_LIDD_CS0_DATA);
//	tab[1]=lcdc_read(item,LCD_LIDD_CS0_DATA);
//	tab[2]=lcdc_read(item,LCD_LIDD_CS0_DATA);
//	tab[3]=lcdc_read(item,LCD_LIDD_CS0_DATA);
//	tab[4]=lcdc_read(item,LCD_LIDD_CS0_DATA);
//	tab[5]=0x12345678;
//	dev_warn(&pdev->dev,"%s: tab---> Status:   0x%08X\n", __func__,tab[0]);
//	dev_warn(&pdev->dev,"%s: tab---> Status:   0x%08X\n", __func__,tab[1]);
//	dev_warn(&pdev->dev,"%s: tab---> Status:   0x%08X\n", __func__,tab[2]);
//	dev_warn(&pdev->dev,"%s: tab---> Status:   0x%08X\n", __func__,tab[3]);
//	dev_warn(&pdev->dev,"%s: tab---> Status:   0x%08X\n", __func__,tab[4]);
//	dev_warn(&pdev->dev,"%s: tab---> Status:   0x%08X\n", __func__,tab[5]);
//	lcdc_write(item,CMD_READ_DISPLAY_STATUS,LCD_LIDD_CS0_ADDR);
//	tab[0]=lcdc_read(item,LCD_LIDD_CS0_DATA);
//	tab[1]=lcdc_read(item,LCD_LIDD_CS0_DATA);
//	tab[2]=lcdc_read(item,LCD_LIDD_CS0_DATA);
//	tab[3]=lcdc_read(item,LCD_LIDD_CS0_DATA);
//	tab[4]=lcdc_read(item,LCD_LIDD_CS0_DATA);
//	tab[5]=0x12345678;
//	dev_warn(&pdev->dev,"%s: tab---> Status:   0x%08X\n", __func__,tab[0]);
//	dev_warn(&pdev->dev,"%s: tab---> Status:   0x%08X\n", __func__,tab[1]);
//	dev_warn(&pdev->dev,"%s: tab---> Status:   0x%08X\n", __func__,tab[2]);
//	dev_warn(&pdev->dev,"%s: tab---> Status:   0x%08X\n", __func__,tab[3]);
//	dev_warn(&pdev->dev,"%s: tab---> Status:   0x%08X\n", __func__,tab[4]);
//	dev_warn(&pdev->dev,"%s: tab---> Status:   0x%08X\n", __func__,tab[5]);

	ili9341_display_init(item);
	ili9341_splashScreen(item,COLOR_BLACK);	// Splash Screen
	dev_dbg(&pdev->dev,"Performed setup\n");

	// Set up all kinds of fun DMA
	lcdc_write(item,0,LCD_DMA_CTRL_REG);					// Start out with a blank slate
	lcd_cfg_dma(item,16,0);								// DMA burst and FIFO threshold
	reg_int=LCD_V2_UNDERFLOW_INT_ENA | LCD_SYNC_LOST | LCD_V2_DONE_INT_ENA;
	lcdc_write(item,reg_int,LCD_INT_ENABLE_SET_REG);

	lcdc_write(item,item->dma_start,LCD_DMA_FRM_BUF_BASE_ADDR_0_REG);
	lcdc_write(item,item->dma_end,LCD_DMA_FRM_BUF_CEILING_ADDR_0_REG);
	lcdc_write(item,item->dma_start,LCD_DMA_FRM_BUF_BASE_ADDR_1_REG);
	lcdc_write(item,item->dma_end,LCD_DMA_FRM_BUF_CEILING_ADDR_1_REG);

	info->fbops=&ili9341fb_fbops;
	ret=register_framebuffer(info);
	if(ret<0)
	{
		ret=-EIO;
		dev_err(&pdev->dev,"%s: unable to register_frambuffer\n",__func__);
		goto out_pages;
	}
	dev_dbg(&pdev->dev,"Registered framebuffer.\n");

	//	// Set up LCD coordinates as necessary
	ili9341_set_addr_win(item,0,WIDTH-1,0,HEIGHT-1);
	ili9341_write_cmd(item,CMD_MEMORY_WRITE);	//set up for data before DMA begins

	// Try to get IRQ for DMA
	ret=request_irq(item->irq,ili9341fb_irq_handler,0,DRIVER_NAME,item);
	if(ret)
	{
		ret=-EIO;
		goto out_framebuffer;
	}

	ili9341fb_dma_setstatus(item,1);	//enable DMA


	//	lcdc_write(item,CMD_READ_DISPLAY_ID_INFO,LCD_LIDD_CS0_ADDR);
	//	tab[0]=lcdc_read(item,LCD_LIDD_CS0_DATA);
	//	tab[1]=lcdc_read(item,LCD_LIDD_CS0_DATA);
	//	tab[2]=lcdc_read(item,LCD_LIDD_CS0_DATA);
	//	tab[3]=lcdc_read(item,LCD_LIDD_CS0_DATA);
	//	tab[4]=lcdc_read(item,LCD_LIDD_CS0_DATA);
	//	tab[5]=0x12345678;
	//	dev_warn(&pdev->dev,"%s: tab---> Status:   0x%08X\n", __func__,tab[0]);
	//	dev_warn(&pdev->dev,"%s: tab---> Status:   0x%08X\n", __func__,tab[1]);
	//	dev_warn(&pdev->dev,"%s: tab---> Status:   0x%08X\n", __func__,tab[2]);
	//	dev_warn(&pdev->dev,"%s: tab---> Status:   0x%08X\n", __func__,tab[3]);
	//	dev_warn(&pdev->dev,"%s: tab---> Status:   0x%08X\n", __func__,tab[4]);
	//	dev_warn(&pdev->dev,"%s: tab---> Status:   0x%08X\n", __func__,tab[5]);
	//	lcdc_write(item,CMD_READ_DISPLAY_STATUS,LCD_LIDD_CS0_ADDR);
	//	tab[0]=lcdc_read(item,LCD_LIDD_CS0_DATA);
	//	tab[1]=lcdc_read(item,LCD_LIDD_CS0_DATA);
	//	tab[2]=lcdc_read(item,LCD_LIDD_CS0_DATA);
	//	tab[3]=lcdc_read(item,LCD_LIDD_CS0_DATA);
	//	tab[4]=lcdc_read(item,LCD_LIDD_CS0_DATA);
	//	tab[5]=0x12345678;
	//	dev_warn(&pdev->dev,"%s: tab---> Status:   0x%08X\n", __func__,tab[0]);
	//	dev_warn(&pdev->dev,"%s: tab---> Status:   0x%08X\n", __func__,tab[1]);
	//	dev_warn(&pdev->dev,"%s: tab---> Status:   0x%08X\n", __func__,tab[2]);
	//	dev_warn(&pdev->dev,"%s: tab---> Status:   0x%08X\n", __func__,tab[3]);
	//	dev_warn(&pdev->dev,"%s: tab---> Status:   0x%08X\n", __func__,tab[4]);
	//	dev_warn(&pdev->dev,"%s: tab---> Status:   0x%08X\n", __func__,tab[5]);

	// ili9341 LCD Driver Check
	//	signature=ili9341_reg_get(item,CMD_READ_DISPLAY_ID_INFO);
	//	printk(KERN_EMERG "%s: driver signature=0x%04x\n",__func__,(unsigned short)signature);
	//	if((unsigned short)signature!=0x8989)
	//		dev_warn(&pdev->dev,"%s: unknown driver signature 0x%04x (reg_get failure?)\n", __func__, (unsigned short)signature);

	//	for(i=0;i<10;i++)
	//	{
	//		lcdc_write(item,CMD_READ_DISPLAY_ID_INFO,LCD_LIDD_CS0_ADDR);
	//		d=lcdc_read(item,LCD_LIDD_CS0_DATA);
	//		d<<=8;
	//		d|=lcdc_read(item,LCD_LIDD_CS0_DATA);
	//		d<<=8;
	//		d|=lcdc_read(item,LCD_LIDD_CS0_DATA);
	//		d<<=8;
	//		d|=lcdc_read(item,LCD_LIDD_CS0_DATA);
	//		dev_warn(&pdev->dev,"%s: %d ---> DisplayID:0x%08X\n", __func__,i,(unsigned short)d);

	//		lcdc_write(item,CMD_READ_DISPLAY_STATUS,LCD_LIDD_CS0_ADDR);
	//		d=lcdc_read(item,LCD_LIDD_CS0_DATA);
	//		dev_warn(&pdev->dev,"%s: %d ---> Status:   0x%08X\n", __func__,i,(unsigned short)d);
	//		d=lcdc_read(item,LCD_LIDD_CS0_DATA);
	//		dev_warn(&pdev->dev,"%s: %d ---> Status:   0x%08X\n", __func__,i,(unsigned short)d);
	//		d=lcdc_read(item,LCD_LIDD_CS0_DATA);
	//		dev_warn(&pdev->dev,"%s: %d ---> Status:   0x%08X\n", __func__,i,(unsigned short)d);
	//		d=lcdc_read(item,LCD_LIDD_CS0_DATA);
	//		dev_warn(&pdev->dev,"%s: %d ---> Status:   0x%08X\n", __func__,i,(unsigned short)d);
	//		d=lcdc_read(item,LCD_LIDD_CS0_DATA);
	//		dev_warn(&pdev->dev,"%s: %d ---> Status:   0x%08X\n", __func__,i,(unsigned short)d);

	//		ili9341_write_cmd(item,CMD_READ_DISPLAY_STATUS);
	//		dev_warn(&pdev->dev,"%s: %d ---> Status:   0x%08X\n", __func__,i,lcdc_read(item,LCD_LIDD_CS0_DATA));
	//		dev_warn(&pdev->dev,"%s: %d ---> Status:   0x%08X\n", __func__,i,lcdc_read(item,LCD_LIDD_CS0_DATA));
	//		dev_warn(&pdev->dev,"%s: %d ---> Status:   0x%08X\n", __func__,i,lcdc_read(item,LCD_LIDD_CS0_DATA));
	//		dev_warn(&pdev->dev,"%s: %d ---> Status:   0x%08X\n", __func__,i,lcdc_read(item,LCD_LIDD_CS0_DATA));
	//		dev_warn(&pdev->dev,"%s: %d ---> Status:   0x%08X\n", __func__,i,lcdc_read(item,LCD_LIDD_CS0_DATA));


	//		lcdc_write(item,CMD_READ_DISPLAY_POWER_MODE,LCD_LIDD_CS0_ADDR);
	//		d=lcdc_read(item,LCD_LIDD_CS0_DATA);
	//		dev_warn(&pdev->dev,"%s: %d ---> PowerMode:   0x%08X\n", __func__,i,(unsigned short)d);
	//		d=lcdc_read(item,LCD_LIDD_CS0_DATA);
	//		dev_warn(&pdev->dev,"%s: %d ---> PowerMode:   0x%08X\n", __func__,i,(unsigned short)d);


	//		dev_warn(&pdev->dev,"___________________________________________________\n");
	//	}

	//	lcdc_write(item,CMD_READ_DISPLAY_STATUS,LCD_LIDD_CS0_ADDR);
	//	tab[0]=ioread32(item->lcd_regs+LCD_LIDD_CS0_DATA);
	//	tab[1]=ioread32(item->lcd_regs+LCD_LIDD_CS0_DATA);
	//	tab[2]=ioread32(item->lcd_regs+LCD_LIDD_CS0_DATA);
	//	tab[3]=ioread32(item->lcd_regs+LCD_LIDD_CS0_DATA);
	//	tab[4]=ioread32(item->lcd_regs+LCD_LIDD_CS0_DATA);
	//	tab[5]=ioread32(item->lcd_regs+LCD_PID);
	//	dev_warn(&pdev->dev,"%s: tab---> Status:   0x%08X\n", __func__,tab[0]);
	//	dev_warn(&pdev->dev,"%s: tab---> Status:   0x%08X\n", __func__,tab[1]);
	//	dev_warn(&pdev->dev,"%s: tab---> Status:   0x%08X\n", __func__,tab[2]);
	//	dev_warn(&pdev->dev,"%s: tab---> Status:   0x%08X\n", __func__,tab[3]);
	//	dev_warn(&pdev->dev,"%s: tab---> Status:   0x%08X\n", __func__,tab[4]);
	//	dev_warn(&pdev->dev,"%s: tab---> Status:   0x%08X\n", __func__,tab[5]);

	//	iounmap((void __iomem *)item->lcd_regs);
	//	release_mem_region(item->res.start,item->lcdc_regs_size);
	//	kfree(item);

	return 0;

out_framebuffer:
	//	unregister_framebuffer(info);
out_pages:
	//	ili9341_video_free(item);
out_info:
	//	framebuffer_release(info);
out_clk_enable:
	//	printk(KERN_EMERG "%s ====> END-0 <====\n", __func__);
	//	clk_disable(item->lcdc_clk);
	//	printk(KERN_EMERG "%s ====> END-1 <====\n", __func__);
	//	clk_disable(item->disp_clk);
	//	printk(KERN_EMERG "%s ====> END-2 <====\n", __func__);
err_clk_get:
	//	pm_runtime_put_sync(&dev->dev);
	//	pm_runtime_disable(&dev->dev);
	//	clk_put(item->lcdc_clk);
out_ioremap:
	iounmap((void __iomem *)item->lcd_regs);
	printk(KERN_EMERG "%s ====> END-3 <====\n", __func__);
out_request_mem:
	//	release_mem_region(res_res->start, lcdc_regs_size);
out_item:
	printk(KERN_EMERG "%s ====> END-4 <====\n", __func__);
	kfree(item);
	printk(KERN_EMERG "%s ====> END-5 <====\n", __func__);
out:
	printk(KERN_EMERG "failed to probe/init ILI9341 driver\n");
	dev_err(&pdev->dev,"failed to probe/init ILI9341 driver\n");
	return ret;
}

static int ili9341fb_remove(struct platform_device *pdev)
{
	struct ili9341fb_par* item=(struct ili9341fb_par*)dev_get_drvdata(&pdev->dev);

	printk(KERN_EMERG "%s ====> 0 <====\n",__func__);
	iounmap((void __iomem *)item->lcd_regs);
	release_mem_region(item->res.start,item->lcdc_regs_size);
	kfree(item);
	return 0;
}

//static const struct spi_device_id ili9341fb_device_id[] = {
//	{.name = "Saak,lcd-2.4",.driver_data = LCD_2_4,},
//	{ },
//};

//MODULE_DEVICE_TABLE(of,ili9341fb_device_id);

//static struct platform_device ili9341fb_device=
//{
//	.name          = "ili9341fb",
//	.id            = 0,
//	.num_resources = ARRAY_SIZE(ili9341_resources),
//	.resource      = ili9341_resources,
//};
//static struct platform_device *ili9341fb_device;

static struct platform_driver ili9341fb_driver=
{
	.driver={
		.name="ili9341fb",
		.owner =THIS_MODULE,
		.of_match_table=ili9341fb_dt_ids,
	},
	//	.id_table=ili9341fb_device_id,
	.probe=ili9341fb_probe,
	.remove=ili9341fb_remove,
	//	.suspend = xxxfb_suspend, /* optional but recommended */
	//	.resume = xxxfb_resume,   /* optional but recommended */
};

static int __init ili9341fb_init(void)
{
	//	int ret;

	printk(KERN_EMERG "================================\n");
	printk(KERN_EMERG "ILI9341Init\n");

	//	ret=platform_device_register(&ili9341fb_device);
	//	if(ret)
	//	{
	//		printk(KERN_EMERG "ILI9341Init --> 0\n");
	//		platform_driver_unregister(&ili9341fb_device);
	//		return ret;
	//	}
	return platform_driver_register(&ili9341fb_driver);
}

static void __exit ili9341fb_exit(void)
{
	printk(KERN_EMERG "ILI9341Exit\n");
	//	platform_device_unregister(&ili9341fb_device);
	platform_driver_unregister(&ili9341fb_driver);
	printk(KERN_EMERG "================================\n");
}

/* ------------------------------------------------------------------------- */

module_init(ili9341fb_init);
module_exit(ili9341fb_exit);

MODULE_DESCRIPTION("FB driver for ILI9341 display controller");
MODULE_AUTHOR("Saak Dertadian <s.dertadian@gmail.com>");
MODULE_LICENSE("GPL");
