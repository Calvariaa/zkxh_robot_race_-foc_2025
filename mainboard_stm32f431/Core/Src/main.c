/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "dma.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "usb_device.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "usbd_cdc_if.h"
#include "ssd1306.h"
#include "icm42688.h"
#include "interface.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

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

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
int32_t Count = 0;
uint8_t Diretion = 0;
int16_t accel[3], gyro[3];
extern uint64_t total_time_10ms;
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART1_UART_Init();
  MX_USB_Device_Init();
  MX_I2C1_Init();
  MX_TIM2_Init();
  MX_I2C2_Init();
  MX_USART2_UART_Init();
  MX_TIM6_Init();
  /* USER CODE BEGIN 2 */
  ssd1306_Init();
  ssd1306_FlipScreenVertically();
  ssd1306_SetColor(White);

  HAL_Delay(100);
  // 初始化ICM42688
  ICM42688_Init(&hi2c2);

  HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);

  HAL_TIM_Base_Start_IT(&htim6);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  // ssd1306_ContUpdateEnable();
  char chr[256] = {0};
  while (1) {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    // printf("%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\r\n ",
    // HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13),
    // HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_14),
    // HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_15),
    // HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_4),
    // HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6),
    // HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5),
    // HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7),
    // HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_4),
    // HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1),
    // HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_15),
    // HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14),
    // HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_6),
    // HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10),
    // HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_10),
    // HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_3),
    // HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_11),
    // HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_4),
    // HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5),
    //
    // HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0),
    // HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_2),
    // HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_11),
    // HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_10),
    // HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12),
    // HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_13),
    // HAL_GPIO_ReadPin(GPIOG, GPIO_PIN_10)
    //
    // );

    // sprintf(chr, "10ms: %ld    ", total_time_10ms);
    // ssd1306_SetCursor(0, 0);
    // ssd1306_SetColor(White);
    // ssd1306_WriteString(chr, Font_7x10);
    // sprintf(chr, "Cnt: %ld    ", Count);
    // ssd1306_SetCursor(0, 10);
    // ssd1306_SetColor(White);
    // ssd1306_WriteString(chr, Font_7x10);
    // sprintf(chr, "gyro_z: %10d ", gyro[2]);
    // ssd1306_SetCursor(0, 20);
    // ssd1306_SetColor(White);
    // ssd1306_WriteString(chr, Font_7x10);
    // ssd1306_UpdateScreen();

    interface_main();
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
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1_BOOST);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSI48;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV4;
  RCC_OscInitStruct.PLL.PLLN = 85;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */


int _write(int file, char *ptr, int len) {
  CDC_Transmit_FS(ptr, len);
  // static uint8_t rc = USBD_OK;
  //
  // do {
  //   rc = CDC_Transmit_FS(ptr, len);
  // } while (USBD_BUSY == rc);
  //
  // if (USBD_FAIL == rc) {
  //   /// NOTE: Should never reach here.
  //   return 0;
  // }
  return len;
}


uint16_t pit_time_1ms = 0, pit_time_2ms = 0, pit_time_5ms = 0, pit_time_10ms = 0;
uint64_t total_time_10ms = 0;


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {

  // 1ms tim interrupt
  if (htim->Instance == TIM6) {

    pit_time_1ms++;
    pit_time_2ms++;
    pit_time_5ms++;
    pit_time_10ms++;

    if (pit_time_1ms == 1) {
      // 读取陀螺仪数据
      if (ICM42688_ReadGyro(&hi2c2, gyro) == HAL_OK) {
        printf("GX=%10d, GY=%10d, GZ=%10d\r\n", gyro[0], gyro[1], gyro[2]);
      }


      Diretion = __HAL_TIM_IS_TIM_COUNTING_DOWN(&htim2);
      Count = (int32_t) __HAL_TIM_GET_COUNTER(&htim2) >> 2;
      pit_time_1ms = 0;
    }
    if (pit_time_2ms == 2) {
      pit_time_2ms = 0;
    }
    if (pit_time_5ms == 5) {
      pit_time_5ms = 0;
    }
    if (pit_time_10ms == 10) {
      pit_time_10ms = 0;

      total_time_10ms++;
    }

  }


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

  char chr[256];
  ssd1306_Init();
  ssd1306_FlipScreenVertically();
  sprintf(chr, "Error_Handler");
  ssd1306_SetCursor(0, 0);
  ssd1306_SetColor(White);
  ssd1306_WriteString(chr, Font_7x10);
  ssd1306_UpdateScreen();
  while (1) {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
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
  char chr[256];
  ssd1306_Init();
  ssd1306_FlipScreenVertically();
  sprintf(chr, "ERR: %s on line %d ", file, line);
  ssd1306_SetCursor(0, 0);
  ssd1306_SetColor(White);
  ssd1306_WriteString(chr, Font_7x10);
  ssd1306_UpdateScreen();
  while (1) {
  }
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
