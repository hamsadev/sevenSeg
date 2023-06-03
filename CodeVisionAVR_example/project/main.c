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
#define SEVEN_SEG2_DIGIT_NUM     2

static SevenSegment sevenSegment1;
static SevenSegment sevenSegment2;
static const SevenSeg_pinConfig sevenSegDataBus1[8] = {
    {GPIOC,0},
    {GPIOC,1},
    {GPIOC,2},
    {GPIOC,3},
    {GPIOC,4},
    {GPIOC,5},
    {GPIOC,6},
    {GPIOC,7},
};
static const SevenSeg_pinConfig sevenSegComsBus1[SEVEN_SEG1_DIGIT_NUM] = {
    {GPIOD,0},
    {GPIOD,1},
    {GPIOD,2},
    {GPIOD,3},
    {GPIOD,4},
    {GPIOD,5},
    {GPIOD,6},
    {GPIOD,7},
};

static const SevenSeg_pinConfig sevenSegDataBus2[8] = {
    {GPIOA,0},
    {GPIOA,1},
    {GPIOA,2},
    {GPIOA,3},
    {GPIOA,4},
    {GPIOA,5},
    {GPIOA,6},
    {GPIOA,7},
};
static const SevenSeg_pinConfig sevenSegComsBus2[SEVEN_SEG2_DIGIT_NUM] = {
    {GPIOB,0},
    {GPIOB,1},
};

static void timerInit (void);

void main (void)
{    
    timerInit();
    sevenSegInit(&SEVEN_SEG_DRIVER);
    sevenSegAdd(&sevenSegment1, sevenSegDataBus1, sevenSegComsBus1, SEVEN_SEG1_DIGIT_NUM);
    sevenSegAdd(&sevenSegment2, sevenSegDataBus2, sevenSegComsBus2, SEVEN_SEG2_DIGIT_NUM);
    

    #asm("sei")      
                                            
    
    
    sevenSegPuts(&sevenSegment1,"LIB TEST");
    sevenSegPutInt(&sevenSegment2, 1);
    delay_ms(2000);  
    sevenSegPuts(&sevenSegment1, "NONBLOCK");
    sevenSegPutInt(&sevenSegment2, 2);
    delay_ms(1000);     
    
    sevenSegPuts(&sevenSegment1, "PUT INT"); 
    sevenSegPutInt(&sevenSegment2, 3);
    delay_ms(1000);
                                          
    sevenSegPutInt(&sevenSegment1, 123);
    delay_ms(1000);
        
    sevenSegPuts(&sevenSegment1, "PUTFLOAT"); 
    sevenSegPutInt(&sevenSegment2, 4);
    delay_ms(1000);
                                  
    sevenSegPutFloat(&sevenSegment1, 123.456, 3);
    sevenSegPutInt(&sevenSegment2, 5);
    
    
    while(1)
    {    
        
    }
}

static void timerInit (void)
{
    TCCR0=(0<<WGM00) | (0<<COM01) | (0<<COM00) | (0<<WGM01) | (1<<CS02) | (0<<CS01) | (0<<CS00);
    TCNT0=0xA2;
    OCR0=0x00;

    TIMSK=(0<<OCIE2) | (0<<TOIE2) | (0<<TICIE1) | (0<<OCIE1A) | (0<<OCIE1B) | (0<<TOIE1) | (0<<OCIE0) | (1<<TOIE0);

} 

interrupt [TIM0_OVF] void timer0_ovf_isr(void)
{
    TCNT0=0xA2;
    sevenSegRefreshIsr();
}
