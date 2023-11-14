#include <stdio.h>
#include <stdlib.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/spi.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/device.h>
#include "accelerometer_low.h"



#define LOG_LEVEL CONFIG_LOG_DEFAULT_LEVEL
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(main);


int main(void)
{
	// adxl362_bus_check_spi();
	// LOG_INF("hello world \n");
	// k_msleep(1000);

	// LOG_INF("Sending command 1 \n");
	// uint8_t  receive_buffer;
	// adxl362_reg_read_spi(ADXL362_REG_DEVID_AD, &receive_buffer, 1);
	// LOG_INF("Device ID: %.2X\n", receive_buffer);
	// k_msleep(1000);

	// LOG_INF("Sending command 2 \n");
	// uint8_t receive_buffer2;
	// adxl362_reg_read_spi(ADXL362_REG_DEVID_MST, &receive_buffer2, 1);
	// LOG_INF("DEVID MST ID: %.2X\n", receive_buffer2);
	// k_msleep(1000);

	// LOG_INF("Sending command 3 \n");
	// uint8_t receive_buffer3;
	// adxl362_reg_read_spi(ADXL362_REG_PARTID, &receive_buffer3, 1);
	// LOG_INF("PART ID: %.2X\n", receive_buffer3);


	while(1){
		adxl362_write_test();
		k_msleep(1000);
	}



	return 0;




}


