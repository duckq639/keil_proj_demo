/**
 * @file    led.c
 * @brief   LED 驱动实现文件。
 */
#include "led.h"

/* 点亮指定编号的 LED */
static void led_on(uint8_t led_num)
{
    /* switch 根据 led_num 的值选择执行哪一个 case */
    switch (led_num)
    {
    case 1:
        HAL_GPIO_WritePin(LED_GPIO_PORT, LED1_PIN, GPIO_PIN_SET);
        break;
    case 2:
        HAL_GPIO_WritePin(LED_GPIO_PORT, LED2_PIN, GPIO_PIN_SET);
        break;
    case 3:
        HAL_GPIO_WritePin(LED_GPIO_PORT, LED3_PIN, GPIO_PIN_SET);
        break;
    case 4:
        HAL_GPIO_WritePin(LED_GPIO_PORT, LED4_PIN, GPIO_PIN_SET);
        break;
    default:
        /* 1~4 以外的编号不处理；break 表示跳出 switch */
        break;
    }
}

/* 熄灭指定编号的 LED */
static void led_off(uint8_t led_num)
{
    switch (led_num)
    {
    case 1:
        HAL_GPIO_WritePin(LED_GPIO_PORT, LED1_PIN, GPIO_PIN_RESET);
        break;
    case 2:
        HAL_GPIO_WritePin(LED_GPIO_PORT, LED2_PIN, GPIO_PIN_RESET);
        break;
    case 3:
        HAL_GPIO_WritePin(LED_GPIO_PORT, LED3_PIN, GPIO_PIN_RESET);
        break;
    case 4:
        HAL_GPIO_WritePin(LED_GPIO_PORT, LED4_PIN, GPIO_PIN_RESET);
        break;
    default:
        break;
    }
}

void led_blink_func(uint32_t delay_ms)
{
    for (uint8_t i = 0; i < LED_COUNT; i++)
    {
        uint8_t led_num = i + 1;
        uint8_t blink_count = led_num;
        while ((blink_count--) > 0){
            led_on(led_num);
            HAL_Delay(delay_ms);
            led_off(led_num);
            HAL_Delay(delay_ms);
        }
    }
}