/**
 * @file main.c (example)
 * @author Hamid Salehi (hamsame.dev@gmail.com)
 * @date 2023-03-18
 * 
 */


#include <io.h>
#include <delay.h>
#include <stdint.h>

#include "seven_seg.h"
#include "seven_seg_port.h"

#define SEVEN_SEG_DIGIT_NUM 8

static const SevenSeg_pinConfig sevenSegDataBus[8] = {
    {GPIOC,0},
    {GPIOC,1},
    {GPIOC,2},
    {GPIOC,3},
    {GPIOC,4},
    {GPIOC,5},
    {GPIOC,6},
    {GPIOC,7},
};
static const SevenSeg_pinConfig sevenSegComsBus[SEVEN_SEG_DIGIT_NUM] = {
    {GPIOD,0},
    {GPIOD,1},
    {GPIOD,2},
    {GPIOD,3},
    {GPIOD,4},
    {GPIOD,5},
    {GPIOD,6},
    {GPIOD,7},
};

static void timerInit (void);

void main (void)
{    
    timerInit();
    sevenSegInit(sevenSegDataBus, sevenSegComsBus, SEVEN_SEG_DIGIT_NUM, &SEVEN_SEG_DRIVER);

    #asm("sei")      
    
    sevenSegPuts("LIB TEST");
    delay_ms(2000);  
    sevenSegPuts("NONBLOCK");
    delay_ms(1000);     
    
    sevenSegPuts("PUT INT");
    delay_ms(1000);
                                          
    sevenSegPutInt(123);
    delay_ms(1000);
        
    sevenSegPuts("PUTFLOAT");
    delay_ms(1000);
                                  
    sevenSegPutFloat(123.456, 3); 
    
    
    while(1)
    {    
        
    }
}

static void timerInit (void)
{
    TCCR0=(0<<WGM00) | (0<<COM01) | (0<<COM00) | (0<<WGM01) | (0<<CS02) | (1<<CS01) | (1<<CS00);
    TCNT0=0x83;
    OCR0=0x00;

    TIMSK=(0<<OCIE2) | (0<<TOIE2) | (0<<TICIE1) | (0<<OCIE1A) | (0<<OCIE1B) | (0<<TOIE1) | (0<<OCIE0) | (1<<TOIE0);
} 

interrupt [TIM0_OVF] void timer0_ovf_isr(void)
{
    TCNT0=0x83;
    sevenSegRefreshIsr();
}
