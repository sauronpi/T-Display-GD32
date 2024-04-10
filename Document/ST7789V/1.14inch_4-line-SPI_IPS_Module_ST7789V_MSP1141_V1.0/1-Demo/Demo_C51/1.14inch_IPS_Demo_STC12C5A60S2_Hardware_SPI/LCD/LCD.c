//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//测试硬件：单片机STC12C5A60S2,晶振30M  单片机工作电压3.3V
//QDtech-TFT液晶驱动 for C51
//xiao冯@ShenZhen QDtech co.,LTD
//公司网站:www.qdtft.com
//淘宝网站：http://qdtech.taobao.com
//wiki技术网站：http://www.lcdwiki.com
//我司提供技术支持，任何技术问题欢迎随时交流学习
//固话(传真) :+86 0755-23594567 
//手机:15989313508（冯工） 
//邮箱:lcdwiki01@gmail.com    support@lcdwiki.com    goodtft@163.com
//技术支持QQ:3002773612  3002778157
//技术交流QQ群:324828016
//创建日期:2018/8/22
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 深圳市全动电子技术有限公司 2018-2028
//All rights reserved
//********************************************************************************
//=========================================电源接线================================================//
//VCC接DC 3.3V电源
//GND接地
//=======================================液晶屏数据线接线==========================================//
//本模块默认数据总线类型为4线制SPI
//液晶屏模块            单片机
//  SDA          接       P15        //SPI写信号
//=======================================液晶屏控制线接线==========================================//
//液晶屏模块            单片机
//  CS           接       P13       //片选信号
//  RES          接       P33       //复位信号
//  DC           接       P12       //数据/命令选择控制信号
//  SCL          接       P17       //SPI时钟信号
//  BLK          接       P32       //背光控制信号，如果不需要控制，接3.3V
//=========================================触摸屏接线=========================================//
//STC89C52RC单片机ROM太小，无法运行带触摸功能的程序，所以触摸屏不需要接线
//不使用触摸或者模块本身不带触摸，则可不连接
//触摸屏使用的数据总线类型为SPI
//触摸屏模块            单片机
//  T_CLK        接       P36       //触摸屏SPI时钟信号
//  T_CS         接       P37       //触摸屏片选控制信号
//  T_DIN        接       P34       //触摸屏SPI写信号
//  T_DO         接       P35       //触摸屏SPI读信号
//  T_IRQ        接       P40       //触摸屏响应检查信号，如单片机无P4组，请自行更改其他可用IO并修改代码定义
//**************************************************************************************************/	
#include "sys.h"
#include "lcd.h"

//LCD的画笔颜色和背景色	   
u16 POINT_COLOR=0x0000;	//画笔颜色
u16 BACK_COLOR=0xFFFF;  //背景色 
//管理LCD重要参数
//默认为竖屏
_lcd_dev lcddev;

void  delay_ms_new(unsigned char ms)
{
     unsigned int i;
	 do{
	      i = MAIN_Fosc / 14000;
		  while(--i)	;   //14T per loop
     }while(--ms);
}

void	SPI_init(void)	
{
	SPCTL = (SSIG << 7)+(SPEN << 6)+(DORD << 5)+(MSTR << 4)+(CPOL << 3)+(CPHA << 2)+SPEED_16;
}

/*****************************************************************************
 * @name       :void SPI_WriteByte(u8 byte)
 * @date       :2018-08-09 
 * @function   :Write a byte of data using C51's Hardware SPI
 * @parameters :d:Data to be written
 * @retvalue   :None
******************************************************************************/
void SPI_WriteByte(u8 byte)
{			
			SPDAT = byte;		//发送一个字节
			while((SPSTAT & SPIF)==0)	;	//等待发送完成
			SPSTAT = SPIF+WCOL;			//清0 SPIF和WCOL标志
}

