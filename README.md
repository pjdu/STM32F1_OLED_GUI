# Atolic STM32f103RCT6 OLED MENU System


display file structure command:

```

 tree -P '*.c|*.h' -I 'Debug|Drivers|startup|Core' > tree.txt

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

```

|-- Command
|   |-- src
|       |-- get_command.c        # analysis command from uart 
|       `-- my_system_fun.c      # checksum for intel hex format
|-- FreeRTOS                     # FreeRTOS Source file
|   |-- croutine.c
|   |-- event_groups.c
|   |-- include
|   |-- list.c
|   |-- portable
|   |   |-- GCC
|   |   |   `-- ARM_CM3
|   |   |       `-- port.c
|   |   `-- MemMang
|   |       `-- heap_4.c
|   |-- queue.c
|   |-- stream_buffer.c
|   |-- tasks.c
|   `-- timers.c
|-- GUI_APP                      # all gui app task 
|   |-- src
|       |-- air_data.c           #(only data format none ui and task)
|       |-- decode_command.c     #decode_commad task  
|       |-- iwdgtask.c           #indepent watch dog task 
|       |-- main_ui_page.c       #main ui page task  
|       |-- menuL1_item.c        #menu ui item
|       |-- menu_ui.c            #menu ui task 
|       |-- rand_task.c          #(x)
|       |-- rtc_ui.c             #time setting ui
|       `-- sensor_ui_template.c #sensor state display task 
`-- GUI_Driver                   # lcd gui driver 
    |-- src
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