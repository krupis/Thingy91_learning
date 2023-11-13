#include "accelerometer_low.h"



static const struct device *spi3_dev = DEVICE_DT_GET(SPI3_NODE);


struct spi_cs_control ctrl =
        SPI_CS_CONTROL_INIT(DT_NODELABEL(adxl362), 2);

static const struct spi_config spi_cfg = {
	.operation = SPI_WORD_SET(8) | SPI_TRANSFER_MSB,
	.frequency = 8000000,
	.slave = 0,
    .cs = &ctrl,
};






