/*************************************************************************
	> File Name: usbTest.cpp
	> Author: lee.C.G
	> Mail: chenguang552@hotmail.com 
	> Created Time: 2019年07月09日 星期二 10时48分40秒
 ************************************************************************/
#include<stdio.h>
#include<iostream>
#include<iomanip>
#include<libusb-1.0/libusb.h>

using namespace std;

int main()
{
	int err = 0;

	libusb_context *ctx;
	err = libusb_init(&ctx);
	if(err)
	{
		std::cout<<"初始化usblib错误";
		return 1; 
	}


	libusb_device **list;
	struct libusb_device_descriptor desc;
	struct libusb_config_descriptor *conf;

	libusb_device_handle* handle = NULL;
	int config = 0;
	int ret;

	int status;
	ssize_t num_devs,i,j,k;

	status = 1;
	
	num_devs = libusb_get_device_list(ctx,&list);
	if(num_devs < 0)
	{
		return status;
	}

	for( i = 0; i < num_devs; ++i)
	{
		libusb_device *dev = list[i];
		libusb_open(dev , &handle);

		uint8_t bnum = libusb_get_bus_number(dev);
		uint8_t dnum = libusb_get_device_address(dev);

		libusb_get_device_descriptor(dev, &desc);
		std::cout<<std::endl<<"device NO."<<i<<std::endl;
		//std::cout<<hex<<setw(4)<<setfill('0')<<"desc.idVendor:"<<desc.idVendor<<std::endl;
		//std::cout<<hex<<setw(4)<<setfill('0')<<"desc.idProduct:"<<desc.idProduct<<std::endl;
		//std::cout<<"desc.idproduct:"<<desc.idProduct<<"  desc.bDeviceSubClass:"
		//		<<desc.bDeviceSubClass<<"  desc.bDeviceClass:"<<desc.bDeviceClass<<std::endl;
		printf("device:\t%04x:%04x\n",desc.idVendor,desc.idProduct);
		printf("bDeviceSubClass:%u    ",desc.bDeviceSubClass);
		printf("bDeviceClass:%u    ",desc.bDeviceClass);
		printf("bDeviceProtocol:%u\n",desc.bDeviceProtocol);

		for(j = 0; j < desc.bNumConfigurations; ++j)
		{
			ret = libusb_get_config_descriptor(dev, j , &conf);
			if(ret)
			{
				std::cout<<"err libusb_get_config_descripto";
		
				return ret;
			}else{
				
				//std::cout<<"bNumberInterface = "<<conf->bNumInterfaces<<std::endl;
				//std::cout<<"bConfigurationValue = "<<conf->bConfigurationValue<<std::endl;
				printf("bNumberInterfaces:%u\n",conf->bNumInterfaces);
				printf("bConfigurationVale:%u\n",conf->bConfigurationValue);
				for(k = 0; k < conf->bNumInterfaces; ++k )
				{
					//dump_interface(handle,&conf->interface[k]);
				}
				libusb_free_config_descriptor(conf);
			}
		}
	}
	
	libusb_free_device_list(list,0);

	return 0;
}
