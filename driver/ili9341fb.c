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

#include "ili9341fb.h"


static struct fb_fix_screeninfo ili9341fb_fix=
{
	.id =			"ILI9341",
	.type =			FB_TYPE_PACKED_PIXELS,
	.visual =		FB_VISUAL_DIRECTCOLOR,
	.xpanstep =		0,
	.ypanstep =		0,
	.ywrapstep =	0,
	.line_length =	WIDTH*BPP/8,
	.accel =		FB_ACCEL_NONE,
};

static struct fb_var_screeninfo ili9341fb_var=
{
	.xres =			WIDTH,
	.yres =			HEIGHT,
	.xres_virtual =	WIDTH,
	.yres_virtual =	HEIGHT,
	.bits_per_pixel=BPP,
	.nonstd	=		0,
};

static int ili9341_write(struct ili9341fb_par *par, u8 data)
{
//	par->buf[0]=data;

//	return spi_write(par->spi,par->buf,1);
	return 0;
}

static void ili9341_write_data(struct ili9341fb_par *par,u8 data)
{
//	int ret = 0;

//	gpio_set_value(par->dc,1);
//	ret=ili9341_write(par,data);
//	if(ret<0)
//		pr_err("%s: write data %02x failed with status %d\n",par->info->fix.id,data,ret);
}

static int ili9341_write_data_buf(struct ili9341fb_par *par,u8 *txbuf, int size)
{
//	printk(KERN_EMERG "==> %02X %02X %02X %02X %02X %02X %02X %02X\n",txbuf[0],txbuf[1],txbuf[2],txbuf[3],txbuf[4],txbuf[5],txbuf[6],txbuf[7]);
//	gpio_set_value(par->dc,1);	// Set data mode
//	return spi_write(par->spi,txbuf,size);	// Write entire buffer
	return 0;
}

static void ili9341_write_cmd(struct ili9341fb_par *par,u8 data)
{
//	int ret;

//	gpio_set_value(par->dc,0);	// Set command mode

//	ret=ili9341_write(par,data);
//	if(ret<0)
//		pr_err("%s: write command %02x failed with status %d\n",par->info->fix.id,data,ret);
	return 0;
}

static void ili9341Cls(struct ili9341fb_par *par,unsigned char fillData)
{
//	char buf[512];
//	int i,j;

//	ili9341_write_cmd(par,CMD_SET_COLUMN_ADDRESS);
//	ili9341_write_data(par,0x1C);

//	ili9341_write_cmd(par,CMD_SET_ROW_ADDRESS);
//	ili9341_write_data(par,0x00);

//	ili9341_write_cmd(par,CMD_WRITE_RAM_CMD);
//	gpio_set_value(par->dc,1);	// Set data mode

//	for(i=0;i<512;i++)
//		buf[i]=fillData;;
//	for(j=0;j<16;j++)
//		spi_write(par->spi,buf,512);
}

static void ili9341_set_addr_win(struct ili9341fb_par *par,int xs,int ys,int xe,int ye)
{
////	printk(KERN_EMERG "----> ili9341_set_addr_win <----%d-%d <> %d-%d, offX:%d - offY:%d\n",xs,xe,ys,ye,par->xoff,par->yoff);
//	ili9341_write_cmd(par,CMD_SET_COLUMN_ADDRESS);
//	ili9341_write_data(par,0x1C+(xs+par->xoff)/BPP);
//	ili9341_write_data(par,0x1C+(xe+par->xoff)/BPP);
////	ili9341_write_data(par,0x1C);
////	ili9341_write_data(par,0x5B);

//	ili9341_write_cmd(par,CMD_SET_ROW_ADDRESS);
//	ili9341_write_data(par,ys+par->yoff);
//	ili9341_write_data(par,ye+par->yoff);
////	ili9341_write_data(par,0x00);
////	ili9341_write_data(par,0x3F);
}

static void ili9341_reset(struct ili9341fb_par *par)
{
//	/* Reset controller */
//	gpio_set_value(par->rst,0);
//	udelay(100);
//	gpio_set_value(par->rst,1);
//	mdelay(200);
}

