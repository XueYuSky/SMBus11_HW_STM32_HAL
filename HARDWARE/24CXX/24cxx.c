#include "24cxx.h" 
#include "delay.h" 	
#include "smbus11_hw.h"
#include "usart.h"
#include "smbus_i2c.h"

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//24CXX ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/6
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

//��ʼ��IIC�ӿ�
void AT24CXX_Init(void)
{
//	IIC_Init();//IIC��ʼ��
//	SMBus_init();
//	SMBus_Init();
	SMBus_HW_init();
}
//��AT24CXXָ����ַ����һ������
//ReadAddr:��ʼ�����ĵ�ַ  
//����ֵ  :����������
u8 AT24CXX_ReadOneByte(u16 ReadAddr)
{				  
	u8 temp=0;		  	    																 
    IIC_Start();  
	if(EE_TYPE>AT24C16)
	{
		IIC_Send_Byte(0XA0);	   //����д����
		IIC_Wait_Ack();
		IIC_Send_Byte(ReadAddr>>8);//���͸ߵ�ַ	    
	}
	else 
		IIC_Send_Byte(0XA0+((ReadAddr/256)<<1));   //����������ַ0XA0,д����
	
	IIC_Wait_Ack(); 
    IIC_Send_Byte(ReadAddr%256);   //���͵͵�ַ
	IIC_Wait_Ack();	    
	IIC_Start();  	 	   
	IIC_Send_Byte(0XA1);           //�������ģʽ			   
	IIC_Wait_Ack();	 
    temp=IIC_Read_Byte(0);		   
    IIC_Stop();//����һ��ֹͣ����	    
	return temp;
}
//��AT24CXXָ����ַд��һ������
//WriteAddr  :д�����ݵ�Ŀ�ĵ�ַ    
//DataToWrite:Ҫд�������
void AT24CXX_WriteOneByte(u16 WriteAddr,u8 DataToWrite)
{				   	  	    																 
    IIC_Start();  
	if(EE_TYPE>AT24C16)
	{
		IIC_Send_Byte(0XA0);	    //����д����
		IIC_Wait_Ack();
		IIC_Send_Byte(WriteAddr>>8);//���͸ߵ�ַ	  
	}
	else 
		IIC_Send_Byte(0XA0+((WriteAddr/256)<<1));   //����������ַ0XA0,д����
	
	IIC_Wait_Ack();	   
    IIC_Send_Byte(WriteAddr%256);   //���͵͵�ַ
	IIC_Wait_Ack(); 	 										  		   
	IIC_Send_Byte(DataToWrite);     //�����ֽ�							   
	IIC_Wait_Ack();  		    	   
    IIC_Stop();//����һ��ֹͣ���� 
	delay_ms(10);	 
}
//��AT24CXX�����ָ����ַ��ʼд�볤��ΪLen������
//�ú�������д��16bit����32bit������.
//WriteAddr  :��ʼд��ĵ�ַ  
//DataToWrite:���������׵�ַ
//Len        :Ҫд�����ݵĳ���2,4
void AT24CXX_WriteLenByte(u16 WriteAddr,u32 DataToWrite,u8 Len)
{  	
	u8 t;
	for(t=0;t<Len;t++)
	{
		AT24CXX_WriteOneByte(WriteAddr+t,(DataToWrite>>(8*t))&0xff);
	}												    
}

//��AT24CXX�����ָ����ַ��ʼ��������ΪLen������
//�ú������ڶ���16bit����32bit������.
//ReadAddr   :��ʼ�����ĵ�ַ 
//����ֵ     :����
//Len        :Ҫ�������ݵĳ���2,4
u32 AT24CXX_ReadLenByte(u16 ReadAddr,u8 Len)
{  	
	u8 t;
	u32 temp=0;
	for(t=0;t<Len;t++)
	{
		temp<<=8;
		temp+=AT24CXX_ReadOneByte(ReadAddr+Len-t-1); 	 				   
	}
	return temp;												    
}
//���AT24CXX�Ƿ�����
//��������24XX�����һ����ַ(255)���洢��־��.
//���������24Cϵ��,�����ַҪ�޸�
//����1:���ʧ��
//����0:���ɹ�
u8 AT24CXX_Check(void)
{
	u8 temp;
	temp=AT24CXX_ReadOneByte(255);//����ÿ�ο�����дAT24CXX			   
	if(temp==0X55)return 0;		   
	else//�ų���һ�γ�ʼ�������
	{
		AT24CXX_WriteOneByte(255,0X55);
	    temp=AT24CXX_ReadOneByte(255);	  
		if(temp==0X55)return 0;
	}
	return 1;											  
}

