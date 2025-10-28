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
#include "adc.h"
#include "spi.h"
#include "tim.h"
#include "usb_device.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <tgmath.h>

#include "fast_foc.h"
#include "usbd_cdc_if.h"
#include "isr.h"
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

uint16_t fake_encoder = 0;
uint16_t encoder_send = 0xffff;
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
  MX_TIM1_Init();
  MX_SPI1_Init();
  MX_SPI3_Init();
  MX_TIM8_Init();
  MX_USB_Device_Init();
  MX_ADC1_Init();
  MX_TIM2_Init();
  MX_TIM4_Init();
  MX_TIM15_Init();
  MX_TIM6_Init();
  /* USER CODE BEGIN 2 */

  mos_init(&htim1);
  mos_init(&htim8);

  // foc_init(&foc_L, &htim1);
  // foc_init(&foc_R, &htim8);

  foc_control_fast_init(&htim1, &motor_left_foc_driver, 16383, PWM_COUNT_PERIOD, 10, 1780, -1, FOC_TRACTION_ANGLE, FOC_PREACT_ANGLE);
  foc_control_fast_init(&htim8, &motor_right_foc_driver, 16383, PWM_COUNT_PERIOD, 10, 1370, -1, FOC_TRACTION_ANGLE, FOC_PREACT_ANGLE);

  HAL_TIM_Base_Start_IT(&htim1);
  HAL_TIM_Base_Start_IT(&htim8);

  HAL_TIM_Base_Start_IT(&htim6);

  HAL_TIM_PWM_Start(&htim15,TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_2);

  HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_3);
  HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_4);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  __HAL_TIM_SET_COMPARE(&htim15, TIM_CHANNEL_2, TX_COUNT_PERIOD / 2);  // L_TX
  __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_2, TX_COUNT_PERIOD / 2);  // R_TX
  while (1) {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

    HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);

    // printf("%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\r\n",
    //        motor_left_foc_driver.ouput_duty[0], motor_left_foc_driver.ouput_duty[1],
    //        motor_left_foc_driver.ouput_duty[2],
    //        motor_right_foc_driver.ouput_duty[0], motor_right_foc_driver.ouput_duty[1],
    //        motor_right_foc_driver.ouput_duty[2],
    //        fake_encoder,  (int16_t)motor_left_foc_driver.encoder_now_data,  (int16_t)motor_right_foc_driver.encoder_now_data,
    //        (int16_t)motor_left_foc_driver.rotate, (int16_t)motor_right_foc_driver.rotate,
    //        (int16_t)motor_left_foc_driver.speed_raw, (int16_t)motor_right_foc_driver.speed_raw,
    //        (int16_t)motor_left_foc_driver.speed_filtered, (int16_t)motor_right_foc_driver.speed_filtered
    //
    //        // fake_encoder, read_left_encoder(), read_right_encoder()
    // );
    // if (L_RX_capture.capture_end_flag == 1 && R_RX_capture.capture_end_flag == 1) {
    // PWM_Capture_Read_And_Clear(&L_RX_capture);
    // PWM_Capture_Read_And_Clear(&R_RX_capture);
      // printf("%ld, %ld, %ld, %ld, %ld, %ld, %ld, %ld\n", (int32_t)L_RX_capture.freq, (int32_t)round(L_RX_capture.duty * 1000.f + 0.5f),  (int32_t)R_RX_capture.freq, (int32_t)round(R_RX_capture.duty * 1000.f + 0.5f), L_RX_capture.high_val, R_RX_capture.high_val,
      //   (int32_t)motor_left_foc_driver.speed_filtered, (int16_t)motor_right_foc_driver.speed_filtered);
      printf("%ld, %ld, %ld, %ld\n", (int32_t)(speed_struct.left * 10000), (int32_t)(speed_struct.right * 10000), (int32_t)motor_left_foc_driver.speed_filtered, (int32_t)motor_right_foc_driver.speed_filtered);


    // L_RX_capture.capture_end_flag = 0;
    // R_RX_capture.capture_end_flag = 0;
    // }
    // HAL_Delay(1);
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
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV4;
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
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
