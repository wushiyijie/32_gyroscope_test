#include "tim.h"

void pwm_GPIO_init(void)
{
	TIM_OCInitTypeDef TIM_OCInitStruct;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	/*Enable TIM clock*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;  //GPIO Alternate function Mode
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	/* Connect TIM pins to AF9 */
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource13, GPIO_AF_TIM1);
	
	/* configure the Time Base  168MHz/16800 = 10000HZ*/
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;  	//二次分频
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;//向上计数
	TIM_TimeBaseInitStruct.TIM_Period = 100;          //计数10ms
	TIM_TimeBaseInitStruct.TIM_Prescaler = 16800 - 1;			//预分频，0~65535
	//TIM_TimeBaseInitStruct.TIM_RepetitionCounter =
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStruct);
	
	/* PWM1 Mode configuration: Channel1 */
	TIM_OCInitStruct.TIM_OCMode= TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_OCNPolarity= TIM_OCPolarity_High;
	TIM_OCInitStruct.TIM_OutputState= TIM_OutputState_Enable;
	TIM_OC3Init(TIM1,&TIM_OCInitStruct);
	
	
	//自动重载初值，不断输出PWM脉冲
	TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);
  //自动重载使能				
	TIM_ARRPreloadConfig(TIM1, ENABLE);														
	/* TIM14 enable counter */
	TIM_Cmd(TIM1, ENABLE);
	
	TIM_CtrlPWMOutputs(TIM1,ENABLE);
	
	
}

void tim1_init(void)
{
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	/* 使能GPIOE与GPIOF的硬件时钟 */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

	/* GPIOF Configuration: TIM1 CH3 (PE13) 和 CH4 (PE14) */
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_13 | GPIO_Pin_14  ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;			//复用功能，使用引脚的第二功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	/* Connect TIM pins to AE13 */
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource13, GPIO_AF_TIM1);
	
	/* Connect TIM pins to AE14 */
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource14, GPIO_AF_TIM1);
	
	/*定时器 TIM1时钟使能  */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	
	/* Time base configuration，100Hz*/
	TIM_TimeBaseStructure.TIM_Period = (10000/100)-1;					//定时计数值,100Hz
	TIM_TimeBaseStructure.TIM_Prescaler = 16800;						//预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;				//再次进行1分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;			//向上计数
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

	/* PWM1 Mode configuration: Channel3 Channel4 */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

	
	TIM_OC3Init(TIM1,&TIM_OCInitStructure);                             //TIM1通道3初始化
	TIM_OC4Init(TIM1,&TIM_OCInitStructure);                             //TIM1通道4初始化

	TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);   				//自动重载初值，不断输出PWM脉冲
	TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);					//自动重载初值，不断输出PWM脉冲
	TIM_ARRPreloadConfig(TIM1, ENABLE);								    //自动重载使能	

	/* TIM1 enable counter */
	TIM_Cmd(TIM1, ENABLE);

	//使能TIM1PWM输出，参考帮助文档TIM/TIM_7PWMOutput
	TIM_CtrlPWMOutputs(TIM1,ENABLE);
}

void Time1_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	/*Enable TIM clock*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
	
	/* configure the Time Base  168MHz/16800 = 10000HZ*/
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV2;  	//二次分频
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;//向上计数
	TIM_TimeBaseInitStruct.TIM_Period =  10000;          
	TIM_TimeBaseInitStruct.TIM_Prescaler = 16800 - 1;			//预分频，0~65535
	//TIM_TimeBaseInitStruct.TIM_RepetitionCounter =
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStruct);
	
	/*Initializes the NVIC */
	NVIC_InitStruct.NVIC_IRQChannel = TIM1_UP_TIM10_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	
	NVIC_Init(&NVIC_InitStruct);
	/*Enable the corresponding interrupt*/
	TIM_ITConfig(TIM1, TIM_IT_Update,ENABLE);
	
	TIM_Cmd(TIM1,ENABLE);
	
}



void TIM1_UP_TIM10_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)
	{
		printf("111");
		PFout(10) ^= 1;	
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
	}

}

