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

#define SEVEN_SEG1_DIGIT_NUM     8

static SevenSegment sevenSegment1;
static const SevenSeg_pinConfig sevenSegDataBus1[8] = {
    {GPIOD,0},
    {GPIOD,1},
    {GPIOD,2},
    {GPIOD,3},
    {GPIOD,4},
    {GPIOD,5},
    {GPIOD,6},
    {GPIOD,7},    
};
static const SevenSeg_pinConfig sevenSegComsBus1[SEVEN_SEG1_DIGIT_NUM] = {
    {GPIOB,7},
    {GPIOB,6},
    {GPIOB,5},
    {GPIOB,4},
    {GPIOB,3},
    {GPIOB,2},
    {GPIOB,1},
    {GPIOB,0},    
};


static void timerInit (void);

void main (void)
{           
    timerInit();
    sevenSegInit(&SEVEN_SEG_DRIVER);
    sevenSegAdd(&sevenSegment1, sevenSegDataBus1, sevenSegComsBus1, SEVEN_SEG1_DIGIT_NUM);

    #asm("sei")      
        
    sevenSegPuts(&sevenSegment1,"LIB TEST");
//    delay_ms(2000);  
//    sevenSegPuts(&sevenSegment1, "NONBLOCK");
//    delay_ms(1000);     
//    
//    sevenSegPuts(&sevenSegment1, "PUT INT"); 
//    delay_ms(1000);
//                                          
//    sevenSegPutInt(&sevenSegment1, 123);
//    delay_ms(1000);
//        
//    sevenSegPuts(&sevenSegment1, "PUTFLOAT"); 
//    delay_ms(1000);
//                                  
//    sevenSegPutFloat(&sevenSegment1, 123.456, 3);
    
    while(1)
    {       
    }
}

static void timerInit (void)
{
    // Timer/Counter 0 initialization
    // Clock source: System Clock
    // Clock value: 125.000 kHz
    TCCR0=(0<<CS02) | (1<<CS01) | (1<<CS00);
    TCNT0=0x83;

    // Timer(s)/Counter(s) Interrupt(s) initialization
    TIMSK=(0<<OCIE2) | (0<<TOIE2) | (0<<TICIE1) | (0<<OCIE1A) | (0<<OCIE1B) | (0<<TOIE1) | (1<<TOIE0);
} 

interrupt [TIM0_OVF] void timer0_ovf_isr(void)
{
    TCNT0=0x83;
    sevenSegRefreshIsr();
}
