#ifndef ST7789V_H
#define ST7789V_H

#include "Foundation.h"
#include "stdlib.h"
#include "gd32vf103_gpio.h"

#define USE_HORIZONTAL  0  //设置横屏或者竖屏显示 0或1为竖屏 2或3为横屏

// #if USE_HORIZONTAL==0||USE_HORIZONTAL==2
// #define ST7789VW 240
// #define ST7789VH 135
// #else
// #define ST7789VW 135
// #define ST7789VH 240
// #endif

typedef unsigned char u8;
typedef unsigned int u16;
typedef unsigned long u32;

#define SPI0_CFG 1  //hardware spi
// #define SPI0_CFG 2  //hardware spi dma
// #define SPI0_CFG 3  //software spi

#define FRAME_SIZE  25600

//-----------------OLED端口定义----------------
#if SPI0_CFG == 1
#define OLED_SCLK_Clr()
#define OLED_SCLK_Set()

#define OLED_SDIN_Clr()
#define OLED_SDIN_Set()

#define OLED_CS_Clr() gpio_bit_reset(GPIOB,GPIO_PIN_2)     //CS PB2
#define OLED_CS_Set() gpio_bit_set(GPIOB,GPIO_PIN_2)
#elif SPI0_CFG == 2
#define OLED_SCLK_Clr()
#define OLED_SCLK_Set()

#define OLED_SDIN_Clr()
#define OLED_SDIN_Set()

#define OLED_CS_Clr()
#define OLED_CS_Set()
#else /* SPI0_CFG */
#define OLED_SCLK_Clr() gpio_bit_reset(GPIOA,GPIO_PIN_5)    //CLK PA5
#define OLED_SCLK_Set() gpio_bit_set(GPIOA,GPIO_PIN_5)

#define OLED_SDIN_Clr() gpio_bit_reset(GPIOA,GPIO_PIN_7)    //DIN PA7
#define OLED_SDIN_Set() gpio_bit_set(GPIOA,GPIO_PIN_7)

#define OLED_CS_Clr()  gpio_bit_reset(GPIOB,GPIO_PIN_2)     //CS PB2
#define OLED_CS_Set()  gpio_bit_set(GPIOB,GPIO_PIN_2)
#endif /* SPI0_CFG */

extern  u16 BACK_COLOR;   //背景色

void ST7789VSPISendData(u8 data);
void ST7789VSPISendData8Bit(u8 data);
void ST7789VSPISendData16Bit(u16 data);
void ST7789VSPISendCommand(u8 command);
void ST7789VAddressSet(u16 x1, u16 y1, u16 x2, u16 y2);
void ST7789VInit(void);
void ST7789VSetRotation(uint8_t m);
void ST7789VBacklightEnable(bool enable);
void ST7789VClear(u16 Color);
void ST7789VSetBackgroundColor(u16 color);
void ST7789VShowChinese(u16 x, u16 y, u8 index, u8 size, u16 color);
void ST7789VDrawPoint(u16 x, u16 y, u16 color);
void ST7789VDrawPoint_big(u16 x, u16 y, u16 color);
void ST7789VFill(u16 xsta, u16 ysta, u16 xend, u16 yend, u16 color);
void ST7789VDrawLine(u16 x1, u16 y1, u16 x2, u16 y2, u16 color);
void ST7789VDrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2, u16 color);
void ST7789VDrawCircle(u16 x0, u16 y0, u8 r, u16 color);
void ST7789VShowChar(u16 x, u16 y, u8 num, u8 mode, u16 color);
void ST7789VShowString(u16 x, u16 y, const u8 *p, u16 color);
u32 ST7789Pow(u8 m, u8 n);
void ST7789VShowNum(u16 x, u16 y, u16 num, u8 len, u16 color);
void ST7789VShowNum1(u16 x, u16 y, float num, u8 len, u16 color);
void ST7789VShowPicture(u16 x1, u16 y1, u16 x2, u16 y2);
void ST7789VShowLogo(void);


//画笔颜色
#define WHITE            0xFFFF
#define BLACK            0x0000
#define BLUE             0x001F
#define BRED             0XF81F
#define GRED             0XFFE0
#define GBLUE            0X07FF
#define RED              0xF800
#define MAGENTA          0xF81F
#define GREEN            0x07E0
#define CYAN             0x7FFF
#define YELLOW           0xFFE0
#define BROWN            0XBC40 //棕色
#define BRRED            0XFC07 //棕红色
#define GRAY             0X8430 //灰色
//GUI颜色

#define DARKBLUE         0X01CF //深蓝色
#define LIGHTBLUE        0X7D7C //浅蓝色  
#define GRAYBLUE         0X5458 //灰蓝色
//以上三色为PANEL的颜色

#define LIGHTGREEN       0X841F //浅绿色
#define LGRAY            0XC618 //浅灰色(PANNEL),窗体背景色

#define LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)

#endif





