obj-m += hello-1.o
obj-m += hello-2.o
obj-m += hello-3.o
obj-m += hello-4.o
obj-m += hello-5.o
#obj-m += buff_ovrflw.o
obj-m += chardev.o
obj-m += sploitable.o

all:
	make -C /home/dcashman/thesis/misc/arm_canary/RPi/linux-rpi-3.6.y M=$(PWD) modules ARCH=arm CROSS_COMPILE=/usr/bin/arm-linux-gnueabi- -k

clean:
	make -C /home/dcashman/thesis/misc/arm_canary/RPi/linux-rpi-3.6.y M=$(PWD)/build M=$(PWD) clean