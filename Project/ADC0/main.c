#include "Foundation.h"
#include "USART.h"

// 系统初始化函数使用标准工具链时没有在main函数前调用,声明后手动调用
extern void _init();

#define BOARD_ADC_CHANNEL          ADC_CHANNEL_0
#define ADC_GPIO_PORT              GPIOA
#define ADC_GPIO_PIN               GPIO_PIN_0

uint16_t adc_value;

void rcu_config(void);
void gpio_config(void);
void adc_config(void);

/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
    _init();
    /* system clocks configuration */
    rcu_config();
    /* GPIO configuration */
    gpio_config();
    /* configures COM port */
    USARTInit();
    /* ADC configuration */
    adc_config();
    adc_software_trigger_enable(ADC0, ADC_REGULAR_CHANNEL);
  
    while(1){
        while(SET != adc_flag_get(ADC0, ADC_FLAG_EOC));
        adc_flag_clear(ADC0, ADC_FLAG_EOC);

        adc_value = ADC_RDATA(ADC0);
        printf("6B: 0x%x\r\n", adc_value);
        delay(500);
    }
}

/*!
    \brief      configure the different system clocks
    \param[in]  none
    \param[out] none
    \retval     none
*/
void rcu_config(void)
{
    /* enable GPIOC clock */
    rcu_periph_clock_enable(RCU_GPIOA);
    /* enable ADC clock */
    rcu_periph_clock_enable(RCU_ADC0);
    /* config ADC clock */
    rcu_adc_clock_config(RCU_CKADC_CKAPB2_DIV8);
}

/*!
    \brief      configure the GPIO peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
void gpio_config(void)
{
    /* config the GPIO as analog mode */
    gpio_init(ADC_GPIO_PORT, GPIO_MODE_AIN, GPIO_OSPEED_50MHZ, ADC_GPIO_PIN);
}

/*!
    \brief      configure the ADC peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
void adc_config(void)
{
    /* reset ADC */
    adc_deinit(ADC0);
    /* ADC mode config */
    adc_mode_config(ADC_MODE_FREE);
    /* ADC resolusion 6B */
    printf("\r\nresolusion 12B:\r\n");
    adc_resolution_config(ADC0, ADC_RESOLUTION_12B);
	/* ADC contineous function enable */
    adc_special_function_config(ADC0, ADC_CONTINUOUS_MODE, ENABLE);
    /* ADC trigger config */
    adc_external_trigger_source_config(ADC0, ADC_REGULAR_CHANNEL, ADC0_1_EXTTRIG_REGULAR_NONE);
    /* ADC data alignment config */
    adc_data_alignment_config(ADC0, ADC_DATAALIGN_RIGHT);
    /* ADC channel length config */
    adc_channel_length_config(ADC0, ADC_REGULAR_CHANNEL, 1);
 
    /* ADC regular channel config */
    adc_regular_channel_config(ADC0, 0, BOARD_ADC_CHANNEL, ADC_SAMPLETIME_55POINT5);
    adc_external_trigger_config(ADC0, ADC_REGULAR_CHANNEL, ENABLE);
    
    /* enable ADC interface */
    adc_enable(ADC0);
    delay(1);
    /* ADC calibration and reset calibration */
    adc_calibration_enable(ADC0);
}
