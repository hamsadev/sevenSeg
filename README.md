
# sevenSeg

A library for setting up 7 segments as a multiplexer for all types of microcontrollers. This library is under development and currently works only on AVR microcontrollers.

 - [x] putInt
 - [x] putString
 - [x] putFloat
 - [x] Common Anode and Common Cathode support
 - [x] multi instance support
 - [x] ~~auto refresh support~~

# Demo
![](https://github.com/hamsaco/sevenSeg/blob/master/header.gif)

# Tests
 - [x] AVR (atmega16)
 - [ ] stm32F1
 - [ ] stm32F3
 - [ ] stm32F4
 - [ ] stm32F7


## How to use

 - First, add the library to your project
 - include the seven_seg.h file
 - include "seven_seg_port.h" file
 
According to the number of seven segments and their number of digits, sevenSegRefreshIsr function should be called continuously.
If the time interval is too short, the interrupt rate may exceed the task time and you will have a virtual loop and the performance will not be correct.
And if the time interval is too long, you may encounter seven segment flashing.
This function is responsible for keeping seven segments alive.


TODO: update readme
