#ifndef __APP_PROCESS_DATA__
#define __APP_PROCESS_DATA__

#include "Int_joystick.h"
#include "Int_key.h"
#include "Com_debug.h"
#include "Com_tool.h"

typedef struct 
{
    int16_t thr;
    int16_t yaw;
    int16_t pit;
    int16_t rol;
    uint8_t shutdown;     // 0 no shutdown 1 shutdown
    uint8_t fix_height;
}Remote_Data;

extern Remote_Data remote_data;

void App_process_key_data(void);

void App_process_joystick_data(void);

#endif // __APP_PROCESS_DATA__
