// lengths
#define C_LEN_MSG (7u)

// message vector constants
#define C_MVEC_RX ((u8)(1u << 0u))
#define C_MVEC_READ ((u8)(1u << 1u))
#define M_MVEC_GET_RX(v) ((size_t)(v & C_MVEC_RX))
#define M_MVEC_GET_TX(v) ((size_t)((u8)(~v) & C_MVEC_RX))

// loop constants
#define C_SLOWER_CYCLE (4U)

// modbus timer constants 
#define C_MODBUS_CLEAR (4U)

// breakout board
//#define BOB = 1
// PCB
#define SOP8 = 1

#if defined(BOB) && defined(SOP8)
#error "Only one board is supposed to be defined"
#endif // BOB && SOP8

#if !defined(BOB) && !defined(SOP8)
#error "At least one board type must be defined"
#endif

// debug pins
#if defined(BOB)
#define M_DBG_0_ACT() GPIOD->BSHR = GPIO_Pin_0;
#define M_DBG_0_INA() GPIOD->BSHR = GPIO_Pin_0 << 16U;
#define M_DBG_1_ACT() GPIOC->BSHR = GPIO_Pin_0;
#define M_DBG_1_INA() GPIOC->BSHR = GPIO_Pin_0 << 16U;
#define M_DBG_2_ACT() GPIOC->BSHR = GPIO_Pin_3;
#define M_DBG_2_INA() GPIOC->BSHR = GPIO_Pin_3 << 16U;
#define M_DBG_3_ACT() GPIOC->BSHR = GPIO_Pin_5;
#define M_DBG_3_INA() GPIOC->BSHR = GPIO_Pin_5 << 16U;
#define M_DBG_4_ACT() GPIOC->BSHR = GPIO_Pin_6;
#define M_DBG_4_INA() GPIOC->BSHR = GPIO_Pin_6 << 16U;
#define M_DBG_5_ACT() GPIOC->BSHR = GPIO_Pin_7;
#define M_DBG_5_INA() GPIOC->BSHR = GPIO_Pin_7 << 16U;
#define M_DBG_6_ACT() GPIOD->BSHR = GPIO_Pin_2;
#define M_DBG_6_INA() GPIOD->BSHR = GPIO_Pin_2 << 16U;
#define M_DBG_7_ACT() GPIOD->BSHR = GPIO_Pin_3;
#define M_DBG_7_INA() GPIOD->BSHR = GPIO_Pin_3 << 16U;
#endif // BOB

// debug output functions
#if defined(SOP8)
#define M_T1_START()
#define M_T1_END()
#define M_TIM2_START()
#define M_TIM2_END()
#define M_USART_START()
#define M_USART_END()
#define M_LOOP_OVER()
#define M_MAIN_START()
#define M_MAIN_END()
#endif // SOP8

#if defined(BOB)
#define M_T1_START() M_DBG_2_ACT()
#define M_T1_END() M_DBG_2_INA()
#define M_TIM2_START()
#define M_TIM2_END()
#define M_USART_START() M_DBG_1_ACT()
#define M_USART_END() M_DBG_1_INA()
#define M_LOOP_OVER() M_DBG_7_ACT()
#define M_MAIN_START() M_DBG_0_ACT()
#define M_MAIN_END() M_DBG_0_INA()
#endif // BOB
