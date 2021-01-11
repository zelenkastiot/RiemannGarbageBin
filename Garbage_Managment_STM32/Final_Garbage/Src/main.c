/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "dwt_stm32_delay.h"
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
TIM_HandleTypeDef htim2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/*
 SS1:
	 - Orange wire (Trig1) - D3 (PB0)
	 - Blue wire (Echo1) - A7 (PA2)

 SS2:
	 - Orange wire (Trig2) - D4 (PB7)
	 - Blue wire (Echo2) - A5 (PA6)

 SS3:
	 - Orange wire (Trig3) - D5 (PB6)
	 - Blue wire (Echo3) - A5 (PA6)
 */

uint32_t vreme, vremeSenzor;
uint32_t distanca1, distanca2, distanca3, distanca4, suma;

uint32_t chitanjeSenzor(GPIO_TypeDef* GPIO_T, uint16_t TRIG,GPIO_TypeDef* GPIO_E, uint16_t ECHO)
{
	vreme=0;


	HAL_GPIO_WritePin(GPIO_T, TRIG, GPIO_PIN_SET);
	HAL_Delay(1);
	//DWT_Delay_us (1);
	HAL_GPIO_WritePin(GPIO_T, TRIG, GPIO_PIN_RESET);



	while (!(HAL_GPIO_ReadPin(GPIO_E, ECHO)));
	while (HAL_GPIO_ReadPin(GPIO_E, ECHO))
	{
	vreme++;
	DWT_Delay_us (1);
	}
	return vreme*2;
}

//CALCULATE VOLUME DEMO
long plot=0; //plot to be calculated
int distance_between_sensors=2; //2cm
int distance_between_mid_sensors=4; //4cm

float radius = 9.5; //cm
float height = 19.0; //cm
volatile float my_volume;

float PI = 3.14;

volatile float procent;

//Function for setting volume
float init_volume(float current_plot){
  return current_plot * PI * radius;
}
//Function for returning the float value of the current volume
float c_volume(float plot){
  return plot * PI * radius;
}

float calculate(uint32_t Sensor1, uint32_t Sensor2,uint32_t Sensor3, uint32_t Sensor4){
  //Trapeziod rule
  plot = 0;
  //1st trapezoid
  plot+=(Sensor1+Sensor2)*distance_between_sensors/2;
  //2nd trapeziod
  plot+=(Sensor2+Sensor3)*distance_between_mid_sensors/2;
  //3rd trapeziod
  plot+=(Sensor3+Sensor4)*distance_between_sensors/2;
  return plot;
  }


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	 volatile float current_plot;
     volatile float my_volume;
     volatile float current_volume;
	 volatile float procent = 1;
     volatile int flag = 1; // TRUE - first time of measuri

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
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
  DWT_Delay_Init ();
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);  //tajmer 2, kanal 1 dava pwm izlez
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */


    /* USER CODE BEGIN 3 */

	  //SERVO: Timer 2, Channel 1 gives PWM OUT

//Calculating volume of bin
	  current_plot = 0;
	  current_volume = 0;
	  procent = 1;
	  if(flag == 1) my_volume = 0;

		    for(int i=200;i<=750;i+=50){
		   			 __HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1, i);
		   			 HAL_Delay(400);
		   			vremeSenzor = chitanjeSenzor(GPIOA, GPIO_PIN_9, GPIOA, GPIO_PIN_2);
		   			distanca1  = vremeSenzor * .034/2;
		   			HAL_Delay(200);

		   			vremeSenzor = chitanjeSenzor(GPIOA, GPIO_PIN_10, GPIOA, GPIO_PIN_7);
		   			distanca2 = vremeSenzor * .034/2;
		   			HAL_Delay(200);

		   			vremeSenzor = chitanjeSenzor(GPIOA, GPIO_PIN_12, GPIOA, GPIO_PIN_6);
		   			distanca3 = vremeSenzor * .034/2;
		   			HAL_Delay(200);

		   			vremeSenzor = chitanjeSenzor(GPIOB, GPIO_PIN_0, GPIOA, GPIO_PIN_5);
		   			distanca4 = vremeSenzor * .034/2;
		   			HAL_Delay(200);

		   			current_plot+=calculate(distanca1,distanca2,distanca3,distanca4);
		   			HAL_Delay(100);
		    }

		    //Goes into this statement IF it measures for the first time
		    if(flag == 1){
		    	my_volume = init_volume(current_plot);
		    	flag = 0;
		    	HAL_Delay(100);
		    }
		    //Goes into this statement after setting the volume of the bin
		    else{
		    	current_volume = c_volume(current_plot);
		        procent = current_volume/my_volume;
		        procent = 100-procent*100;
		        HAL_Delay(100);
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

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 16;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure the main internal regulator output voltage 
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 100;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 2000;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */
  HAL_TIM_MspPostInit(&htim2);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(Trig4_GPIO_Port, Trig4_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, Trig1_Pin|Trig2_Pin|Trig3_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : Echo1_Pin Echo4_Pin Echo3_Pin Echo2_Pin */
  GPIO_InitStruct.Pin = Echo1_Pin|Echo4_Pin|Echo3_Pin|Echo2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : Trig4_Pin */
  GPIO_InitStruct.Pin = Trig4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(Trig4_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : Trig1_Pin Trig2_Pin Trig3_Pin */
  GPIO_InitStruct.Pin = Trig1_Pin|Trig2_Pin|Trig3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

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
void assert_failed(char *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
