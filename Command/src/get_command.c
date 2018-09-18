#include "get_command.h"
#include "my_system_fun.h"

#define SEQ_COMMAND_BUFFER_SIZE 16
#define RAW_DATA_BUFFER_SIZE 256

static uint8_t sequenceHead;
static uint8_t sequenceTail;
static uint8_t sequence_command[SEQ_COMMAND_BUFFER_SIZE][16];

volatile uint8_t commandBufferRxCount;

void getCOMMANDInitial(void)
{
    sequenceHead = 0;
    sequenceTail = 0;
    commandBufferRxCount = 0;
}

uint8_t* readCOMMANDData(void)
{
    uint8_t *cmd_ptr;
    
    cmd_ptr = sequence_command[sequenceTail++];
    if(SEQ_COMMAND_BUFFER_SIZE <= sequenceTail) {
        sequenceTail = 0;
    }
    
    commandBufferRxCount--;
    
    return cmd_ptr;
}

uint8_t analysisCOMMAND(uint8_t* ptrdata, uint16_t datasize)
{
    if(datasize < 4) {
        return 0;
    }

    for(uint16_t i = 0; i < (datasize - 3); i++) {
        uint8_t len;
        if(*ptrdata == ':' ) {
            len = *(ptrdata + 1);
            if((!checksumIETELHex(ptrdata, len)) &&
            (SEQ_COMMAND_BUFFER_SIZE > commandBufferRxCount)) {
                memcpy(sequence_command[sequenceHead++], ptrdata, len);
                i = i + len - 1;
                ptrdata = ptrdata + len;
                if(SEQ_COMMAND_BUFFER_SIZE <= sequenceHead) {
                    sequenceHead = 0;
                }
                commandBufferRxCount++;
            }
        } else {
            ptrdata++;
        }
    }
    return commandBufferRxCount;
}