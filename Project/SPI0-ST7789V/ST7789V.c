#include "ST7789V.h"
#include "oledfont.h"
#include "bmp.h"

#include "PCBConfig.h"
#define ST7789V_CS_PORT                     GPIOB
#define ST7789V_CS_PIN                      GPIO_PIN_2
#define ST7789V_CS_ACTIVE                   RESET
#define ST7789VChipSelect()                 gpio_bit_write(ST7789V_CS_PORT, ST7789V_CS_PIN, ST7789V_CS_ACTIVE)
#define ST7789VChipDeselect()               gpio_bit_write(ST7789V_CS_PORT, ST7789V_CS_PIN, ~ST7789V_CS_ACTIVE)

#define ST7789V_RESET_PORT                  GPIOB
#define ST7789V_RESET_PIN                   GPIO_PIN_1
#define ST7789V_RESET_ACTIVE                RESET
#define ST7789VResetActive()                gpio_bit_write(ST7789V_RESET_PORT, ST7789V_RESET_PIN, ST7789V_RESET_ACTIVE)
#define ST7789VResetDeactive()              gpio_bit_write(ST7789V_RESET_PORT, ST7789V_RESET_PIN, ~ST7789V_RESET_ACTIVE)

#define ST7789V_DC_PORT                     GPIOB
#define ST7789V_DC_PIN                      GPIO_PIN_0
#define ST7789VDataMode()                   gpio_bit_write(ST7789V_DC_PORT, ST7789V_DC_PIN, SET)
#define ST7789VCommandMode()                gpio_bit_write(ST7789V_DC_PORT, ST7789V_DC_PIN, RESET)

#define ST7789V_BLACKLIGHT_PORT             GPIOB
#define ST7789V_BLACKLIGHT_PORT_MODE        GPIO_MODE_OUT_PP
#define ST7789V_BLACKLIGHT_PIN              GPIO_PIN_10
#define ST7789V_BLACKLIGHT_ACTIVE           SET
#define ST7789VBlackLightActive()           gpio_bit_write(ST7789V_BLACKLIGHT_PORT, ST7789V_BLACKLIGHT_PIN, ST7789V_BLACKLIGHT_ACTIVE)
#define ST7789VBlackLightDeactive()         gpio_bit_write(ST7789V_BLACKLIGHT_PORT, ST7789V_BLACKLIGHT_PIN, ~ST7789V_BLACKLIGHT_ACTIVE)

#define ST7789_SLPOUT       0x11
#define ST7789_NORON        0x13
#define ST7789_MADCTL       0x36      // Memory data access control
#define TFT_MAD_RGB         0x08
#define ST7789_COLMOD       0x3A
#define ST7789_PORCTRL      0xB2      // Porch control
#define ST7789_GCTRL        0xB7      // Gate control
#define ST7789_VCOMS        0xBB      // VCOMS setting
#define ST7789_LCMCTRL      0xC0      // LCM control
#define ST7789_VDVVRHEN     0xC2      // VDV and VRH command enable
#define ST7789_VRHS         0xC3      // VRH set
#define ST7789_VDVSET       0xC4      // VDV setting
#define ST7789_FRCTR2       0xC6      // FR Control 2
#define ST7789_PWCTRL1      0xD0      // Power control 1
#define ST7789_PVGAMCTRL    0xE0      // Positive voltage gamma control
#define ST7789_NVGAMCTRL    0xE1      // Negative voltage gamma control
#define ST7789_INVON        0x21
#define ST7789_CASET        0x2A
#define ST7789_RASET        0x2B
#define ST7789_RAMWR        0x2C
#define ST7789_DISPOFF      0x28
#define ST7789_DISPON       0x29
#define TFT_MAD_COLOR_ORDER TFT_MAD_RGB
#define TFT_MAD_MY          0x80
#define TFT_MAD_MX          0x40
#define TFT_MAD_MV          0x20
#define TFT_MAD_ML          0x10


