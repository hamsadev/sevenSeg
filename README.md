
# sevenSeg

A library for setting up 7 segments as a multiplexer for all types of microcontrollers. This library is under development and currently works only on AVR microcontrollers.

 - [x] putInt
 - [x] putString
 - [ ] putFloat
 - [x] Common Anode and Common Cathode support
 - [ ] multi instance support
 - [x] auto refresh support

## How to use

 - First, add the library to your project
 - enter the seven_seg.h file and set SEVEN_SEG_TIMER and SEVEN_SEG_IS_CA
 
 ```
 /*
    TIMER0 -> 0
    TIMER1 -> 1
    TIMER2 -> 2
    MANUAL -> -1
*/ 
#define SEVEN_SEG_TIMER         0    

/*
    COMMON ANODE -> 0
    COMMON CATHODE -> 1
*/   
#define SEVEN_SEG_IS_CA         0  
 ```
In this library, sevenSegRefreshIsr should be called at maximum speed. This function is responsible for keeping seven segments alive.
But the good news is that you don't have to call this function.
By setting SEVEN_SEG_TIMER to the numbers (0, 1, 2), the library starts a timer by calling the sevenSegInit function at the beginning of the program and calls the sevenSegRefreshIsr function every 1ms.
If the SEVEN_SEG_TIMER parameter is set to -1, the sevenSegRefreshIsr function will be exposed and the user can call the sevenSegRefreshIsr function at any time interval he wants.
 -  Defining pins related to data bus and address
 ```
 #define SEVEN_SEG_DIGITS_NUM   5

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
SevenSegPin sevenSegComsBus[SEVEN_SEG_DIGITS_NUM] = {
    {GPIOD,0},
    {GPIOD,1},
    {GPIOD,2},
    {GPIOD,3},
    {GPIOD,4},
    {GPIOD,5},
    {GPIOD,6},
    {GPIOD,7},
};
 ```
 - Passing the data bus, address bus and the number of digits to the sevenSegInit function
 ```
 sevenSegInit(sevenSegDataBus, sevenSegComsBus, SEVEN_SEG_DIGITS_NUM);  
 ```
  - Enable global interrupt
  - The library is ready to work and by calling the function sevenSegPutInt you can display any integer on the seven segment
  ```
  sevenSegPutInt(12345); 
  ```


## Example
```
// Code vision avr project

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

```
