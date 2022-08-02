/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
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

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

CAN_HandleTypeDef hcan1;
CAN_HandleTypeDef hcan2;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/


/* USER CODE BEGIN PV */
CAN_TxHeaderTypeDef   TxHeader1;
CAN_RxHeaderTypeDef   RxHeader1;
uint8_t               TxData1[8];
uint8_t               RxData1[8];
uint32_t              TxMailbox1;

CAN_TxHeaderTypeDef   TxHeader2;
CAN_RxHeaderTypeDef   RxHeader2;
uint8_t               TxData2[8];
uint8_t               RxData2[8];
uint32_t              TxMailbox2;


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_CAN1_Init(void);
static void MX_CAN2_Init(void);
/* USER CODE BEGIN PFP */

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
  MX_CAN1_Init();
  MX_CAN2_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  RCC_OscInitStruct.PLL2.PLL2State = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSE;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure the Systick interrupt time
  */
  __HAL_RCC_PLLI2S_ENABLE();
}

/**
  * @brief CAN1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_CAN1_Init(void)
{

  /* USER CODE BEGIN CAN1_Init 0 */
	CAN_FilterTypeDef  sFilterConfig1;
  /* USER CODE END CAN1_Init 0 */

  /* USER CODE BEGIN CAN1_Init 1 */

  /* USER CODE END CAN1_Init 1 */
  hcan1.Instance = CAN1;
  hcan1.Init.Prescaler = 5;
  hcan1.Init.Mode = CAN_MODE_NORMAL;
  hcan1.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan1.Init.TimeSeg1 = CAN_BS1_8TQ;
  hcan1.Init.TimeSeg2 = CAN_BS2_1TQ;
  hcan1.Init.TimeTriggeredMode = DISABLE;
  hcan1.Init.AutoBusOff = DISABLE;
  hcan1.Init.AutoWakeUp = DISABLE;
  hcan1.Init.AutoRetransmission = DISABLE;
  hcan1.Init.ReceiveFifoLocked = DISABLE;
  hcan1.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&hcan1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CAN1_Init 2 */

  /* Configure the CAN Filter */
    sFilterConfig1.FilterBank = 0;
    sFilterConfig1.FilterMode = CAN_FILTERMODE_IDMASK;
    sFilterConfig1.FilterScale = CAN_FILTERSCALE_32BIT;
    sFilterConfig1.FilterIdHigh = 0x0000;
    sFilterConfig1.FilterIdLow = 0x0000;
    sFilterConfig1.FilterMaskIdHigh = 0x0000;
    sFilterConfig1.FilterMaskIdLow = 0x0000;
    sFilterConfig1.FilterFIFOAssignment = CAN_RX_FIFO0;
    sFilterConfig1.FilterActivation = ENABLE;
    sFilterConfig1.SlaveStartFilterBank = 14;

    if (HAL_CAN_ConfigFilter(&hcan1, &sFilterConfig1) != HAL_OK)
    {
      /* Filter configuration Error */
      Error_Handler();
    }

    /* Start the CAN peripheral */
    if (HAL_CAN_Start(&hcan1) != HAL_OK)
    {
      /* Start Error */
      Error_Handler();
    }

    /* Activate CAN RX notification */
    if (HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK)
    {
      /* Notification Error */
      Error_Handler();
    }
  /* USER CODE END CAN1_Init 2 */

}

/**
  * @brief CAN2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_CAN2_Init(void)
{

  /* USER CODE BEGIN CAN2_Init 0 */
  	CAN_FilterTypeDef  sFilterConfig2;
  /* USER CODE END CAN2_Init 0 */

  /* USER CODE BEGIN CAN2_Init 1 */

  /* USER CODE END CAN2_Init 1 */
  hcan2.Instance = CAN2;
  hcan2.Init.Prescaler = 5;
  hcan2.Init.Mode = CAN_MODE_NORMAL;
  hcan2.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan2.Init.TimeSeg1 = CAN_BS1_8TQ;
  hcan2.Init.TimeSeg2 = CAN_BS2_1TQ;
  hcan2.Init.TimeTriggeredMode = DISABLE;
  hcan2.Init.AutoBusOff = DISABLE;
  hcan2.Init.AutoWakeUp = DISABLE;
  hcan2.Init.AutoRetransmission = DISABLE;
  hcan2.Init.ReceiveFifoLocked = DISABLE;
  hcan2.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&hcan2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CAN2_Init 2 */

  /* Configure the CAN Filter */
      sFilterConfig2.FilterBank = 14;
      sFilterConfig2.FilterMode = CAN_FILTERMODE_IDMASK;
      sFilterConfig2.FilterScale = CAN_FILTERSCALE_32BIT;
      sFilterConfig2.FilterIdHigh = 0x0000;
      sFilterConfig2.FilterIdLow = 0x0000;
      sFilterConfig2.FilterMaskIdHigh = 0x0000;
      sFilterConfig2.FilterMaskIdLow = 0x0000;
      sFilterConfig2.FilterFIFOAssignment = CAN_RX_FIFO1;
      sFilterConfig2.FilterActivation = ENABLE;
      sFilterConfig2.SlaveStartFilterBank = 14;
  /* Init same Filter as CAN1 */
  if (HAL_CAN_ConfigFilter(&hcan2, &sFilterConfig2) != HAL_OK)
 {
	/* Filter configuration Error */
	Error_Handler();
  }

  /* Start the CAN peripheral */
  if (HAL_CAN_Start(&hcan2) != HAL_OK)
  {
	/* Start Error */
	Error_Handler();
  }

  /* Activate CAN RX notification */
  if (HAL_CAN_ActivateNotification(&hcan2, CAN_IT_RX_FIFO1_MSG_PENDING) != HAL_OK)
  {
	/* Notification Error */
	Error_Handler();
  }

  /* USER CODE END CAN2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

}

/* USER CODE BEGIN 4 */