//��AT24CXX�����ָ����ַ��ʼ����ָ������������
//ReadAddr :��ʼ�����ĵ�ַ ��24c02Ϊ0~255
//pBuffer  :���������׵�ַ
//NumToRead:Ҫ�������ݵĸ���
void AT24CXX_Read(u16 ReadAddr,u8 *pBuffer,u16 NumToRead)
{
	while(NumToRead)
	{
		*pBuffer++=AT24CXX_ReadOneByte(ReadAddr++);	
		NumToRead--;
	}
}  
//��AT24CXX�����ָ����ַ��ʼд��ָ������������
//WriteAddr :��ʼд��ĵ�ַ ��24c02Ϊ0~255
//pBuffer   :���������׵�ַ
//NumToWrite:Ҫд�����ݵĸ���
void AT24CXX_Write(u16 WriteAddr,u8 *pBuffer,u16 NumToWrite)
{
	while(NumToWrite--)
	{
		AT24CXX_WriteOneByte(WriteAddr,*pBuffer);
		WriteAddr++;
		pBuffer++;
	}
}

/*---------------------------------------------------------*/
uint8_t Write_Data1[2] = {0};

void Delay_ms(void)
{
	uint8_t i,j,z;
	for(i=0; i<50; i++)
//		for(j=0; j<250; j++)
			for(z=0; z<50; z++)
				{}
}

uint8_t AT24C02_WriteOneByte(u16 WriteAddr, u8 DataToWrite)
{
	uint8_t status = 0;
	
    IIC_Start();  
	IIC_Send_Byte(WriteAddr);   //����������ַ0XA0,д����
	status = IIC_Wait_Ack();	   
//	if(status == 1)
//	{
//		printf("��һ��Ӧ��fail��%d\n", WriteAddr);
//		return status;
//	}
	
    IIC_Send_Byte(0);   		//���͵͵�ַ
	status = IIC_Wait_Ack(); 	 
	if(status == 1)
	{
		printf("�ڶ���Ӧ��fail\n");
		return status;
	}
	
	IIC_Send_Byte(DataToWrite);     //�����ֽ�							   
	status = IIC_Wait_Ack(); 
//	if(status == 1)
//	{
//		printf("������Ӧ��fail\n");
//		return status;
//	}
	
    IIC_Stop();//����һ��ֹͣ���� 
	delay_ms(10);	 
	
	return status;
}

int16_t Get2_Battery_Info(uint8_t slaveAddr, uint8_t Comcode)
{
     int16_t Value;
     uint8_t data[2] = {0};
     IIC_Start(); 
 
     IIC_Send_Byte(slaveAddr);	//���͵�ַ
	 if(IIC_Wait_Ack() == 1)
     {
//       printf("SlaveAddr wait ack fail!\r\n");
         return -1;
     }
 
     IIC_Send_Byte(Comcode);
     delay_us(90);
     if(IIC_Wait_Ack() == 1)
     {
//       printf("Comcode wait ack fail!\r\n");
         return -1;
     }
     
	 IIC_Start();
     IIC_Send_Byte(slaveAddr+0x01);//���͵�ַ
     if(IIC_Wait_Ack() == 1)
     {
//       printf("slaveAddr+1 wait ack fail!\r\n");
         return -1;
     }
     delay_us(60);
 
     data[0] = IIC_Read_Byte(1);
     IIC_Ack();
     delay_us(125);
     data[1] = IIC_Read_Byte(1);
     IIC_NAck();
     delay_us(58);
     IIC_Stop();
//     printf("data[0]:%x,data[1]:%x\r\n",data[0],data[1]);
     Value = (data[1] |(data[0]<<8));
     delay_us(100);
     return Value;
	 
 }

void SMBus_Test(void)
{
	uint8_t i;
	uint8_t* data;
	int16_t Value = 0;

	for(i=21; i<22; i++)
	{
//		Value = AT24C02_WriteOneByte(0x16, 0x09);
//		Value = SMBus_Write_Test(i);
//		Value = SMBus_WriteOneByte(i, 0x09);
//		Value = SMBus_ReadOneByte(i);
//		Value = Get_Battery_Info(0x16, 0x17, 0x09);
//		Value = SMBus_ReadOneByte(0x09);
//		Value = SMBus_WriteWord(i, 0x09, 0x09);
//		Value = SMBus_Word_Get_Info(0x16, 0x03);
		
//		SMBus_Block_Get_Info(0x16, data, 0x020);

//		if(Value != 0)
//		{
//			printf("   i=%c,%c,%d,%#x\n", Value&0xFF, (Value&0xFF00)>>8,Value, i);
//		}

		Delay_ms();
	}
	
//	status = SMBus_ReadOneByte(0x08);
	
	
	printf("������status=%d\n", Value);
}















