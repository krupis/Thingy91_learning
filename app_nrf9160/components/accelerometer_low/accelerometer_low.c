#include "accelerometer_low.h"





#define DEFAULT_ADXL362_NODE DT_ALIAS(adxl362)
BUILD_ASSERT(DT_NODE_HAS_STATUS(DEFAULT_ADXL362_NODE, okay),
			 "ADXL362 not specified in DT");


#define LOG_LEVEL CONFIG_LOG_DEFAULT_LEVEL
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(accelerometer_low);



//DEVICE TREE STRUCTURE
const struct device *const adxl1362_sens = DEVICE_DT_GET(DEFAULT_ADXL362_NODE);

//CHIP SELECT CONTROL
struct spi_cs_control ctrl =
        SPI_CS_CONTROL_INIT(DT_NODELABEL(adxl362), 2);

//SPI CONFIG
static const struct spi_config spi_cfg = {
	.operation = SPI_WORD_SET(8) | SPI_TRANSFER_MSB,
	.frequency = 8000000,
	.slave = 0,
    .cs = &ctrl,
};



int adxl362_bus_check_spi()
{
    int ret;
	if (!device_is_ready(adxl1362_sens))
	{
		printk("sensor: device %s not ready.\n", adxl1362_sens->name);
		return 0;
	}
}


int adxl362_reg_read_spi(uint8_t start, uint8_t *buf, int size)
{
	uint8_t addr;
	const struct spi_buf tx_buf = {
		.buf = &addr,
		.len = 1
	};
	const struct spi_buf_set tx = {
		.buffers = &tx_buf,
		.count = 1
	};
	struct spi_buf rx_buf[2];
	const struct spi_buf_set rx = {
		.buffers = rx_buf,
		.count = ARRAY_SIZE(rx_buf)
	};
	int i;

	rx_buf[0].buf = NULL;
	rx_buf[0].len = 1;
	rx_buf[1].len = 1;

	for (i = 0; i < size; i++) {
		int ret;

		addr = (start + i) | 0x80;
		rx_buf[1].buf = &buf[i];

		ret = spi_transceive(adxl1362_sens, &spi_cfg, &tx, &rx);
		if (ret) {
			LOG_DBG("spi_transceive FAIL %d\n", ret);
			return ret;
		}
	}

	return 0;
}


int adxl362_reg_write_spi(uint8_t reg, uint8_t val)
{
	uint8_t cmd[] = { reg & 0x7F, val };
	const struct spi_buf tx_buf = {
		.buf = cmd,
		.len = sizeof(cmd)
	};
	const struct spi_buf_set tx = {
		.buffers = &tx_buf,
		.count = 1
	};
	int ret;

	ret = spi_write(adxl1362_sens, &spi_cfg, &tx);
	if (ret) {
		LOG_DBG("spi_write FAIL %d\n", ret);
		return ret;
	}	
	return 0;

}




