#include "Int_IP5305T.h"


/**
 * @brief ����IP5305T��Դ �����Զ��ػ�
 * 
 */
void Int_IP5305T_start(void)
{
    HAL_GPIO_WritePin(POWER_KEY_GPIO_Port,POWER_KEY_Pin, GPIO_PIN_RESET);
    vTaskDelay(100);
    HAL_GPIO_WritePin(POWER_KEY_GPIO_Port,POWER_KEY_Pin, GPIO_PIN_SET);
}

void Int_IP5305T_shutdown(void)
{
    /* 第一次模拟按键 */
    HAL_GPIO_WritePin(
        POWER_KEY_GPIO_Port,
        POWER_KEY_Pin,
        GPIO_PIN_RESET
    );

    vTaskDelay(pdMS_TO_TICKS(100));

    HAL_GPIO_WritePin(
        POWER_KEY_GPIO_Port,
        POWER_KEY_Pin,
        GPIO_PIN_SET
    );

    vTaskDelay(pdMS_TO_TICKS(200));

    /* 第二次模拟按键 */
    HAL_GPIO_WritePin(
        POWER_KEY_GPIO_Port,
        POWER_KEY_Pin,
        GPIO_PIN_RESET
    );

    vTaskDelay(pdMS_TO_TICKS(100));

    HAL_GPIO_WritePin(
        POWER_KEY_GPIO_Port,
        POWER_KEY_Pin,
        GPIO_PIN_SET
    );
}
