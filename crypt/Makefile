obj-m+=crypto.o
obj-m+=sha1.o
 
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) modules
 
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) clean
 