/*****************************************************************************
 * @name       :void LCD_WR_REG(u8 data)
 * @date       :2018-08-09 
 * @function   :Write an 8-bit command to the LCD screen
 * @parameters :data:Command value to be written
 * @retvalue   :None
******************************************************************************/
void LCD_WR_REG(u8 Reg)	 
{	
	LCD_CS=0;
	LCD_RS=0;
	SPI_WriteByte(Reg);
	LCD_CS=1;
} 

/*****************************************************************************
 * @name       :void LCD_WR_DATA(u8 data)
 * @date       :2018-08-09 
 * @function   :Write an 8-bit data to the LCD screen
 * @parameters :data:data value to be written
 * @retvalue   :None
******************************************************************************/
void LCD_WR_DATA(u8 Data)
{
	LCD_CS=0;
	LCD_RS=1;
	SPI_WriteByte(Data);
	LCD_CS=1;
}

/*****************************************************************************
 * @name       :void LCD_WR_DATA_16Bit(u16 Data)
 * @date       :2018-08-09 
 * @function   :Write an 16-bit command to the LCD screen
 * @parameters :Data:Data to be written
 * @retvalue   :None
******************************************************************************/	 
void LCD_WR_DATA_16Bit(u16 Data)
{
	LCD_CS=0;
	LCD_RS=1;
	SPI_WriteByte(Data>>8);
	SPI_WriteByte(Data);
	LCD_CS=1;
}

/*****************************************************************************
 * @name       :void LCD_WriteReg(u8 LCD_Reg, u16 LCD_RegValue)
 * @date       :2018-08-09 
 * @function   :Write data into registers
 * @parameters :LCD_Reg:Register address
                LCD_RegValue:Data to be written
 * @retvalue   :None
******************************************************************************/
void LCD_WriteReg(u8 LCD_Reg, u8 LCD_RegValue)
{
  LCD_WR_REG(LCD_Reg);
	LCD_WR_DATA(LCD_RegValue);
}

/*****************************************************************************
 * @name       :void LCD_WriteRAM_Prepare(void)
 * @date       :2018-08-09 
 * @function   :Write GRAM
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	
void LCD_WriteRAM_Prepare(void)
{
 	LCD_WR_REG(lcddev.wramcmd);	  
}

/*****************************************************************************
 * @name       :void LCD_Clear(u16 Color)
 * @date       :2018-08-09 
 * @function   :Full screen filled LCD screen
 * @parameters :color:Filled color
 * @retvalue   :None
******************************************************************************/	
void LCD_Clear(u16 Color)
{
	u16 i,j;
	LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);	
	LCD_CS=0;
	LCD_RS=1;
    for(i=0;i<lcddev.width;i++)
	{
	  for (j=0;j<lcddev.height;j++)
	   	{
        SPI_WriteByte(Color>>8);
				SPI_WriteByte(Color);
	    }
	}
	LCD_CS=1;
}

/*****************************************************************************
 * @name       :void LCD_DrawPoint(u16 x,u16 y)
 * @date       :2018-08-09 
 * @function   :Write a pixel data at a specified location
 * @parameters :x:the x coordinate of the pixel
                y:the y coordinate of the pixel
 * @retvalue   :None
******************************************************************************/	
void LCD_DrawPoint(u16 x,u16 y)
{
	LCD_SetWindows(x,y,x,y);//设置光标位置 
	LCD_WR_DATA_16Bit(POINT_COLOR); 	    
} 	 

