/*!
    \file    i2c.c
    \brief   I2C configuration file

    \version 2020-08-04, V1.0.0, firmware for GD32VF103
    \version 2020-12-08, V1.0.1, firmware for GD32VF103
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
#include "i2c.h"
#include <stdio.h>
#include "systick.h"

/*!
    \brief      configure the GPIO ports
    \param[in]  none
    \param[out] none
    \retval     none
*/
void gpio_config(void)
{
    /* enable DMA0 clock */
    rcu_periph_clock_enable(RCU_DMA0);
    /* enable GPIOB clock */
    rcu_periph_clock_enable(RCU_GPIO_I2C);

    /* connect I2C_SCL_PIN to I2C_SCL */
    /* connect I2C_SDA_PIN to I2C_SDA */
    gpio_init(I2C_SCL_PORT, GPIO_MODE_AF_OD, GPIO_OSPEED_50MHZ, I2C_SCL_PIN);
    gpio_init(I2C_SDA_PORT, GPIO_MODE_AF_OD, GPIO_OSPEED_50MHZ, I2C_SDA_PIN);
}

/*!
    \brief      configure the I2C0 interfaces
    \param[in]  none
    \param[out] none
    \retval     none
*/
void i2c_config(void)
{
    /* enable I2C clock */
    rcu_periph_clock_enable(RCU_I2C);
    /* configure I2C clock */
    i2c_clock_config(I2CX, I2C_SPEED, I2C_DTCY_2);
    /* configure I2C address */
    i2c_mode_addr_config(I2CX, I2C_I2CMODE_ENABLE, I2C_ADDFORMAT_7BITS, I2CX_SLAVE_ADDRESS7);
    /* enable I2CX */
    i2c_enable(I2CX);
    /* enable acknowledge */
    i2c_ack_config(I2CX, I2C_ACK_ENABLE);
}

/*!
    \brief      reset i2c bus
    \param[in]  none
    \param[out] none
    \retval     none
*/
void i2c_bus_reset()
{
    i2c_deinit(I2CX);
    /* configure SDA/SCL for GPIO */
    GPIO_BC(I2C_SCL_PORT) |= I2C_SCL_PIN;
    GPIO_BC(I2C_SDA_PORT) |= I2C_SDA_PIN;
    gpio_init(I2C_SCL_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, I2C_SCL_PIN);
    gpio_init(I2C_SDA_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, I2C_SDA_PIN);
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    GPIO_BOP(I2C_SCL_PORT) |= I2C_SCL_PIN;
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    GPIO_BOP(I2C_SDA_PORT) |= I2C_SDA_PIN;
    /* connect I2C_SCL_PIN to I2C_SCL */
    /* connect I2C_SDA_PIN to I2C_SDA */
    gpio_init(I2C_SCL_PORT, GPIO_MODE_AF_OD, GPIO_OSPEED_50MHZ, I2C_SCL_PIN);
    gpio_init(I2C_SDA_PORT, GPIO_MODE_AF_OD, GPIO_OSPEED_50MHZ, I2C_SDA_PIN);
    /* configure the I2CX interface */
    i2c_config();
}
