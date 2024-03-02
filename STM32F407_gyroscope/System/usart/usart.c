#include "usart.h"


//重定义fputc函数 
int fputc(int ch, FILE *f)
{ 	
	USART_SendData(USART1,ch);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);  	
	return ch;
}

void Usart(int bind)
{
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	//Enable peripheral外围和GPIOA clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	
	//GPIO配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	//上拉
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_10;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);
	
	USART_InitStructure.USART_BaudRate = bind;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);
	
	//开启相关中断 USART_IT_RXNE:接收数据寄存器不是空中断
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	/* Enable USART */
	USART_Cmd(USART1, ENABLE);
	
	/* Enable the USARTx Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}

////接收数据
//uint16_t data = 65;


extern char buf[50];
extern uint8_t cnt;
extern uint8_t flag;

void USART1_IRQHandler(void)
{
	uint16_t data;

	/* 串口接收中断 */
	if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
	{
		if(flag == 0)
		{
			
			/* 读取串口1接收到的数据 */
			data =  USART_ReceiveData(USART1);
			buf[cnt++] = data;
			
			//超过界限重新开始接收
			if(cnt == 50)
				cnt = 0;
			
			//接收到结束符
			if(data == '\n')
				flag = 1;
			
			/* 将接收到的数据发生到串口1 
			USART_SendData(USART1, data);
			//等待数据发送完成
			while( USART_GetFlagStatus(USART1, USART_FLAG_TXE)==RESET);
			*/
		
		}
		
		/* 清除串口中断标志 */
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	}
}