/*****************************************************************************
 * @name       :void LCD_Reset(void)
 * @date       :2018-08-09 
 * @function   :Reset LCD screen
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	
void LCD_Reset(void)
{	
	LCD_RESET=0;
	delay_ms(20);
	LCD_RESET=1;
	delay_ms(20);
}

/*****************************************************************************
 * @name       :void LCD_Init(void)
 * @date       :2018-08-09 
 * @function   :Initialization LCD screen
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	 	 
void LCD_Init(void)
{
	SPI_init(); //初始化硬件SPI
	LCD_Reset(); //初始化之前复位
//************* ST7789V初始化**********//	
	delay_ms(120);                //ms
	LCD_WR_REG(0x11);     //Sleep out
	delay_ms(120);                //Delay 120ms
	LCD_WR_REG(0x36);     
	LCD_WR_DATA(0x00);  
	LCD_WR_REG(0x3A);     
	LCD_WR_DATA(0x05);  //0x05( 65K Color) 
	LCD_WR_REG(0x21);     
	LCD_WR_REG(0xB2);     
	LCD_WR_DATA(0x05);   
	LCD_WR_DATA(0x05);   
	LCD_WR_DATA(0x00);   
	LCD_WR_DATA(0x33);   
	LCD_WR_DATA(0x33);   
	LCD_WR_REG(0xB7);     
	LCD_WR_DATA(0x23);   
	LCD_WR_REG(0xBB);     
	LCD_WR_DATA(0x22);   
	LCD_WR_REG(0xC0);     
	LCD_WR_DATA(0x2C);   
	LCD_WR_REG(0xC2);     
	LCD_WR_DATA(0x01);   
	LCD_WR_REG(0xC3);     
	LCD_WR_DATA(0x13);   
	LCD_WR_REG(0xC4);     
	LCD_WR_DATA(0x20);   
	LCD_WR_REG(0xC6);     
	LCD_WR_DATA(0x0F);   
	LCD_WR_REG(0xD0);     
	LCD_WR_DATA(0xA4);   
	LCD_WR_DATA(0xA1);   
	LCD_WR_REG(0xD6);     
	LCD_WR_DATA(0xA1);   
	LCD_WR_REG(0xE0);
	LCD_WR_DATA(0x70);
	LCD_WR_DATA(0x06);
	LCD_WR_DATA(0x0C);
	LCD_WR_DATA(0x08);
	LCD_WR_DATA(0x09);
	LCD_WR_DATA(0x27);
	LCD_WR_DATA(0x2E);
	LCD_WR_DATA(0x34);
	LCD_WR_DATA(0x46);
	LCD_WR_DATA(0x37);
	LCD_WR_DATA(0x13);
	LCD_WR_DATA(0x13);
	LCD_WR_DATA(0x25);
	LCD_WR_DATA(0x2A);
	LCD_WR_REG(0xE1);
	LCD_WR_DATA(0x70);
	LCD_WR_DATA(0x04);
	LCD_WR_DATA(0x08);
	LCD_WR_DATA(0x09);
	LCD_WR_DATA(0x07);
	LCD_WR_DATA(0x03);
	LCD_WR_DATA(0x2C);
	LCD_WR_DATA(0x42);
	LCD_WR_DATA(0x42);
	LCD_WR_DATA(0x38);
	LCD_WR_DATA(0x14);
	LCD_WR_DATA(0x14);
	LCD_WR_DATA(0x27);
	LCD_WR_DATA(0x2C);
	LCD_WR_REG(0x29);     //Display on
	//	LCD_WR_REG(0x2A);     //Column Address Set
	//	LCD_WR_DATA(0x00);   
	//	LCD_WR_DATA(0x34);   //52
	//	LCD_WR_DATA(0x00);   
	//	LCD_WR_DATA(0xBA);   //186
	//	LCD_WR_REG(0x2B);     //Row Address Set
	//	LCD_WR_DATA(0x00);   
	//	LCD_WR_DATA(0x28);   //40
	//	LCD_WR_DATA(0x01);   
	//	LCD_WR_DATA(0x17);   //279
	//	LCD_WR_REG(0x2C);
	//	LCD_WR_REG(0x11);     //Sleep out
	//	delay_ms(120);                //Delay 120ms
	//	LCD_WR_REG(0x10);     //Sleep in
	//	delay_ms(120);                //Delay 120ms
	//设置LCD属性参数
	LCD_direction(USE_HORIZONTAL);//设置LCD显示方向 
	LCD_BL=1;//点亮背光	 
}
 
