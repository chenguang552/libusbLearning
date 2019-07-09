/*************************************************************************
	> File Name: usbToast.cpp
	> Author: lee.C.G
	> Mail: chenguang552@hotmail.com 
	> Created Time: 2019年07月09日 星期二 15时39分14秒
 ************************************************************************/
#include<stdlib.h>
#include<iostream>
#include<stdio.h>
#include<libusb-1.0/libusb.h>

//热插拔回调函数，不能再此调用阻塞的函数，防止其他libusb函数失效
static int hotplug_callback(struct libusb_context *ctx, struct libusb_device *device,
							libusb_hotplug_event event,	void *Data)
{
	uint8_t deviceAdd = libusb_get_device_address(device);
	struct libusb_device_descriptor ldDesc_st;
	libusb_get_device_descriptor(device, &ldDesc_st);

	if(event == LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED)
	{
		printf("设备连接.\n");
		printf("设备地址：%d\n",deviceAdd);
		printf("设备VID：%04x\n",ldDesc_st.idVendor);
	}else{
		if(event == LIBUSB_HOTPLUG_EVENT_DEVICE_LEFT)
		{
			printf("设备断开.\n");
			printf("设备地址：%d\n",deviceAdd);
			printf("设备VID：%04x\n",ldDesc_st.idVendor);
		}
	}
	return 0;
}


int main()
{
	printf("usb监听开启中...\n");

	//usb上下文
	libusb_context *ctx;

	//初始化libusb
	int iRet = libusb_init(&ctx);
	if(iRet != LIBUSB_SUCCESS)
	{
		printf("libusb初始化错误：err::%d\n",iRet);
		return -1;
	}

	//判断当前库是否支持热插拔
	if(!libusb_has_capability(LIBUSB_CAP_HAS_HOTPLUG))
	{
		printf("该版本动态库不支持设备热插拔!!\n");
		return -1;
	}
	
	//要监控的设备的vid和pid,LIBUSB_HOTPLUG_MATCH_ANY表示匹配所有设备
	int idVendor = LIBUSB_HOTPLUG_MATCH_ANY;
	int idProduct = LIBUSB_HOTPLUG_MATCH_ANY;

	int deviceClass = LIBUSB_HOTPLUG_MATCH_ANY;

	//设置监听事件 --插入--拔出--
	libusb_hotplug_event lhEvent = static_cast<libusb_hotplug_event>(
			LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED |	
			LIBUSB_HOTPLUG_EVENT_DEVICE_LEFT	);	
	
	libusb_hotplug_flag lbFlag = static_cast<libusb_hotplug_flag>(1);
	
	//热插拔回调句柄
	libusb_hotplug_callback_handle lhcHandle;

	//接收设备回调数据
	void *pvData;
	
	//注册回调
	int iRegResult = libusb_hotplug_register_callback(ctx, lhEvent, lbFlag, idVendor, 
			idProduct, deviceClass, hotplug_callback, &pvData, &lhcHandle);
	if(iRegResult != LIBUSB_SUCCESS)
	{
		printf("注册回调函数失败!!\n");
		return -1;
	}

	while(1)
	{
		//循环调用，以监听热插拔事件
		libusb_handle_events(ctx);
	}

	//注销回调事件
	libusb_hotplug_deregister_callback(ctx, lhcHandle);
	libusb_exit(ctx);

	return 0;
}
