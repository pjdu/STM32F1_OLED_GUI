#ifndef __CONFIG_H
#define __CONFIG_H

#define MAX_DEVICE_SIZE 6

#define HEADER_SIZE 4  //4: Header + Length + FC + CS = 4byte
#define MAX_MSG_SIZE 16
#define MSG_DATA_SIZE (MAX_MSG_SIZE - HEADER_SIZE)


#endif
