/*!
    \file    i2c.h
    \brief   the header file of I2C

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

#ifndef I2C_H
#define I2C_H

#include "gd32vf103.h"

#define I2CX_SLAVE_ADDRESS7     0xA0
#define I2C_SPEED               100000
#define I2C_PAGE_SIZE           8
#define I2CX                    I2C0
#define RCU_GPIO_I2C            RCU_GPIOB
#define RCU_I2C                 RCU_I2C0
#define I2C_SCL_PORT            GPIOB
#define I2C_SDA_PORT            GPIOB
#define I2C_SCL_PIN             GPIO_PIN_6
#define I2C_SDA_PIN             GPIO_PIN_7

typedef enum {
    I2C_SEND_ADDRESS_FIRST = 0,
    I2C_CLEAR_ADDRESS_FLAG_FIRST,
    I2C_TRANSMIT_WRITE_READ_ADD,
    I2C_SEND_ADDRESS_SECOND,
    I2C_CLEAR_ADDRESS_FLAG_SECOND,
    I2C_TRANSMIT_DATA,
    I2C_STOP,
} i2c_process_enum;

extern   volatile uint8_t      *i2c_write;
extern   volatile uint8_t      *i2c_read;
extern  volatile uint16_t       i2c_nbytes;
extern  volatile uint16_t       i2c_write_dress;
extern  volatile uint16_t       i2c_read_dress;
extern  volatile uint8_t        i2c_process_flag;

/* configure the GPIO ports */
void gpio_config(void);
/* configure the I2CX interfaces */
void i2c_config(void);
/* handle I2CX event interrupt request */
void i2cx_event_irq_handler(void);
/* handle I2CX error interrupt request */
void i2cx_error_irq_handler(void);

#endif  /* I2C_H */