u16 BACK_COLOR = 0;   //背景色
u16 colstart = 52;
u16 rowstart = 40;
u16 _init_height = 240;
u16 _init_width = 135;
u16 _width = 135;
u16 _height = 240;
u8 rotation = 0;

/******************************************************************************
      函数说明：LCD串行数据写入函数
      入口数据：dat  要写入的串行数据
      返回值：  无
******************************************************************************/
void ST7789VSPISendData(u8 data)
{
#if SPI0_CFG == 1
    OLED_CS_Clr();

    while (RESET == spi_i2s_flag_get(SPI0, SPI_FLAG_TBE));
    spi_i2s_data_transmit(SPI0, data);
    while (RESET == spi_i2s_flag_get(SPI0, SPI_FLAG_RBNE));
    spi_i2s_data_receive(SPI0);

    OLED_CS_Set();
#elif SPI0_CFG == 2
    spi_dma_enable(SPI0, SPI_DMA_TRANSMIT);
#else
    u8 i;
    OLED_CS_Clr();
    for (i = 0; i < 8; i++) {
        OLED_SCLK_Clr();
        if (data & 0x80)
            OLED_SDIN_Set();
        else
            OLED_SDIN_Clr();
        OLED_SCLK_Set();
        data <<= 1;
    }
    OLED_CS_Set();
#endif
}


/******************************************************************************
      函数说明：LCD写入数据
      入口数据：dat 写入的数据
      返回值：  无
******************************************************************************/
void ST7789VSPISendData8Bit(u8 data)
{
    ST7789VDataMode();
    ST7789VSPISendData(data);
}


/******************************************************************************
      函数说明：LCD写入数据
      入口数据：dat 写入的数据
      返回值：  无
******************************************************************************/
void ST7789VSPISendData16Bit(u16 data)
{
    ST7789VDataMode();
    ST7789VSPISendData(data >> 8);
    ST7789VSPISendData(data);
}


/******************************************************************************
      函数说明：LCD写入命令
      入口数据：dat 写入的命令
      返回值：  无
******************************************************************************/
void ST7789VSPISendCommand(u8 command)
{
    ST7789VCommandMode();
    ST7789VSPISendData(command);
}


/******************************************************************************
      函数说明：设置起始和结束地址
      入口数据：x1,x2 设置列的起始和结束地址
                y1,y2 设置行的起始和结束地址
      返回值：  无
******************************************************************************/
void ST7789VAddressSet(u16 x1, u16 y1, u16 x2, u16 y2)
{
    ST7789VSPISendCommand(0x2a);//列地址设置
    ST7789VSPISendData16Bit(x1 + colstart);
    ST7789VSPISendData16Bit(x2 + colstart);
    ST7789VSPISendCommand(0x2b);//行地址设置
    ST7789VSPISendData16Bit(y1 + rowstart);
    ST7789VSPISendData16Bit(y2 + rowstart);
    ST7789VSPISendCommand(0x2c);//储存器写
}

#if SPI0_CFG == 2
/*!
    \brief      configure the DMA peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
void dma_config(void)
{
    dma_parameter_struct dma_init_struct;

    /* SPI0 transmit dma config:DMA0,DMA_CH2 */
    dma_deinit(DMA0, DMA_CH2);
    dma_struct_para_init(&dma_init_struct);

    dma_init_struct.periph_addr  = (uint32_t)&SPI_DATA(SPI0);
    dma_init_struct.memory_addr  = (uint32_t)image;
    dma_init_struct.direction    = DMA_MEMORY_TO_PERIPHERAL;
    dma_init_struct.memory_width = DMA_MEMORY_WIDTH_8BIT;
    dma_init_struct.periph_width = DMA_PERIPHERAL_WIDTH_8BIT;
    dma_init_struct.priority     = DMA_PRIORITY_LOW;
    dma_init_struct.number       = FRAME_SIZE;
    dma_init_struct.periph_inc   = DMA_PERIPH_INCREASE_DISABLE;
    dma_init_struct.memory_inc   = DMA_MEMORY_INCREASE_ENABLE;
    dma_init(DMA0, DMA_CH2, &dma_init_struct);
    /* configure DMA mode */
    dma_circulation_disable(DMA0, DMA_CH2);
    dma_memory_to_memory_disable(DMA0, DMA_CH2);
}
#endif