static void ili9341fb_update_display(struct ili9341fb_par *par)
{
//	int ret=0;
//	u16 *vmem;
//	int i;

////	printk(KERN_EMERG "----> ili9341fb_update_display <----\n");
//	u16 *vmem16=(u16 *)par->info->screen_base;
//	vmem=par->ssbuf;
//	for (i=0;i<WIDTH*HEIGHT*BPP/8/2;i++)
//	{
////		vmem[i]=(vmem16[i]&0xFFFF);

//		vmem[i]=0x0000;

//		if(vmem16[i]&0x0F00)	vmem[i]|=(vmem16[i]&0x0F00)<<5;
//		if(vmem16[i]&0xF000)	vmem[i]|=(vmem16[i]&0xF000)>>3;
//		if(vmem16[i]&0x000F)	vmem[i]|=(vmem16[i]&0x000F)<<5;
//		if(vmem16[i]&0x00F0)	vmem[i]|=(vmem16[i]&0x00F0)>>3;
//	}

//	mutex_lock(&(par->io_lock));

//	/* Set row/column data window */
//	ili9341_set_addr_win(par,0,0,WIDTH-1,HEIGHT-1);

//	/* Internal RAM write command */
//	ili9341_write_cmd(par,CMD_WRITE_RAM_CMD);

//	/* Blast framebuffer to ILI9341 internal display RAM */
//	ret=ili9341_write_data_buf(par,(u8 *)vmem,WIDTH*HEIGHT*BPP/8);
//	if(ret<0)
//		pr_err("%s: spi_write failed to update display buffer\n",par->info->fix.id);

//	mutex_unlock(&(par->io_lock));
}

static int ili9341fb_init_display(struct ili9341fb_par *par)
{
//	int i;
	int ret=0;

//	/* Request GPIOs and initialize to default values */
//	ret=gpio_request_one(par->rst,GPIOF_OUT_INIT_HIGH,"ILI9341 Reset Pin");
//	if(ret<0)
//	{
//		pr_err("%s: failed to claim reset pin\n",par->info->fix.id);
//		goto out;
//	}
//	ret=gpio_request_one(par->dc, GPIOF_OUT_INIT_LOW,"ILI9341 Data/Command Pin");
//	if(ret<0)
//	{
//		pr_err("%s: failed to claim data/command pin\n",par->info->fix.id);
//		goto out;
//	}
//	ili9341_reset(par);
//	ili9341_run_cfg_script(par);

//	// Splash Screen
//	for(i=0x00;i<0x100;i+=0x11)
//	{
//		ili9341Cls(par,i);
//		mdelay(10);
//	}
//	for(i=0xFF;i>=0;i-=0x11)
//	{
//		ili9341Cls(par,i);
//		mdelay(10);
//	}
//out:
	return ret;
}

static void ili9341fb_deferred_io(struct fb_info *info,struct list_head *pagelist)
{
//	printk(KERN_EMERG "----> ili9341fb_deferred_io <----\n");
	ili9341fb_update_display(info->par);
}

static void ili9341fb_update_display_deferred(struct fb_info *info)
{
	struct fb_deferred_io *fbdefio=info->fbdefio;
//	printk(KERN_EMERG "----> ili9341fb_update_display_deferred <----\n");
	schedule_delayed_work(&info->deferred_work,fbdefio->delay);
}

static void ili9341fb_fillrect(struct fb_info *info,const struct fb_fillrect *rect)
{
//	printk(KERN_INFO "----> ili9341fb_fillrect <----\n");
	sys_fillrect(info,rect);
	ili9341fb_update_display_deferred(info);
}

static void ili9341fb_copyarea(struct fb_info *info,const struct fb_copyarea *area)
{
//	printk(KERN_INFO "----> ili9341fb_copyarea <----\n");
	sys_copyarea(info, area);
	ili9341fb_update_display_deferred(info);
}

static void ili9341fb_imageblit(struct fb_info *info,const struct fb_image *image)
{
//	printk(KERN_INFO "----> ili9341fb_imageblit <----\n");
	sys_imageblit(info,image);
	ili9341fb_update_display_deferred(info);
}

