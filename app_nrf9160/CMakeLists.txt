# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(hello_world)

FILE(GLOB app_sources src/*.c)
target_sources(app PRIVATE ${app_sources})


target_sources(app PRIVATE components/accelerometer/accelerometer.c)
target_include_directories(app PRIVATE components/accelerometer)

target_sources(app PRIVATE components/machine_learning/machine_learning.c)
target_include_directories(app PRIVATE components/machine_learning)

target_sources(app PRIVATE components/accelerometer_low/accelerometer_low.c)
target_include_directories(app PRIVATE components/accelerometer_low)