#if SPI0_CFG == 1
/*!
    \brief      configure the SPI peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
void SPIConfig(void)
{
    spi_parameter_struct spi_init_struct;
    /* deinitilize SPI and the parameters */
    ST7789VChipSelect();
    spi_struct_para_init(&spi_init_struct);

    /* SPI0 parameter config */
    spi_init_struct.trans_mode           = SPI_TRANSMODE_FULLDUPLEX;
    spi_init_struct.device_mode          = SPI_MASTER;
    spi_init_struct.frame_size           = SPI_FRAMESIZE_8BIT;
    spi_init_struct.clock_polarity_phase = SPI_CK_PL_HIGH_PH_2EDGE;
    spi_init_struct.nss                  = SPI_NSS_SOFT;
    spi_init_struct.prescale             = SPI_PSC_8;
    spi_init_struct.endian               = SPI_ENDIAN_MSB;
    spi_init(SPI0, &spi_init_struct);

    spi_crc_polynomial_set(SPI0, 7);
    spi_enable(SPI0);
}
#endif

void ST7789VGPIOInit(void)
{
    /* SPI0 GPIO config: NSS/PA4, SCK/PA5, MOSI/PA7 */
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_AF);
    rcu_periph_clock_enable(RCU_SPI0);
    gpio_init(SPI0_GPIO_PORT, SPI0_GPIO_MODE, GPIO_OSPEED_50MHZ, SPI0_SCK_PIN | SPI0_MISO_PIN | SPI0_MOSI_PIN);
    SPIConfig();

    rcu_periph_clock_enable(RCU_GPIOB);
    /* DC/PB0, Rest/PB1 */
    gpio_bit_reset(GPIOB, GPIO_PIN_0 | GPIO_PIN_1);
    gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_0 | GPIO_PIN_1);

    /* Backlight power control pin / PB10 */    
    gpio_bit_write(ST7789V_BLACKLIGHT_PORT, ST7789V_BLACKLIGHT_PIN, ~ST7789V_BLACKLIGHT_ACTIVE);
    gpio_init(ST7789V_BLACKLIGHT_PORT, ST7789V_BLACKLIGHT_PORT_MODE, GPIO_OSPEED_50MHZ, ST7789V_BLACKLIGHT_PIN);
}

/******************************************************************************
      函数说明：LCD初始化函数
      入口数据：无
      返回值：  无
******************************************************************************/
void ST7789VInit(void)
{
    // rcu_periph_clock_enable(RCU_GPIOA);
    // rcu_periph_clock_enable(RCU_GPIOB);

    // /* LCD backlight power control pin, high active  */
    // gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_10);

#if SPI0_CFG == 1
    ST7789VGPIOInit();
#elif SPI0_CFG == 2
    rcu_periph_clock_enable(RCU_DMA0);
    rcu_periph_clock_enable(RCU_SPI0);

    /* SPI0 GPIO config: NSS/PA4, SCK/PA5, MOSI/PA7 */
    gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_7);
    /* SPI0 GPIO config: MISO/PA6 */
    gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_6);

    dma_config();

    dma_channel_enable(DMA0, DMA_CH2);
#elif SPI0_CFG == 3
    gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_5 | GPIO_PIN_7);
    gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_2);

    gpio_bit_reset(GPIOA, GPIO_PIN_5 | GPIO_PIN_7);
    gpio_bit_reset(GPIOB, GPIO_PIN_2);
