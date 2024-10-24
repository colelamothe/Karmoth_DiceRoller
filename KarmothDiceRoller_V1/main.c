/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <matrixLED.h>
#include "stdlib.h"
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
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */
int readButton1();
int readButton2();
void goToSleep();

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/* Buttons */
uint16_t button1Count = 0;
uint16_t button1Released = 1;
uint32_t button1HoldStart = 0;

uint16_t button2Count = 0;
uint16_t button2Released = 1;
uint32_t button2HoldStart = 0;

uint8_t currentNumber = 20;
uint32_t lastIncrement = 0;
uint16_t ledRefresh = 0;
uint16_t ledRefreshRate = 1;
uint16_t timePerDigit = 1000;

uint8_t currentDice = 20;

uint8_t lastRoll = 0;

uint8_t diceValues[] = {4,6,8,10,12,20,100};
uint8_t buttonCounter = 5;

// Sleep variables
uint32_t timeToSleep = 0;
uint16_t sleepInterval = 10000;

uint32_t x = 0;
const uint32_t RANDMAX = 2147483647;

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
  /* USER CODE BEGIN 2 */

  updateNumber(currentNumber);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  srand (HAL_GetTick());
  while (1)
  {
	  rand(); // Constantly pull random numbers to decrease predictability with similar seeds

	  if(HAL_GetTick() - ledRefresh > ledRefreshRate){
		  ledRefresh = HAL_GetTick();
		  refreshDisplay();
	  }
	  if(readButton1()){
		  if(buttonCounter >= 6){
			  buttonCounter = 0;
		  }
		  else{
			  buttonCounter++;
		  }
		  currentDice = diceValues[buttonCounter];
		  if(currentDice == 100){
			  updateNumber(99);
		  }
		  else{
			  updateNumber(currentDice);
		  }
		  timeToSleep = HAL_GetTick();
	  }
	  if(readButton2()){
		  // Using only % biases the rand to low numbers
		  do {
		      x = rand();
		  } while (x >= RANDMAX - (RANDMAX % currentDice)); // generate rand # until one divides nicely

		  x %= currentDice;
		  lastRoll = x + 1; // Scale 0-currentDice-1 to proper values
		  if(lastRoll > 99){ // Display 100 as 00
			  lastRoll = 0;
		  }
		  //lastRoll = (rand()% currentDice) + 1;
		  scrollDisplay();

		  updateNumber(lastRoll);
		  timeToSleep = HAL_GetTick();
	  }


	  //Iterates through LED array for testing

	  if(HAL_GetTick() - timeToSleep > sleepInterval){
		  /*Configure GPIO pin Output Level */
		  HAL_GPIO_WritePin(GPIOA, LDC1_Pin|LDC2_Pin|LDC3_Pin|LDC4_Pin
		                          |LDC5_Pin|LDC6_Pin | LDR1_Pin|LDR2_Pin|LDR3_Pin|LDR4_Pin
		                          |LDR5_Pin, GPIO_PIN_RESET);
		  goToSleep();
		  timeToSleep = HAL_GetTick();
	  }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV32;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV16;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LDC1_Pin|LDC2_Pin|LDC3_Pin|LDC4_Pin
                          |LDC5_Pin|LDC6_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LDR1_Pin|LDR2_Pin|LDR3_Pin|LDR4_Pin
                          |LDR5_Pin, GPIO_PIN_SET);

  /*Configure GPIO pins : PC14 PC15 PC6 */
  GPIO_InitStruct.Pin = GPIO_PIN_14|GPIO_PIN_15|GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PF2 */
  GPIO_InitStruct.Pin = GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /*Configure GPIO pins : LDC1_Pin LDC2_Pin LDC3_Pin LDC4_Pin
                           LDC5_Pin LDC6_Pin LDR1_Pin LDR2_Pin
                           LDR3_Pin LDR4_Pin LDR5_Pin */
  GPIO_InitStruct.Pin = LDC1_Pin|LDC2_Pin|LDC3_Pin|LDC4_Pin
                          |LDC5_Pin|LDC6_Pin|LDR1_Pin|LDR2_Pin
                          |LDR3_Pin|LDR4_Pin|LDR5_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : BT1_Pin BT2_Pin */
  GPIO_InitStruct.Pin = BT1_Pin|BT2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PA15 */
  GPIO_InitStruct.Pin = GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB3 PB4 PB5 PB6
                           PB7 PB8 */
  GPIO_InitStruct.Pin = GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6
                          |GPIO_PIN_7|GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI0_1_IRQn);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

int readButton1(){
	if(!HAL_GPIO_ReadPin(GPIOB, BT1_Pin)){
		if(button1Count > 100 && button1Released == 1){
			button1Released = 0;
			button1HoldStart = HAL_GetTick();
			return 1;
		}
		if(button1Count > 100){
			button1Count = 101;
		}
		else{
			button1Count++;
		}
	}
	else{

		if(button1Count < 4){
			button1Released = 1;
			return 0;
		}
		if(button1Count < 4){
			button1Count = 0;
		}
		else{
			button1Count = button1Count -2;
		}
	}
	return 0;
}

int readButton2(){
	if(!HAL_GPIO_ReadPin(GPIOB, BT2_Pin)){
		if(button2Count > 100 && button2Released == 1){
			button2Released = 0;
			button2HoldStart = HAL_GetTick();
			return 1;
		}
		if(button2Count > 100){
			button2Count = 101;
		}
		else{
			button2Count++;
		}
	}
	else{

		if(button2Count < 4){
			button2Released = 1;
			return 0;
		}
		if(button2Count < 4){
			button2Count = 0;
		}
		else{
			button2Count = button2Count -2;
		}
	}
	return 0;
}

void goToSleep(){
	  GPIO_InitTypeDef GPIO_InitStruct = {0};
	  /*Configure GPIO pins : LDC1_Pin LDC2_Pin LDC3_Pin LDC4_Pin
	                           LDC5_Pin LDC6_Pin LDR1_Pin LDR2_Pin
	                           LDR3_Pin LDR4_Pin LDR5_Pin */
	  GPIO_InitStruct.Pin = LDC1_Pin|LDC2_Pin|LDC3_Pin|LDC4_Pin
	                          |LDC5_Pin|LDC6_Pin|LDR1_Pin|LDR2_Pin
	                          |LDR3_Pin|LDR4_Pin|LDR5_Pin;
	  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  //GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	HAL_SuspendTick();
	HAL_PWR_EnterSTOPMode(0, PWR_STOPENTRY_WFI);
	//HAL_PWR_EnableWakeUpPin(0);
	//HAL_PWR_EnterSTANDBYMode();
    HAL_ResumeTick();
    SystemClock_Config();
    GPIO_InitStruct.Pin = LDC1_Pin|LDC2_Pin|LDC3_Pin|LDC4_Pin
                            |LDC5_Pin|LDC6_Pin|LDR1_Pin|LDR2_Pin
                            |LDR3_Pin|LDR4_Pin|LDR5_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	  //SystemClock_Config ();
	  //HAL_ResumeTick();
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
