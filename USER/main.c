#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "lcd.h"
#include "24cxx.h"
#include "key.h"  
#include "smbus11_hw.h"

//ALIENTEK 探索者STM32F407开发板 实验24
//IIC 实验 --库函数版本
//技术支持：www.openedv.com
//淘宝店铺：http://eboard.taobao.com  
//广州市星翼电子科技有限公司  
//作者：正点原子 @ALIENTEK



//要写入到24c02的字符串数组
const u8 TEXT_Buffer[]={"wwwwww STM32F4 IIC TEST"};
#define SIZE sizeof(TEXT_Buffer)	 
	
int main(void)
{ 
	u8 key;
	u16 i=0;
	u8 datatemp[SIZE];	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);    //初始化延时函数
	uart_init(115200);	//初始化串口波特率为115200
	SMBus_HW_init();
	LED_Init();					//初始化LED 
//	KEY_Init(); 				//按键初始化  
//	AT24CXX_Init();				//IIC初始化 

// 	while(AT24CXX_Check())//检测不到24c02
//	{

//		delay_ms(500);
//		LED0=!LED0;//DS0闪烁
//	}

	
	//SMBus_FindAddr_Test();
	
	/* first group*/
	/* 读取制造商名称 */
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
	/* Voltage- Subclass 0 */
	SMBus_Get_Parameter("    CelVoltage4 ",0x3c);
	SMBus_Get_Parameter("    CelVoltage3 ",0x3d);
	SMBus_Get_Parameter("    CelVoltage2 ",0x3e);
	SMBus_Get_Parameter("    CelVoltage1 ",0x3f);
	SMBus_Get_Parameter("    SafetyStatus",0x51);
	SMBus_Get_Parameter("    TemppRange  ",0x72);
	/* Current- Subclass 1 */
	
	/* Temperature- Subclass 2 */
	
	while(1)
	{
//		key=KEY_Scan(0);
//		if(key==KEY1_PRES)//KEY1按下,写入24C02
//		{

////			AT24CXX_Write(0,(u8*)TEXT_Buffer,SIZE);
//			SMBus_FindAddr_Test();
////			SMBus_Get_Voltage();
////			SMBus_Get_ManuName();
//		}
//		if(key==KEY0_PRES)//KEY0按下,读取字符串并显示
//		{

//			AT24CXX_Read(0,datatemp,SIZE);
//			printf("temp=%s,%d\n", datatemp, datatemp[8]);
//		}
		i++;
		delay_ms(100);
		if(i==20)
		{
			LED0=!LED0;//提示系统正在运行	
			i=0;
			
//				SMBus_Get_ManuName();
//				/* second group*/
//				SMBus_Get_Temp();
//				SMBus_Get_Voltage();
//				SMBus_Get_Current();
//				SMBus_Get_AverageCurrent();
//				SMBus_Get_MaxError();
//				SMBus_Get_RelativeStateOfCharge();
//				SMBus_Get_AbsouluteStateOfCharge();
//				SMBus_Get_RemainingCapacity();
//				/* third group*/
//				SMBus_Get_FullChargeCapacity();
//				/* Voltage- Subclass 0 */
//				SMBus_Get_Parameter("    CelVoltage4 ",0x3c);
//				SMBus_Get_Parameter("    CelVoltage3 ",0x3d);
//				SMBus_Get_Parameter("    CelVoltage2 ",0x3e);
//				SMBus_Get_Parameter("    CelVoltage1 ",0x3f);
//				SMBus_Get_Parameter("    SafetyStatus",0x51);
//				SMBus_Get_Parameter("    TemppRange  ",0x72);
		}		   
	} 	    
}
