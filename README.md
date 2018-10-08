# Atolic STM32f103RCT6 OLED MENU System with FreeRTOS


## debug print:

在config.h，Debug 1 會列印出除錯列印訊息，0則關閉

## airdata_api:
 
```c
	
	void airdata_set_shortaddress(uint16_t data, int device_number);
	uint16_t airdata_get_shortaddress(int device_number);
	
	void airdata_set_temperature(uint16_t data, int device_number);
	uint16_t airdata_get_temperature(int device_number);
	
	void airdata_set_humidity(uint16_t data, int device_number);
	uint16_t airdata_get_humidity(int device_number);
	
	void airdata_set_voc(uint16_t data, int device_number);
	uint16_t airdata_get_voc(int device_number);
	
	void airdata_set_co2(uint16_t data, int device_number);
	uint16_t airdata_get_co2(int device_number);
	
	void airdata_set_pm2_5(uint16_t data, int device_number);
	uint16_t airdata_get_pm2_5(int device_number);
	
	
```


## printf Usage:
```c
	
	//license on header of printf-stdarg.c
	//"in the 3rd party format-string %d %x %u %s %c is supported ,but %f is not support on printf sprintf" 
	// printf %f alternative solution 
	
	char *buf;
	buf = pvPortMalloc(100);
	snprintf(buf,"%f",1.23); //comment snprintf from printf-stdarg.c and change to use stdlib provide 
	printf("%s",buf);
	
	
```

## Event  wait exmple:

```c

	static EventBits_t val = 0;
	
	val = xEventGroupWaitBits(EventGroupHandler,
				  BUTTON_PRESS_EVENT | BUTTON_PRESS_1S_EVENT, //短按事件和長按事件
				  pdTRUE, 	//等待完成自動清除
				  pdFALSE,	//任一事件發生進入
				  10 / portTICK_PERIOD_MS);//等10ms看看有無事件發生
	switch(val)
	{
		case BUTTON_PRESS_EVENT:
			//do somthing
			break;
		case BUTTON_PRESS_1S_EVENT:
			//do somthing
			break;
		case NULL_EVENT_RAISE:
			//do somthing
			break;
	}
							  
```

## Event  set exmple:

can wathch Drives/ROTARY_ENCODER/rotary_encoder.c

## File Structure(important)

display file structure command:

```
	 # -P -> source file pattern want to add
	 # -I -> source file pattern want to ingnore 

	 tree -P '*.c' -I 'Debug|STM32F1xx_HAL_Driver|CMSIS|startup|FreeRTOS' > tree.txt

```

```

	.
	|-- Command
	|   |-- inc
	|   `-- src
	|       |-- get_command.c 	  // analysis command and command store 
	|       `-- my_system_fun.c	// checksum for intel hex format
	|-- Core
	|   |-- Inc
	|   `-- Src
	|       |-- main.c
	|       |-- stm32f1xx_hal_msp.c
	|       |-- stm32f1xx_it.c
	|       `-- system_stm32f1xx.c
	|-- Drivers    
	|   |-- OLED
	|   |   `-- SSD1322.c     	 	//SSD1322 OLED Driver
	|   |-- ROTARY_ENCODER
	|   |   `-- rotary_encoder.c		//Rotary_Encoder rotate and button press driver
	|   |-- RTC
	|   |   `-- rtc.c					// rtc driver add datetime convert 
	|   `-- UART
	|       |-- printf-stdarg.c 		// redirect printf  to uart  
	|       `-- uart.c				// uart driver 
	|-- GUI_APP                      
	|   |-- src
	|       |-- air_data.c           //(only data format and api  none ui and task)
	|       |-- decode_command.c     //decode_commad task  
	|       |-- iwdgtask.c           //indepent watch dog task 
	|       |-- main_ui_page.c       //main ui page task  
	|       |-- menuL1_item.c        //menu ui item
	|       |-- menu_ui.c            //menu ui task 
	|       |-- rand_task.c          //(x) no use this is test task
	|       |-- rtc_ui.c             //time setting ui
	|       `-- sensor_ui_template.c //sensor state display task 
	`-- GUI_Driver
	    |-- inc
	    `-- src
	        |-- button.c
	        |-- font.c
	        |-- gui_basic.c
	        |-- gui_menu.c
	        |-- lcmdrv.c
	        |-- messagebox.c
	        |-- scrollbar.c
	        |-- text.c
	        `-- windows.c       
```