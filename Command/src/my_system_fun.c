#include "my_system_fun.h"

uint8_t checksumIETELHex(uint8_t *str, uint8_t cnt)
{
    uint16_t temp = 0;
    uint16_t chksum = 0;
    
    for(uint8_t i = 0; i < cnt; i++) {
        temp += *(str++);
    }
    
    chksum = (~temp);
    chksum ++;
    return (uint8_t) chksum;
}