#include <stdio.h>
#include <stdlib.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/spi.h>
#include <zephyr/device.h>

K_SEM_DEFINE(sem, 0, 1);


#define DEFAULT_ADXL362_NODE DT_ALIAS(adxl362)
BUILD_ASSERT(DT_NODE_HAS_STATUS(DEFAULT_ADXL362_NODE, okay),
			 "ADXL362 not specified in DT");

// DEVICE TREE STRUCTURE
const struct device *adxl1362_sens = DEVICE_DT_GET(DEFAULT_ADXL362_NODE);


static void trigger_handler(const struct device *dev,
			    const struct sensor_trigger *trig)
{
	switch (trig->type) {
	case SENSOR_TRIG_DATA_READY:
		if (sensor_sample_fetch(dev) < 0) {
			printf("Sample fetch error\n");
			return;
		}
		k_sem_give(&sem);
		break;
	case SENSOR_TRIG_THRESHOLD:
		printf("Threshold trigger\n");
		break;
	default:
		printf("Unknown trigger\n");
	}
}

void main(void)
{
	struct sensor_value accel[3];

	const struct device *dev = device_get_binding(DT_LABEL(DT_INST(0, adi_adxl362)));
	if (dev == NULL) {
		printf("Device get binding device\n");
		return;
	}

	if (IS_ENABLED(CONFIG_ADXL362_TRIGGER)) {
		struct sensor_trigger trig = { .chan = SENSOR_CHAN_ACCEL_XYZ };

		trig.type = SENSOR_TRIG_THRESHOLD;
		if (sensor_trigger_set(dev, &trig, trigger_handler)) {
			printf("Trigger set error\n");
			return;
		}

		trig.type = SENSOR_TRIG_DATA_READY;
		if (sensor_trigger_set(dev, &trig, trigger_handler)) {
			printf("Trigger set error\n");
		}
	}

	while (true) {
		if (IS_ENABLED(CONFIG_ADXL362_TRIGGER)) {
			k_sem_take(&sem, K_FOREVER);
		} else {
			k_sleep(K_MSEC(1000));
			if (sensor_sample_fetch(dev) < 0) {
				printf("Sample fetch error\n");
				return;
			}
		}

		if (sensor_channel_get(dev, SENSOR_CHAN_ACCEL_X, &accel[0]) < 0) {
			printf("Channel get error\n");
			return;
		}

		if (sensor_channel_get(dev, SENSOR_CHAN_ACCEL_Y, &accel[1]) < 0) {
			printf("Channel get error\n");
			return;
		}

		if (sensor_channel_get(dev, SENSOR_CHAN_ACCEL_Z, &accel[2]) < 0) {
			printf("Channel get error\n");
			return;
		}

		printf("x: %.1f, y: %.1f, z: %.1f (m/s^2)\n",
		       sensor_value_to_double(&accel[0]),
		       sensor_value_to_double(&accel[1]),
		       sensor_value_to_double(&accel[2]));
	}
}