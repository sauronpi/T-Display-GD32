/*!
    \file    main.c
    \brief   SPI simplex communication use DMA

    \version 2020-08-04, V1.0.0, firmware for GD32VF103
*/

/*
    Copyright (c) 2020, GigaDevice Semiconductor Inc.

    Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors
       may be used to endorse or promote products derived from this software without
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
OF SUCH DAMAGE.
*/

#include "gd32vf103.h"
#include "gd32vf103v_eval.h"

#define MASTER_TRAMSMIT_SLAVE_RECEIVE
//#define SLAVE_TRAMSMIT_MASTER_RECEIVE

#define  ARRAYSIZE         10

#define SET_SPI2_NSS_HIGH          gpio_bit_set(GPIOA,GPIO_PIN_2);
#define SET_SPI2_NSS_LOW           gpio_bit_reset(GPIOA,GPIO_PIN_2);

uint8_t spi0_send_array[ARRAYSIZE] = {0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA};
uint8_t spi2_send_array[ARRAYSIZE] = {0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xB8, 0xB9, 0xBA};
uint8_t spi0_receive_array[ARRAYSIZE];
uint8_t spi2_receive_array[ARRAYSIZE];
uint32_t send_n = 0, receive_n = 0;

void rcu_config(void);
void gpio_config(void);
void dma_config(void);
void spi_config(void);
ErrStatus memory_compare(uint8_t *src, uint8_t *dst, uint8_t length);

/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
    /* initialize the LEDs */
    gd_eval_led_init(LED2);
    gd_eval_led_init(LED3);

    /* enable peripheral clock */
    rcu_config();
    /* configure GPIO */
    gpio_config();

    SET_SPI2_NSS_HIGH

    /* configure DMA */
    dma_config();
    /* configure SPI */
    spi_config();

#ifdef MASTER_TRAMSMIT_SLAVE_RECEIVE
    /* enable DMA channel */
    /* SPI0_Tx DMA channel */
    dma_channel_enable(DMA0, DMA_CH2);
    /* SPI2_Rx DMA channel */
    dma_channel_enable(DMA1, DMA_CH0);

    /* enable SPI DMA */
    spi_dma_enable(SPI2, SPI_DMA_RECEIVE);
    spi_dma_enable(SPI0, SPI_DMA_TRANSMIT);
#endif /* master send and slave receive */

#ifdef SLAVE_TRAMSMIT_MASTER_RECEIVE
    /* enable DMA channel */
    /* SPI0_Rx DMA channel */
    dma_channel_enable(DMA0, DMA_CH1);
    /* SPI2_Tx DMA channel */
    dma_channel_enable(DMA1, DMA_CH1);
    /* enable SPI DMA */
    spi_dma_enable(SPI2, SPI_DMA_TRANSMIT);
    spi_dma_enable(SPI0, SPI_DMA_RECEIVE);
#endif /* slave send and master receive */

    SET_SPI2_NSS_LOW

    /* enable SPI */
    spi_enable(SPI2);
    spi_enable(SPI0);

#ifdef MASTER_TRAMSMIT_SLAVE_RECEIVE
    /* wait DMA transmit completed */
    while(!dma_flag_get(DMA0, DMA_CH2, DMA_FLAG_FTF));
    while(!dma_flag_get(DMA1, DMA_CH0, DMA_FLAG_FTF));

    /* compare receive data with send data */
    if(ERROR != memory_compare(spi2_receive_array, spi0_send_array, ARRAYSIZE)) {
        gd_eval_led_on(LED2);
    } else {
        gd_eval_led_off(LED2);
    }
#endif /* master send and slave receive */

#ifdef SLAVE_TRAMSMIT_MASTER_RECEIVE
    /* wait DMA transmit completed */
    while(!dma_flag_get(DMA0, DMA_CH1, DMA_FLAG_FTF));
    while(!dma_flag_get(DMA1, DMA_CH1, DMA_FLAG_FTF));

    /* compare receive data with send data */
    if(ERROR != memory_compare(spi0_receive_array, spi2_send_array, ARRAYSIZE)) {
        gd_eval_led_on(LED3);
    } else {
        gd_eval_led_off(LED3);
    }
#endif /* slave send and master receive */

    spi_disable(SPI2);
    spi_disable(SPI0);

    SET_SPI2_NSS_HIGH

    while(1) {
    }
}

/*!
    \brief      configure different peripheral clocks
    \param[in]  none
    \param[out] none
    \retval     none
*/
void rcu_config(void)
{
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOC);
    rcu_periph_clock_enable(RCU_AF);
    rcu_periph_clock_enable(RCU_DMA0);
    rcu_periph_clock_enable(RCU_DMA1);
    rcu_periph_clock_enable(RCU_SPI0);
    rcu_periph_clock_enable(RCU_SPI2);
}

/*!
    \brief      configure the GPIO peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
void gpio_config(void)
{
    /* configure SPI0 GPIO : SCK/PA5, MISO/PA6, MOSI/PA7 */
    gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_5 | GPIO_PIN_7);
    gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_6);
    /* set GPIO PA2 as SPI0 NSS output */
    gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_2);

    /* configure SPI2 GPIO : SCK/PC10, MISO/PC11, MOSI/PC12, NSS/PA4 */
    gpio_pin_remap_config(GPIO_SPI2_REMAP, ENABLE);
    gpio_init(GPIOC, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_10 | GPIO_PIN_12);
    gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_4);
    gpio_init(GPIOC, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_11);
}

