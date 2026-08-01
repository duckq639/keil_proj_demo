/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  * @note           : 新人示例工程：程序入口，演示变量、宏、循环、判断和函数。
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "buzzer.h"   /* 蜂鸣器驱动的函数声明 */
#include "led.h"      /* LED 驱动的函数声明 */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* #define 是宏定义：编译前会把名字替换成后面的内容 */
#define DEMO_LED_COUNT   4U    /* 板载 LED 数量，U 表示 unsigned 无符号数 */
#define DEMO_BLINK_TIMES 3U    /* 每颗 LED 闪烁次数 */
#define DEMO_DELAY_MS    250U  /* LED 亮/灭持续时间，单位毫秒 */
#define DEMO_BEEP_MS     120U  /* 蜂鸣器响一声的时长 */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
/* 函数声明（原型）：告诉编译器函数名、参数和返回值，定义在下方 USER CODE 4 */
void demo_blink_led(uint8_t led_num, uint16_t times, uint32_t delay_ms);
void demo_beep(uint32_t beep_ms);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
  /* 局部变量：只能在 main 函数内使用 */
  uint8_t  current_led = 1U;              /* 当前要操作的 LED 编号 */
  uint16_t blink_times = DEMO_BLINK_TIMES;/* 每颗 LED 闪烁次数 */
  uint32_t delay_ms    = DEMO_DELAY_MS;   /* 亮/灭延时 */
  const uint8_t led_count = DEMO_LED_COUNT; /* const 表示该变量不允许修改 */
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  /* HAL 库初始化：配置 SysTick、Flash 等基础功能 */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  /* 系统时钟配置，本工程使用 HSI + PLL 跑到 168 MHz */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  /* GPIO 初始化：PA4~PA7 是 LED，PA8 是蜂鸣器，具体配置见 Core/Src/gpio.c */
  MX_GPIO_Init();
  /* USER CODE BEGIN 2 */
  buzzer_init(); /* 让蜂鸣器引脚先处于关闭状态 */
  /* USER CODE END 2 */

  /* Infinite loop */
  /* 嵌入式主程序通常用 while(1) 死循环，让程序一直运行 */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    current_led = 1U; /* 练习点：改成 2U，观察从哪颗 LED 开始 */

    /* while 循环：条件成立就反复执行 {} 里的代码 */
    while (current_led <= led_count)
    {
      demo_blink_led(current_led, blink_times, delay_ms);
      current_led++; /* 等价于 current_led = current_led + 1 */
    }

    demo_beep(DEMO_BEEP_MS);

    /* if / else 判断：让延时每次变快一点，到 100 后重新回到初始值 */
    if (delay_ms > 100U)
    {
      delay_ms -= 20U; /* 练习点：改成 += 20U 看速度变化方向 */
    }
    else
    {
      delay_ms = DEMO_DELAY_MS;
    }
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
/* 函数定义：这里才是函数的具体实现 */
void demo_blink_led(uint8_t led_num, uint16_t times, uint32_t delay_ms)
{
  uint16_t i = 0U; /* 循环计数变量 */

  /* if 判断：LED 编号只允许 1~4 */
  if (led_num > DEMO_LED_COUNT)
  {
    return; /* return 直接结束当前函数 */
  }

  /* for 循环：初始化; 判断条件; 每次循环后执行 */
  for (i = 0U; i < times; i++)
  {
    led_on(led_num);          /* 点亮指定 LED */
    HAL_Delay(delay_ms);      /* 延时一段时间 */
    led_off(led_num);         /* 熄灭指定 LED */
    HAL_Delay(delay_ms);
  }
}

void demo_beep(uint32_t beep_ms)
{
  buzzer_on();          /* 打开蜂鸣器 */
  HAL_Delay(beep_ms);   /* 保持响一段时间 */
  buzzer_off();         /* 关闭蜂鸣器 */
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
