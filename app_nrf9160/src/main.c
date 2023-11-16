#include <stdio.h>
#include <stdlib.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/spi.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>



#define LOG_LEVEL CONFIG_LOG_DEFAULT_LEVEL
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(main);


#define SPI_MESSAGE 0x48


#define DEFAULT_ADXL362_NODE DT_ALIAS(adxl362)
BUILD_ASSERT(DT_NODE_HAS_STATUS(DEFAULT_ADXL362_NODE, okay),
			 "ADXL362 not specified in DT");

//DEVICE TREE STRUCTURE
const struct device *const adxl1362_sens = DEVICE_DT_GET(DEFAULT_ADXL362_NODE);

//TEST1
//CHIP SELECT CONTROL
struct spi_cs_control ctrl = SPI_CS_CONTROL_INIT(DT_NODELABEL(adxl362), 0);\

//SPI CONFIG
static const struct spi_config spi_cfg = {
	.operation = SPI_WORD_SET(8) | SPI_TRANSFER_MSB,
	.frequency = 4000000, // 8 mhz
	.slave = 0,
    .cs = &ctrl,
};

//TEST2
//CHIP SELECT CONTROL2
struct spi_cs_control ctrl2 = {
        .gpio = SPI_CS_GPIOS_DT_SPEC_GET(DT_NODELABEL(spidev)),
        .delay = 2,
};
//SPI CONFIG2
static const struct spi_config spi_cfg2 = {
	.operation = SPI_WORD_SET(8) | SPI_TRANSFER_MSB,
	.frequency = 4000000, // 8 mhz
	.slave = 0,
    .cs = &ctrl2,
};






int main(void)
{
	while(1){
		printk("hello world \n");
		k_sleep(K_MSEC(1000));
	}


	// if (!device_is_ready(adxl1362_sens))
	// {
	// 	printk("sensor: device %s not ready.\n", adxl1362_sens->name);
	// 	return 0;
	// }
	// else
	// {
	// 	printk("sensor: device %s ready.\n", adxl1362_sens->name);
	// }

	// int ret;
	// int err;
	// uint8_t cmd = SPI_MESSAGE;
	// struct spi_buf tx_buf = {.buf = &cmd, .len = 1};
	// struct spi_buf_set tx_bufs = {.buffers = &tx_buf, .count = 1};
	// while (1) {
	// 	LOG_INF("SPI writing test data \n");
	// 	err = spi_write(adxl1362_sens, &spi_cfg, &tx_bufs);
	// 	if (err) {
	// 		LOG_ERR("SPI write failed with error %d\n", err);
	// 		return err;
	// 	}
	// 	k_sleep(K_MSEC(1000));
	// }
	
	return 0;
}


