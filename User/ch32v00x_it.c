/********************************** (C) COPYRIGHT *******************************
 * File Name          : ch32v00x_it.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2023/12/22
 * Description        : Main Interrupt Service Routines.
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for 
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/
#include <ch32v00x_it.h>
#include <PetitModbus.h>
#include <PetitModbusPort.h>

extern T_PETIT_MODBUS Petit;

void NMI_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void HardFault_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void TIM1_UP_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void USART1_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void TIM2_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));

/*********************************************************************
 * @fn      NMI_Handler
 *
 * @brief   This function handles NMI exception.
 *
 * @return  none
 */
void NMI_Handler(void)
{
  while (1)
  {
  }
}

/*********************************************************************
 * @fn      HardFault_Handler
 *
 * @brief   This function handles Hard Fault exception.
 *
 * @return  none
 */
void HardFault_Handler(void)
{
  while (1)
  {
  }
}

/*
 * timer 1 IRQ handler for the system ticking
 */

void TIM1_UP_IRQHandler(void)
{
	static u8 t1_internal = 0;
	M_T1_START();
    TIM_ClearFlag(TIM1, TIM_FLAG_Update);
	t1_internal += 1u;
	if (t1_internal >= 4)
	{
		t1_internal = 0;
    	t1_count += 1U;
		// modbus timer implementation
		if (modbus_arm == true 
				&& (t1_count - modbus_timer) >= C_MODBUS_CLEAR)

		{
			PetitRxBufferReset(&Petit);
			modbus_arm = false;
		}
	}
	M_T1_END();
	return;
}

/*
 * USART1_IRQHandler
 */
void USART1_IRQHandler(void)
{
	M_USART_START();
	pu8_t tmp;
	// transmission complete
	if (USART1->STATR & USART_STATR_TC)
	{
		// clear bit
		USART1->STATR &= ~USART_STATR_TC;
		if (Petit.Xmit_State == E_PETIT_RXTX_RX)
		{
			PetitPortDirRx();
		}
	}
	// transmission buffer empty
	if (USART1->STATR & USART_STATR_TXE)
	{
		// disable the interrupt or add more data
		if (PetitTxBufferPop(&Petit, &tmp) != 0u)
		{
			USART1->DATAR = tmp;
		}
		else
		{
			// disable interrupt
			USART1->CTLR1 &= ~USART_CTLR1_TXEIE;
		}
	}
	// receive buffer *not* empty
	if (USART1->STATR & USART_STATR_RXNE)
	{
		USART1->STATR = ~USART_STATR_RXNE;
		tmp = USART1->DATAR;
		PetitRxBufferInsert(&Petit, tmp);
	}
	M_USART_END();
	return;
}

void TIM2_IRQHandler(void)
{
	M_TIM2_START();
	// falling edge
	if ((TIM2->INTFR & TIM_IT_CC2) != RESET)
	{
		TIM2->INTFR = (u16)~TIM_IT_CC2; // clear interrupt bit
	}
	// update interrupt
	if ((TIM2->INTFR & TIM_IT_Update) != RESET)
	{
		TIM2->INTFR = (u16)~TIM_IT_Update;
		// register a reset condition
		if ((GPIOC->INDR & GPIO_Pin_2) != RESET)
		{
			// reset the structure if sent and new message is ready
			if (mvec & C_MVEC_READ && byte_i >= 7)
			{
				// read the message count and increment
				u8 count = ws_byte[M_MVEC_GET_RX(mvec)][0];
				count += 1;

				mvec ^= C_MVEC_RX;
				mvec &= (u8)~C_MVEC_READ; // clear "read" bit
				bit_i = 0;
				byte_i = 0;

				// write the message count
				ws_byte[M_MVEC_GET_RX(mvec)][0] = count;
			}

			// timer settings
    		TIM2->SMCFGR |= TIM_SlaveMode_Trigger; // set slave mode to trigger
    		TIM2->CTLR1 |= TIM_OPM; // one pulse mode
			TIM2->CTLR1 &= (u16)~TIM_CEN; // disable timer
		}
	}
	// sample
	if ((TIM2->INTFR & TIM_IT_CC1) != RESET)
	{
		TIM2->INTFR = (u16)~TIM_IT_CC1;
		u8 val = (GPIOC->INDR & GPIO_Pin_2) != RESET;
		// timer setting change
    	TIM2->SMCFGR &= (u16)~(0x2); // set slave mode to reset
    	TIM2->CTLR1 &= (u16)~TIM_OPM; // set to repetitive mode
    	TIM2->CTLR1 |= TIM_CEN; // run the timer continuously
		// bit processing
		if (bit_i >= 16)
		{
			bit_i = 0;
			// byte doublbing defaults to 0 if the bytes do not match
			u8 write_val = ws_bit[0u] == ws_bit[1u] ? ws_bit[1u] : 0u;
			if (byte_i < C_LEN_MSG)
			{
				ws_byte[M_MVEC_GET_RX(mvec)][byte_i++] = write_val;
			}
			else
			{
				byte_i = 1u;
				ws_byte[M_MVEC_GET_RX(mvec)][byte_i++] = write_val;
			}
			ws_bit[0] = 0;
			ws_bit[1] = 0;
		}
		// bit order is transmitted LSB to MSB
		ws_bit[bit_i >> 3U] |= (u8)(val << (bit_i & 0x7u));
		bit_i++;
	}
	M_TIM2_END();
	return;
}
