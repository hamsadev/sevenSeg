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

typedef struct {
#if USE_DYNAMIC_MEMORY
    SevenSegment** sevenSegs;
#else
    SevenSegment* sevenSegs[SEVEN_SEG_MAX_NUM];
#endif //USE_DYNAMIC_MEMORY    
    uint8_t len;
}SevenSegHandler;

static const SevenSeg_Driver* sevenSegDriver;
static SevenSegHandler handler;

static void initDataBus (SevenSeg_pinConfig* dataBus);
static void initComBus (SevenSeg_pinConfig* comBus, uint8_t len);
static uint32_t intPow(uint32_t x, uint32_t y);
static void resetComBus (SevenSegment* sg);

void sevenSegAdd(SevenSegment* sg, SevenSeg_pinConfig* dataBus, SevenSeg_pinConfig* comBus, uint8_t digitsNum){
#if !USE_DYNAMIC_MEMORY
    if(sg == NULL || handler.len >= SEVEN_SEG_MAX_NUM)
    {  
        return;
    }                 
#endif //!USE_DYNAMIC_MEMORY
    
    sg->dataBus = dataBus;
    sg->comBus = comBus;
    sg->digitNum = digitsNum;  
    sg->currentDigit = 0;
#if USE_DYNAMIC_MEMORY
    sg->digit = (uint8_t*)malloc(digitsNum);  
    handler.sevenSegs = handler.sevenSegs ? 
        realloc(handler.sevenSegs, sizeof(SevenSegment*) * (handler.len + 1) ) :
        malloc(sizeof(SevenSegment*));
#endif //USE_DYNAMIC_MEMORY                                                 

    handler.sevenSegs[handler.len++] = sg;    
    
    // Initial dataBus                          
    initDataBus(dataBus); 
                      
    // Initial AddressBus
    initComBus(comBus, digitsNum);
}

void sevenSegInit(SevenSeg_Driver* driver){
    sevenSegDriver = driver;    
    handler.len = 0;
    handler.sevenSegs = NULL;
}

void sevenSegPutInt(SevenSegment* sg, uint32_t num){
    uint8_t i;
    if(sg == NULL)
    {  
        return;
    }       
    
    /*char* temp = malloc(sg->digitNum + 1);
    itoa(num, temp);
    sevenSegPuts(temp);
    free(temp);*/  

    for(i = 0; i < sg->digitNum; i++)
    {                      
        sg->digit[i] = sevenSegNum[num % 10];
        num /= 10;
    }
}

void sevenSegPuts(SevenSegment* sg, char* str){
    uint8_t len = sg->digitNum;
    uint8_t* probe = sg->digit + sg->digitNum - 1;
    
    if(sg == NULL)
    {  
        return;
    }
    
    while(len-- > 0)
    {             
        *probe-- = (*str != '\0') ? 
            SevenSegmentASCII[*str - 0x20] :
            SevenSegmentASCII[0];
        str++;
    }
}

void sevenSegPutFloat(SevenSegment* sg, float num, uint8_t decimals){
    uint32_t temp = num * intPow(10, decimals);
    uint8_t len = sg->digitNum;
    uint8_t* probe = sg->digit;
    
    if(sg == NULL)
    {  
        return;
    }
    
    /*char* temp = malloc(sg->digitNum + 1);
    ftoa(num, decimals, temp);
    sevenSegPuts(temp);
    free(temp);*/          
         
    while(len-- > 0)
    {                                       
        *probe++ = sevenSegNum[temp % 10];
        temp /= 10;   
    }       
              
    // Set DP
    #if SEVEN_SEG_IS_CA     
    sg->digit[decimals] &= ~0x80;     
    #else                     
    sg->digit[decimals] |= 0x80;            
    #endif //SEVEN_SEG_IS_CA          
}

void sevenSegRefreshIsr(void){

    uint8_t i;     
    uint8_t seg;
                    
    for(seg = 0; seg < handler.len; seg++)
    {                    
        uint8_t currentDigit = handler.sevenSegs[seg]->currentDigit;  
                    
        resetComBus(handler.sevenSegs[seg]); 
        for(i = 0; i < 8; i++)
        {                        
            uint8_t needSet = (handler.sevenSegs[seg]->digit[currentDigit] & (1 << i)) != 0; 
            sevenSegDriver->writePin(&handler.sevenSegs[seg]->dataBus[i], needSet ? GPIO_PIN_SET : GPIO_PIN_RESET);           
        }  
        
        #if SEVEN_SEG_IS_CA != 0
        sevenSegDriver->writePin(&handler.sevenSegs[seg]->comBus[currentDigit], GPIO_PIN_SET); 
        #else                                    
        sevenSegDriver->writePin(&handler.sevenSegs[seg]->comBus[currentDigit], GPIO_PIN_RESET); 
        #endif // SEVEN_SEG_IS_CA != 0 
            
        handler.sevenSegs[seg]->currentDigit++;                      
        if(currentDigit >= handler.sevenSegs[seg]->digitNum - 1)
        {
            handler.sevenSegs[seg]->currentDigit = 0;
        } 
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

static void resetComBus (SevenSegment* sg){
    SevenSeg_pinConfig* comBus = sg->comBus;
    uint8_t len = sg->digitNum;

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
