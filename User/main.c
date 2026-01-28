/*
 *  main.c
 *
 *  This file was adopted a long time ago and spent a lot of time sitting in
 *  prototyping, so an accurate date of its modification is not available.
 *  Author: kirisaki
 */

/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2023/12/22
 * Description        : Main program body.
 *********************************************************************************
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * Attention: This software (modified or not) and binary are used for 
 * microcontroller manufactured by Nanjing Qinheng Microelectronics.
 *******************************************************************************/

/*
 *@Note
 *This project is designed for the ch32v003JxMx SOP-8 package.
 *      Hardware connection:
 *            PC1 (5) -- Inverter
 *
 * 			  PA1 (1) -- UTX_2
 * 			  PA2 (3) -- Dir
 *
 */

#include "debug.h"
#include "init.h"
#include "consts.h"
#include <PetitModbus.h>

/* Constants */

/* Macros */

/* Types */

/* Global variables */
volatile u32 t1_count = 0;
u32 last_t1_count = 0;
u8 loop_overrun = 0;
T_PETIT_MODBUS Petit;
volatile u8 modbus_arm = false;
volatile u32 modbus_timer;
vu8 mvec = 0;
vu8 bit_i = 0;
vu8 byte_i = 0;
vu8 ws_bit[2] = {0};
vu8 ws_byte[6] = {0};

void PetitPortDirTx(void)
{
#if defined(SOP8)
	GPIOA->BSHR = GPIO_Pin_2;
#endif // HMI_PCB
#if defined(BOB)
	GPIOD->BSHR = GPIO_Pin_4;
#endif // BOB
}

void PetitPortDirRx(void)
{
#if defined(SOP8)
	GPIOA->BSHR = GPIO_Pin_2 << 16U;
#endif // HMI_PCB
#if defined(BOB)
	GPIOD->BSHR = GPIO_Pin_4 << 16U;
#endif // BOB
}

void PetitUserTxBegin(pu8_t data)
{
	PetitPortDirTx();
	// output the first octet
	USART1->DATAR = data;
	// enable the transmit empty interrupt
	USART1->CTLR1 |= USART_CTLR1_TXEIE;
}

void PetitT15TimerStart(void)
{
	modbus_arm = true;
	modbus_timer = t1_count;
}

void PetitT15TimerStop(void)
{
	modbus_arm = false;
}

/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
int main(void)
{
	SystemCoreClockUpdate();

	APP_GPIO_Init();

	TIME_Init();

	UART_Init();

	PETIT_MODBUS_Init(&Petit);
	Petit.Timer_Start = &PetitT15TimerStart;
	Petit.Timer_Stop = &PetitT15TimerStop;
	Petit.Tx_Begin = &PetitUserTxBegin;

	// start time
	TIM_Cmd(TIM1, ENABLE);
	TIM_ClearFlag(TIM1, TIM_FLAG_Update);

	IIC_TX(C_CH455_ADDR_SP, C_MY_CH455_SP);
	while (1U)
	{
		// main loop timer overflow
		if (t1_count - last_t1_count != 0U)
		{
			loop_overrun = 1U;
			M_LOOP_OVER();
		}

		// wait
		while (t1_count - last_t1_count == 0U)
		{
			// wfi stops the t1 system timer from time to time, so do not use it
		}
		M_MAIN_START();

		// process modbus
		PETIT_MODBUS_Process(&Petit);

		// increment by one to indicate one execution cycle
		last_t1_count += 1U;
		M_MAIN_END();
	}
}
