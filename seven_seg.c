/**
 * @file seven_seg.c
 * @author Hamid Salehi (hamsame.dev@gmail.com) * 
 * @date 2023-03-18
 * 
 */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "seven_seg.h"
#include "seven_seg_ascii.h"

static const SevenSeg_Driver* sevenSegDriver;

static SevenSeg_pinConfig* gDataBus;
static SevenSeg_pinConfig* gComBus;
static uint8_t gDigitsNum;
static uint8_t* sevenSegDigit;

static void initDataBus (SevenSeg_pinConfig* dataBus);
static void initComBus (SevenSeg_pinConfig* comBus, uint8_t len);
static uint32_t intPow(uint32_t x, uint32_t y);

void _resetComBus (void){
    SevenSeg_pinConfig* comBus = gComBus;
    uint8_t len = gDigitsNum;

    while(len-- > 0)
    {          
        #if SEVEN_SEG_IS_CA
        sevenSegDriver->writePin(comBus, GPIO_PIN_RESET);   
        #else                                    
        sevenSegDriver->writePin(comBus, GPIO_PIN_SET);
        #endif // SEVEN_SEG_IS_CA     
        comBus++;                
    }
}

void sevenSegInit(SevenSeg_pinConfig* dataBus, SevenSeg_pinConfig* comBus, uint8_t digitsNum, SevenSeg_Driver* driver){       
    sevenSegDigit = (uint8_t*)malloc(digitsNum);                   
    sevenSegDriver = driver;      
    gDigitsNum = digitsNum; 
    gDataBus = dataBus;
    gComBus  = comBus;
     
    // Initial dataBus                          
    initDataBus(dataBus); 
                      
    // Initial AddressBus
    initComBus(comBus, digitsNum);
}

void sevenSegPutInt(uint32_t num){
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

void sevenSegPuts(char* str){
    uint8_t i;
    for(i = 0; i < gDigitsNum; i++)
    {             
        sevenSegDigit[gDigitsNum - i - 1] = (*str != '\0') ? 
            SevenSegmentASCII[*str++ - 0x20] :
            SevenSegmentASCII[0];
    }
}

void sevenSegPutFloat(float num, uint8_t decimals){
    /*char* temp = malloc(gDigitsNum + 1);
    ftoa(num, decimals, temp);
    sevenSegPuts(temp);
    free(temp);*/          
           
    // TODO: save integer when decimals is too large      
    uint32_t temp = num * intPow(10, decimals);
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

void sevenSegRefreshIsr(void){
    uint8_t i;
    static uint8_t digitSelect = 0;   
        
    _resetComBus(); 
    for(i = 0; i < 8; i++)
    {                        
        uint8_t needSet = (sevenSegDigit[digitSelect] & (1 << i)) != 0; 
        sevenSegDriver->writePin(&gDataBus[i], needSet ? GPIO_PIN_SET : GPIO_PIN_RESET);           
    }  
    
    #if SEVEN_SEG_IS_CA != 0
    sevenSegDriver->writePin(&gComBus[digitSelect], GPIO_PIN_SET); 
    #else                                    
    sevenSegDriver->writePin(&gComBus[digitSelect], GPIO_PIN_RESET); 
    #endif // SEVEN_SEG_IS_CA != 0     
                          
    if(++digitSelect >= gDigitsNum)
    {
        digitSelect = 0;
    } 
}

static uint32_t intPow(uint32_t x, uint32_t y){   
    uint32_t ret = 1;
    while(y-- > 0)
    {
        ret *= x;        
    }       
    return ret;         
}

static void initDataBus (SevenSeg_pinConfig* dataBus){
    uint8_t len = 8;
    while(len-- > 0)
    {                    
        sevenSegDriver->initPin(dataBus);
        #if SEVEN_SEG_IS_CA != 0
        sevenSegDriver->writePin(dataBus, GPIO_PIN_RESET); 
        #else                                    
        sevenSegDriver->writePin(dataBus, GPIO_PIN_RESET); 
        #endif // SEVEN_SEG_IS_CA != 0   
        dataBus++; 
    } 
}

static void initComBus (SevenSeg_pinConfig* comBus, uint8_t len){
    while(len-- > 0)
    {                     
        sevenSegDriver->initPin(comBus); 
        #if SEVEN_SEG_IS_CA != 0
        sevenSegDriver->writePin(comBus, GPIO_PIN_RESET); 
        #else                                    
        sevenSegDriver->writePin(comBus, GPIO_PIN_SET); 
        #endif // SEVEN_SEG_IS_CA != 0  
        comBus++;
    }
}