static ssize_t ili9341fb_write(struct fb_info *info,const char __user *buf,size_t count,loff_t *ppos)
{
	unsigned long p=*ppos;
	void *dst;
	int err=0;
	unsigned long total_size;

//	printk(KERN_EMERG "----> ili9341fb_write <---- %d %d %d %d\n",count,buf[0],buf[1],buf[2]);
	if(info->state!=FBINFO_STATE_RUNNING)
		return -EPERM;

	total_size=info->fix.smem_len;
	if(p>total_size)
		return -EFBIG;

	if(count>total_size)
	{
		err=-EFBIG;
		count=total_size;
	}

	if(count+p>total_size)
	{
		if (!err)
			err=-ENOSPC;
		count=total_size-p;
	}

	dst=(void __force *)(info->screen_base+p);

	if(copy_from_user(dst,buf,count))
		err=-EFAULT;
	if(!err)
		*ppos+=count;
	ili9341fb_update_display_deferred(info);
	return (err)?err:count;
}

static int ili9341fb_setcolreg(unsigned regno,unsigned red,unsigned green,
							   unsigned blue,unsigned transp,
							   struct fb_info *info)
{
//	if(regno>=MAX_PALETTE)
//		return -EINVAL;

////	((u32*)(info->pseudo_palette))[regno]=(MAX_PALETTE-1)-regno;	// Negative
//	((u32*)(info->pseudo_palette))[regno]=regno;
////	printk(KERN_EMERG "----> ili9341fb_setcolreg <----%d - R:%04X, G:%04X, B:%04X, B:%04X X:%04X\n",regno,red,green,blue,c,((u16*)(info->pseudo_palette))[regno]);
	return 0;
}

static struct fb_ops ili9341fb_ops=
{
	.owner			= THIS_MODULE,
	.fb_read		= fb_sys_read,
	.fb_write		= ili9341fb_write,
	.fb_fillrect	= ili9341fb_fillrect,
	.fb_copyarea	= ili9341fb_copyarea,
	.fb_imageblit	= ili9341fb_imageblit,
	.fb_setcolreg	= ili9341fb_setcolreg,
};

static struct fb_deferred_io ili9341fb_defio=
{
	.delay			= HZ/20,
	.deferred_io	= ili9341fb_deferred_io,
};

static int ili9341fb_probe(struct spi_device *spi)
{
	int vmem_size=WIDTH*HEIGHT*BPP/8;
	u8 *vmem;
	struct fb_info *info;
	struct ili9341fb_par *par;
	int retval=-ENOMEM;
	struct device_node *np=spi->dev.of_node;
	struct pinctrl *pinctrl;

	printk(KERN_EMERG "ili9341fb_probe: rst=%d dc=%d",gpio_rst,gpio_dc);

//	pinctrl=devm_pinctrl_get_select_default(&spi->dev);
//	if(IS_ERR(pinctrl))
//		dev_warn(&spi->dev,"pins are not configured from the driver\n");

//#ifdef __LITTLE_ENDIAN
//	vmem=(u8 *)vmalloc(vmem_size);
//#else
//	vmem=(u8 *)kmalloc(vmem_size,GFP_KERNEL);
//#endif
//	if(!vmem)
//		return retval;

//	info=framebuffer_alloc(sizeof(struct ili9341fb_par),&spi->dev);
//	if(!info)
//		goto fballoc_fail;

//	info->pseudo_palette=kmalloc(sizeof(u32)*MAX_PALETTE,GFP_KERNEL);
//	if(!info->pseudo_palette)
//		goto palette_fail;

//	info->screen_base=(u8 __force __iomem *)vmem;
//	info->fbops=&ili9341fb_ops;
//	info->fix=ili9341fb_fix;
//	info->fix.smem_len=vmem_size;
//	info->var=ili9341fb_var;
//	/* Choose any packed pixel format as long as it's RGB565 */
////	info->var.red.offset = 0;
////	info->var.red.length = 4;
////	info->var.green.offset = 4;
////	info->var.green.length = 4;
////	info->var.blue.offset = 8;
////	info->var.blue.length = 4;
////	info->var.transp.offset = 12;
////	info->var.transp.length = 4;
//	info->flags=FBINFO_FLAG_DEFAULT | FBINFO_VIRTFB;

//	info->fbdefio = &ili9341fb_defio;
//	fb_deferred_io_init(info);

//	retval=fb_alloc_cmap(&info->cmap, MAX_PALETTE,0);
//	if(retval < 0)
//		goto cmap_fail;
//	info->cmap.len=MAX_PALETTE;

//	par=info->par;
//	par->info=info;
//	par->spi=spi;

//	mutex_init(&par->io_lock);

//	par->xoff = 0;
//	par->yoff = 0;

//	/* TODO: fix all exit paths for cleanup */
//	par->rst=of_get_named_gpio(np, "oled-rs", 0);
//	if(par->rst < 0)
//	{
//		printk(KERN_EMERG "failed to find ILI9341-rst node!\n");
//		return -EINVAL;
//	}
//	gpio_rst=par->rst;

//	par->dc=of_get_named_gpio(np, "oled-dc", 0);
//	if(par->dc<0)
//	{
//		printk(KERN_EMERG "failed to find ILI9341-dc node!\n");
//		return -EINVAL;
//	}
//	gpio_dc=par->dc;
//	printk(KERN_EMERG "DC=%d, RS=%d\n",gpio_dc,gpio_rst);

////	gpio_rst=60;
////	gpio_dc=49;

//	par->buf=kmalloc(1,GFP_KERNEL);
//	if(!par->buf)
//	{
//		retval=-ENOMEM;
//		goto buf_fail;
//	}

//	/* Allocated swapped shadow buffer */
//	par->ssbuf=kmalloc(vmem_size, GFP_KERNEL);
//	if(!par->ssbuf)
//	{
//		retval=-ENOMEM;
//		goto ssbuf_fail;
//	}

//	retval=ili9341fb_init_display(par);
//	if(retval<0)
//		goto init_fail;

//	retval=register_framebuffer(info);
//	if(retval<0)
//		goto fbreg_fail;

//	spi_set_drvdata(spi,info);

//	printk(KERN_INFO "fb%d: %s frame buffer device,\n\tusing %d KiB of video memory\n",info->node,info->fix.id,vmem_size);

//	return 0;

//	spi_set_drvdata(spi,NULL);

//fbreg_fail:
//	/* TODO: release gpios on fail */
//	/* TODO: and unwind everything in init */

//init_fail:
//	kfree(par->ssbuf);

//ssbuf_fail:
//	kfree(par->buf);

//buf_fail:
//	fb_dealloc_cmap(&info->cmap);

//cmap_fail:
//	kfree(info->pseudo_palette);

//palette_fail:
//	framebuffer_release(info);

//fballoc_fail:
//	vfree(vmem);
////	kfree(vmem);

	return retval;
}

