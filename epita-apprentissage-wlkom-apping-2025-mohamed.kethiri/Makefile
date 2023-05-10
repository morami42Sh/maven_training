obj-m += _epirandom.o  

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

device:
	sudo rm -f /dev/epirandom
	sudo insmod _epirandom.ko
	sudo mknod /dev/epirandom c $(shell grep epiRandom /proc/devices | awk '{print $$1}') 0
	sudo chmod 666 /dev/epirandom

deldevice:
	sudo rm -f /dev/epirandom
	sudo rmmod _epirandom.ko

testcat:
	cat /dev/epirandom

testdd:
	dd if=/dev/epirandom of=out bs=512 count=1

testalphabet:
	sudo insmod _epirandom.ko alphabet=a,b,c,X,Y,Z,0,1,2,3,4,5,6,7,8,9,\\,\\?,\\\
	sudo rm -f /dev/epirandom
	sudo mknod /dev/epirandom c $(shell grep epiRandom /proc/devices | awk '{print $$1}') 0
	sudo chmod 666 /dev/epirandom

testalphabet2:
	sudo insmod _epirandom.ko alphabet='abcXYZ0123456789,?\'
	sudo rm -f /dev/epirandom
	sudo mknod /dev/epirandom c $(shell grep epiRandom /proc/devices | awk '{print $$1}') 0
	sudo chmod 666 /dev/epirandom

