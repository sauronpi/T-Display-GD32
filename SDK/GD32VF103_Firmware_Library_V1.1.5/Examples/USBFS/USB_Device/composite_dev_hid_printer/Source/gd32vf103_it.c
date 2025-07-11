/*!
    \file    gd32vf103_it.c
    \brief   main interrupt service routines

    \version 2020-08-04, V1.1.0, firmware for GD32VF103
    \version 2022-06-30, V2.1.0, firmware for GD32VF103
*/

/*
    Copyright (c) 2022, GigaDevice Semiconductor Inc.

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

#include "drv_usbd_int.h"
#include "drv_usb_hw.h"
#include "gd32vf103_it.h"
#include "custom_hid_core.h"

extern usb_core_driver usbd_hid_printer;
extern uint32_t usbfs_prescaler;

uint8_t send_buffer1[4] = {0x00, 0x01, 0x00, 0x00};
uint8_t send_buffer2[4] = {0x00, 0x01, 0x00, 0x00};

void usb_timer_irq(void);

/* local function prototypes ('static') */
static void resume_mcu_clk(void);

/*!
    \brief      this function handles EXTI0_IRQ Handler.
    \param[in]  none
    \param[out] none
    \retval     none
*/
void EXTI0_IRQHandler (void)
{
    if (RESET != exti_interrupt_flag_get(KEY_A_EXTI_LINE)) {
        if (USBD_CONFIGURED == usbd_hid_printer.dev.cur_status) {
            send_buffer1[0] = 0x15U;

            if (RESET == gd_eval_key_state_get(KEY_A)) {
                if(send_buffer1[1]) {
                    send_buffer1[1] = 0x00U;
                } else {
                    send_buffer1[1] = 0x01U;
                }
            }

            custom_hid_report_send (&usbd_hid_printer, send_buffer1, 2U);
        }

        /* clear the EXTI line interrupt flag */
        exti_interrupt_flag_clear(KEY_A_EXTI_LINE);
    }
}

/*!
    \brief      this function handles EXTI10_15_IRQ Handler.
    \param[in]  none
    \param[out] none
    \retval     none
*/
void EXTI10_15_IRQHandler (void)
{
    if (RESET != exti_interrupt_flag_get(KEY_B_EXTI_LINE)) {
        if (USBD_CONFIGURED == usbd_hid_printer.dev.cur_status) {
            send_buffer2[0] = 0x16U;

            if (RESET == gd_eval_key_state_get(KEY_B)) {
                if(send_buffer2[1]) {
                    send_buffer2[1] = 0x00U;
                } else {
                    send_buffer2[1] = 0x01U;
                }
            }

            custom_hid_report_send (&usbd_hid_printer, send_buffer2, 2U);
        }

        /* clear the EXTI line interrupt flag */
        exti_interrupt_flag_clear(KEY_B_EXTI_LINE);
    }
}

/*!
    \brief      this function handles USBFS interrupt
    \param[in]  none
    \param[out] none
    \retval     none
*/
void USBFS_IRQHandler (void)
{
    usbd_isr (&usbd_hid_printer);
}

/*!
    \brief      this function handles USBFS wakeup interrupt request.
    \param[in]  none
    \param[out] none
    \retval     none
*/
void USBFS_WKUP_IRQHandler(void)
{
    if (usbd_hid_printer.bp.low_power) {
        resume_mcu_clk();

        rcu_usb_clock_config(usbfs_prescaler);

        rcu_periph_clock_enable(RCU_USBFS);

        usb_clock_active(&usbd_hid_printer);
    }

    exti_interrupt_flag_clear(EXTI_18);
}

/*!
    \brief      this function handles Timer2 update interrupt request.
    \param[in]  none
    \param[out] none
    \retval     none
*/
void TIMER2_IRQHandler(void)
{
    usb_timer_irq();
}

/*!
    \brief      resume mcu clock
    \param[in]  none
    \param[out] none
    \retval     none
*/
static void resume_mcu_clk(void)
{
    /* enable HSE */
    rcu_osci_on(RCU_HXTAL);

    /* wait till HSE is ready */
    while(RESET == rcu_flag_get(RCU_FLAG_HXTALSTB)){
    }

    if(HXTAL_VALUE == 25000000){
        /* enable PLL1 */
        rcu_osci_on(RCU_PLL1_CK);

        /* wait till PLL1 is ready */
        while(RESET == rcu_flag_get(RCU_FLAG_PLL1STB)){
        }
    }

    /* enable PLL */
    rcu_osci_on(RCU_PLL_CK);

    /* wait till PLL is ready */
    while(RESET == rcu_flag_get(RCU_FLAG_PLLSTB)){
    }

    /* select PLL as system clock source */
    rcu_system_clock_source_config(RCU_CKSYSSRC_PLL);

    /* wait till PLL is used as system clock source */
    while(RCU_SCSS_PLL != rcu_system_clock_source_get()){
    }
}
