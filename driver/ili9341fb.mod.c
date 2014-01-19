#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x17634745, "module_layout" },
	{ 0x61036ea2, "platform_driver_unregister" },
	{ 0x2e5810c6, "__aeabi_unwind_cpp_pr1" },
	{ 0x737474f9, "platform_driver_register" },
	{ 0xf53d8915, "arm_delay_ops" },
	{ 0x86c8ad2e, "malloc_sizes" },
	{ 0x432fd7f6, "__gpio_set_value" },
	{ 0x403f9529, "gpio_request_one" },
	{ 0x3757c9b, "of_get_named_gpio_flags" },
	{ 0xe9525093, "dev_warn" },
	{ 0x1eac9fb, "pinctrl_select_state" },
	{ 0x3f91b9ef, "pinctrl_lookup_state" },
	{ 0x13371c7b, "devm_pinctrl_get" },
	{ 0xe914e41e, "strcpy" },
	{ 0x107dbeac, "__pm_runtime_resume" },
	{ 0x623106b8, "pm_runtime_enable" },
	{ 0xadaa2657, "cpufreq_register_notifier" },
	{ 0xa710e381, "clk_enable" },
	{ 0x3a247c7e, "clk_get" },
	{ 0x40a6f522, "__arm_ioremap" },
	{ 0xadf42bd5, "__request_region" },
	{ 0xe5524eab, "of_address_to_resource" },
	{ 0x365fab92, "of_match_device" },
	{ 0x8ffa9e07, "dev_set_drvdata" },
	{ 0xbda0261f, "dev_err" },
	{ 0x8758668a, "kmem_cache_alloc_trace" },
	{ 0x3c5d3680, "clk_get_rate" },
	{ 0xefd6cf06, "__aeabi_unwind_cpp_pr0" },
	{ 0x788fe103, "iomem_resource" },
	{ 0x37a0cba, "kfree" },
	{ 0x9bce482f, "__release_region" },
	{ 0xc2165d85, "__arm_iounmap" },
	{ 0x3382adf, "dev_get_drvdata" },
	{ 0x27e1a049, "printk" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

MODULE_ALIAS("of:N*T*CSaak,lcd-2.4*");

MODULE_INFO(srcversion, "B4611466F91BA9D80708AEE");
