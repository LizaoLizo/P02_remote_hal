#include "Int_joystick.h"
#include "adc.h"

uint16_t adc_buff[4] = {0};

void Int_joystick_init(void)
{
    // use the function in HAL
    HAL_ADC_Start_DMA(&hadc1, (uint32_t *)adc_buff, 4);
}

void Int_joystick_get(Joystick_Struct * joystick)
{
    joystick->thr = adc_buff[0];
    joystick->yaw = adc_buff[1];
    joystick->rol = adc_buff[2];
    joystick->pit = adc_buff[3];

}
