CC=gcc
obj-m += my_device.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	$(CC) -o device_usage_example device_usage_example.c
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm device_usage_example
