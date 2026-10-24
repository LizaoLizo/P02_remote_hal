#include "App_FreeRTOS_Task.h"

// STM32F103C8T6 => SRAM 20k  => ����12K������ϵͳ

// Joystick Data Structure

// ��Դ��������
void power_task(void *args);
// ��С�Ƽ���д128 => 128*4 = 512B
#define POWER_TASK_STACK_SIZE 128
// �������ȼ� => ��ֵԽС ���ȼ�ԽС  => ���4  => ���Ƽ�ʹ����С���ȼ�0
#define POWER_TASK_PRIORITY 4
TaskHandle_t power_task_handle;
#define POWER_TASK_PERIOD 10000

// ͨѶ����
void com_task(void *args);
#define COM_TASK_STACK_SIZE 128
#define COM_TASK_PRIORITY 3
TaskHandle_t com_task_handle;
// ��������
#define COM_TASK_PERIOD 10

void key_task(void * args);
#define KEY_TASK_STACK_SIZE 128
#define KEY_TASK_PRIORITY 2
TaskHandle_t key_task_handle;

#define KEY_TASK_PERIOD 20



// joystick task
void joy_task(void *args);
#define JOY_TASK_STACK_SIZE 128
#define JOY_TASK_PRIORITY 2
TaskHandle_t joy_task_handle;

#define JOY_TASK_PERIOD 20

void oled_task(void * args);
#define OLED_TASK_STACK_SIZE 128
#define OLED_TASK_PRIORITY 1
TaskHandle_t oled_task_handle;
#define OLED_TASK_PERIOD 100




/**
 * @brief ����freeRTOS����ϵͳ
 *
 */
void App_FreeRTOS_start(void)
{
    xTaskCreate(power_task, "power_task",
                POWER_TASK_STACK_SIZE, NULL,
                POWER_TASK_PRIORITY, &power_task_handle);

    xTaskCreate(com_task, "com_task",
                COM_TASK_STACK_SIZE, NULL,
                COM_TASK_PRIORITY, &com_task_handle);

    xTaskCreate(key_task, "key_task", KEY_TASK_STACK_SIZE, NULL, KEY_TASK_PRIORITY, &key_task_handle);

    xTaskCreate(joy_task, "joy_task", JOY_TASK_STACK_SIZE, NULL, JOY_TASK_PRIORITY, &joy_task_handle);

    xTaskCreate(oled_task, "oled_task", OLED_TASK_STACK_SIZE, NULL, OLED_TASK_PRIORITY, &oled_task_handle);

    vTaskStartScheduler();
}

void power_task(void *args)
{
    while (1)
    {
        uint32_t notified = ulTaskNotifyTake(
            pdTRUE,
            pdMS_TO_TICKS(10000)
        );

        if (notified != 0)
        {
            Int_IP5305T_shutdown();
            vTaskSuspend(NULL);
        }
        else
        {
            Int_IP5305T_start();
        }
    }
}

void joy_task(void * args)
{
    TickType_t xLastWakeTime = xTaskGetTickCount();
    // Initialize Joystick ADC
    Int_joystick_init();
    while (1)
    {
        App_process_joystick_data();
        vTaskDelayUntil(&xLastWakeTime, JOY_TASK_PERIOD);
    }
}

void key_task(void * args)
{
    TickType_t xLastWakeTime = xTaskGetTickCount();
    while(1)
    {
        App_process_key_data();

        vTaskDelayUntil(&xLastWakeTime, KEY_TASK_PERIOD);
    }
}

uint8_t com_data[TX_PLOAD_WIDTH + 1] = {0};

void com_task(void *args)
{
    TickType_t xLastWakeTime = xTaskGetTickCount();
    uint8_t shutdown_active = 0;
    uint8_t shutdown_packets = 0;

    while (1)
    {
        if (shutdown_active == 0 && remote_data.shutdown == 1)
        {
            shutdown_active = 1;
            shutdown_packets = 20;
        }

        App_transmit_data();

        if (shutdown_active)
        {
            shutdown_packets--;

            if (shutdown_packets == 0)
            {
                remote_data.shutdown = 0;

                /* 通知遥控器电源任务关机 */
                xTaskNotifyGive(power_task_handle);

                /* 停止继续发送 */
                vTaskSuspend(NULL);
            }
        }



        vTaskDelay(
          COM_TASK_PERIOD
        );
    }
}

void oled_task(void * args)
{
    TickType_t xLastWakeTime = xTaskGetTickCount();

    App_display_init();


    while (1)
    {
        App_display_show();

        vTaskDelayUntil(&xLastWakeTime, OLED_TASK_PERIOD);
    }
}
