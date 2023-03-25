/**
 * @file seven_seg.c
 * @author Hamid Salehi (hamsame.dev@gmail.com) * 
 * @date 2023-03-18
 * 
 */

#include <io.h>
#include <stdint.h>
#include <stdlib.h>
#include "seven_seg.h"
#include "seven_seg_ascii.h"

static SevenSegPin* gDataBus;
static SevenSegPin* gComBus;
static uint8_t gDigitsNum;
static uint8_t* sevenSegDigit;

void _resetComBus (void)
{
    uint8_t i;
    for(i = 0; i < gDigitsNum; i++)
    {          
        #if SEVEN_SEG_IS_CA != 0 
        gComBus[i].gpio->PORT.reg &= ~(1 << gComBus[i].pin);        
        #else                                    
        gComBus[i].gpio->PORT.reg |= (1 << gComBus[i].pin);
        #endif // SEVEN_SEG_IS_CA != 0                        
    }
}

void sevenSegInit(SevenSegPin* dataBus, SevenSegPin* comBus, uint8_t digitsNum)
{       
    uint8_t i;              
    gDigitsNum = digitsNum;
    sevenSegDigit = (uint8_t*)malloc(digitsNum);      
    
    #if SEVEN_SEG_TIMER == 0   
    TCCR0 = (1 << CS01) | (1 << CS00);
    TCNT0 = 0x83;                    
    OCR0 = 0x00;
    TIMSK |= (1 << TOIE0); 
    #elif SEVEN_SEG_TIMER == 1 
    TCCR1A = 0;
    TCCR1B = (1 << CS10);
    TCNT1H = 0xE0;
    TCNT1L = 0xC0;
    ICR1H = 0x00;
    ICR1L = 0x00;
    OCR1AH = 0x00;
    OCR1AL =0x00;
    OCR1BH = 0x00;
    OCR1BL = 0x00;
    TIMSK |= (1 << TOIE1); 
    #elif SEVEN_SEG_TIMER == 2  
    ASSR= 0 << AS2;
    TCCR2 = (1 << CS21) | (1 << CS20);
    TCNT2 = 0x06;
    TIMSK = (1 << TOIE2); 
    #endif   
    
    gDataBus = dataBus;
    gComBus  =  comBus;
     
    // Initial dataBus                          
    for(i = 0; i < 8; i++)
    {                    
        dataBus[i].gpio->DDR.reg |= 1 << dataBus[i].pin; 
        #if SEVEN_SEG_IS_CA != 0
        dataBus[i].gpio->PORT.reg |= 1 << dataBus[i].pin;
        #else                                    
        dataBus[i].gpio->PORT.reg &= ~(1 << dataBus[i].pin);
        #endif // SEVEN_SEG_IS_CA != 0
    }  
                      
    // Initial AddressBus
    for(i = 0; i < digitsNum; i++)
    {                     
        comBus[i].gpio->DDR.reg |= 1 << dataBus[i].pin;
        #if SEVEN_SEG_IS_CA != 0
        comBus[i].gpio->PORT.reg &= ~(1 << comBus[i].pin);
        #else                                    
        comBus[i].gpio->PORT.reg |= 1 << comBus[i].pin;
        #endif // SEVEN_SEG_IS_CA != 0
    }
}

void sevenSegPutInt(uint32_t num)
{
    uint8_t i;
    for(i = 0; i < gDigitsNum; i++)
    {                      
        sevenSegDigit[i] = sevenSegNum[num % 10];
        num /= 10;
    }
}

void sevenSegPuts(char* str)
{
    uint8_t i;
    for(i = 0; i < gDigitsNum; i++)
    {             
        sevenSegDigit[gDigitsNum - i - 1] = (*str != '\0') ? 
            SevenSegmentASCII[*str++ - 0x20] :
            SevenSegmentASCII[0];
    }
}


void sevenSegRefreshIsr(void)                                      
{
    uint8_t i;
    static uint8_t digitSelect = 0;   
        
    _resetComBus(); 
    for(i = 0; i < 8; i++)
    {                        
        uint8_t needSet = (sevenSegDigit[digitSelect] & (1 << i)) != 0; 
        if(needSet)
        { 
            gDataBus[i].gpio->PORT.reg |= needSet << i;  
        }
        else
        {  
            gDataBus[i].gpio->PORT.reg &= ~(1 << i);
        }     
    }  
    
    #if SEVEN_SEG_IS_CA != 0
    gComBus[digitSelect].gpio->PORT.reg |= (1 << gComBus[digitSelect].pin);
    #else                                    
    gComBus[digitSelect].gpio->PORT.reg &= ~(1 << gComBus[digitSelect].pin);
    #endif // SEVEN_SEG_IS_CA != 0     
                          
    if(++digitSelect >= gDigitsNum)
    {
        digitSelect = 0;
    } 
}

#if SEVEN_SEG_TIMER == 0
interrupt [TIM0_OVF] void timer0_ovf_isr(void)
{
    sevenSegRefreshIsr();
    TCNT0=0x83;
}
#elif SEVEN_SEG_TIMER == 1
interrupt [TIM1_OVF] void timer1_ovf_isr(void)
{    
    sevenSegRefreshIsr();
    TCNT1H=0xE0C0 >> 8;
    TCNT1L=0xE0C0 & 0xff;
    
}
#elif SEVEN_SEG_TIMER == 2
interrupt [TIM2_OVF] void timer2_ovf_isr(void)
{
    sevenSegRefreshIsr();
    TCNT2=0x06;
}
#endif //SEVEN_SEG_TIMER  

