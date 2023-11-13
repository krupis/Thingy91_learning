#include <stdio.h>
#include <stdlib.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/spi.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/device.h>
#include <ei_wrapper.h>


#define DEFAULT_ADXL372_NODE DT_ALIAS(adxl372)
BUILD_ASSERT(DT_NODE_HAS_STATUS(DEFAULT_ADXL372_NODE, okay),
			 "ADXL372 not specified in DT");

#define DEFAULT_ADXL362_NODE DT_ALIAS(adxl362)
BUILD_ASSERT(DT_NODE_HAS_STATUS(DEFAULT_ADXL362_NODE, okay),
			 "ADXL362 not specified in DT");

const struct device *const adxl1372_dev = DEVICE_DT_GET(DEFAULT_ADXL372_NODE);
const struct device *const adxl1362_dev = DEVICE_DT_GET(DEFAULT_ADXL362_NODE);

static const enum sensor_channel channels[] = {
	SENSOR_CHAN_ACCEL_X,
	SENSOR_CHAN_ACCEL_Y,
	SENSOR_CHAN_ACCEL_Z,
};


static size_t frame_surplus;
static void result_ready_cb(int err);
static int get_accel_data(const struct device *dev);
static int print_accels(const struct device *dev);

/**
 * @brief Example usage: 
  	// 	ret = print_accels(adxl1362_dev);
	// 	if (ret < 0)
	// 	{
	// 		return 0;
	// 	}
 * 
 * @param dev 
 * @return int 
 */
static int print_accels(const struct device *dev)
{
	int ret;
	struct sensor_value accel[3];

	ret = sensor_sample_fetch(dev);
	if (ret < 0)
	{
		printk("%s: sensor_sample_fetch() failed: %d\n", dev->name, ret);
		return ret;
	}

	for (size_t i = 0; i < ARRAY_SIZE(channels); i++)
	{
		ret = sensor_channel_get(dev, channels[i], &accel[i]);
		if (ret < 0)
		{
			printk("%s: sensor_channel_get(%c) failed: %d\n", dev->name, 'X' + i, ret);
			return ret;
		}
	}

	printk("%16s [m/s^2]:    (%12.6f, %12.6f, %12.6f)\n", dev->name,
		   sensor_value_to_double(&accel[0]), sensor_value_to_double(&accel[1]), sensor_value_to_double(&accel[2]));

	return 0;
}







static int get_accel_data(const struct device *dev){
	int ret;
	struct sensor_value accel[156];

	ret = sensor_sample_fetch(dev);
	if (ret < 0)
	{
		printk("%s: sensor_sample_fetch() failed: %d\n", dev->name, ret);
		return ret;
	}
	for(size_t j = 0; j < 52; j ++){
		for (size_t i = 0; i < ARRAY_SIZE(channels); i++)
		{
			ret = sensor_channel_get(dev, channels[i], &accel[(3*j)+i]);
			if (ret < 0)
			{
				printk("%s: sensor_channel_get(%c) failed: %d\n", dev->name, 'X' + i, ret);
				return ret;
			}
		}
		k_msleep(10);
	}

	for(int i = 0;i < 156 ; i++){
		printk("%16s [m/s^2]:    (%12.6f)\n", dev->name,
		   sensor_value_to_double(&accel[i]));
	}

	// printk("%16s [m/s^2]:    (%12.6f, %12.6f, %12.6f)\n", dev->name,
	// 	   sensor_value_to_double(&accel[0]), sensor_value_to_double(&accel[1]), sensor_value_to_double(&accel[2]));

	return 0;
}




int main(void)
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

	int err = ei_wrapper_init(result_ready_cb);

	if (err) {
		printk("Edge Impulse wrapper failed to initialize (err: %d)\n",
		       err);
		return 0;
	};

	printk("Machine learning model sampling frequency: %zu\n",
	       ei_wrapper_get_classifier_frequency());
	printk("Labels assigned by the model:\n");
	for (size_t i = 0; i < ei_wrapper_get_classifier_label_count(); i++) {
		printk("- %s\n", ei_wrapper_get_classifier_label(i));
	}
	printk("\n");

	size_t cnt = 0;

	while(1){
		ret = print_accels(adxl1362_dev);
		if (ret < 0)
		{
			return 0;
		}
		k_msleep(100);
	}
	return 0;
}










// Machine learning part
static void result_ready_cb(int err)
{
	if (err) {
		printk("Result ready callback returned error (err: %d)\n", err);
		return;
	}

	const char *label;
	float value;
	float anomaly;

	printk("\nClassification results\n");
	printk("======================\n");

	while (true) {
		err = ei_wrapper_get_next_classification_result(&label, &value, NULL);

		if (err) {
			if (err == -ENOENT) {
				err = 0;
			}
			break;
		}

		printk("Value: %.2f\tLabel: %s\n", value, label);
	}

	if (err) {
		printk("Cannot get classification results (err: %d)", err);
	} else {
		if (ei_wrapper_classifier_has_anomaly()) {
			err = ei_wrapper_get_anomaly(&anomaly);
			if (err) {
				printk("Cannot get anomaly (err: %d)\n", err);
			} else {
				printk("Anomaly: %.2f\n", anomaly);
			}
		}
	}

	if (frame_surplus > 0) {
		err = ei_wrapper_start_prediction(0, 1);
		if (err) {
			printk("Cannot restart prediction (err: %d)\n", err);
		} else {
			printk("Prediction restarted...\n");
		}

		frame_surplus--;
	}
}