#endif


    ST7789VResetActive();
    delay(200);
    ST7789VResetDeactive();
    delay(20);
    ST7789VBlackLightActive();

    ST7789VSPISendCommand(ST7789_SLPOUT);   // Sleep out
    delay(120);

    ST7789VSPISendCommand(ST7789_NORON);    // Normal display mode on

    //------------------------------display and color format setting--------------------------------//
    ST7789VSPISendCommand(ST7789_MADCTL);
    ST7789VSPISendData8Bit(0x00);
    // ST7789VSPISendData8Bit(TFT_MAD_RGB);

    // JLX240 display datasheet
    // ST7789VSPISendCommand(0xB6);
    // ST7789VSPISendData8Bit(0x0A);
    // ST7789VSPISendData8Bit(0x82);

    /* The Command 3Ah should be set at 55h when writing 16-bit/pixel data into frame memory, 
    but 3Ah should be re-set to 66h when reading pixel data from frame memory. */
    ST7789VSPISendCommand(ST7789_COLMOD);
    ST7789VSPISendData8Bit(0x55);
    delay(10);

    //--------------------------------ST7789V Frame rate setting----------------------------------//
    ST7789VSPISendCommand(ST7789_PORCTRL);
    ST7789VSPISendData8Bit(0x0c);
    ST7789VSPISendData8Bit(0x0c);
    ST7789VSPISendData8Bit(0x00);
    ST7789VSPISendData8Bit(0x33);
    ST7789VSPISendData8Bit(0x33);

    ST7789VSPISendCommand(ST7789_GCTRL);      // Voltages: VGH / VGL
    ST7789VSPISendData8Bit(0x35);

    //---------------------------------ST7789V Power setting--------------------------------------//
    ST7789VSPISendCommand(ST7789_VCOMS);
    ST7789VSPISendData8Bit(0x28);       // JLX240 display datasheet

    ST7789VSPISendCommand(ST7789_LCMCTRL);
    ST7789VSPISendData8Bit(0x0C);

    ST7789VSPISendCommand(ST7789_VDVVRHEN);
    ST7789VSPISendData8Bit(0x01);
    ST7789VSPISendData8Bit(0xFF);

    ST7789VSPISendCommand(ST7789_VRHS);       // voltage VRHS
    ST7789VSPISendData8Bit(0x10);

    ST7789VSPISendCommand(ST7789_VDVSET);
    ST7789VSPISendData8Bit(0x20);

    ST7789VSPISendCommand(ST7789_FRCTR2);
    ST7789VSPISendData8Bit(0x0f);

    ST7789VSPISendCommand(ST7789_PWCTRL1);
    ST7789VSPISendData8Bit(0xa4);
    ST7789VSPISendData8Bit(0xa1);

    //--------------------------------ST7789V gamma setting---------------------------------------//
    ST7789VSPISendCommand(ST7789_PVGAMCTRL);
    ST7789VSPISendData8Bit(0xd0);
    ST7789VSPISendData8Bit(0x00);
    ST7789VSPISendData8Bit(0x02);
    ST7789VSPISendData8Bit(0x07);
    ST7789VSPISendData8Bit(0x0a);
    ST7789VSPISendData8Bit(0x28);
    ST7789VSPISendData8Bit(0x32);
    ST7789VSPISendData8Bit(0x44);
    ST7789VSPISendData8Bit(0x42);
    ST7789VSPISendData8Bit(0x06);
    ST7789VSPISendData8Bit(0x0e);
    ST7789VSPISendData8Bit(0x12);
    ST7789VSPISendData8Bit(0x14);
    ST7789VSPISendData8Bit(0x17);

    ST7789VSPISendCommand(ST7789_NVGAMCTRL);
    ST7789VSPISendData8Bit(0xd0);
    ST7789VSPISendData8Bit(0x00);
    ST7789VSPISendData8Bit(0x02);
    ST7789VSPISendData8Bit(0x07);
    ST7789VSPISendData8Bit(0x0a);
    ST7789VSPISendData8Bit(0x28);
    ST7789VSPISendData8Bit(0x31);
    ST7789VSPISendData8Bit(0x54);
    ST7789VSPISendData8Bit(0x47);
    ST7789VSPISendData8Bit(0x0e);
    ST7789VSPISendData8Bit(0x1c);
    ST7789VSPISendData8Bit(0x17);
    ST7789VSPISendData8Bit(0x1b);
    ST7789VSPISendData8Bit(0x1e);

    ST7789VSPISendCommand(ST7789_INVON);

    ST7789VSPISendCommand(ST7789_CASET);    // Column address set
    ST7789VSPISendData8Bit(0x00);
    ST7789VSPISendData8Bit(0x00);
    ST7789VSPISendData8Bit(0x00);
    ST7789VSPISendData8Bit(0xE5);    // 239

    ST7789VSPISendCommand(ST7789_RASET);    // Row address set
    ST7789VSPISendData8Bit(0x00);
    ST7789VSPISendData8Bit(0x00);
    ST7789VSPISendData8Bit(0x01);
    ST7789VSPISendData8Bit(0x3F);    // 319


    delay(120);

    ST7789VSetRotation(0);

    ST7789VSPISendCommand(ST7789_DISPON);    //Display on
    delay(120);
}

