#include "air_data.h"
#include "string.h"
#include "config.h"

//static Device_Msg dev_array[CONFIG_MAX_DEVICE_SIZE];

static Device_Msg *dev_array = NULL;

#define AIR_DECLARE_SET_MEMBER_FUNC(member) \
    void airdata_set_##member(uint16_t data, int device_number){ \
	if(dev_array == NULL) \
		return; \
	else \
		dev_array[device_number].member = data;} \

#define AIR_DECLARE_GET_MEMBER_FUNC(member) \
	uint16_t airdata_get_##member(int device_number){ \
    if(dev_array == NULL) \
		return 0; \
	else \
		return dev_array[device_number].member;} \


void airdata_create_dev(int NumberOfDev){
	if(NumberOfDev <= 0 )return;
	if(dev_array == NULL)
	{
		dev_array = pvPortMalloc(sizeof(Device_Msg) * NumberOfDev);
	}
	else
	{
		airdata_delete_dev();
		dev_array = pvPortMalloc(sizeof(Device_Msg) * NumberOfDev);
	}
}

void airdata_delete_dev(){
	int temp = airdata_get_numberofDevice(0);
	vPortFree(dev_array);
	dev_array = NULL;
}

AIR_DECLARE_SET_MEMBER_FUNC(shortaddress)
AIR_DECLARE_GET_MEMBER_FUNC(shortaddress)

AIR_DECLARE_SET_MEMBER_FUNC(temperature)
AIR_DECLARE_GET_MEMBER_FUNC(temperature)

AIR_DECLARE_SET_MEMBER_FUNC(humidity)
AIR_DECLARE_GET_MEMBER_FUNC(humidity)

AIR_DECLARE_SET_MEMBER_FUNC(voc)
AIR_DECLARE_GET_MEMBER_FUNC(voc)

AIR_DECLARE_SET_MEMBER_FUNC(co2)
AIR_DECLARE_GET_MEMBER_FUNC(co2)

AIR_DECLARE_SET_MEMBER_FUNC(pm2_5)
AIR_DECLARE_GET_MEMBER_FUNC(pm2_5)


AIR_DECLARE_SET_MEMBER_FUNC(numberofDevice)
AIR_DECLARE_GET_MEMBER_FUNC(numberofDevice)


//void setNumberofDevice(uint16_t num, int device_number)
//{
//	dev_array[device_number].numberofDevice = num;
//}
//
//uint16_t getNumberofDevice(int device_number)
//{
//	return dev_array[device_number].numberofDevice;
//}

