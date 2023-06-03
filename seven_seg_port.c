#include "seven_seg_port.h"
#include "seven_seg.h"
#include "io_define.h"

const SevenSeg_Driver SEVEN_SEG_DRIVER = {
    SevenSeg_initPin,
    SevenSeg_writePin,
};

#if SEVEN_SEG_HW == SEVEN_SEG_HW_AVR | SEVEN_SEG_HW == SEVEN_SEG_HW_STM32F0 

void SevenSeg_initPin(const SevenSeg_pinConfig* config) {
    config->IO->Direction.reg |= (1 << config->pin);    
}
void SevenSeg_writePin(const SevenSeg_pinConfig* config, GPIO_PinState state) {    
    if(state != GPIO_PIN_RESET)
    {
        config->IO->OutputData.reg |= (1 << config->pin); 
    }
    else
    {
        config->IO->OutputData.reg &= ~(1 << config->pin); 
    }
}

#elif SEVEN_SEG_HW == SEVEN_SEG_HW_STM32F0  
    // TODO: Implement me later
#elif SEVEN_SEG_HW == SEVEN_SEG_HW_STM32F1  
    // TODO: Implement me later
#elif SEVEN_SEG_HW == SEVEN_SEG_HW_STM32F2
    // TODO: Implement me later
#elif SEVEN_SEG_HW == SEVEN_SEG_HW_STM32F3
    // TODO: Implement me later
#elif SEVEN_SEG_HW == SEVEN_SEG_HW_STM32F4
void SevenSeg_initPin(const SevenSeg_pinConfig* config) {
    config->IO->MODER |= (0x03 << (config->pin + config->pin));
}
void SevenSeg_writePin(const SevenSeg_pinConfig* config, GPIO_PinState state) {
    if(state != GPIO_PIN_RESET)
    {
        config->IO->BSRR = GPIO_Pin;
    }
    else
    {
        config->IO->BSRR = (uint32_t)GPIO_Pin << 16U;
    }
}
#else

    #error "Please select your hardware"

#endif // SEVEN_SEG_HW
