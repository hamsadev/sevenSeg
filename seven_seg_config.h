#ifndef _SEVEN_SEG_CONFIG_H_
#define _SEVEN_SEG_CONFIG_H_

#define SEVEN_SEG_HW_AVR        1
#define SEVEN_SEG_HW_STM32F0    2
#define SEVEN_SEG_HW_STM32F1    3
#define SEVEN_SEG_HW_STM32F2    4
#define SEVEN_SEG_HW_STM32F3    5
#define SEVEN_SEG_HW_STM32F4    6

/**
 * @brief set your hardware from HardwareType enum
 */
#define SEVEN_SEG_HW   SEVEN_SEG_HW_AVR  
  
/**
 * @brief Set 1 if seven seg is CA
 */    
#define SEVEN_SEG_IS_CA         1

/**
 * @brief maximum number of seven segment
 * *Note that this number is seven segment, not digits
 */
#define SEVEN_SEG_MAX_NUM       3   

#endif //_SEVEN_SEG_CONFIG_H_