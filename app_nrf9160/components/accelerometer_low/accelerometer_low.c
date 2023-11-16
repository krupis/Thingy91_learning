#include "accelerometer_low.h"





#define DEFAULT_ADXL362_NODE DT_ALIAS(adxl362)
BUILD_ASSERT(DT_NODE_HAS_STATUS(DEFAULT_ADXL362_NODE, okay),
			 "ADXL362 not specified in DT");


#define LOG_LEVEL CONFIG_LOG_DEFAULT_LEVEL
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(accelerometer_low);







