#include "air_data.h"
#include "string.h"
#include "config.h"

static Device_Msg dev_array[CONFIG_MAX_DEVICE_SIZE];

#define AIR_DECLARE_SET_MEMBER_FUNC(member) \
    void airdata_set_##member(uint16_t data, int device_number){ \
    uint16_t tmp; \
    tmp = data & 0x00ff; \
    tmp = (tmp << 8) | ((data >> 8) & 0x00ff); \
    dev_array[device_number].member = tmp;} \

#define AIR_DECLARE_GET_MEMBER_FUNC(member) \
	uint16_t airdata_get_##member(int device_number){ \
    return dev_array[device_number].member;} \


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


void setNumberofDevice(uint16_t num, int device_number)
{
	dev_array[device_number].numberofDevice = num;
}

uint16_t getNumberofDevice(int device_number)
{
	return dev_array[device_number].numberofDevice;
}

