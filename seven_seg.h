/**
 * @file seven_seg.h
 * @author Hamid Salehi (hamsame.dev@gmail.com) * @brief 
 * @date 2023-03-18
 * 
 */

#ifndef SEVEN_SEG_H_
#define SEVEN_SEG_H_
#include "io_define.h"

typedef struct {
    Gpio* gpio;
    uint8_t pin;
} SevenSegPin;

/*
    TIMER0 -> 0
    TIMER1 -> 1
    TIMER2 -> 2
    MANUAL -> -1
*/ 
#define SEVEN_SEG_TIMER         0         
#define SEVEN_SEG_IS_CA         0       

void sevenSegInit(SevenSegPin* dataBus, SevenSegPin* comBus, uint8_t digitsNum);
void sevenSegPutInt(uint32_t num);
#if SEVEN_SEG_TIMER == -1 
void sevenSegRefreshIsr(void);
#endif
#endif // SEVEN_SEG_H_
