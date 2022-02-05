#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "lcd.h"
#include "24cxx.h"
#include "key.h"  
#include "smbus11_hw.h"

//ALIENTEK ̽����STM32F407������ ʵ��24
//IIC ʵ�� --�⺯���汾
//����֧�֣�www.openedv.com
//�Ա����̣�http://eboard.taobao.com  
//������������ӿƼ����޹�˾  
//���ߣ�����ԭ�� @ALIENTEK



//Ҫд�뵽24c02���ַ�������
const u8 TEXT_Buffer[]={"wwwwww STM32F4 IIC TEST"};
#define SIZE sizeof(TEXT_Buffer)	 
	
int main(void)
{ 
	u8 key;
	u16 i=0;
	u8 datatemp[SIZE];	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);    //��ʼ����ʱ����
	uart_init(115200);	//��ʼ�����ڲ�����Ϊ115200
	SMBus_HW_init();
	LED_Init();					//��ʼ��LED 
//	KEY_Init(); 				//������ʼ��  
//	AT24CXX_Init();				//IIC��ʼ�� 

// 	while(AT24CXX_Check())//��ⲻ��24c02
//	{

//		delay_ms(500);
//		LED0=!LED0;//DS0��˸
//	}

	
	//SMBus_FindAddr_Test();
	
	/* first group*/
	/* ��ȡ���������� */
	SMBus_Get_ManuName();
	/* second group*/
	SMBus_Get_Temp();
	SMBus_Get_Voltage();
	SMBus_Get_Current();
	SMBus_Get_AverageCurrent();
	SMBus_Get_MaxError();
	SMBus_Get_RelativeStateOfCharge();
	SMBus_Get_AbsouluteStateOfCharge();
  SMBus_Get_RemainingCapacity();
	/* third group*/
  SMBus_Get_FullChargeCapacity();
	
	
	while(1)
	{
//		key=KEY_Scan(0);
//		if(key==KEY1_PRES)//KEY1����,д��24C02
//		{

////			AT24CXX_Write(0,(u8*)TEXT_Buffer,SIZE);
//			SMBus_FindAddr_Test();
////			SMBus_Get_Voltage();
////			SMBus_Get_ManuName();
//		}
//		if(key==KEY0_PRES)//KEY0����,��ȡ�ַ�������ʾ
//		{

//			AT24CXX_Read(0,datatemp,SIZE);
//			printf("temp=%s,%d\n", datatemp, datatemp[8]);
//		}
		i++;
		delay_ms(10);
		if(i==20)
		{
			LED0=!LED0;//��ʾϵͳ��������	
			i=0;
		}		   
	} 	    
}
