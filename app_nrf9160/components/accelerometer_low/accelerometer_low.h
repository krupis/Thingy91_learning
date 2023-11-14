/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef ACCELEROMETER_LOW_H
#define ACCELEROMETER_LOW_H

#include <zephyr/kernel.h>
#include <zephyr/drivers/spi.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/device.h>



//COMMAND REGISTERS
#define ADXL362_READ_REG        0x0B // 0x0B = 11   0000 1011
#define ADXL362_WRITE_REG       0x0A // 0x0A = 10   0000 1010
#define ADXL362_READ_FIFO       0x0D


//REGISTER SUMMARY
#define ADXL362_REG_DEVID_AD    0x00
#define ADXL362_REG_DEVID_MST   0x01
#define ADXL362_REG_PARTID      0x02




int adxl362_bus_check_spi();
int adxl362_reg_read_spi(uint8_t address, uint8_t *buf, int size);
int adxl362_reg_write_spi(uint8_t reg, uint8_t val);


int adxl362_write_test();





#endif