/*****************************************************************************
 * @name       :void LCD_SetWindows(u16 xStar, u16 yStar,u16 xEnd,u16 yEnd)
 * @date       :2018-08-09 
 * @function   :Setting LCD display window
 * @parameters :xStar:the bebinning x coordinate of the LCD display window
								yStar:the bebinning y coordinate of the LCD display window
								xEnd:the endning x coordinate of the LCD display window
								yEnd:the endning y coordinate of the LCD display window
 * @retvalue   :None
******************************************************************************/ 
void LCD_SetWindows(u16 xStar, u16 yStar,u16 xEnd,u16 yEnd)
{	
	LCD_WR_REG(lcddev.setxcmd);	
	LCD_WR_DATA((xStar+lcddev.xoffset)>>8);
	LCD_WR_DATA(xStar+lcddev.xoffset);		
	LCD_WR_DATA((xEnd+lcddev.xoffset)>>8);
	LCD_WR_DATA(xEnd+lcddev.xoffset);

	LCD_WR_REG(lcddev.setycmd);	
	LCD_WR_DATA((yStar+lcddev.yoffset)>>8);
	LCD_WR_DATA(yStar+lcddev.yoffset);		
	LCD_WR_DATA((yEnd+lcddev.yoffset)>>8);
	LCD_WR_DATA(yEnd+lcddev.yoffset);

	LCD_WriteRAM_Prepare();	//开始写入GRAM				
}   

/*****************************************************************************
 * @name       :void LCD_SetCursor(u16 Xpos, u16 Ypos)
 * @date       :2018-08-09 
 * @function   :Set coordinate value
 * @parameters :Xpos:the  x coordinate of the pixel
								Ypos:the  y coordinate of the pixel
 * @retvalue   :None
******************************************************************************/ 
void LCD_SetCursor(u16 Xpos, u16 Ypos)
{	  	    			
	LCD_SetWindows(Xpos,Ypos,Xpos,Ypos);	
} 

/*****************************************************************************
 * @name       :void LCD_direction(u8 direction)
 * @date       :2018-08-09 
 * @function   :Setting the display direction of LCD screen
 * @parameters :direction:0-0 degree
                          1-90 degree
													2-180 degree
													3-270 degree
 * @retvalue   :None
******************************************************************************/ 
void LCD_direction(u8 direction)
{ 
	lcddev.setxcmd=0x2A;
			lcddev.setycmd=0x2B;
			lcddev.wramcmd=0x2C;
	switch(direction){		  
		case 0:						 	 		
			lcddev.width=LCD_W;
			lcddev.height=LCD_H;	
			lcddev.xoffset=52;
			lcddev.yoffset=40;
			LCD_WriteReg(0x36,0);//BGR==1,MY==0,MX==0,MV==0
		break;
		case 1:
			lcddev.width=LCD_H;
			lcddev.height=LCD_W;
			lcddev.xoffset=40;
			lcddev.yoffset=53;
			LCD_WriteReg(0x36,(1<<6)|(1<<5));//BGR==1,MY==1,MX==0,MV==1
		break;
		case 2:						 	 		
			lcddev.width=LCD_W;
			lcddev.height=LCD_H;
     		lcddev.xoffset=53;
			lcddev.yoffset=40;			
			LCD_WriteReg(0x36,(1<<6)|(1<<7));//BGR==1,MY==0,MX==0,MV==0
		break;
		case 3:
			lcddev.width=LCD_H;
			lcddev.height=LCD_W;
			lcddev.xoffset=40;
			lcddev.yoffset=52;
			LCD_WriteReg(0x36,(1<<7)|(1<<5));//BGR==1,MY==1,MX==0,MV==1
		break;	
		default:break;
	}		
}	 





