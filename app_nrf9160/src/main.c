#include <stdio.h>
#include <stdlib.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/spi.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/device.h>
#include <ei_wrapper.h>
#include "accelerometer_low.h"

int main(void)
{

	int ret;
	ret = Initialize_machine_learning();

	int ret;
	ret = Initialize_accelerometers();
	if (ret == 0)
	{
		printk("Accelerometers not initialized\n");
	}
	else
	{
		printk("Accelerometers initialized sucesfully\n");
	}

	ret = print_adxl362_data();
	size_t cnt = 0;
	return 0;
}
