#ifndef GET_COMMAND_H
#define	GET_COMMAND_H

#include <stdint.h>
#include <string.h>

void getCOMMANDInitial(void);
uint8_t* readCOMMANDData(void);
uint8_t analysisCOMMAND(uint8_t* ptrdata, uint16_t datasize);

//extern volatile uint8_t commandBufferRxCount;
uint8_t getCurrentCommandNum(void);
#endif
