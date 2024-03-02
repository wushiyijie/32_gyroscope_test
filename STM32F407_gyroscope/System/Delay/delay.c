#include "delay.h"

static u8  fac_us=0;							//us��ʱ������			   
static u16 fac_ms=0;							//ms��ʱ������

/*******************************************************************************
* �� �� ��         : SysTick_Init
* ��������		   : SysTick��ʼ����SYSTICK��ʱ�ӹ̶�ΪAHBʱ�ӵ�1/8
* ��    ��         : SYSCLK:ϵͳʱ��Ƶ��
* ��    ��         : ��
*******************************************************************************/
void SysTick_Init(u8 SYSCLK)
{
 	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8); 
	fac_us=SYSCLK/8;						//SYSCLK��8��Ƶ ����1us����ļ�������
	fac_ms=(u16)fac_us*1000;				//ÿ��ms��Ҫ��systickʱ����   
}

void delay_us(uint32_t nus)
{
	uint32_t temp;	 
	
	SysTick->CTRL = 0; // �ر�ϵͳ��ʱ��
	SysTick->LOAD = nus*21; //������װ�üĴ�����ֵ�����ü���ֵ
	SysTick->VAL = 0; // ���õ�ǰ��ֵ�Ĵ�����ֵΪ0
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; //ʹ�ܶ�ʱ��
	
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));			//�ȴ�ʱ�䵽�� 
	
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk; 		//�رռ�����
	SysTick->VAL =0X00;    
	
}

void delay_ms(int nms)   //����
{
    uint32_t m,n;
    m = nms/500;

    n = nms%500;
    while(m--)
    {
        SysTick->LOAD = (SystemCoreClock/8/1000) * 500;		//��ʱʱ��
        
        SysTick->CTRL |= 1;							//������ʱ������ʼ����
        
        while((SysTick->CTRL & (1<<16)) == 0);		//�ȴ���ʱʱ�䵽
        
        SysTick->CTRL &=~1;							//�رն�ʱ��
    }
    if(n)
    {
        SysTick->LOAD = (SystemCoreClock/8/1000) * n;		//��ʱʱ��
        
        SysTick->CTRL |= 1;							//������ʱ������ʼ����
        
        while((SysTick->CTRL & (1<<16)) == 0);		//�ȴ���ʱʱ�䵽
        
        SysTick->CTRL &=~1;							//�رն�ʱ��
    }	
}
