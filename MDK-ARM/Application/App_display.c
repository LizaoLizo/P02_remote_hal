#include "App_display.h"
extern Remote_Data remote_data;

extern uint8_t post_buff[TX_PLOAD_WIDTH];

void App_display_show_bar(uint8_t x, uint8_t y, uint8_t count)
{
    if (count < 13)
    {
        OLED_Show_CH(x, y, 12 + count, 12, 1);
    }
}

void App_display_init(void)
{
    OLED_Init();
}

void App_display_show(void)
{
    // x-coordinate, y-coordinate, string, font size, mode
    //OLED_ShowString(0, 0, "Hello World!", 12, 1);

    for (uint8_t i = 0; i < 2; i++)
{
    OLED_Show_CH(LINE1_BEGIN + 12 * i, Y0, i, 12, 1);
}

 uint8_t buff[4] = {0};   // 3位数字还需要1字节保存 '\0'
sprintf((char *)buff, "%03d", CHANNEL);
OLED_ShowString(LINE2_BEGIN, Y1, buff, 12, 1);

OLED_ShowString(LINE2_BEGIN + 64, Y1, "V: ", 12, 1);
OLED_ShowString(LINE2_BEGIN + 88, Y1, post_buff , 12, 1);



uint8_t count = 0;

OLED_ShowString(LINE3_BEGIN, Y2, "THR:", 12, 1);

if (remote_data.thr > 500)
{
    count = (remote_data.thr - 500) / 41;

    App_display_show_bar(BAR1_BEGIN, Y2, 12);     // 前半段全满
    App_display_show_bar(BAR2_BEGIN, Y2, count);  // 后半段变化
}
else
{
    count = remote_data.thr / 41;

    App_display_show_bar(BAR1_BEGIN, Y2, count);  // 前半段变化
    App_display_show_bar(BAR2_BEGIN, Y2, 0);      // 后半段清空
}

OLED_ShowString(LINE3_BEGIN2, Y2, "ROL:", 12, 1);

if (remote_data.rol > 500)
{
    count = (remote_data.rol - 500) /41;

    App_display_show_bar(BAR1_BEGIN2, Y2, 12);
    App_display_show_bar(BAR2_BEGIN2, Y2, count);
}
else
{
    count = (remote_data.rol) / 41;

    App_display_show_bar(BAR1_BEGIN2, Y2, count);
    App_display_show_bar(BAR2_BEGIN2, Y2, 0);

}

OLED_ShowString(LINE4_BEGIN, Y3, "YAW:", 12, 1);

if (remote_data.yaw > 500)
{
    count = (remote_data.yaw - 500) / 41;

    App_display_show_bar(BAR1_BEGIN, Y3, 12);     // 前半段全满
    App_display_show_bar(BAR2_BEGIN, Y3, count);  // 后半段变化
}
else
{
    count = remote_data.yaw / 41;

    App_display_show_bar(BAR1_BEGIN, Y3, count);  // 前半段变化
    App_display_show_bar(BAR2_BEGIN, Y3, 0);      // 后半段清空
}

OLED_ShowString(LINE4_BEGIN2, Y3, "Pit:", 12, 1);

if (remote_data.pit > 500)
{
    count = (remote_data.pit - 500) / 41;

    App_display_show_bar(BAR1_BEGIN2, Y3, 12);     // 前半段全满
    App_display_show_bar(BAR2_BEGIN2, Y3, count);  // 后半段变化
}
else
{
    count = remote_data.thr / 41;

    App_display_show_bar(BAR1_BEGIN2, Y3, count);  // 前半段变化
    App_display_show_bar(BAR2_BEGIN2, Y3, 0);      // 后半段清空
}


OLED_Refresh_Gram();
}
