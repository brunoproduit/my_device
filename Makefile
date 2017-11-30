CC=gcc
obj-m += my_device.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	$(CC) -o device_usage_example device_usage_example.c
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm device_usage_example
	sudo rmmod my_device
	sudo rm /dev/my_device
test:
	sudo dmesg -C
	sudo insmod my_device.ko
	sudo mknod /dev/my_device c 42 0
	sudo chmod 666 /dev/my_device
	./device_usage_example
	dmesg
