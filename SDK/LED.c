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

void LEDInitWithEnable(bool enable)
{
    /* LED R:PC13 G:PA1 B:PA2 低电平有效 */
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOC);

    gpio_bit_write(LED_R_GPIO, LED_R_PIN, enable ? LED_ON : LED_OFF);
    gpio_bit_write(LED_G_GPIO, LED_G_PIN, enable ? LED_ON : LED_OFF);
    gpio_bit_write(LED_B_GPIO, LED_B_PIN, enable ? LED_ON : LED_OFF);

    gpio_init(LED_R_GPIO, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, LED_R_PIN);
    gpio_init(LED_G_GPIO, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, LED_G_PIN);
    gpio_init(LED_B_GPIO, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, LED_B_PIN);
}

void LEDInit(void)
{
    LEDInitWithEnable(false);
}

void LEDControl(LED led, bool enable)
{
    switch (led)
    {
    case LED_RED:
        gpio_bit_write(LED_R_GPIO, LED_R_PIN, enable ? LED_ON : LED_OFF);
        break;
    case LED_GREEN:
        gpio_bit_write(LED_G_GPIO, LED_G_PIN, enable ? LED_ON : LED_OFF);
        break;
    case LED_BLUE:
        gpio_bit_write(LED_B_GPIO, LED_B_PIN, enable ? LED_ON : LED_OFF);
        break;
    }
}

void LEDToggle(LED led)
{
    switch (led)
    {
    case LED_RED:
        gpio_bit_write(LED_R_GPIO, LED_R_PIN, gpio_input_bit_get(LED_R_GPIO, LED_R_PIN) == LED_ON ? LED_OFF : LED_ON);
        break;
    case LED_GREEN:
        gpio_bit_write(LED_G_GPIO, LED_G_PIN, gpio_input_bit_get(LED_G_GPIO, LED_G_PIN) == LED_ON ? LED_OFF : LED_ON);
        break;
    case LED_BLUE:
        gpio_bit_write(LED_B_GPIO, LED_B_PIN, gpio_input_bit_get(LED_B_GPIO, LED_B_PIN) == LED_ON ? LED_OFF : LED_ON);
        break;
    }
}