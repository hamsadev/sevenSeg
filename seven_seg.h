/**
 * @file seven_seg.h
 * @author Hamid Salehi (hamsame.dev@gmail.com) * @brief 
 * @date 2023-03-18
 * 
 */

#ifndef SEVEN_SEG_H_
#define SEVEN_SEG_H_

#include "io_define.h"
#include "seven_seg_config.h"

/**
 * @brief Hold sevenSeg io
 * *user can change it to GPIO_TypeDef or anything else that system want
 */
typedef GPIO_TypeDef* SevenSeg_IO;
/**
 * @brief Hold kesevenSegy pin num or pin bit
 * *user can change it to uint8_t for 8-bit systems like AVR
 */
typedef uint16_t SevenSeg_pin;

/**
 * @brief Hold pin configuration that use for handle 7seg pins
 */
typedef struct {
    SevenSeg_IO     	IO;
    SevenSeg_pin        pin;
} SevenSeg_pinConfig;

/**
 * @brief Initialize pin in output mode
 */
typedef void (*SevenSeg_initPinFn)(const SevenSeg_pinConfig* config);
/**
 * @brief Set pin state
 */
typedef void (*SevenSeg_writePinFn)(const SevenSeg_pinConfig* config, GPIO_PinState state);

/**
 * @brief hold minimum function for sevenSeg lib to work
 */
typedef struct {
    SevenSeg_initPinFn      initPin;
    SevenSeg_writePinFn     writePin;
} SevenSeg_Driver;


#pragma used+    

void sevenSegInit(SevenSeg_pinConfig* dataBus, SevenSeg_pinConfig* comBus, uint8_t digitsNum, SevenSeg_Driver* driver);
void sevenSegPutInt(uint32_t num);
void sevenSegPuts(char* str);
void sevenSegPutFloat(float num, uint8_t decimals);
void sevenSegRefreshIsr(void);

#pragma used-

#endif // SEVEN_SEG_H_