static int ili9341fb_remove(struct spi_device *spi)
{
	struct fb_info *info=spi_get_drvdata(spi);

	spi_set_drvdata(spi,NULL);

	if(info)
	{
		unregister_framebuffer(info);
		fb_dealloc_cmap(&info->cmap);
		kfree(info->pseudo_palette);
		vfree(info->screen_base);
		framebuffer_release(info);
	}

	/* TODO: release gpios */
	printk(KERN_EMERG "ili9341fb_remove: rst=%d dc=%d",gpio_rst,gpio_dc);
//	gpio_free(gpio_rst);
//	gpio_free(gpio_dc);

	return 0;
}

static const struct spi_device_id ili9341fb_device_id[] = {
	{.name = "saak,lcd-2.r",.driver_data = LCD_2_4,},
	{ },
};
MODULE_DEVICE_TABLE(spi,ili9341fb_device_id);

static const struct of_device_id ili9341fb_dt_ids[] = {
	{ .compatible = "saak,lcd-2.4", .data = (void *) LCD_2_4, },
	{ },
};
MODULE_DEVICE_TABLE(of,ili9341fb_dt_ids);

static struct spi_driver ili9341fb_driver=
{
	.driver=
	{
		.name="ili9341",
		.owner =THIS_MODULE,
		.of_match_table=ili9341fb_dt_ids,
	},
	.id_table=ili9341fb_device_id,
	.probe=ili9341fb_probe,
	.remove=ili9341fb_remove,
};

static int __init ili9341fb_init(void)
{
	printk(KERN_EMERG "================================\n");
	printk(KERN_INFO "ILI9341Init\n");
	return 1;
}

static void __exit ili9341fb_exit(void)
{
	printk(KERN_INFO "ILI9341Exit\n");
	printk(KERN_EMERG "================================\n");
}

/* ------------------------------------------------------------------------- */

module_init(ili9341fb_init);
module_exit(ili9341fb_exit);

MODULE_DESCRIPTION("FB driver for ILI9341 display controller");
MODULE_AUTHOR("Saak Dertadian <s.dertadian@gmail.com>");
MODULE_LICENSE("GPL");
