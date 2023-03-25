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

#define SEVEN_SEG_DIGIT_NUM 8

SevenSegPin sevenSegDataBus[8] = {
    {GPIOC,0},
    {GPIOC,1},
    {GPIOC,2},
    {GPIOC,3},
    {GPIOC,4},
    {GPIOC,5},
    {GPIOC,6},
    {GPIOC,7},
};
SevenSegPin sevenSegComsBus[SEVEN_SEG_DIGIT_NUM] = {
    {GPIOD,0},
    {GPIOD,1},
    {GPIOD,2},
    {GPIOD,3},
    {GPIOD,4},
    {GPIOD,5},
    {GPIOD,6},
    {GPIOD,7},
};

void main (void)
{    
    sevenSegInit(sevenSegDataBus, sevenSegComsBus, SEVEN_SEG_DIGIT_NUM);      
    #asm("SEI")    
    
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
