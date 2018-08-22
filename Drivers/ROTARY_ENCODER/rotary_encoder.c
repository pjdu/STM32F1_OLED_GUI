#include "rotary_encorder.h"
#include "math.h"

//rotary Encoder rotate var
static int32_t current_count = 0, last_count = 0,range_max = 32767,range_min = 0;

//rotary Encoder sw var
static int old_state = 1, current_state = 1, press_count = 0;


Button_state RotaryEcncorder_ButtonScan(void) {
	Button_state button_state;
	current_state = HAL_GPIO_ReadPin(ROTARY_SW_GPIO_Port, ROTARY_SW_Pin);
	if (current_state == 0 && old_state == 1) {
		old_state = current_state;
		button_state = button_debouce;
	} else if (current_state == 0 && old_state == 0 && press_count == 0) {
		button_state = button_press;
		press_count = 1;
	}
//	else if (current_state == 0 && old_state == 0 && press_count == 1) {
//		button_state = button_long_press;}
	else if (current_state == 1 && old_state == 0) {
		press_count = 0;
		button_state = button_release;
	}
	return button_state;

}

void RotaryEcncorder_Init() {
	HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);
	RotaryEcncorder_SetCount(0);
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//   設定旋轉編碼器數值
//   count:設定值範圍-32767~32767
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
int32_t RotaryEcncorder_SetCount(int32_t count) {
	__HAL_TIM_SET_COUNTER(&htim2,32767+count);
}
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//   設定旋轉編碼器讀值範圍
//   min~max:設定值範圍-32767~32767
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
int32_t RotaryEcncorder_SetRange(int32_t min ,int32_t max) {

	int32_t temp;

	if(max < min)
	{
		temp = max;
		max = min;
		min = max;
	}

	if(min < -32767)min = -32767;
	if(max > 32767)max = 32767;

	range_min = min;
	range_max = max;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//   取得旋轉編碼器計數值
//   返回值範圍0~32767
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
int32_t RotaryEcncorder_GetCount() {
	int32_t count;

	count = __HAL_TIM_GET_COUNTER(&htim2);  //value: cw:0->65535  or  ccw:0<-65535<-0
	count -= 32767; //-32767 ~ 32767
	if(count > range_max)
	{
		count = range_max;
		RotaryEcncorder_SetCount(count);
	}
	else if(count < range_min)
	{
		count = range_min;
		RotaryEcncorder_SetCount(count);
	}
	return count;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//   取得旋轉編碼器狀態
//   返回值範圍-32767~32767
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
Rotary_state RotaryEcncorder_GetState() {
	Rotary_state rstate;
	current_count = RotaryEcncorder_GetCount();  //value range: -32767~32767
	if (current_count - last_count > 0) {
		rstate = state_clock_wise;
	} else if (current_count - last_count < 0) {
		rstate = state_counter_clock_wise;
	} else {
		rstate = state_no_changed;
	}
	last_count = current_count;
	return rstate;
}

