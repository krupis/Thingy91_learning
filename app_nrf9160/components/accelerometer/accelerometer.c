#include "accelerometer.h"

static const enum sensor_channel channels[] = {
	SENSOR_CHAN_ACCEL_X,
	SENSOR_CHAN_ACCEL_Y,
	SENSOR_CHAN_ACCEL_Z,
};

#define DEFAULT_ADXL372_NODE DT_ALIAS(adxl372)
BUILD_ASSERT(DT_NODE_HAS_STATUS(DEFAULT_ADXL372_NODE, okay),
			 "ADXL372 not specified in DT");

#define DEFAULT_ADXL362_NODE DT_ALIAS(adxl362)
BUILD_ASSERT(DT_NODE_HAS_STATUS(DEFAULT_ADXL362_NODE, okay),
			 "ADXL362 not specified in DT");

const struct device *const adxl1372_dev = DEVICE_DT_GET(DEFAULT_ADXL372_NODE);
const struct device *const adxl1362_dev = DEVICE_DT_GET(DEFAULT_ADXL362_NODE);

int Initialize_accelerometers()
{
	int ret;
	if (!device_is_ready(adxl1372_dev))
	{
		printk("sensor: device %s not ready.\n", adxl1372_dev->name);
		return 0;
	}
	else
	{
		printk("sensor: device %s ready.\n", adxl1372_dev->name);
	}

	if (!device_is_ready(adxl1362_dev))
	{
		printk("sensor: device %s not ready.\n", adxl1362_dev->name);
		return 0;
	}
	else
	{
		printk("sensor: device %s ready.\n", adxl1362_dev->name);
	}
}

int print_adxl362_data()
{
	int ret;
	struct sensor_value accel[3];
	ret = sensor_sample_fetch(adxl1362_dev);
	if (ret < 0)
	{
		printk("%s: sensor_sample_fetch() failed: %d\n", adxl1362_dev->name, ret);
		return ret;
	}

	for (size_t i = 0; i < ARRAY_SIZE(channels); i++)
	{
		ret = sensor_channel_get(adxl1362_dev, channels[i], &accel[i]);
		if (ret < 0)
		{
			printk("%s: sensor_channel_get(%c) failed: %d\n", adxl1362_dev->name, 'X' + i, ret);
			return ret;
		}
	}

	printk("%16s [m/s^2]:    (%12.6f, %12.6f, %12.6f)\n", adxl1362_dev->name,
		   sensor_value_to_double(&accel[0]), sensor_value_to_double(&accel[1]), sensor_value_to_double(&accel[2]));

	return 0;
}