/**
  * @brief  Rx Fifo 0 message pending callback in non blocking mode
  * @param  hcan1: pointer to a CAN_HandleTypeDef structure that contains
  *         the configuration information for the specified CAN.
  * @retval None
  */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan1)
{
  /* Get RX message FROM car TO speedo*/
  if (HAL_CAN_GetRxMessage(hcan1, CAN_RX_FIFO0, &RxHeader1, RxData1) != HAL_OK)
  {
    /* Reception Error */
    Error_Handler();
  }



  /* Block ID for speed an manipulate data  */
  if (RxHeader1.StdId == 0x3e9)
  {
    /*------------Data manipulation----------------
	#scale
		if(kmh > 20) => 4.2
			- 4.2 km/h
		if(3 < kmh > 20) => 3
			- 1.5 km/h	
		if(kmh < 3) => 1.5
			none
		

		HEX
		00 | 00
		...			8 Bit -> 256/scale =>
		00 | FF
		01 | 00
		01 | 01
	--------------------------------------------------*/
	  
	  
	  if(RxData1[0] <= 0x03){							//don't manipulate
		  TxData2[0] = RxData1[0];						//
		  TxData2[1] = RxData1[1];
	  }

	  if(RxData1[0] < 0x05 && RxData1[0] > 0x03){					// manipulate +1/2*scale between 03 00 and 05 00
		  if(RxData1[1] < 0x80){						// check if we can add 1/2*scale without going over 255
		  		TxData2[0] = RxData1[0];				// don't change the first bit
		  		TxData2[1] = RxData1[1] + 0x7f;				// + 1/2*scale
		  }else{								// RxData1[1] + 127 = > 256
		  		TxData2[0] = RxData1[0] + 0x01;				// +1
		  		TxData2[1] = RxData1[1] - 0x80;				// RxData[1] - 127 = difference for bit overflow
		  }
	  }

	  if(RxData1[0] >= 0x05){							// ca. 20 km/h
		  TxData2[0] = RxData1[0] + 0x01;					// CAN1 -> +4,2km/h -> speedo
		  TxData2[1] = RxData1[1];						//
	  }

  }else{
	  //redirect every Frame from Can1 to Can2
	  int i;
	  for (i=0;i<RxHeader1.DLC;i++)
	     {
		  TxData2[i] = RxData1[i];
	     }


  }

  TxHeader2.StdId = RxHeader1.StdId;
  TxHeader2.ExtId = RxHeader1.ExtId;
  TxHeader2.RTR = RxHeader1.RTR;
  TxHeader2.IDE = RxHeader1.IDE;
  TxHeader2.DLC = RxHeader1.DLC;


  /* Start the Transmission process */
  if (HAL_CAN_AddTxMessage(&hcan2, &TxHeader2, TxData2, &TxMailbox2) != HAL_OK)
  {
	 /* Transmission request Error */
	 Error_Handler();
  }

}


/**
  * @brief  Rx Fifo 1 message pending callback in non blocking mode
  * @param  hcan1: pointer to a CAN_HandleTypeDef structure that contains
  *         the configuration information for the specified CAN.
  * @retval None
  */
void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan2)
{
  /* Get RX message FROM speedo BACK to car */
  if (HAL_CAN_GetRxMessage(hcan2, CAN_RX_FIFO1, &RxHeader2, RxData2) != HAL_OK)
  {
    /* Reception Error */
    Error_Handler();
  }

  //redirect every Frame from Can2 to Can1
  int i;
  for (i=0;i<RxHeader2.DLC;i++)
	 {
	  TxData1[i] = RxData2[i];
	 }
  TxHeader1.StdId = RxHeader2.StdId;
  TxHeader1.ExtId = RxHeader2.ExtId;
  TxHeader1.RTR = RxHeader2.RTR;
  TxHeader1.IDE = RxHeader2.IDE;
  TxHeader1.DLC = RxHeader2.DLC;

	  /* Start the Transmission process */
	  if (HAL_CAN_AddTxMessage(&hcan1, &TxHeader1, TxData1, &TxMailbox1) != HAL_OK)
	  {
	     /* Transmission request Error */
	     Error_Handler();
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

