/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>
/* STEP 6 - Include the header file of printk */
#include <zephyr/sys/printk.h>



void main(void)
{	
	while (1) {
        k_msleep(1000);
		printk("Hello world\n");
	}
}