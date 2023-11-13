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
	static const struct spi_dt_spec spi_dev = SPI_DT_SPEC_GET(SPI3_NODE, SPI_OP_MODE_MASTER, 0);
	return 0;
}


