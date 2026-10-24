#ifndef __INT_JOYSTICK__
#define __INT_JOYSTICK__

#include <stdint.h>
#include "adc.h"

typedef struct
{
    int16_t thr;
    int16_t yaw;
    int16_t pit;
    int16_t rol;
}Joystick_Struct;

// Initialise ADC joystick
void Int_joystick_init(void);

void Int_joystick_get(Joystick_Struct * joystick);



#endif // __INT_JOYSTICK__