void ST7789VSetRotation(uint8_t m)
{
    rotation = m % 4;
    ST7789VSPISendCommand(0x36);
    switch (rotation) {
    case 0:
        colstart = 52;
        rowstart = 40;
        _width  = _init_width;
        _height = _init_height;
        ST7789VSPISendData8Bit(TFT_MAD_COLOR_ORDER);
        break;

    case 1:
        colstart = 40;
        rowstart = 53;
        _width  = _init_height;
        _height = _init_width;
        ST7789VSPISendData8Bit(TFT_MAD_MX | TFT_MAD_MV | TFT_MAD_COLOR_ORDER);
        break;
    case 2:
        colstart = 52;
        rowstart = 40;
        _width  = _init_width;
        _height = _init_height;
        ST7789VSPISendData8Bit(TFT_MAD_MX | TFT_MAD_MY | TFT_MAD_COLOR_ORDER);
        break;
    case 3:
        colstart = 40;
        rowstart = 52;
        _width  = _init_height;
        _height = _init_width;
        ST7789VSPISendData8Bit(TFT_MAD_MV | TFT_MAD_MY | TFT_MAD_COLOR_ORDER);
        break;
    }
}

void ST7789VBacklightEnable(bool enable)
{
    enable ?  ST7789VBlackLightActive() : ST7789VBlackLightDeactive();
}

/******************************************************************************
      函数说明：LCD清屏函数
      入口数据：无
      返回值：  无
******************************************************************************/
void ST7789VClear(u16 Color)
{
    u16 i, j;
    ST7789VAddressSet(0, 0, _width - 1, _height - 1);
    for (i = 0; i < _width; i++) {
        for (j = 0; j < _height; j++) {
            ST7789VSPISendData16Bit(Color);
        }
    }
}

void ST7789VSetBackgroundColor(u16 color)
{
    BACK_COLOR = color;
}

/******************************************************************************
      函数说明：LCD显示汉字
      入口数据：x,y   起始坐标
                index 汉字的序号
                size  字号
      返回值：  无
******************************************************************************/
void ST7789VShowChinese(u16 x, u16 y, u8 index, u8 size, u16 color)
{
    u8 i, j;
    u8 *temp, size1;
    if (size == 16) {
        temp = Hzk16;   //选择字号
    }
    if (size == 32) {
        temp = Hzk32;
    }
    ST7789VAddressSet(x, y, x + size - 1, y + size - 1); //设置一个汉字的区域
    size1 = size * size / 8; //一个汉字所占的字节
    temp += index * size1; //写入的起始位置
    for (j = 0; j < size1; j++) {
        for (i = 0; i < 8; i++) {
            if ((*temp & (1 << i)) != 0) { //从数据的低位开始读
                ST7789VSPISendData16Bit(color);//点亮
            } else {
                ST7789VSPISendData16Bit(BACK_COLOR);//不点亮
            }
        }
        temp++;
    }
}


/******************************************************************************
      函数说明：LCD显示汉字
      入口数据：x,y   起始坐标
      返回值：  无
******************************************************************************/
void ST7789VDrawPoint(u16 x, u16 y, u16 color)
{
    ST7789VAddressSet(x, y, x, y); //设置光标位置
    ST7789VSPISendData16Bit(color);
}


