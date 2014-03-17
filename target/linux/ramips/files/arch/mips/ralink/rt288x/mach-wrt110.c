/*
 *  Linksys WRT110 support
 *
 *  Copyright (C) 2014 Claudio Leite <leitec@staticky.com>
 *
 *  Based on V11ST-FE support code:
 *
 *  Copyright (C) 2012 Florian Fainelli <florian@openwrt.org>
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License version 2 as published
 *  by the Free Software Foundation.
 */

#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/ethtool.h>

#include <asm/mach-ralink/machine.h>
#include <asm/mach-ralink/dev-gpio-buttons.h>
#include <asm/mach-ralink/dev-gpio-leds.h>
#include <asm/mach-ralink/rt288x.h>
#include <asm/mach-ralink/rt288x_regs.h>
#include <asm/mach-ralink/ramips_eth_platform.h>

#include "devices.h"

/*
 * I can't figure out how to turn these on.
 * They come on fine in the factory firmware
 * so there may be another GPIO or something
 * else ANDed with the LED pins here?
 */
#define WRT110_GPIO_WPS_RED_LED         8
#define WRT110_GPIO_WPS_BLUE_LED        13

#define WRT110_GPIO_POWER_LED           12

#define WRT110_GPIO_BUTTON_WPS	        0
#define WRT110_GPIO_BUTTON_RESET        9

#define WRT110_KEYS_POLL_INTERVAL	20
#define WRT110_KEYS_DEBOUNCE_INTERVAL	(3 * WRT110_KEYS_POLL_INTERVAL)

static struct gpio_led wrt110_leds_gpio[] __initdata = {
        {
            .name           = "wrt110:blue:power",
            .gpio           = WRT110_GPIO_POWER_LED,
            .active_low     = 0,
        },
};

static struct gpio_keys_button wrt110_gpio_buttons[] __initdata = {
	{
		.desc		= "wps",
		.type		= EV_KEY,
		.code		= KEY_WPS_BUTTON,
		.debounce_interval = WRT110_KEYS_DEBOUNCE_INTERVAL,
		.gpio		= WRT110_GPIO_BUTTON_WPS,
                .active_low     = 1,
	},
	{
		.desc		= "reset",
		.type		= EV_KEY,
		.code		= KEY_RESTART,
		.debounce_interval = WRT110_KEYS_DEBOUNCE_INTERVAL,
		.gpio		= WRT110_GPIO_BUTTON_RESET,
		.active_low	= 1,
	} 
};

static void __init rt_wrt110_init(void)
{
	rt288x_gpio_init(RT2880_GPIO_MODE_UART0);

	rt288x_register_flash(0);

	ramips_register_gpio_leds(-1, ARRAY_SIZE(wrt110_leds_gpio),
				  wrt110_leds_gpio);

	ramips_register_gpio_buttons(-1, WRT110_KEYS_POLL_INTERVAL,
				     ARRAY_SIZE(wrt110_gpio_buttons),
				     wrt110_gpio_buttons);

	rt288x_register_wifi();

	/* Board is connected to an IC+ IP175C Fast Ethernet switch */
	rt288x_eth_data.speed = SPEED_100;
	rt288x_eth_data.duplex = DUPLEX_FULL;
	rt288x_eth_data.tx_fc = 1;
	rt288x_eth_data.rx_fc = 1;
	rt288x_eth_data.phy_mask = BIT(0);
	rt288x_register_ethernet();

	rt288x_register_wdt();
}

MIPS_MACHINE(RAMIPS_MACH_WRT110, "WRT110", "Linksys WRT110", rt_wrt110_init);
