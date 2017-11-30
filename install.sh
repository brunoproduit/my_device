#!/bin/bash
make

sudo insmod my_device.ko
sudo mknod /dev/my_device c 42 0
sudo chmod 666 /dev/my_device

./device_usage_example