/******************************************************************************
      函数说明：LCD画一个大的点
      入口数据：x,y   起始坐标
      返回值：  无
******************************************************************************/
void ST7789VDrawPoint_big(u16 x, u16 y, u16 color)
{
    ST7789VFill(x - 1, y - 1, x + 1, y + 1, color);
}


/******************************************************************************
      函数说明：在指定区域填充颜色
      入口数据：xsta,ysta   起始坐标
                xend,yend   终止坐标
      返回值：  无
******************************************************************************/
void ST7789VFill(u16 xsta, u16 ysta, u16 xend, u16 yend, u16 color)
{
    u16 i, j;
    ST7789VAddressSet(xsta, ysta, xend, yend);   //设置光标位置
    for (i = ysta; i <= yend; i++) {
        for (j = xsta; j <= xend; j++)ST7789VSPISendData16Bit(color); //设置光标位置
    }
}


/******************************************************************************
      函数说明：画线
      入口数据：x1,y1   起始坐标
                x2,y2   终止坐标
      返回值：  无
******************************************************************************/
void ST7789VDrawLine(u16 x1, u16 y1, u16 x2, u16 y2, u16 color)
{
    u16 t;
    int xerr = 0, yerr = 0, delta_x, delta_y, distance;
    int incx, incy, uRow, uCol;
    delta_x = x2 - x1; //计算坐标增量
    delta_y = y2 - y1;
    uRow = x1; //画线起点坐标
    uCol = y1;
    if (delta_x > 0)incx = 1; //设置单步方向
    else if (delta_x == 0)incx = 0; //垂直线
    else {
        incx = -1;
        delta_x = -delta_x;
    }
    if (delta_y > 0)incy = 1;
    else if (delta_y == 0)incy = 0; //水平线
    else {
        incy = -1;
        delta_y = -delta_x;
    }
    if (delta_x > delta_y)distance = delta_x; //选取基本增量坐标轴
    else distance = delta_y;
    for (t = 0; t < distance + 1; t++) {
        ST7789VDrawPoint(uRow, uCol, color); //画点
        xerr += delta_x;
        yerr += delta_y;
        if (xerr > distance) {
            xerr -= distance;
            uRow += incx;
        }
        if (yerr > distance) {
            yerr -= distance;
            uCol += incy;
        }
    }
}


/******************************************************************************
      函数说明：画矩形
      入口数据：x1,y1   起始坐标
                x2,y2   终止坐标
      返回值：  无
******************************************************************************/
void ST7789VDrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2, u16 color)
{
    ST7789VDrawLine(x1, y1, x2, y1, color);
    ST7789VDrawLine(x1, y1, x1, y2, color);
    ST7789VDrawLine(x1, y2, x2, y2, color);
    ST7789VDrawLine(x2, y1, x2, y2, color);
}


/******************************************************************************
      函数说明：画圆
      入口数据：x0,y0   圆心坐标
                r       半径
      返回值：  无
******************************************************************************/
void ST7789VDrawCircle(u16 x0, u16 y0, u8 r, u16 color)
{
    int a, b;
    // int di;
    a = 0; b = r;
    while (a <= b) {
        ST7789VDrawPoint(x0 - b, y0 - a, color);       //3
        ST7789VDrawPoint(x0 + b, y0 - a, color);       //0
        ST7789VDrawPoint(x0 - a, y0 + b, color);       //1
        ST7789VDrawPoint(x0 - a, y0 - b, color);       //2
        ST7789VDrawPoint(x0 + b, y0 + a, color);       //4
        ST7789VDrawPoint(x0 + a, y0 - b, color);       //5
        ST7789VDrawPoint(x0 + a, y0 + b, color);       //6
        ST7789VDrawPoint(x0 - b, y0 + a, color);       //7
        a++;
        if ((a * a + b * b) > (r * r)) { //判断要画的点是否过远
            b--;
        }
    }
}


