#ifndef ACCELEROMETER_H
#define ACCELEROMETER_H


#include <zephyr/kernel.h>
#include <zephyr/drivers/spi.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/device.h>

int Initialize_accelerometers();
int print_adxl362_data();



#endif