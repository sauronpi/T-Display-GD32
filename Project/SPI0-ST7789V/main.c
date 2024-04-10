/*!
    \file    main.c
    \brief   LCD test demo

    \version 2019-06-05, V1.0.0, firmware for GD32VF103
    \version 2019-09-18, V1.0.1, firmware for GD32VF103
    \version 2020-08-04, V1.1.0, firmware for GD32VF103
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

#include "Foundation.h"
#include "USART.h"
#include "ST7789V.h"

extern void _init();

/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
    int index = 0;
    /* systesm init */
    _init();

    USARTInit();

    ST7789VInit();
    ST7789VBacklightEnable(true);
    printf("ST7789VInit\r\n");
    ST7789VClear(BLACK);
    printf("ST7789VClear\r\n");
    ST7789VSetBackgroundColor(YELLOW);
    ST7789VSetRotation(1);
    ST7789VShowString(0, 0, "Sauron Pi", RED);
    ST7789VShowString(0, 16, "Sauron Pi", GREEN);
    // ST7789VShowString(0, 32, "Sauron Pi", BLUE);
    // ST7789VShowString(0, 48, (char *)("Sauron Pi"), GBLUE);
    // ST7789VShowString(0, 64, (char *)("Sauron Pi"), RED);
    // printf("ST7789VShowString\r\n");
    // while (1)
    // {
    //     delay(1000);
    //     ST7789VClear(BLACK);
    //     ST7789VShowString(0, index * 16, (char *)("Sauron Pi"), GREEN);
    //     index++;
    //     if (index * 16 > 135 - 16) {
    //         index = 0;
    //     }
    // }
}
