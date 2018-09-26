# Atolic STM32f103RCT6 OLED MENU System


display file structure command:

```

 tree -P '*.c|*.h|' -I 'Debug|Drivers|startup|Core' > tree.txt

```

## File Structure(important)

```

|-- Command
|   |-- inc
|   `-- src
|       |-- get_command.c    	 # analysis command from uart 
|       `-- my_system_fun.c  	 # checksum for intel hex format
|-- FreeRTOS				 	 # FreeRTOS Source file
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
|-- GUI_APP		  				 # all gui app task 
|   |-- inc
|   `-- src
|       |-- air_data.c 		 	 #(only data format none ui and task)
|       |-- decode_command.c 	 #decode_commad task  
|       |-- iwdgtask.c		 	 #indepent watch dog task 
|       |-- main_ui_page.c   	 #main ui page task  
|       |-- menuL1_item.c	 	 #menu ui item
|       |-- menu_ui.c		 	 #menu ui task 
|       |-- rand_task.c
|       |-- rtc_ui.c		 	 #time setting ui
|       `-- sensor_ui_template.c #sensor state display task 
`-- GUI_Driver				 	 # lcd gui driver 
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