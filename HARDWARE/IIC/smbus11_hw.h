#ifndef _SMBus11_HW_H_
#define _SMBus11_HW_H_

#include "stm32f4xx.h"
#include "stm32f4xx_i2c.h"
#include <inttypes.h>

#define SMBus_NAME             I2C2
#define SMBus_RCC_Periph       RCC_APB1Periph_I2C2
#define SMBus_Port             GPIOB
#define SMBus_SCL_Pin          GPIO_Pin_10
#define SMBus_SDA_Pin          GPIO_Pin_11
#define SMBus_SCL_PinSource    GPIO_PinSource10
#define SMBus_SDA_PinSource    GPIO_PinSource11
#define SMBus_RCC_Port         RCC_AHB1Periph_GPIOB
#define SMBus_Speed            100000
#define SMBus_GPIO_AF          GPIO_AF_I2C2
#define SMBus_Max_Delay_Cycles 10000


/* 从机设备地址，设备写地址0x16,读地址0x17 */
// 后期需用结构体重新定义
#define SLAVE_ADDE			0x16

/* Smart Battery Slave Functions */
#define	ManufacturerAccess			    0x00
#define	RemainingCapacityAlarm			0x01
#define	RemainingTimeAlarm			    0x02
#define	BatteryMode			    		  0x03
#define	AtRate			    			    0x04
#define	AtRateTimeToFull			    0x05
#define	AtRateTimeToEmpty			    0x06
#define	AtRateOK			    		    0x07
#define	Temperature			    	   	0x08   // 0.1 K (开尔文)
#define	Voltage			    			    0x09
#define	Current			    			    0x0a
#define	AverageCurrent			    	0x0b

#define MaxError                  0x0c
#define RelativeStateOfCharge     0x0d
#define AbsouluteStateOfCharge    0x0e
#define RemainingCapacity         0x0f

#define FullChargeCapacity        0x10
#define RunTimeToEmpty            0x11
#define AverageTimeToEmpty        0x12
#define AverageTimeToFull         0x13
#define	ChargingCurrent			    	0x14
#define	ChargingVoltage			    	0x15
#define	BatteryStatus			    	  0x16
#define	CycleCount			    		  0x17
#define	DesignCapacity			    	0x18
#define	DesignVoltage			    	  0x19
#define	SpecificationInfo			    0x1a
#define	ManufactureDate			    	0x1b
#define	SerialNumber			    	  0x1c

#define	ManufacturerName			    0x20
#define	DeviceName			    		  0x21
#define	DeviceChemistry			    	0x22
#define	ManufacturerData			    0x23
#define	OptionalMfgFunction5			0x2f
#define Authenticate              0x2f

/* Extended SBS Commands Start*/
// 后期需用结构体重新定义
// Reserved
#define AFEData                   0x45
#define FETControl                0x46
#define PendingEDV                0x47
#define StateOfHealth             0x4f

#define SafetyAlert               0x50
#define SafetyStatus              0x51
#define PFAlert                   0x52
#define PFStatus                  0x53
#define OperationStauts           0x54
#define ChargingStatus            0x55
#define FETStatus                 0x56
#define ResetData                 0x57
#define WDResetData               0x58
#define PackVoltage               0x5a
#define AverageVoltage            0x5d
#define TS1Temperature            0x5e       // 0.1 ℃
#define TS2Temperature            0x5f       // 0.1 ℃
#define UnSealKey                 0x60
#define FullAccessKey             0x61
#define PFKey                     0x62
#define AuthenKey3                0x63
#define AuthenKey2                0x64
#define AuthenKey1                0x65
#define AuthenKey0                0x66

#define ManufacturerInfo          0x70
#define SenseResistor             0x71
#define TempRange                 0x72
#define DataFlashSubClassID       0x77
#define DataFlashSubClassPage1    0x78
#define DataFlashSubClassPage2    0x79
#define DataFlashSubClassPage3    0x7a
#define DataFlashSubClassPage4    0x7b
#define DataFlashSubClassPage5    0x7c
#define DataFlashSubClassPage6    0x7d
#define DataFlashSubClassPage7    0x7e
#define DataFlashSubClassPage8    0x7f

/* Extended SBS Commands Ends*/


void SMBus_HW_init(void);
uint8_t SMBus_Write_Word(uint8_t slaveAddr, uint16_t data, uint8_t WriteCmd);
void SMBus_Read_Word(uint8_t slaveAddr, uint16_t* data, uint8_t ReadAddr);
uint8_t SMBus_Write_Byte(uint8_t slaveAddr, uint8_t WriteCmd);

uint16_t SMBus_Get_Temp(void);
uint16_t SMBus_Get_Voltage(void);
void SMBus_Get_Current(void);
void SMBus_Get_AverageCurrent(void);
void SMBus_Get_MaxError(void);
void SMBus_Get_RelativeStateOfCharge(void);
void SMBus_Get_AbsouluteStateOfCharge(void);
void SMBus_Get_RemainingCapacity(void);

void SMBus_Get_FullChargeCapacity(void);


void SMBus_Get_ManuName(void);
void SMBus_FindAddr_Test(void);




#endif  /* _SMBus11_HW_H_ */
