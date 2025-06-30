#include "LED.h"
#include "Foundation.h"
#include "gd32vf103_rcu.h"

#define LED_ON RESET
#define LED_OFF SET

#define LED_R_GPIO GPIOC
#define LED_R_PIN GPIO_PIN_13

#define LED_G_GPIO GPIOA
#define LED_G_PIN GPIO_PIN_1

#define LED_B_GPIO GPIOA
#define LED_B_PIN GPIO_PIN_2


void LEDInit(void)
{
    LEDInitWithStatus(false);
}

void LEDInitWithStatus(LEDStatus status)
{
    /* LED Red:PC13 Green:PA1 Blue:PA2 低电平有效 */
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOC);

    gpio_bit_write(LED_R_GPIO, LED_R_PIN, status ? LED_ON : LED_OFF);
    gpio_bit_write(LED_G_GPIO, LED_G_PIN, status ? LED_ON : LED_OFF);
    gpio_bit_write(LED_B_GPIO, LED_B_PIN, status ? LED_ON : LED_OFF);

    gpio_init(LED_R_GPIO, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, LED_R_PIN);
    gpio_init(LED_G_GPIO, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, LED_G_PIN);
    gpio_init(LED_B_GPIO, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, LED_B_PIN);
}

LEDStatus GetLEDStatus(LEDItem item)
{
    LEDStatus result = LEDStatusOff;
    switch (item)
    {
    case LEDItemRed:
        result = gpio_input_bit_get(LED_R_GPIO, LED_R_PIN) == LED_ON ? LEDStatusOn : LEDStatusOff;
        break;
    case LEDItemGreen:
        result = gpio_input_bit_get(LED_G_GPIO, LED_G_PIN) == LED_ON ? LEDStatusOn : LEDStatusOff;
        break;
    case LEDItemBlue:
        result = gpio_input_bit_get(LED_B_GPIO, LED_B_PIN) == LED_ON ? LEDStatusOn : LEDStatusOff;
        break;
    }
    return result;
}

void SetLEDStatus(LEDItem item, LEDStatus status)
{
    switch (item)
    {
    case LEDItemRed:
        gpio_bit_write(LED_R_GPIO, LED_R_PIN, status ? LED_ON : LED_OFF);
        break;
    case LEDItemGreen:
        gpio_bit_write(LED_G_GPIO, LED_G_PIN, status ? LED_ON : LED_OFF);
        break;
    case LEDItemBlue:
        gpio_bit_write(LED_B_GPIO, LED_B_PIN, status ? LED_ON : LED_OFF);
        break;
    }
}

void ToggleLED(LEDItem item)
{
    SetLEDStatus(item, GetLEDStatus(item) == LEDStatusOn ? LEDStatusOff : LEDStatusOn);
}