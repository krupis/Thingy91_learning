/*
 * Copyright (c) 2019 Manivannan Sadhasivam
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/device.h>
#include <zephyr/kernel.h>

#ifdef CONFIG_USB_DEVICE_STACK
#include <zephyr/usb/usb_device.h>
#include <zephyr/usb/usbd.h>
#include <zephyr/drivers/uart.h>
#endif


#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(main);

int main(void)
{
	if (usb_enable(NULL)) {
		return 0;
	}
	
	while(1){
		printk("Hello World from nRF58240 printf \n");
		LOG_INF("Hello World from nRF58240 LOG INF \n ");
		k_sleep(K_SECONDS(1));
	}

}
