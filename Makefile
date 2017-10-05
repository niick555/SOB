obj-m += cryptodevice.o

cryptodevice.o:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) modules

programa.o: programa.c
	gcc programa.c -o programa

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) clean
