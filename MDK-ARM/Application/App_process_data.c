#include "App_process_data.h"
#include <stdio.h>

Joystick_Struct joystick = {0, 0, 0, 0};
Remote_Data remote_data = {0};

int16_t key_pit_offset = 0;     // fly forward positive
int16_t key_roll_offset = 0;     // fly right positive
int16_t key_yaw_offset = 0;

// Record Joystick Offset
int16_t thr_offset = 0;
int16_t yaw_offset = 0;
int16_t pit_offset = 0;
int16_t rol_offset = 0;

// Adjustment Joystick Function
void App_calibrate_joystick(void)
{
    key_pit_offset = 0;
    key_roll_offset = 0;

    int32_t thr_sum = 0;
    int32_t yaw_sum = 0;
    int32_t pit_sum = 0;
    int32_t rol_sum = 0;

    Joystick_Struct sample;

    for (uint8_t i = 0; i < 10; i++)
    {
        Int_joystick_get(&sample);

        sample.thr = 1000 - ((int32_t)sample.thr * 1000 / 4095);
        sample.yaw = 1000 - ((int32_t)sample.yaw * 1000 / 4095);
        sample.pit = 1000 - ((int32_t)sample.pit * 1000 / 4095);
        sample.rol = 1000 - ((int32_t)sample.rol * 1000 / 4095);

        thr_sum += sample.thr;          // 油门目标：0
        yaw_sum += sample.yaw - 500;    // 其他通道目标：500
        pit_sum += sample.pit - 500;
        rol_sum += sample.rol - 500;

        vTaskDelay(pdMS_TO_TICKS(10));
    }

    thr_offset = thr_sum / 10;
    yaw_offset = yaw_sum / 10;
    pit_offset = pit_sum / 10;
    rol_offset = rol_sum / 10;
}



void App_process_key_data(void)
{
    Key_type key = Int_key_get();

    if (key == KEY_UP)
    {
        // fly forward, pitch angle increase
        key_pit_offset += 10;
    }
    else if (key == KEY_DOWN)
    {
        // fly backward, pitch angle decrease
        key_pit_offset -= 10;
    }
    else if (key == KEY_LEFT)
    {
        key_roll_offset -= 10;
    }
    else if (key == KEY_RIGHT)
    {
        key_roll_offset += 10;
    }
    else if (key == KEY_LEFT_X)
    {
        remote_data.shutdown = 1;
    }
    else if (key == KEY_RIGHT_X)
    {
        remote_data.fix_height = 1;
    }
    else if (key == KEY_RIGHT_X_LONG)
    {
        App_calibrate_joystick();
    }
}

void App_process_joystick_data(void)
{
    taskENTER_CRITICAL();


    Int_joystick_get(&joystick);



    joystick.thr = 1000 - (joystick.thr * 1000 / 4095);
    joystick.yaw = 1000 - (joystick.yaw * 1000 / 4095);
    joystick.pit = 1000 - (joystick.pit * 1000 / 4095);
    joystick.rol = 1000 - (joystick.rol * 1000 / 4095);

    // Zero Offset Adjustment
    joystick.thr -= thr_offset;
    joystick.yaw -= yaw_offset;
    joystick.pit -= pit_offset;
    joystick.rol -= rol_offset;

    joystick.pit += key_pit_offset;
    joystick.rol += key_roll_offset;

    joystick.thr = Com_limit(joystick.thr, 0, 1000);
    joystick.yaw = Com_limit(joystick.yaw, 0, 1000);
    joystick.pit = Com_limit(joystick.pit, 0, 1000);
    joystick.rol = Com_limit(joystick.rol, 0, 1000);

    taskEXIT_CRITICAL();

    remote_data.thr = joystick.thr;
    remote_data.yaw = joystick.yaw;
    remote_data.pit = joystick.pit;
    remote_data.rol = joystick.rol;


    printf("joy:%d,%d,%d,%d\r\n",
       joystick.thr,
       joystick.yaw,
       joystick.pit,
       joystick.rol);
}