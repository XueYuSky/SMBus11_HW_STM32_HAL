#include "smbus_i2c.h"
#include "delay.h"
#include "usart.h"

/********************************
*��ע��ʹ��I2C2
       PB10��������SCL��PB11��������SDA
********************************/
void SMBus_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//ʹ��GPIOBʱ��

	//SMBus_SCL_Pin,SMBus_SDA_Pin��ʼ������
	GPIO_InitStructure.GPIO_Pin = SMBus_SCL_Pin | SMBus_SDA_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ  
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��
	SMBus_SCL_H;
	SMBus_SDA_H;
}
 
 
//����IIC��ʼ�ź�
void SMBus_Start(void)
{
    SDA_OUT();
	  SMBus_SDA_H; 
    delay_us(1);
    SMBus_SCL_H;
    delay_us(5);
    SMBus_SDA_L;//START:when CLK is high,DATA change form high to low
    delay_us(5);
    SMBus_SCL_L;//ǯסI2C���ߣ�׼�����ͻ��������
	  delay_us(2);
}  
  

//����IICֹͣ�ź�
void SMBus_Stop(void)
{
    SDA_OUT();
    SMBus_SCL_L;
    delay_us(1);
    SMBus_SDA_L;
    delay_us(5);
    SMBus_SCL_H;
    delay_us(5);   
    SMBus_SDA_H;//����I2C���߽����ź�
//    delay_us(5);                                       
}

//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
uint8_t SMBus_Wait_Ack(void)
{
    uint8_t ucErrTime=0;
    SDA_IN();      //SDA����Ϊ����
    SMBus_SDA_H;
	delay_us(6);    
    SMBus_SCL_H;
	delay_us(6);  
    while(SMBus_READ_SDA)
    {
        ucErrTime++;
        if(ucErrTime>250)
        {
            SMBus_Stop();
            return 1;
        }
    }
    SMBus_SCL_L;//ʱ�����0  
    delay_us(2);
    return 0; 
}
  
   
//����ACKӦ��
void SMBus_Ack(void)
{
    SMBus_SCL_L;
    SDA_OUT();
    SMBus_SDA_L;
    delay_us(6);
    SMBus_SCL_H;
    delay_us(6);
    SMBus_SCL_L;
}


//������ACKӦ��         
void SMBus_NAck(void)
{
    SMBus_SCL_L;
    SDA_OUT();
    SMBus_SDA_H;
    delay_us(6);
    SMBus_SCL_H;
    delay_us(6);
    SMBus_SCL_L;
}                                       
  
  
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��          
void SMBus_Send_Byte(uint8_t txd)
{                       
    uint8_t t;  
    SDA_OUT();       
    SMBus_SCL_L;	//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++){
		
        if((txd&0x80)>>7)
        {
            SMBus_SDA_H;
        }
        else
        {
            SMBus_SDA_L;
        }
        txd<<=1; 
        delay_us(2);  
        SMBus_SCL_H;
        delay_us(5);
        SMBus_SCL_L; 
        delay_us(5);
    }
}
  
  
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK  
uint8_t SMBus_Read_Byte(uint8_t ack)
{
    unsigned char i,receive=0;
    SDA_IN();		//SDA����Ϊ����
	for(i=0;i<8;i++ ){
		
		SMBus_SCL_L;
		delay_us(8);
		SMBus_SCL_H;
		receive<<=1;
		if(SMBus_READ_SDA)
			receive++;  
		delay_us(5);
    }                   
	if (!ack)
        SMBus_NAck();//����nACK
    else
        SMBus_Ack(); //����ACK  
	
    return receive;
}


uint8_t SMBus_WriteOneByte(uint8_t WriteAddr, uint8_t DataToWrite)
{
	uint8_t status = 0;
	
    SMBus_Start();  
	SMBus_Send_Byte(WriteAddr);   //����������ַ0XA0,д����
	status = SMBus_Wait_Ack();	   
//	if(status == 1)
//	{
//		printf("��һ��Ӧ��fail��%d\n", WriteAddr);
//		return status;
//	}
	
    SMBus_Send_Byte(0);   		//���͵͵�ַ
	delay_us(90);
	status = SMBus_Wait_Ack(); 	 
//	if(status == 1)
//	{
//		printf("�ڶ���Ӧ��fail\n");
//		return status;
//	}
	
	SMBus_Send_Byte(DataToWrite);     //�����ֽ�							   
	status = SMBus_Wait_Ack(); 
//	if(status == 1)
//	{
//		printf("������Ӧ��fail\n");
//		return status;
//	}
	
    SMBus_Stop();//����һ��ֹͣ���� 
	delay_ms(10);	 
	
	return status;
}

//��AT24CXXָ����ַ����һ������
//ReadAddr:��ʼ�����ĵ�ַ  
//����ֵ  :����������
u8 time=10;	
uint8_t SMBus_ReadOneByte(uint16_t ReadAddr)
{			  
	u8 temp=0;		  	    																 
    SMBus_Start();  
	SMBus_Send_Byte(ReadAddr+((ReadAddr/256)<<1));   //����������ַ0XA0,д����
	if(SMBus_Wait_Ack()==1)
	{
       printf(" 1 fail!\r\n");
       return -1;
    }
    SMBus_Send_Byte(ReadAddr%256);   //���͵͵�ַ
	delay_us(time);
	if(SMBus_Wait_Ack()==1)
	{
       printf(" 2 fail!\r\n");
       return -1;
    }
	SMBus_Start();  	 	   
	SMBus_Send_Byte(ReadAddr|0x01);           //�������ģʽ			   
	if(SMBus_Wait_Ack()==1)
	{
       printf(" 3 fail!\r\n");
       return -1;
    }
    temp=SMBus_Read_Byte(0);		   
    SMBus_Stop();//����һ��ֹͣ����	    
	return temp;
}

int16_t Get_Battery_Info(uint8_t slaveAddr,uint8_t slaveAddr1, uint8_t Comcode)
{
     int16_t Value;
     uint8_t data[2] = {0};
     SMBus_Start(); 
 
     SMBus_Send_Byte(slaveAddr);	//���͵�ַ
	 if(SMBus_Wait_Ack() == 1)
     {
       printf("SlaveAddr wait ack fail!\r\n");
         return -1;
     }
 
     SMBus_Send_Byte(Comcode);
     delay_us(90);
     if(SMBus_Wait_Ack() == 1)
     {
       printf("Comcode wait ack fail!\r\n");
         return -1;
     }
     
	   SMBus_Start();
     SMBus_Send_Byte(slaveAddr1);//���͵�ַ
     if(SMBus_Wait_Ack() == 1)
     {
       printf("slaveAddr+1 wait ack fail!\r\n");
         return -1;
     }
     delay_us(60);
 
     data[0] = SMBus_Read_Byte(1);
     SMBus_Ack();
     delay_us(125);
     data[1] = SMBus_Read_Byte(0);
     SMBus_NAck();
     delay_us(58);
     SMBus_Stop();
//     printf("data[0]:%x,data[1]:%x\r\n",data[0],data[1]);
     Value = (data[0] |(data[1]<<8));
     delay_us(100);
     return Value;
	 
 }





