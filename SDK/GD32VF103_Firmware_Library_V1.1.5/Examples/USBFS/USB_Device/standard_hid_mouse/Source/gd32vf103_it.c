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
#include "standard_hid_core.h"
#include "gd32vf103v_eval.h"

#define CURSOR_STEP           5

extern usb_core_driver hid_mice;
extern uint32_t usbfs_prescaler;

void usb_timer_irq(void);

/* local function prototypes ('static') */
static uint8_t  joystate_get(void);
static uint8_t* usbd_mice_pos_get(void);
static void resume_mcu_clk(void);

/*!
    \brief      this function handles Core timer handler
    \param[in]  none
    \param[out] none
    \retval     none
*/
void eclic_mtip_handler(void)
{
    uint8_t* buf;

    *(uint64_t*)(TIMER_CTRL_ADDR + TIMER_MTIME) = 0;

    buf = usbd_mice_pos_get();

    if ((buf[1] != 0) || (buf[2] != 0)) {
        hid_report_send(&hid_mice, buf, 4);
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
    usbd_isr (&hid_mice);
}

/*!
    \brief      this function handles EXTI4_IRQ Handler
    \param[in]  none
    \param[out] none
    \retval     none
*/
void EXTI4_IRQHandler(void)
{
    if (exti_interrupt_flag_get(KEY_CET_EXTI_LINE) != RESET) {
        if (hid_mice.dev.pm.dev_remote_wakeup) {
            resume_mcu_clk();

            rcu_usb_clock_config(usbfs_prescaler);

            rcu_periph_clock_enable(RCU_USBFS);

            usb_clock_active(&hid_mice);

            usb_rwkup_set(&hid_mice);

            /* add delay time */
            for(__IO uint16_t i = 0; i < 1000; i++){
                for(__IO uint16_t i = 0; i < 200; i++);
            }

            usb_rwkup_reset(&hid_mice);

            hid_mice.dev.cur_status = hid_mice.dev.backup_status;

            hid_mice.dev.pm.dev_remote_wakeup = 0U;
        }

        /* clear the exti line pending bit */
        exti_interrupt_flag_clear(KEY_CET_EXTI_LINE);
    }
}

/*!
    \brief      this function handles USBFS wakeup interrupt request.
    \param[in]  none
    \param[out] none
    \retval     none
*/
void USBFS_WKUP_IRQHandler(void)
{
    if (hid_mice.bp.low_power) {
        resume_mcu_clk();

        rcu_usb_clock_config(usbfs_prescaler);

        rcu_periph_clock_enable(RCU_USBFS);

        usb_clock_active(&hid_mice);
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
    \brief      get joystick state
    \param[in]  none
    \param[out] none
    \retval     state
*/
static uint8_t joystate_get(void)
{
    if (0 == gd_eval_key_state_get(KEY_A)) {
        return JOY_UP;
    } else if (0 == gd_eval_key_state_get(KEY_B)) {
        return JOY_LEFT;
    } else if (0 == gd_eval_key_state_get(KEY_C)) {
        return JOY_DOWN;
    } else if (0 == gd_eval_key_state_get(KEY_D)) {
        return JOY_RIGHT;
    } else {
        return JOY_NONE;
    }
}

/*!
    \brief      get mice position
    \param[in]  none
    \param[out] none
    \retval     the value of position
*/
static uint8_t* usbd_mice_pos_get(void)
{
    int8_t x = 0, y = 0;
    static uint8_t mice_buf[4] = {0,0,0,0};

    switch (joystate_get()) {
        case JOY_UP:
            y = -CURSOR_STEP;
            break;

        case JOY_DOWN:
            y = CURSOR_STEP;
            break;

        case JOY_LEFT:
            x = -CURSOR_STEP;
            break;

        case JOY_RIGHT:
            x = CURSOR_STEP;
            break;

        default:
            break;
    }

    mice_buf[0] = 0;
    mice_buf[1] = x;
    mice_buf[2] = y;
    mice_buf[3] = 0;

    return mice_buf;
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
