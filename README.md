# my_device
This is an example of a kernel device which works as a parrallel port. When writing or reading from the device, it will write in a buffer and print in the KERN_DEBUG.

## Install

```
git clone https://github.com/brunoproduit/my_device.git
cd my_device
./install.sh
```

Note: needs sudo for insmod.The device driver is created in the nodefile `/dev/my_device` as a character device with major number *42*.

## Remove 

```
./remove.sh
```

Note: needs sudo for rmmod

The device will write in KERN_DEBUG, which can be read with `dmesg`.
