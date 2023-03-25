/**
 * @file seven_seg.c
 * @author Hamid Salehi (hamsame.dev@gmail.com) * 
 * @date 2023-03-18
 * 
 */

#include <io.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "seven_seg.h"
#include "seven_seg_ascii.h"

static SevenSegPin* gDataBus;
static SevenSegPin* gComBus;
static uint8_t gDigitsNum;
static uint8_t* sevenSegDigit;

#ifndef F_CPU
#define F_CPU 8000000UL
#endif //F_CPU

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

uint32_t _intPow(uint32_t x, uint32_t y)
{   
    uint32_t ret = 1;
    while(y-- > 0)
    {
        ret *= x;        
    }       
    return ret;         
}

void _setup_timer(uint8_t timer, uint16_t interval) {
  uint16_t prescaler;
  uint16_t timer_value;

  // set the prescaler based on the desired interval
  if (interval < 256) 
  {
    prescaler = 1;
  } 
  else if (interval < 2048) 
  {
    prescaler = 8;
  } 
  else if (interval < 16384) 
  {
    prescaler = 64;
  } 
  else if (interval <= 65535) 
  {
    prescaler = 256;
  } 
  else 
  {
    prescaler = 1024;
  }

  // calculate the timer value based on the prescaler and desired interval
  timer_value = ((F_CPU / prescaler) * interval) / 1000UL - 1;

  // set the prescaler and timer value for the selected timer
  switch (timer) 
  {
    case 0:
      TCCR0 |= (1 << CS00) | (1 << CS01); // set prescaler to 64
      OCR0 = timer_value; // set output compare register value
      TIMSK |= (1 << OCIE0); // enable timer compare interrupt
      break;
    case 1:
      TCCR1B |= (1 << CS10) | (1 << CS11); // set prescaler to 64
      OCR1A = timer_value; // set output compare register value
      TIMSK |= (1 << OCIE1A); // enable timer compare interrupt
      break;
    case 2:
      TCCR2 |= (1 << CS20) | (1 << CS21); // set prescaler to 64
      OCR2 = timer_value; // set output compare register value
      TIMSK |= (1 << OCIE2); // enable timer compare interrupt
      break;
    default:
      break;
  }
}

void sevenSegInit(SevenSegPin* dataBus, SevenSegPin* comBus, uint8_t digitsNum)
{       
    uint8_t i;              
    gDigitsNum = digitsNum;
    sevenSegDigit = (uint8_t*)malloc(digitsNum);      
    
    #if SEVEN_SEG_TIMER == 0   
    _setup_timer(0, 1);
    #elif SEVEN_SEG_TIMER == 1 
    _setup_timer(1, 1);
    #elif SEVEN_SEG_TIMER == 2  
    _setup_timer(2, 1);
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
    /*char* temp = malloc(gDigitsNum + 1);
    itoa(num, temp);
    sevenSegPuts(temp);
    free(temp);*/  

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

void sevenSegPutFloat(float num, uint8_t decimals)
{
    /*char* temp = malloc(gDigitsNum + 1);
    ftoa(num, decimals, temp);
    sevenSegPuts(temp);
    free(temp);*/          
           
    // TODO: save integer when decimals is too large      
    uint32_t temp = num * _intPow(10, decimals);
    uint8_t i;       
    for(i = 0; i < gDigitsNum; i++)
    {                                       
        sevenSegDigit[i] = sevenSegNum[temp % 10];
        temp /= 10;   
    }       
              
    // Set DP
    #if SEVEN_SEG_IS_CA     
    sevenSegDigit[decimals] &= ~0x80;     
    #else                     
    sevenSegDigit[decimals] |= 0x80;            
    #endif //SEVEN_SEG_IS_CA          
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
interrupt [TIM0_COMP] void timer0_ovf_isr(void)
{
    sevenSegRefreshIsr();
}
#elif SEVEN_SEG_TIMER == 1
interrupt [TIM1_COMP] void timer1_ovf_isr(void)
{    
    sevenSegRefreshIsr();
    
}
#elif SEVEN_SEG_TIMER == 2
interrupt [TIM2_COMP] void timer2_ovf_isr(void)
{
    sevenSegRefreshIsr();
}
#endif //SEVEN_SEG_TIMER  

