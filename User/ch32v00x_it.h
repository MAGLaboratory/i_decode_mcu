/********************************** (C) COPYRIGHT *******************************
 * File Name          : ch32v00x_it.h
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2022/08/08
 * Description        : This file contains the headers of the interrupt handlers.
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for 
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/
#ifndef __CH32V00x_IT_H
#define __CH32V00x_IT_H

#include "debug.h"
#include "consts.h"

extern vu32 t1_count;
extern vu8 modbus_arm;
extern vu32 modbus_timer;
extern vu8 mvec;
extern vu8 bit_i;
extern vu8 byte_i;
extern vu8 ws_bit[2];
extern vu8 ws_byte[2][C_LEN_MSG];

#endif /* __CH32V00x_IT_H */