/******************************************************************************
      函数说明：显示字符
      入口数据：x,y    起点坐标
                num    要显示的字符
                mode   1叠加方式  0非叠加方式
      返回值：  无
******************************************************************************/
void ST7789VShowChar(u16 x, u16 y, u8 num, u8 mode, u16 color)
{
    u8 temp;
    u8 pos, t;
    u16 x0 = x;
    if (x > _width - 16 || y > _height - 16)return; //设置窗口
    num = num - ' '; //得到偏移后的值
    ST7789VAddressSet(x, y, x + 8 - 1, y + 16 - 1); //设置光标位置
    if (!mode) { //非叠加方式
        for (pos = 0; pos < 16; pos++) {
            temp = asc2_1608[(u16)num * 16 + pos];   //调用1608字体
            for (t = 0; t < 8; t++) {
                if (temp & 0x01)ST7789VSPISendData16Bit(color);
                else ST7789VSPISendData16Bit(BACK_COLOR);
                temp >>= 1;
                x++;
            }
            x = x0;
            y++;
        }
    } else { //叠加方式
        for (pos = 0; pos < 16; pos++) {
            temp = asc2_1608[(u16)num * 16 + pos];   //调用1608字体
            for (t = 0; t < 8; t++) {
                if (temp & 0x01)ST7789VDrawPoint(x + t, y + pos, color); //画一个点
                temp >>= 1;
            }
        }
    }
}


/******************************************************************************
      函数说明：显示字符串
      入口数据：x,y    起点坐标
                *p     字符串起始地址
      返回值：  无
******************************************************************************/
void ST7789VShowString(u16 x, u16 y, const u8 *p, u16 color)
{
    while (*p != '\0') {
        if (x > _width - 16) {
            x = 0;
            y += 16;
        }
        if (y > _height - 16) {
            y = x = 0;
            ST7789VClear(RED);
        }
        ST7789VShowChar(x, y, *p, 0, color);
        x += 8;
        p++;
    }
}


/******************************************************************************
      函数说明：显示数字
      入口数据：m底数，n指数
      返回值：  无
******************************************************************************/
u32 ST7789Pow(u8 m, u8 n)
{
    u32 result = 1;
    while (n--)result *= m;
    return result;
}


/******************************************************************************
      函数说明：显示数字
      入口数据：x,y    起点坐标
                num    要显示的数字
                len    要显示的数字个数
      返回值：  无
******************************************************************************/
void ST7789VShowNum(u16 x, u16 y, u16 num, u8 len, u16 color)
{
    u8 t, temp;
    u8 enshow = 0;
    for (t = 0; t < len; t++) {
        temp = (num / ST7789Pow(10, len - t - 1)) % 10;
        if (enshow == 0 && t < (len - 1)) {
            if (temp == 0) {
                ST7789VShowChar(x + 8 * t, y, ' ', 0, color);
                continue;
            } else enshow = 1;

        }
        ST7789VShowChar(x + 8 * t, y, temp + 48, 0, color);
    }
}


/******************************************************************************
      函数说明：显示小数
      入口数据：x,y    起点坐标
                num    要显示的小数
                len    要显示的数字个数
      返回值：  无
******************************************************************************/
void ST7789VShowNum1(u16 x, u16 y, float num, u8 len, u16 color)
{
    u8 t, temp;
    // u8 enshow=0;
    u16 num1;
    num1 = num * 100;
    for (t = 0; t < len; t++) {
        temp = (num1 / ST7789Pow(10, len - t - 1)) % 10;
        if (t == (len - 2)) {
            ST7789VShowChar(x + 8 * (len - 2), y, '.', 0, color);
            t++;
            len += 1;
        }
        ST7789VShowChar(x + 8 * t, y, temp + 48, 0, color);
    }
}

extern unsigned char image[12800];
/******************************************************************************
      函数说明：显示40x40图片
      入口数据：x,y    起点坐标
      返回值：  无
******************************************************************************/
void ST7789VShowPicture(u16 x1, u16 y1, u16 x2, u16 y2)
{
    int i;
    ST7789VAddressSet(x1, y1, x2, y2);
    for (i = 0; i < 12800; i++) {
        ST7789VSPISendData8Bit(image[i]);
    }
}
