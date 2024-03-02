#include "flash.h"

/***************
功能：通过操作的地址获取FLASH中的扇区号
参数：Address代表的是要操作的存储器地址
返回：地址所在扇区的号码
****************/
uint32_t GetSector(uint32_t Address)
{
	uint32_t sector = 0;
	
	if((Address < ADDR_FLASH_SECTOR_1) && (Address >= ADDR_FLASH_SECTOR_0))
	{
		sector = FLASH_Sector_0;  
	}
	
	else if((Address < ADDR_FLASH_SECTOR_2) && (Address >= ADDR_FLASH_SECTOR_1))
	{
		sector = FLASH_Sector_1;  
	}
	else if((Address < ADDR_FLASH_SECTOR_3) && (Address >= ADDR_FLASH_SECTOR_2))
	{
		sector = FLASH_Sector_2;  
	}
	else if((Address < ADDR_FLASH_SECTOR_4) && (Address >= ADDR_FLASH_SECTOR_3))
	{
		sector = FLASH_Sector_3;  
	}
	else if((Address < ADDR_FLASH_SECTOR_5) && (Address >= ADDR_FLASH_SECTOR_4))
	{
		sector = FLASH_Sector_4;  
	}
	else if((Address < ADDR_FLASH_SECTOR_6) && (Address >= ADDR_FLASH_SECTOR_5))
	{
		sector = FLASH_Sector_5;  
	}
	else if((Address < ADDR_FLASH_SECTOR_7) && (Address >= ADDR_FLASH_SECTOR_6))
	{
		sector = FLASH_Sector_6;  
	}
	else if((Address < ADDR_FLASH_SECTOR_8) && (Address >= ADDR_FLASH_SECTOR_7))
	{
		sector = FLASH_Sector_7;  
	}
	else if((Address < ADDR_FLASH_SECTOR_9) && (Address >= ADDR_FLASH_SECTOR_8))
	{
		sector = FLASH_Sector_8;  
	}

	return sector;
}

int flash_run(void)
{
	uint32_t uwStartSector, uwEndSector, uwSectorCounter, uwAddress, cnt=0;
	__IO uint32_t uwMemoryProgramStatus = 0, uwData32;
	
	 /*  解锁 FLASH  */
	FLASH_Unlock();
	
	/* 清楚状态寄存器中的错误标志 */  
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | 
					FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR); 
	
	
	 /* 获取要操作的扇区号  */
	uwStartSector = GetSector(ADDR_FLASH_SECTOR_6);
	uwEndSector = GetSector(ADDR_FLASH_SECTOR_6);

	/* 擦除要操作的扇区中的数据 */
	uwSectorCounter = uwStartSector;
	while (uwSectorCounter <= uwEndSector) 
	{
		/* Device voltage range supposed to be [2.7V to 3.6V], the operation will
		be done by word */ 
		if (FLASH_EraseSector(uwSectorCounter, VoltageRange_3) != FLASH_COMPLETE)
		{ 
			/* 如果擦除失败，错误处理  */
			while (1){
				printf("EraseSector err\n");
			}
		}
		/* jump to the next sector */
		if (uwSectorCounter == FLASH_Sector_11)
		{
			uwSectorCounter += 40;
		} 
		else 
		{
			uwSectorCounter += 8;
		}
	}
	
	/*  按字（4字节）写入数据 */
	/* 在起始位置和结束位置之间 */	
	
	uwAddress = ADDR_FLASH_SECTOR_6;	//要写入数据的起始位置
	
	while (uwAddress < ADDR_FLASH_SECTOR_7)	//在扇区6起始位置到扇区7的起始位置
	{
		//按字写入数据到指定地址
		if (FLASH_ProgramWord(uwAddress, 0x12345678) == FLASH_COMPLETE)
		{
			//写入数据成功，让地址往后偏移4字节（字）
			uwAddress = uwAddress + 4;
		}
		else
		{ 
			// 写入的错误处理 
			while (1){
				printf("ProgramWord err %x\n", uwAddress);
				return -1;
			}
		}
	}
	
	/* 加锁 FLASH */
	FLASH_Lock(); 
	
	
	/* 检查写入的数据是否正常   ***********/
	/*  MemoryProgramStatus = 0: data programmed correctly
		MemoryProgramStatus != 0: number of words not programmed correctly */
	uwAddress = ADDR_FLASH_SECTOR_6;
	uwMemoryProgramStatus = 0;

	while (uwAddress < ADDR_FLASH_SECTOR_7)
	{
		uwData32 = *(__IO uint32_t*)uwAddress;
	
		if (uwData32 != 0x12345678)//读取的数据和写入数据进行对比
		{
			uwMemoryProgramStatus++;  	//有一个不同就标志自加
		}
			uwAddress = uwAddress + 4;
		
		cnt++;
		if(cnt == 200)
		{
			printf("uwData32:%x\n", uwData32);
			cnt = 0;
		}
	}  
	
	printf("uwMemoryProgramStatus:%d\n", uwMemoryProgramStatus);
	
	return 1;

}



