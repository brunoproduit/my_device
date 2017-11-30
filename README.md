# my_device
This is an example of a kernel device which works as a parallel port. When writing or reading from the device, it will write in a buffer and print in the KERN_DEBUG.

## Install

```
git clone git@github.com:brunoproduit/my_device.git
cd my_device
make
```

Note: needs sudo for insmod, mknod & chmod.

The device driver is created in the nodefile `/dev/my_device` as a character device with major number *42*.

## Test

The device will write in KERN_DEBUG, which can be read with `dmesg`.
In order to test you can use:

```
make test
```

This will insert the modue in the kernel, clean dmesg, run device_usage_example.c and print dmesg.
## Remove

```
make clean
```

Note: needs sudo for rmmod & rm.
