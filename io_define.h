/**
 * @file io_define.h
 * @author Hamid Salehi (hamsame.dev@gmail.com)
 * @date 2023-03-18
 * 
 */

#ifndef IO_DEFINE_H_
#define IO_DEFINE_H_

#include <io.h>

typedef struct {
    uint8_t bit0 : 1;
    uint8_t bit1 : 1;
    uint8_t bit2 : 1;
    uint8_t bit3 : 1;
    uint8_t bit4 : 1;
    uint8_t bit5 : 1;
    uint8_t bit6 : 1;
    uint8_t bit7 : 1;
} BitAccess;

typedef union{
    BitAccess bits;
    uint8_t reg;
}Register;

typedef struct{
    Register PIN;
    Register DDR;
    Register PORT;
}Gpio;


#define GPIOA   (Gpio *)&PINA             //(*(Gpio*)0x3A)
#define GPIOB   (Gpio *)&PINB             //(*(Gpio*)0x36)
#define GPIOC   (Gpio *)&PINC             //(*(Gpio*)0x33)
#define GPIOD   (Gpio *)&PIND             //(*(Gpio*)0x30)



#endif /* IO_DEFINE_H_ */
