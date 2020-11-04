CC = $(CROSS_COMPILE)gcc

LD = $(CROSS_COMPILE)ld

obj-m := pbutton.o

KDIR := /home/$(shell whoami)/android_build/out/target/product/evk_8mq/obj/KERNEL_OBJ/

PWD := $(shell pwd) 

default:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules
clean:
	rm -f *.ko
	rm -f *.mod.*
	rm -f .*.cmd
	rm -f *.o
	rm -f modules.order Module.symvers

