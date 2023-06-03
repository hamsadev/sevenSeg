/**
 * @file seven_seg_port.h
 * @author Hamid Salehi (hamsame.dev@gmail.com)
 * @brief this file show how to port lib into new hardware
 *          and have multiple of pre-implemented ports
 * @date 2023-03-06
 * 
 * @copyright Copyright (c) 2021 Hamid_Salehi
 * 
 */

#ifndef _SEVEN_SEG_PORT_H_
#define _SEVEN_SEG_PORT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "seven_seg.h"

extern const SevenSeg_Driver SEVEN_SEG_DRIVER;

void SevenSeg_initPin(const SevenSeg_pinConfig* config);
void SevenSeg_writePin(const SevenSeg_pinConfig* config, GPIO_PinState state);

#ifdef __cplusplus
};
#endif

#endif /* _SEVEN_SEG_PORT_H_ */
