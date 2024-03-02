#include "delay.h"

static u8  fac_us=0;							//us延时倍乘数			   
static u16 fac_ms=0;							//ms延时倍乘数

/*******************************************************************************
* 函 数 名         : SysTick_Init
* 函数功能		   : SysTick初始化，SYSTICK的时钟固定为AHB时钟的1/8
* 输    入         : SYSCLK:系统时钟频率
* 输    出         : 无
*******************************************************************************/
void SysTick_Init(u8 SYSCLK)
{
 	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8); 
	fac_us=SYSCLK/8;						//SYSCLK的8分频 保存1us所需的计数次数
	fac_ms=(u16)fac_us*1000;				//每个ms需要的systick时钟数   
}

void delay_us(uint32_t nus)
{
	uint32_t temp;	 
	
	SysTick->CTRL = 0; // 关闭系统定时器
	SysTick->LOAD = nus*21; //设置重装置寄存器的值，设置计数值
	SysTick->VAL = 0; // 设置当前数值寄存器的值为0
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; //使能定时器
	
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));			//等待时间到达 
	
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk; 		//关闭计数器
	SysTick->VAL =0X00;    
	
}

void delay_ms(int nms)   //毫秒
{
    uint32_t m,n;
    m = nms/500;

    n = nms%500;
    while(m--)
    {
        SysTick->LOAD = (SystemCoreClock/8/1000) * 500;		//定时时间
        
        SysTick->CTRL |= 1;							//开启定时器，开始计数
        
        while((SysTick->CTRL & (1<<16)) == 0);		//等待定时时间到
        
        SysTick->CTRL &=~1;							//关闭定时器
    }
    if(n)
    {
        SysTick->LOAD = (SystemCoreClock/8/1000) * n;		//定时时间
        
        SysTick->CTRL |= 1;							//开启定时器，开始计数
        
        while((SysTick->CTRL & (1<<16)) == 0);		//等待定时时间到
        
        SysTick->CTRL &=~1;							//关闭定时器
    }	
}
