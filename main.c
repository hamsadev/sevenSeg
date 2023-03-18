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
SevenSegPin sevenSegComsBus[8] = {
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
    uint32_t num = 12345678;    
    sevenSegInit(sevenSegDataBus, sevenSegComsBus, 8);  
    sevenSegPutInt(num); 
    #asm("SEI")
    
    while(1)
    { 
    }
}
