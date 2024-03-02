#include "flash.h"

/***************
���ܣ�ͨ�������ĵ�ַ��ȡFLASH�е�������
������Address�������Ҫ�����Ĵ洢����ַ
���أ���ַ���������ĺ���
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
	
	 /*  ���� FLASH  */
	FLASH_Unlock();
	
	/* ���״̬�Ĵ����еĴ����־ */  
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | 
					FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR); 
	
	
	 /* ��ȡҪ������������  */
	uwStartSector = GetSector(ADDR_FLASH_SECTOR_6);
	uwEndSector = GetSector(ADDR_FLASH_SECTOR_6);

	/* ����Ҫ�����������е����� */
	uwSectorCounter = uwStartSector;
	while (uwSectorCounter <= uwEndSector) 
	{
		/* Device voltage range supposed to be [2.7V to 3.6V], the operation will
		be done by word */ 
		if (FLASH_EraseSector(uwSectorCounter, VoltageRange_3) != FLASH_COMPLETE)
		{ 
			/* �������ʧ�ܣ�������  */
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
	
	/*  ���֣�4�ֽڣ�д������ */
	/* ����ʼλ�úͽ���λ��֮�� */	
	
	uwAddress = ADDR_FLASH_SECTOR_6;	//Ҫд�����ݵ���ʼλ��
	
	while (uwAddress < ADDR_FLASH_SECTOR_7)	//������6��ʼλ�õ�����7����ʼλ��
	{
		//����д�����ݵ�ָ����ַ
		if (FLASH_ProgramWord(uwAddress, 0x12345678) == FLASH_COMPLETE)
		{
			//д�����ݳɹ����õ�ַ����ƫ��4�ֽڣ��֣�
			uwAddress = uwAddress + 4;
		}
		else
		{ 
			// д��Ĵ����� 
			while (1){
				printf("ProgramWord err %x\n", uwAddress);
				return -1;
			}
		}
	}
	
	/* ���� FLASH */
	FLASH_Lock(); 
	
	
	/* ���д��������Ƿ�����   ***********/
	/*  MemoryProgramStatus = 0: data programmed correctly
		MemoryProgramStatus != 0: number of words not programmed correctly */
	uwAddress = ADDR_FLASH_SECTOR_6;
	uwMemoryProgramStatus = 0;

	while (uwAddress < ADDR_FLASH_SECTOR_7)
	{
		uwData32 = *(__IO uint32_t*)uwAddress;
	
		if (uwData32 != 0x12345678)//��ȡ�����ݺ�д�����ݽ��жԱ�
		{
			uwMemoryProgramStatus++;  	//��һ����ͬ�ͱ�־�Լ�
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



