/**********************************************************************************************************

              液晶演示程序JLX12864G-330-PN-S，串行接口！  
                        驱动IC是:ST7567  
***********************************************************************************************************/  
#include "c8051f020.h"
#include "lcd.h"
#include "delay.h"
#include "keydown.h"

typedef unsigned int u16;
typedef unsigned char u8;

extern int Clock[];
extern int Set_Begin[],Set_End[];
int Time_Count[16]={0};
u8 Time_Count_flag=0,Judge_flag=0;
sbit Motor_IN = P1^7;


char code maohao[]={
		0x00,0x00,0x00,0x30,0x30,0x00,0x00,0x00,
		0x00,0x00,0x00,0x18,0x18,0x00,0x00,0x00};
//--  符号-
char code heng[]={
		0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

char code xing[]={
		0x00,0x00,0x00,0xBE,0x2A,0x2A,0x2A,0xEA,0x2A,0x2A,0x2A,0x3E,0x00,0x00,0x00,0x00,
		0x00,0x44,0x42,0x49,0x49,0x49,0x49,0x7F,0x49,0x49,0x49,0x49,0x41,0x40,0x00,0x00};/*星*/

char code qi[]={
		0x00,0x04,0xFF,0x24,0x24,0x24,0xFF,0x04,0x00,0xFE,0x22,0x22,0x22,0xFE,0x00,0x00,
		0x88,0x48,0x2F,0x09,0x09,0x19,0xAF,0x48,0x30,0x0F,0x02,0x42,0x82,0x7F,0x00,0x00};/*期*/
char code rain[]={
		0x00,0x00,0x80,0xC0,0xE0,0xF0,0xFC,0xFF,0xFC,0xF0,0xE0,0xC0,0x80,0x00,0x00,0x00,
		0x00,0x0F,0x1F,0x3F,0x7F,0x7F,0xFF,0xFF,0xFF,0x7F,0x7F,0x3F,0x1F,0x0F,0x00,0x00};/*雨滴*/

u8 code begin_time[]={
		0x40,0x40,0x42,0xCC,0x00,0x40,0xA0,0x9E,0x82,0x82,0x82,0x9E,0xA0,0x20,0x20,0x00,
		0x00,0x00,0x00,0x3F,0x90,0x88,0x40,0x43,0x2C,0x10,0x28,0x46,0x41,0x80,0x80,0x00,
		0x00,0x17,0x15,0xD5,0x55,0x57,0x55,0x7D,0x55,0x57,0x55,0xD5,0x15,0x17,0x00,0x00,
		0x40,0x40,0x40,0x7F,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x7F,0x40,0x40,0x40,0x00,
		0x10,0x60,0x02,0x0C,0xC0,0x00,0x88,0x88,0x4F,0x58,0x24,0x54,0x84,0xE4,0x00,0x00,
		0x04,0x04,0x7C,0x03,0x00,0x82,0x42,0x32,0x0E,0x02,0x02,0x3E,0x42,0x42,0x72,0x00,
		0x00,0x20,0x20,0x20,0xA0,0x60,0x00,0xFF,0x60,0x80,0x40,0x20,0x18,0x00,0x00,0x00,
		0x20,0x10,0x08,0x06,0x01,0x40,0x80,0x7F,0x00,0x01,0x02,0x04,0x08,0x10,0x10,0x00,
		0x00,0xFC,0x84,0x84,0x84,0xFC,0x00,0x10,0x10,0x10,0x10,0x10,0xFF,0x10,0x10,0x00,
		0x00,0x3F,0x10,0x10,0x10,0x3F,0x00,0x00,0x01,0x06,0x40,0x80,0x7F,0x00,0x00,0x00,
		0x00,0xF8,0x01,0x06,0x00,0xF0,0x12,0x12,0x12,0xF2,0x02,0x02,0x02,0xFE,0x00,0x00,
		0x00,0xFF,0x00,0x00,0x00,0x1F,0x11,0x11,0x11,0x1F,0x00,0x40,0x80,0x7F,0x00,0x00};//设置初始时间（6）

u8 code end_time[]={
		0x40,0x40,0x42,0xCC,0x00,0x40,0xA0,0x9E,0x82,0x82,0x82,0x9E,0xA0,0x20,0x20,0x00,
		0x00,0x00,0x00,0x3F,0x90,0x88,0x40,0x43,0x2C,0x10,0x28,0x46,0x41,0x80,0x80,0x00,
		0x00,0x17,0x15,0xD5,0x55,0x57,0x55,0x7D,0x55,0x57,0x55,0xD5,0x15,0x17,0x00,0x00,
		0x40,0x40,0x40,0x7F,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x7F,0x40,0x40,0x40,0x00,
		0x20,0x30,0xAC,0x63,0x20,0x18,0x08,0x48,0x48,0x48,0x7F,0x48,0x48,0x48,0x08,0x00,
		0x22,0x67,0x22,0x12,0x12,0x12,0x00,0xFE,0x42,0x42,0x42,0x42,0x42,0xFE,0x00,0x00,
		0x04,0x04,0xE4,0x24,0x24,0x24,0x24,0xFF,0x24,0x24,0x24,0x24,0xE4,0x04,0x04,0x00,
		0x40,0x40,0x27,0x22,0x12,0x0A,0x06,0xFF,0x06,0x0A,0x12,0x22,0x27,0x40,0x40,0x00,
		0x00,0xFC,0x84,0x84,0x84,0xFC,0x00,0x10,0x10,0x10,0x10,0x10,0xFF,0x10,0x10,0x00,
		0x00,0x3F,0x10,0x10,0x10,0x3F,0x00,0x00,0x01,0x06,0x40,0x80,0x7F,0x00,0x00,0x00,
		0x00,0xF8,0x01,0x06,0x00,0xF0,0x12,0x12,0x12,0xF2,0x02,0x02,0x02,0xFE,0x00,0x00,
		0x00,0xFF,0x00,0x00,0x00,0x1F,0x11,0x11,0x11,0x1F,0x00,0x40,0x80,0x7F,0x00,0x00};//设置结束时间（6）

u8 code water_time[]={
		0x00,0x00,0xE2,0x82,0x82,0x82,0x82,0x82,0x82,0x82,0x82,0xFE,0x00,0x00,0x00,0x00,
		0x00,0x00,0x3F,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x78,0x00,0x00,
		0x10,0x60,0x02,0x0C,0xC0,0x00,0x88,0x88,0x4F,0x58,0x24,0x54,0x84,0xE4,0x00,0x00,
		0x04,0x04,0x7C,0x03,0x00,0x82,0x42,0x32,0x0E,0x02,0x02,0x3E,0x42,0x42,0x72,0x00,
		0x00,0x20,0x20,0x20,0xA0,0x60,0x00,0xFF,0x60,0x80,0x40,0x20,0x18,0x00,0x00,0x00,
		0x20,0x10,0x08,0x06,0x01,0x40,0x80,0x7F,0x00,0x01,0x02,0x04,0x08,0x10,0x10,0x00,
		0x00,0xFC,0x84,0x84,0x84,0xFC,0x00,0x10,0x10,0x10,0x10,0x10,0xFF,0x10,0x10,0x00,
		0x00,0x3F,0x10,0x10,0x10,0x3F,0x00,0x00,0x01,0x06,0x40,0x80,0x7F,0x00,0x00,0x00,
		0x00,0xF8,0x01,0x06,0x00,0xF0,0x12,0x12,0x12,0xF2,0x02,0x02,0x02,0xFE,0x00,0x00,
		0x00,0xFF,0x00,0x00,0x00,0x1F,0x11,0x11,0x11,0x1F,0x00,0x40,0x80,0x7F,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x36,0x36,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};//已浇水时间：（6）

u8 code not_water[]={
		0x40,0x40,0x42,0xCC,0x00,0x00,0x02,0x82,0x42,0xF2,0x0E,0x42,0x82,0x02,0x00,0x00,
		0x00,0x40,0x20,0x1F,0x20,0x42,0x41,0x40,0x40,0x5F,0x40,0x40,0x40,0x43,0x40,0x00,
		0x80,0x80,0x88,0x88,0x88,0x88,0x88,0xFF,0x88,0x88,0x88,0x88,0x88,0x80,0x80,0x00,
		0x20,0x20,0x10,0x08,0x04,0x02,0x01,0xFF,0x01,0x02,0x04,0x08,0x10,0x20,0x20,0x00,
		0x80,0x82,0x82,0x82,0xFE,0x82,0x82,0x82,0x82,0x82,0xFE,0x82,0x82,0x82,0x80,0x00,
		0x00,0x80,0x40,0x30,0x0F,0x00,0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0x00,0x00,
		0x10,0x10,0xF0,0x1F,0x10,0xF0,0x00,0x40,0xE0,0x58,0x47,0x40,0x50,0x60,0xC0,0x00,
		0x40,0x22,0x15,0x08,0x16,0x21,0x00,0x00,0xFE,0x42,0x42,0x42,0x42,0xFE,0x00,0x00,
		0x10,0x60,0x02,0x0C,0xC0,0x00,0x88,0x88,0x4F,0x58,0x24,0x54,0x84,0xE4,0x00,0x00,
		0x04,0x04,0x7C,0x03,0x00,0x82,0x42,0x32,0x0E,0x02,0x02,0x3E,0x42,0x42,0x72,0x00,
		0x00,0x20,0x20,0x20,0xA0,0x60,0x00,0xFF,0x60,0x80,0x40,0x20,0x18,0x00,0x00,0x00,
		0x20,0x10,0x08,0x06,0x01,0x40,0x80,0x7F,0x00,0x01,0x02,0x04,0x08,0x10,0x10,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	  0x00,0x00,0x36,0x36,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};//未开始浇水:  (6)

u8 code end_water[]={
		0x00,0x00,0xE2,0x82,0x82,0x82,0x82,0x82,0x82,0x82,0x82,0xFE,0x00,0x00,0x00,0x00,
		0x00,0x00,0x3F,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x78,0x00,0x00,
		0x20,0x30,0xAC,0x63,0x20,0x18,0x08,0x48,0x48,0x48,0x7F,0x48,0x48,0x48,0x08,0x00,
		0x22,0x67,0x22,0x12,0x12,0x12,0x00,0xFE,0x42,0x42,0x42,0x42,0x42,0xFE,0x00,0x00,
		0x04,0x04,0xE4,0x24,0x24,0x24,0x24,0xFF,0x24,0x24,0x24,0x24,0xE4,0x04,0x04,0x00,
		0x40,0x40,0x27,0x22,0x12,0x0A,0x06,0xFF,0x06,0x0A,0x12,0x22,0x27,0x40,0x40,0x00,
		0x10,0x60,0x02,0x0C,0xC0,0x00,0x88,0x88,0x4F,0x58,0x24,0x54,0x84,0xE4,0x00,0x00,
		0x04,0x04,0x7C,0x03,0x00,0x82,0x42,0x32,0x0E,0x02,0x02,0x3E,0x42,0x42,0x72,0x00,
		0x00,0x20,0x20,0x20,0xA0,0x60,0x00,0xFF,0x60,0x80,0x40,0x20,0x18,0x00,0x00,0x00,
		0x20,0x10,0x08,0x06,0x01,0x40,0x80,0x7F,0x00,0x01,0x02,0x04,0x08,0x10,0x10,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x36,0x36,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};//已结束浇水:  (6)
		
u8 code weekday[]={
		0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x00,0x00,0x00,
		0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x00,
		0x00,0x04,0x84,0x84,0x84,0x84,0x84,0x84,0x84,0x84,0x84,0x84,0x84,0x04,0x00,0x00,
		0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x00,
		0x00,0xFC,0x04,0x04,0x04,0xFC,0x04,0x04,0x04,0xFC,0x04,0x04,0x04,0xFC,0x00,0x00,
		0x00,0x7F,0x28,0x24,0x23,0x20,0x20,0x20,0x20,0x21,0x22,0x22,0x22,0x7F,0x00,0x00,
		0x00,0x02,0x42,0x42,0x42,0xC2,0x7E,0x42,0x42,0x42,0x42,0xC2,0x02,0x02,0x00,0x00,
		0x40,0x40,0x40,0x40,0x78,0x47,0x40,0x40,0x40,0x40,0x40,0x7F,0x40,0x40,0x40,0x00,
		0x20,0x20,0x20,0x20,0x20,0x20,0x21,0x22,0x2C,0x20,0x20,0x20,0x20,0x20,0x20,0x00,
		0x00,0x40,0x20,0x10,0x0C,0x03,0x00,0x00,0x00,0x01,0x02,0x04,0x18,0x60,0x00,0x00,
		0x40,0x40,0x42,0x42,0x42,0x42,0x42,0xFE,0x42,0x42,0x42,0x42,0x42,0x40,0x40,0x00,
		0x80,0x80,0x40,0x20,0x10,0x0C,0x03,0x00,0x03,0x0C,0x10,0x20,0x40,0x80,0x80,0x00};
	
//--  文字:  0,1,2,3,4,5,6,7,8,9    宽x高=8x16
u8 code num[]={	
		0x00,0xE0,0x10,0x08,0x08,0x10,0xE0,0x00,0x00,0x0F,0x10,0x20,0x20,0x10,0x0F,0x00,
		0x00,0x10,0x10,0xF8,0x00,0x00,0x00,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00,
		0x00,0x70,0x08,0x08,0x08,0x88,0x70,0x00,0x00,0x30,0x28,0x24,0x22,0x21,0x30,0x00,
		0x00,0x30,0x08,0x88,0x88,0x48,0x30,0x00,0x00,0x18,0x20,0x20,0x20,0x11,0x0E,0x00,
		0x00,0x00,0xC0,0x20,0x10,0xF8,0x00,0x00,0x00,0x07,0x04,0x24,0x24,0x3F,0x24,0x00,
		0x00,0xF8,0x08,0x88,0x88,0x08,0x08,0x00,0x00,0x19,0x21,0x20,0x20,0x11,0x0E,0x00,
		0x00,0xE0,0x10,0x88,0x88,0x18,0x00,0x00,0x00,0x0F,0x11,0x20,0x20,0x11,0x0E,0x00,
		0x00,0x38,0x08,0x08,0xC8,0x28,0x18,0x00,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x00,
		0x00,0x70,0x88,0x08,0x08,0x88,0x70,0x00,0x00,0x1C,0x22,0x21,0x21,0x22,0x1C,0x00,
		0x00,0xE0,0x10,0x08,0x08,0x10,0xE0,0x00,0x00,0x08,0x31,0x22,0x22,0x11,0x0F,0x00};

u8 code kong[]={	
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
u8 code kong1[]={	
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};





/*******************************此程序初始化系统时钟使用22.1184MHz晶体为时钟源*****************************/   
/*void SYSCLK_Init (void)
{
    INT8U i;
    OSCXCN = 0x67; 							// 开启外部振荡器22.1184MHz晶体
    for (i=0; i < 255; i++) ; 				// 等待振荡器启振
    while (!(OSCXCN & 0x80)) ; 				// 等待晶体振荡器稳定
    OSCICN = 0x88; 							// 选择外部振荡器为系统时钟源并允许丢失时钟检测器
}			*/

/*************************************************LCD模块初始化********************************************/   
void Initial_Lcd()   
{   
    reset=0;                    /*低电平复位*/   
    delay(10);   
    reset=1;                    /*复位完毕*/   
    delay(10);                               
	transfer_command(0xe2);     /*软复位*/   
	delay(5);   
	transfer_command(0x2c);     /*升压步聚1*/   
	delay(5);       
	transfer_command(0x2e);     /*升压步聚2*/   
	delay(5);   
	transfer_command(0x2f);     /*升压步聚3*/   
	delay(5);   
	transfer_command(0x24);     /*粗调对比度，可设置范围 0x20～0x27*/   
	transfer_command(0x81);     /*微调对比度*/   
	transfer_command(0x12);     /*0x1a,微调对比度的值，可设置范围 0x00～0x3f*/   
	transfer_command(0xa2);     /*1/9偏压比（bias）*/   
	transfer_command(0xc8);     /*行扫描顺序：从上到下*/   
	transfer_command(0xa0);     /*列扫描顺序：从左到右*/   
	transfer_command(0x40);     /*起始行：第一行开始*/   
	transfer_command(0xaf);     /*开显示*/   
}   



/************************************************写指令到LCD模块***********************************************************/   
void transfer_command(int data1)           
{   
    char i;   
    cs1=0;   
    rs=0;   
    for(i=0;i<8;i++)   
    {   
        sclk=0; 
        lcddelay_us(2);   
        if(data1&0x80) sda=1;   
        else sda=0;   
        sclk=1;   
        lcddelay_us(2);   
        data1=data1<<=1;   
    }   
    cs1=1;   
}   



/**************************************************写数据到LCD模块*********************************************************/   
void transfer_data(int data1)   
{   
  char i;   
	cs1=0;   
	rs=1;   
	for(i=0;i<8;i++)   
	{   
		sclk=0;   
	  //lcddelay_us(1);   
		if(data1&0x80) sda=1;   
		else sda=0;   
		sclk=1;   
	  //lcddelay_us(1);   
		data1=data1<<=1;   
	}   
	cs1=1;   
}  

 
/******************************************************延时**********************************************************/   
void delay(int i)                                                           
{   
	int j,k;   
	for(j=0;j<i;j++)   
	for(k=0;k<110;k++);   
}   
/******************************************************延时*********************************************************/   
void lcddelay_us(int i)                                                           
{   
	int j,k;   
	for(j=0;j<i;j++)   
    for(k=0;k<1;k++);   
} 		  

/*void waitkey()   
{   
repeat:   
	if(key==1) goto repeat;   
	else delay(1000);   
} */

/************************************************定义地址***********************************************************/ 	  
void lcd_address(uchar page,uchar column)   
{   
	column=column-1;                               //我们平常所说的第1列，在LCD驱动IC里是第0列。所以在这里减去1. 
  page=page-1;   				                   /*设置页地址。每页是8行。一个画面的64行被分成8个页。我们平常所说的第1页，
	                                                 在LCD驱动IC里是第0页,所以在这里减去1*/ 
	transfer_command(0xb0+page);           
  transfer_command(((column>>4)&0x0f)+0x10);   //设置列地址的高4位  
	transfer_command(column&0x0f);   			 //设置列地址的低4位  
	 
}   
	
	
/************************************************全屏清屏***********************************************************/ 
void clear_screen() 
{   
	unsigned char i,j;   
    for(i=0;i<9;i++)
	 //for(i=0;i<2;i++)                //test lxq5.22   
	{   
		lcd_address(1+i,1);   
		for(j=0;j<132;j++)
    // 	for(j=0;j<2;j++)             //test lxq5.22
		{   
	         transfer_data(0x00);   
	    }   
    }   
}


/***********************写入的程序****************************/

void display_graphic_8x16(uchar page,uchar column,uchar *dp)   //x,y(page 为行，conlumn为列)
{   
	uchar i,j;   
	for(j=0;j<2;j++)   
	{   
		lcd_address(page+j,column);   
		for (i=0;i<8;i++)   
		{   
			transfer_data(*dp);   
			dp++;                 /*写数据到LCD,每写完一个8位的数据后列地址自动加1*/   
		}   
	}   
}

void display_graphic_16x16(uchar page,uchar column,uchar *dp)   
{   
	uchar i,j;   
	for(j=0;j<2;j++)   
	{   
		lcd_address(page+j,column);   
		for (i=0;i<16;i++)   
		{   
			transfer_data(*dp);   
			dp++;                 /*写数据到LCD,每写完一个8位的数据后列地址自动加1*/   
		}   
	}   
}

/*
void display_graphic_32x32(uchar page,uchar column,uchar *dp)   
{   
		uchar i,j;   
	for(j=0;j<4;j++)   
	{   
		lcd_address(page+j,column);   
		for (i=0;i<31;i++)   
		{   
		transfer_data(*dp);   
		dp++;   
															//写数据到LCD,每写完一个8位的数据后列地址自动加1 
		}   
	}   
}   */



void Time_Display() //时间显示
{
		display_graphic_8x16(5,(8+8*0),&num[2*16]);
	  display_graphic_8x16(5,(8+8*1),&num[0*16]);
		display_graphic_8x16(5,(8+8*2),&num[Clock[8]/16*16]);
		display_graphic_8x16(5,(8+8*3),&num[Clock[8]%16*16]);//年
		display_graphic_8x16(5,(8+8*4),heng);
		display_graphic_8x16(5,(8+8*5),&num[(Clock[7]&0x1f)/16*16]);
		display_graphic_8x16(5,(8+8*6),&num[(Clock[7]&0x1f)%16*16]);//月
		display_graphic_8x16(5,(8+8*7),heng);
		display_graphic_8x16(5,(8+8*8),&num[(Clock[5]&0x3f)/16*16]);
		display_graphic_8x16(5,(8+8*9),&num[(Clock[5]&0x3f)%16*16]);//日
		display_graphic_8x16(7,(16+8*0),&num[(Clock[4]&0x3f)/16*16]);
		display_graphic_8x16(7,(16+8*1),&num[(Clock[4]&0x3f)%16*16]);//时
		display_graphic_8x16(7,(16+8*2),maohao);
		display_graphic_8x16(7,(16+8*3),&num[(Clock[3]&0x7f)/16*16]);
		display_graphic_8x16(7,(16+8*4),&num[(Clock[3]&0x7f)%16*16]);//分
		display_graphic_8x16(7,(16+8*5),maohao);
		display_graphic_8x16(7,(16+8*6),&num[(Clock[2]&0x7f)/16*16]);
		display_graphic_8x16(7,(16+8*7),&num[(Clock[2]&0x7f)%16*16]);//秒

		display_graphic_16x16(3,(112),xing);
		display_graphic_16x16(5,(112),qi);
		display_graphic_16x16(7,(112),&weekday[(Clock[6]&0x07)*32]);
		//zifukong();//第一页时，第一行文字不显示即为空
}
int bcd_dec(uchar bat)	//BCD码转十进制
{
	uchar temp1,temp2,tol;
	temp1=bat&0x0f;
	temp2=(bat&0xf0)>>4;
	tol=temp2*10+temp1;
	return tol; 
}

void Time_Watch()
{
		u8 x,y,z,k;
		Time_Count[4]=((bcd_dec(Clock[4]&0x3f))-(Set_Begin[4]));//时 判断开始浇水时间
		Time_Count[3]=((bcd_dec(Clock[3]&0x7f))-(Set_Begin[3]));//分
		Time_Count[2]=((bcd_dec(Clock[2]&0x7f))-(Set_Begin[2]));//秒
		
		Time_Count[7]=((bcd_dec(Clock[4]&0x3f))-(Set_End[4]));//时   判断结束浇水时间
		Time_Count[6]=((bcd_dec(Clock[3]&0x7f))-(Set_End[3]));//分
		Time_Count[5]=((bcd_dec(Clock[2]&0x7f))-(Set_End[2]));//秒
		if(Time_Count[3]<0){Time_Count[4]-=1;Time_Count[3]+=60;}//当分数 （SET）>分数（TIME）为负 +60修正,小时-1
	  if(Time_Count[2]<0){Time_Count[3]-=1;Time_Count[2]+=60;}//当秒数 （SET）>秒数（TIME）为负 +60修正,分钟-1
		if(Time_Count[5]<0){Time_Count[6]-=1;Time_Count[5]+=60;}//当秒数 （END）>秒数（TIME）为负 +60修正,分钟-1
		if(Time_Count[6]<0){Time_Count[7]-=1;Time_Count[6]+=60;}//当分数 （END）>秒数（TIME）为负 +60修正,小时-1
		if(Judge_flag == 1)
		{
			if( (Time_Count[4]>0) )	Time_Count_flag=1;
			else if ( (Time_Count[4]==0)&&(Time_Count[3]>0) )Time_Count_flag=1;
			else if ( (Time_Count[4]==0)&&(Time_Count[3]==0)&&(Time_Count[2]>0) )Time_Count_flag=1;
			else Time_Count_flag=0;
		}
	
		if( (Time_Count[7]>=0)&&(Time_Count[6]>=0)&&(Time_Count[5]>=0)){Time_Count_flag=2;Judge_flag=0;}//结束浇水字幕显示一小时
		else Judge_flag=1;

		if(Time_Count_flag == 0)//未开始浇水
		{
			for(x=0;x<6;x++)
			{display_graphic_16x16(1,(8+16*x),&not_water[x*32]);}//还未开始浇水
			
			for(y=0;y<8;y++)
			{display_graphic_8x16(3,(16+8*y),kong);}
			Motor_IN=1;
		}
		
		if(Time_Count_flag == 1)//开始浇水
		{
			for(z=0;z<6;z++)
			{display_graphic_16x16(1,(8+16*z),&water_time[z*32]);}//已浇水时间：
			display_graphic_8x16(3,(16+8*0),&num[(Time_Count[4]/10*16)]);
			display_graphic_8x16(3,(16+8*1),&num[(Time_Count[4]%10*16)]);//时
			display_graphic_8x16(3,(16+8*2),maohao);
			display_graphic_8x16(3,(16+8*3),&num[(Time_Count[3]/10*16)]);
			display_graphic_8x16(3,(16+8*4),&num[(Time_Count[3]%10*16)]);//分
			display_graphic_8x16(3,(16+8*5),maohao);
			display_graphic_8x16(3,(16+8*6),&num[(Time_Count[2]/10*16)]);
			display_graphic_8x16(3,(16+8*7),&num[(Time_Count[2]%10*16)]);//秒
			display_graphic_16x16(1,(0+16*7),rain);
			Delay_ms(100);
			display_graphic_16x16(1,(0+16*7),kong1);
			Delay_ms(100);
			Motor_IN=0;
		}
		
		if(Time_Count_flag == 2)//结束浇水
		{
			for(k=0;k<6;k++)
			{display_graphic_16x16(1,(8+16*k),&end_water[k*32]);}//已结束浇水:
			
			for(y=0;y<8;y++)
			{display_graphic_8x16(3,(16+8*y),kong);}
			Motor_IN=1;
		}
	
	
		
}


