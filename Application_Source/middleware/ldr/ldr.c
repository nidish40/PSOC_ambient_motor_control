#include "ldr.h"
#include "drivers/adc/adc.h"

/* ================= USER CONFIG ================= */

/* Adjust these based on wiring */
#define LDR_ADC_PIN     1     // which pin is routed in SAR_MUX_SWITCH0
#define LDR_ADC_CHANNEL  0     // SAR channel number

#define ADC_MAX_VALUE    4095U

/* ================= API ================= */

void ldr_init(void)
{
    /* Initialize SAR ADC */
    adc_init(LDR_ADC_PIN);

    /* Configure LDR ADC channel */
    adc_channel_init(LDR_ADC_CHANNEL);
}

uint8_t ldr_get_light_percent(void)
{
    uint16_t raw;

    /* Start ADC conversion */
    start_adc();

    /* Poll for conversion complete */
    while (!check_adc())
        ;

    /* Read ADC result */
    raw = adc_read(LDR_ADC_CHANNEL);

    if (raw > ADC_MAX_VALUE)
        raw = ADC_MAX_VALUE;

    /* Map to 0–100 % */
    return (uint8_t)((raw * 100U) / ADC_MAX_VALUE);
}
