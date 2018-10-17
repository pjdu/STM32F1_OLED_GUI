#ifndef _MYIIC_H
#define _MYIIC_H
#include "sys.h"


#define SDA_IN()  {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=(u32)8<<28;}//PB7 設為輸入
#define SDA_OUT() {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=(u32)3<<28;}//PB7設為輸出

// IO 讀寫操作
#define IIC_SCL   PBout(6) //SCL
#define IIC_SDA   PBout(7) //SDA
#define READ_SDA  PBin(7)  //SDA


void IIC_Init(void);
void IIC_Start(void);
void IIC_Stop(void);
void IIC_Send_Byte(u8 txd);
u8 IIC_Read_Byte(unsigned char ack);
u8 IIC_Wait_Ack(void);
void IIC_Ack(void);
void IIC_NAck(void);

void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 IIC_Read_One_Byte(u8 daddr,u8 addr);	 
#endif

