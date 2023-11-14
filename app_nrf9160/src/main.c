#include <stdio.h>
#include <stdlib.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/spi.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/device.h>
#include <ei_wrapper.h>
#include "accelerometer_low.h"


struct spi_cs_control spi_cs = {
    /* PA4 as CS pin */
    .gpio_dev = DEVICE_DT_GET(DT_NODELABEL(gpioa)),
    .gpio_pin = 4,
    .gpio_dt_flags = GPIO_ACTIVE_LOW,
    /* delay in microseconds to wait before starting the transmission and before releasing the CS line */
    .delay = 10,
};


#define SPI_CS (&spi_cs)

struct spi_config spi_cfg = {
    .frequency = 350000,
    .operation = SPI_OP_MODE_MASTER | SPI_TRANSFER_MSB | SPI_WORD_SET(8) | SPI_LINES_SINGLE | SPI_LOCK_ON,
    .cs = SPI_CS,
};



int main(void)
{
	return 0;
}


