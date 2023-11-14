#include "accelerometer_low.h"





#define DEFAULT_ADXL362_NODE DT_ALIAS(adxl362)
BUILD_ASSERT(DT_NODE_HAS_STATUS(DEFAULT_ADXL362_NODE, okay),
			 "ADXL362 not specified in DT");


#define LOG_LEVEL CONFIG_LOG_DEFAULT_LEVEL
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(accelerometer_low);



#define SPI_MESSAGE 0xA5


//DEVICE TREE STRUCTURE
const struct device *const adxl1362_sens = DEVICE_DT_GET(DEFAULT_ADXL362_NODE);

//CHIP SELECT CONTROL
struct spi_cs_control ctrl = SPI_CS_CONTROL_INIT(DT_NODELABEL(adxl362), 2);

//SPI CONFIG
static const struct spi_config spi_cfg = {
	.operation = SPI_WORD_SET(8) | SPI_TRANSFER_MSB,
	.frequency = 4000000, // 8 mhz
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


int adxl362_reg_read_spi(uint8_t address, uint8_t *buf, int size)
{
	uint8_t tx_send_buf[2] = { ADXL362_READ_REG, address };
	const struct spi_buf tx_buf = {
		.buf = &tx_send_buf,
		.len = 2
	};

	const struct spi_buf_set tx = {
		.buffers = &tx_buf,
		.count = 1
	};
	
	const struct spi_buf rx_buf = {
		.buf = buf,
		.len = size,
	};
	const struct spi_buf_set rx = {
		.buffers = &rx_buf,
		.count = 1
	};

	int ret = spi_transceive(adxl1362_sens, &spi_cfg, &tx, &rx);
	if (ret) {
		LOG_DBG("spi_transceive FAIL %d\n", ret);
		return ret;
	}
	return 0;
}


int adxl362_write_test(){
	uint8_t cmd = SPI_MESSAGE;
    struct spi_buf tx_buf = {.buf = &cmd, .len = 1};
    struct spi_buf_set tx_bufs = {.buffers = &tx_buf, .count = 1};


    while (1) {
        spi_write(adxl1362_sens, &spi_cfg, &tx_bufs);
        k_sleep(K_MSEC(1000));
    }
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




