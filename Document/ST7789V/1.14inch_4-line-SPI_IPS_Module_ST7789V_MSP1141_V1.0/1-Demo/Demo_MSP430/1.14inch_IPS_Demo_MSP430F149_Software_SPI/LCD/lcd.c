//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//测试硬件：单片机MSP430F149,正点原子MSP430开发板,主频8MHZ，晶振8MHZ
//QDtech-TFT液晶驱动 for MSP430 IO模拟
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
//创建日期:2018/08/22
//版本：V1.0
//版权所有，盗版必究。 
//Copyright(C) 深圳市全动电子技术有限公司 2018-2028
//All rights reserved
/****************************************************************************************************
//=========================================电源接线================================================//
//     LCD模块                STM32单片机
//      VCC          接          3.3V         //电源
//      GND          接          GND          //电源地
//=======================================液晶屏数据线接线==========================================//
//本模块默认数据总线类型为4线制SPI总线
//     LCD模块                STM32单片机    
//       SDA         接          P31         //液晶屏SPI总线数据写信号
//=======================================液晶屏控制线接线==========================================//
//     LCD模块 					      STM32单片机 
//       BLK         接          P20         //液晶屏背光控制信号，如果不需要控制，接3.3V
//       SCL         接          P33         //液晶屏SPI总线时钟信号
//       DC          接          P21         //液晶屏数据/命令控制信号
//       RES         接          P22         //液晶屏复位控制信号
//       CS          接          P23         //液晶屏片选控制信号
//=========================================触摸屏触接线=========================================//
//如果模块不带触摸功能或者带有触摸功能，但是不需要触摸功能，则不需要进行触摸屏接线
//	   LCD模块                STM32单片机 
//      T_IRQ        接          PC10         //触摸屏触摸中断信号
//      T_DO         接          PC2          //触摸屏SPI总线读信号
//      T_DIN        接          PC3          //触摸屏SPI总线写信号
//      T_CS         接          PC13         //触摸屏片选控制信号
//      T_CLK        接          PC0          //触摸屏SPI总线时钟信号
**************************************************************************************************/	
 /* @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, QD electronic SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
**************************************************************************************************/
#include "lcd.h"
#include "stdlib.h"
#include "delay.h"	 
#include "spi.h"

	   
//管理LCD重要参数
//默认为竖屏
_lcd_dev lcddev;

//画笔颜色,背景颜色
u16 POINT_COLOR = 0x0000,BACK_COLOR = 0xFFFF;  
u16 DeviceCode;	 

/*****************************************************************************
 * @name       :void LCD_WR_REG(u8 data)
 * @date       :2018-08-09 
 * @function   :Write an 8-bit command to the LCD screen
 * @parameters :data:Command value to be written
 * @retvalue   :None
******************************************************************************/
void LCD_WR_REG(u8 data)
{    
   LCD_CS_CLR;     
   LCD_RS_CLR;	  
   SPI_WriteByte(data);
   LCD_CS_SET;	
}

/*****************************************************************************
 * @name       :void LCD_WR_DATA(u8 data)
 * @date       :2018-08-09 
 * @function   :Write an 8-bit data to the LCD screen
 * @parameters :data:data value to be written
 * @retvalue   :None
******************************************************************************/
void LCD_WR_DATA(u8 data)
{
   LCD_CS_CLR;
   LCD_RS_SET;
   SPI_WriteByte(data);
   LCD_CS_SET;
}

/*****************************************************************************
 * @name       :void LCD_WriteReg(u8 LCD_Reg, u16 LCD_RegValue)
 * @date       :2018-08-09 
 * @function   :Write data into registers
 * @parameters :LCD_Reg:Register address
                LCD_RegValue:Data to be written
 * @retvalue   :None
******************************************************************************/
void LCD_WriteReg(u8 LCD_Reg, u16 LCD_RegValue)
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
 * @name       :void Lcd_WriteData_16Bit(u16 Data)
 * @date       :2018-08-09 
 * @function   :Write an 16-bit command to the LCD screen
 * @parameters :Data:Data to be written
 * @retvalue   :None
******************************************************************************/	 
void Lcd_WriteData_16Bit(u16 Data)
{	
   LCD_CS_CLR;
   LCD_RS_SET;  
   SPI_WriteByte(Data>>8);
   SPI_WriteByte(Data);
   LCD_CS_SET;
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
  LCD_SetCursor(x,y);//设置光标位置 
  Lcd_WriteData_16Bit(POINT_COLOR); 
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
  unsigned int i,m;  
  LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);   
	LCD_CS_CLR;
  LCD_RS_SET;
  for(i=0;i<lcddev.height;i++)
  {
    for(m=0;m<lcddev.width;m++)
    {	
	SPI_WriteByte(Color>>8);
	SPI_WriteByte(Color);
    }
  }
	 LCD_CS_SET;
} 

/*****************************************************************************
 * @name       :void LCD_GPIOInit(void)
 * @date       :2018-08-09 
 * @function   :Initialization LCD screen GPIO
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	
void LCD_GPIOInit(void)
{
  P2DIR|=LED+LCD_RS+LCD_RST+LCD_CS;
}

/*****************************************************************************
 * @name       :void LCD_RESET(void)
 * @date       :2018-08-09 
 * @function   :Reset LCD screen
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	
void LCD_RESET(void)
{
	LCD_RST_CLR;
	delay_ms(20);	
	LCD_RST_SET;
	delay_ms(20);
}

/*****************************************************************************
 * @name       :void LCD_RESET(void)
 * @date       :2018-08-09 
 * @function   :Initialization LCD screen
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	 	 
void LCD_Init(void)
{  
	SPI_Init(); //硬件SPI初始化
	LCD_GPIOInit();//LCD GPIO初始化										 
 	LCD_RESET(); //LCD 复位
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
	
        LCD_direction(USE_HORIZONTAL);//设置LCD显示方向
	LCD_LED(1);//点亮背光	 
	LCD_Clear(WHITE);//清全屏白色
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
