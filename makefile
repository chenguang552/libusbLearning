header_src=/usr/include/libusb-1.0

lsusb	:	usbTest.cpp
			g++ usbTest.cpp -o lsusbinfo -lusb-1.0 -I$(header_src)
usbToast:	usbToast.cpp
			g++ usbToast.cpp -o usbToast -lusb-1.0 -I$(header_src)

clean:
			rm ./lsusbinfo ./usbToast
