/*!
    \file    usbh_usr.h
    \brief   user application layer header file

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

#ifndef __USBH_USR_H
#define __USBH_USR_H

#include "usbh_core.h"
#include "usbh_usr_lcd.h"
#include "usb_conf.h"
#include <stdio.h>

extern usbh_user_cb usr_cb;

/* function declarations */
/* user operation for host-mode initialization */
void usbh_user_init                          (void);
/* de-init user state and associated variables */
void usbh_user_deinit                        (void);
/* user operation for device attached */
void usbh_user_device_connected              (void);
/* user operation for reset USB Device */
void usbh_user_device_reset                  (void);
/* user operation for device disconnect event */
void usbh_user_device_disconnected           (void);
/* user action for device overcurrent detection event */
void usbh_user_over_current_detected         (void);
/* user operation for detecting device speed */
void usbh_user_device_speed_detected         (uint32_t DeviceSpeed);
/* user operation when device descriptor is available */
void usbh_user_device_desc_available         (void *);
/* USB device is successfully assigned the address */
void usbh_user_device_address_assigned       (void);
/* user operation when configuration descriptor is available */
void usbh_user_configuration_descavailable   (usb_desc_config *cfgDesc,
                                              usb_desc_itf *itfDesc,
                                              usb_desc_ep *epDesc);
/* user operation when manufacturer string exists */
void usbh_user_manufacturer_string           (void *);
/* user operation when manufacturer string exists */
void usbh_user_product_string                (void *);
/* user operation when serialnum string exists */
void usbh_user_serialnum_string              (void *);
/* user response request is displayed to ask for application jump to class */
void usbh_user_enumeration_finish            (void);
/* user action for application state entry */
usbh_user_status usbh_user_userinput         (void);
/* user operation when device is not supported */
void usbh_user_device_not_supported          (void);
/* user operation for unrecovered error happens */
void usbh_user_unrecovered_error             (void);

#endif /*__USBH_USR_H */
