/**
 * @file io_define.h
 * @author Hamid Salehi (hamsame.dev@gmail.com)
 * @brief set mcu hardware
 * @date 2023-03-18
 */

#ifndef IO_DEFINE_H_
#define IO_DEFINE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "seven_seg_config.h"

#if SEVEN_SEG_HW == SEVEN_SEG_HW_AVR
typedef union{
    uint8_t reg;
    struct {
        uint8_t bit0 : 1;
        uint8_t bit1 : 1;
        uint8_t bit2 : 1;
        uint8_t bit3 : 1;
        uint8_t bit4 : 1;
        uint8_t bit5 : 1;
        uint8_t bit6 : 1;
        uint8_t bit7 : 1;
    };
}GPIO_Register;

typedef struct{
    // PIN
    volatile GPIO_Register InputData;
    // DDR
    volatile GPIO_Register Direction;
    // PORT
    volatile GPIO_Register OutputData;
}GPIO_TypeDef;

/** 
  * @brief  GPIO Bit SET and Bit RESET enumeration 
  */
typedef enum
{
  GPIO_PIN_RESET = 0,
  GPIO_PIN_SET
}GPIO_PinState;

#define GPIOA   ((GPIO_TypeDef *)&PINA)             //(*(Gpio*)0x3A)
#define GPIOB   ((GPIO_TypeDef *)&PINB)             //(*(Gpio*)0x36)
#define GPIOC   ((GPIO_TypeDef *)&PINC)             //(*(Gpio*)0x33)
#define GPIOD   ((GPIO_TypeDef *)&PIND)             //(*(Gpio*)0x30)

#elif SEVEN_SEG_HW == SEVEN_SEG_HW_STM32F0 
    #include "stm32f0xx.h"
#elif SEVEN_SEG_HW == SEVEN_SEG_HW_STM32F1 
    #include "stm32f1xx.h"
#elif SEVEN_SEG_HW == SEVEN_SEG_HW_STM32F2
    #include "stm32f2xx.h"
#elif SEVEN_SEG_HW == SEVEN_SEG_HW_STM32F3
    #include "stm32f3xx.h"
#elif SEVEN_SEG_HW == SEVEN_SEG_HW_STM32F4
    #include "stm32f4xx.h"
#else
    #error "Please select your hardware"
#endif

#ifdef __cplusplus
};
#endif

#endif /* IO_DEFINE_H_ */
