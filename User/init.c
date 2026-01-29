/*
 * init.c
 *
 *  Created on: Mar 27, 2024
 *      Author: brandon
 */
#include "init.h"
#include "consts.h"

/*********************************************************************
 * @fn      UART_Init
 *
 * @brief   Initializes the UART peripheral
 *
 * Under SOP8, pin PD6 is initialized here
 *
 * Under BOB, pin PD5 and PD6 are initialized here
 *
 * @return  none
 */
void UART_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure = { 0 };
	USART_InitTypeDef USART_InitStructure = { 0 };

	RCC_APB2PeriphClockCmd(
		RCC_APB2Periph_USART1 |
		RCC_APB2Periph_GPIOD |
		RCC_APB2Periph_AFIO, ENABLE);

#if defined(SOP8)
	// pin 1 with PD6 is used as the UART half-duplex pin
	// this pin is high by default.
	GPIOD->BSHR = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	GPIO_PinRemapConfig(GPIO_PartialRemap2_USART1, ENABLE);
#endif //HMI PCB

#if defined(BOB)
	// Pin 2 PD5
	GPIOD->BSHR = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	// Pin 3 PD6
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
#endif // BOB

	USART_InitStructure.USART_BaudRate = 38400;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl 
			= USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;

	USART_Init(USART1, &USART_InitStructure);
	USART_Cmd(USART1, ENABLE);

#if defined(SOP8)
	USART_HalfDuplexCmd(USART1, ENABLE);
#endif // SOP8
	// interrputs
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART1->CTLR1 |= USART_CTLR1_TCIE;

	NVIC_EnableIRQ(USART1_IRQn);
}

/*********************************************************************
 * @fn      APP_GPIO_Init
 *
 * @brief   Initializes GPIOs (buttons) for the application
 *
 * @return  none
 */
void APP_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(
			RCC_APB2Periph_GPIOA |
			RCC_APB2Periph_GPIOC |
			RCC_APB2Periph_GPIOD, ENABLE);

#if defined(SOP8)
	// Pin 1 is handled by the UART function
	
	// Pin 2 is the VSS pin

	// Pin 3: DIR (PA2)
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	// Pin 4 is the VDD pin
	//
	// Pin 5 
	//
	// Pin 6: T2CH2 inverter input (PC2)
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	// Pin 7: Debug? (PC4)
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
#endif // SOP8
#if defined(BOB)
	// Pin 1 is DIR (PD4)
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	// Pin 2 is handled by the UART function

	// Pin 3 is handled by the UART function

	// Pin 4 is the reset pin

	// Pin 5 is handled by the oscillator

	// Pin 6 is handled by the oscillator

	// Pin 7 is the VSS pin

	// Pin 8 PD0
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	// Pin 9 is the VDD pin

	// Pin 10 PC0
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	// Pin 11 is handled in the I2C function

	// Pin 12: T2CH2 inverter input (PC2)
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	// Pin 13 PC3
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	// Pin 14 is PC4 which is used for the relay
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	// Pin 15 is PC5
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	// Pin 16 is PC6
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	// Pin 17 is PC7 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	// Pin 18 is SWIO

	// Pin 19 PD2
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	// Pin 20 is PD3
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
#endif // BOB
}

/*********************************************************************
 * @fn      TIME_Init
 *
 * @brief   Initializes the timer for the 4 kHz system clock.
 *
 * The system clock should be 4 kHz in order to satisfy the minimum 250 us
 * common denominator from T_1.5 and T_3.5 on modbus.
 * The system clock would have to be 16 kHz to satisfy the relay PWM drive
 * requirement.  This would force the timer 1 ISR to divide the call rate by 4
 * {system clock} / {desired fcy} = {scaler}
 * 24e6 / 16e3 = 1 500
 * {prescaler} * {period} = {scale}
 * 15 * 100 / 1 500
 *
 * Also, 100 is a good period rate for the PWM drive because it will directly
 * represent a duty cycle percentage.
 * 
 * TIM2 is initialized to have a period of 3.5 inverter bytes
 * @return  none
 */
void TIME_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitSt = {0};
	TIM_ICInitTypeDef TIM_ICInitSt = {0};
	TIM_OCInitTypeDef TIM_OCInitSt = {0};

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	TIM_DeInit(TIM1);

	// configure timebase TIM1
	TIM_TimeBaseInitSt.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitSt.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitSt.TIM_Period = 100U - 1U;
	TIM_TimeBaseInitSt.TIM_Prescaler = 15U - 1U;
	TIM_TimeBaseInitSt.TIM_RepetitionCounter = 0U;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitSt);

	TIM_ARRPreloadConfig(TIM1, ENABLE);
	TIM_InternalClockConfig(TIM1);
	TIM_SelectOutputTrigger(TIM1, TIM_TRGOSource_Update);

	// configure interrupts
	TIM_ClearFlag(TIM1, TIM_FLAG_Update);
	TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);
	NVIC_SetPriority(TIM1_UP_IRQn, NVIC_PriorityGroup_1);
	NVIC_EnableIRQ(TIM1_UP_IRQn);

	// configure timebase TIM2
	TIM_TimeBaseStructInit(&TIM_TimeBaseInitSt);
	TIM_TimeBaseInitSt.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitSt.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitSt.TIM_Period = 3514U - 1U;
	TIM_TimeBaseInitSt.TIM_Prescaler = 52U - 1U;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitSt);

	TIM_InternalClockConfig(TIM2);
	TIM_SelectInputTrigger(TIM2, TIM_TS_TI2FP2);
	TIM_SelectSlaveMode(TIM2, TIM_SlaveMode_Trigger);
	TIM_SelectOnePulseMode(TIM2, TIM_OPMode_Single);
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	NVIC_SetPriority(TIM2_IRQn, NVIC_PriorityGroup_1);
	NVIC_EnableIRQ(TIM2_IRQn);

	// configure OC1 on TIM2
	TIM_OCStructInit(&TIM_OCInitSt);
	TIM_OCInitSt.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitSt.TIM_OutputState = TIM_OutputState_Enable; // debugging
	TIM_OCInitSt.TIM_OutputNState = TIM_OutputNState_Disable;
	TIM_OCInitSt.TIM_Pulse = 88U;
	TIM_OCInitSt.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitSt.TIM_OCNPolarity = TIM_OCPolarity_High;
	TIM_OCInitSt.TIM_OCIdleState = TIM_OCIdleState_Reset;
	TIM_OCInitSt.TIM_OCNIdleState = TIM_OCIdleState_Reset;
	TIM_OC1Init(TIM2, &TIM_OCInitSt);
	TIM_ITConfig(TIM2, TIM_IT_CC1, ENABLE);

	TIM_CtrlPWMOutputs(TIM2, ENABLE); // debugging

	// configure IC2 on TIM2
	TIM_ICStructInit(&TIM_ICInitSt);
	TIM_ICInitSt.TIM_Channel = TIM_Channel_2;
	TIM_ICInitSt.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitSt.TIM_ICFilter = 0;
	TIM_ICInitSt.TIM_ICPolarity = TIM_ICPolarity_Rising;
	TIM_ICInitSt.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInit(TIM2, &TIM_ICInitSt);

	TIM_ITConfig(TIM2, TIM_IT_CC2, ENABLE);
}