/*!
    \brief      configure the DMA peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
void dma_config(void)
{
    dma_parameter_struct dma_init_struct;
    dma_struct_para_init(&dma_init_struct);

    /* configure SPI0 transmit DMA : DMA0, DMA_CH2 */
    dma_deinit(DMA0, DMA_CH2);

    dma_init_struct.periph_addr  = (uint32_t)&SPI_DATA(SPI0);
    dma_init_struct.memory_addr  = (uint32_t)spi0_send_array;
    dma_init_struct.direction    = DMA_MEMORY_TO_PERIPHERAL;
    dma_init_struct.memory_width = DMA_MEMORY_WIDTH_8BIT;
    dma_init_struct.periph_width = DMA_PERIPHERAL_WIDTH_8BIT;
    dma_init_struct.priority     = DMA_PRIORITY_LOW;
    dma_init_struct.number       = ARRAYSIZE;
    dma_init_struct.periph_inc   = DMA_PERIPH_INCREASE_DISABLE;
    dma_init_struct.memory_inc   = DMA_MEMORY_INCREASE_ENABLE;
    dma_init(DMA0, DMA_CH2, &dma_init_struct);
    /* configure DMA mode */
    dma_circulation_disable(DMA0, DMA_CH2);
    dma_memory_to_memory_disable(DMA0, DMA_CH2);

    /* configure SPI0 receive DMA : DMA0, DMA_CH1 */
    dma_deinit(DMA0, DMA_CH1);
    dma_init_struct.periph_addr  = (uint32_t)&SPI_DATA(SPI0);
    dma_init_struct.memory_addr  = (uint32_t)spi0_receive_array;
    dma_init_struct.direction    = DMA_PERIPHERAL_TO_MEMORY;
    dma_init_struct.priority     = DMA_PRIORITY_HIGH;
    dma_init(DMA0, DMA_CH1, &dma_init_struct);
    /* configure DMA mode */
    dma_circulation_disable(DMA0, DMA_CH1);
    dma_memory_to_memory_disable(DMA0, DMA_CH1);

    /* configure SPI2 transmit DMA : DMA1, DMA_CH1 */
    dma_deinit(DMA1, DMA_CH1);
    dma_init_struct.periph_addr  = (uint32_t)&SPI_DATA(SPI2);
    dma_init_struct.memory_addr  = (uint32_t)spi2_send_array;
    dma_init_struct.direction    = DMA_MEMORY_TO_PERIPHERAL;
    dma_init_struct.priority     = DMA_PRIORITY_MEDIUM;
    dma_init(DMA1, DMA_CH1, &dma_init_struct);
    /* configure DMA mode */
    dma_circulation_disable(DMA1, DMA_CH1);
    dma_memory_to_memory_disable(DMA1, DMA_CH1);

    /* configure SPI2 receive DMA : DMA1, DMA_CH0 */
    dma_deinit(DMA1, DMA_CH0);
    dma_init_struct.periph_addr  = (uint32_t)&SPI_DATA(SPI2);
    dma_init_struct.memory_addr  = (uint32_t)spi2_receive_array;
    dma_init_struct.direction    = DMA_PERIPHERAL_TO_MEMORY;
    dma_init_struct.priority     = DMA_PRIORITY_ULTRA_HIGH;
    dma_init(DMA1, DMA_CH0, &dma_init_struct);
    /* configure DMA mode */
    dma_circulation_disable(DMA1, DMA_CH0);
    dma_memory_to_memory_disable(DMA1, DMA_CH0);
}

/*!
    \brief      configure the SPI peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
void spi_config(void)
{
    spi_parameter_struct spi_init_struct;
    /* deinitilize SPI and the parameters */
    spi_i2s_deinit(SPI0);
    spi_i2s_deinit(SPI2);
    spi_struct_para_init(&spi_init_struct);

    /* configure SPI0 parameter */
#ifdef MASTER_TRAMSMIT_SLAVE_RECEIVE
    spi_init_struct.trans_mode           = SPI_TRANSMODE_FULLDUPLEX;
#endif /* master send and slave receive */
#ifdef SLAVE_TRAMSMIT_MASTER_RECEIVE
    spi_init_struct.trans_mode           = SPI_TRANSMODE_RECEIVEONLY;
#endif /* slave send and master receive */
    spi_init_struct.device_mode          = SPI_MASTER;
    spi_init_struct.frame_size           = SPI_FRAMESIZE_8BIT;
    spi_init_struct.clock_polarity_phase = SPI_CK_PL_HIGH_PH_2EDGE;
    spi_init_struct.nss                  = SPI_NSS_SOFT;
    spi_init_struct.prescale             = SPI_PSC_8;
    spi_init_struct.endian               = SPI_ENDIAN_MSB;
    spi_init(SPI0, &spi_init_struct);

    /* configure SPI1 parameter */
#ifdef MASTER_TRAMSMIT_SLAVE_RECEIVE
    spi_init_struct.trans_mode           = SPI_TRANSMODE_RECEIVEONLY;
#endif /* master send and slave receive */
#ifdef SLAVE_TRAMSMIT_MASTER_RECEIVE
    spi_init_struct.trans_mode           = SPI_TRANSMODE_FULLDUPLEX;
#endif /* slave send and master receive */
    spi_init_struct.device_mode = SPI_SLAVE;
    spi_init_struct.nss         = SPI_NSS_HARD;
    spi_init(SPI2, &spi_init_struct);
}

/*!
    \brief      memory compare function
    \param[in]  src: source data pointer
    \param[in]  dst: destination data pointer
    \param[in]  length: the compare data length
    \param[out] none
    \retval     ErrStatus : ERROR or SUCCESS
*/
ErrStatus memory_compare(uint8_t *src, uint8_t *dst, uint8_t length)
{
    while(length--) {
        if(*src++ != *dst++) {
            return ERROR;
        }
    }
    return SUCCESS;
}
