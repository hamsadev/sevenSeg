#ifndef _SEVEN_SEG_CONFIG_H_
#define _SEVEN_SEG_CONFIG_H_

#include <stdbool.h>

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
#define SEVEN_SEG_IS_CA         true

/**
 * @brief Use dynamic memory for seven seg buffer
 * 
 */
#define USE_DYNAMIC_MEMORY      false

#if !USE_DYNAMIC_MEMORY
/**
 * @brief maximum number of seven segment's digits
 * 
*/
#define SEVEN_SEG_DIGIT_MAX_NUM     8
/**
 * @brief maximum number of seven segment
 */
#define SEVEN_SEG_MAX_NUM       3
#endif

#endif //_SEVEN_SEG_CONFIG_H_