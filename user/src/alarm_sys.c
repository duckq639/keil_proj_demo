#include "alarm_sys.h"

static alarm_sys_mode current_mode = IDEL;
static uint32_t last_time = 0;

void alarm_sys_func(alarm_sys_mode target_mode)
{
    if (target_mode != current_mode)
    {
        current_mode = target_mode;
        HAL_GPIO_WritePin(LED_GPIO_PORT, LED1_PIN | LED2_PIN | LED3_PIN | LED4_PIN, GPIO_PIN_RESET);//用于关闭所有的led
        HAL_GPIO_WritePin(BUZZER_GPIO_PORT, BUZZER_PIN, GPIO_PIN_RESET);//关闭蜂鸣器
    }
    switch (current_mode)
    {
    case IDEL:
        HAL_GPIO_WritePin(LED_GPIO_PORT, LED1_PIN | LED2_PIN | LED3_PIN | LED4_PIN, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(BUZZER_GPIO_PORT, BUZZER_PIN, GPIO_PIN_RESET);
        break;
    case RUN:
        led_blink_func(250);
        HAL_GPIO_WritePin(BUZZER_GPIO_PORT, BUZZER_PIN, GPIO_PIN_RESET);
        break;
    case ALARM:
        led_blink_func(50);

        HAL_GPIO_TogglePin(BUZZER_GPIO_PORT, BUZZER_PIN);
        last_time = HAL_GetTick();
    }
}
// 思考:如何才能进行几乎无延迟的模式切换