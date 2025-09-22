#include "debug.h"

#pragma import(__use_no_semihosting_swi)
struct __FILE { int handle;  };
FILE __stdout;
FILE __stdin;
int fputc(int ch, FILE *f) 
{
	while(USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET);
        USART_SendData(USART1, ch);
    return ch;
}
void _sys_exit(int return_code)
{
	label: goto label;
}

volatile uint16_t tmp;

void Debug_WriteLine(const char *str)
{
	const char *p=str;
	while(p&&*p!='\0')
    {
		 while(USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET);
	     USART_SendData(USART1, *p);
	     p++;
	}
}

void Debug_Init(uint32_t baud)
{
	GPIO_InitTypeDef         GPIO_InitStructure;
	USART_InitTypeDef        USART_InitStructure;
	NVIC_InitTypeDef         NVIC_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource6,GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource7,GPIO_AF_USART1);
	GPIO_InitStructure.GPIO_Pin= GPIO_Pin_6| GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed= GPIO_High_Speed ;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	USART_InitStructure.USART_BaudRate = baud;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	USART_Cmd(USART1,ENABLE);
}

void USART1_IRQHandler(void)
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE)==SET)
	{
	    tmp = USART_ReceiveData(USART1);
		while(USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET);
        USART_SendData(USART1,tmp);
	}
}
