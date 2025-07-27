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
#define Time_Pulse_Start 9000
#define Time_Pulse_Swap 5000
#define Time_Pulse_Zero_Data 600
#define Time_Pulse_One_Data 1700

 uint32_t state_edge = 0;
 int32_t capture_value1 = 0 ;
 int32_t capture_value2 = 0 ;
 volatile int32_t stage = 0;
 uint32_t countt = 0 ;
 uint16_t count_bits = 0; 
 uint8_t flag = 0;

 uint32_t element = 0;
 uint32_t buffer [20];
 uint16_t buffer_index = 0;
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
uint32_t *RCC_APB2ENR = (uint32_t*)(0x40021018);
uint32_t *TIMER1_CR1_REG = (uint32_t*) (0x40012C00);
uint32_t *TIMER1_CNT_REG = (uint32_t*) (0x40012C24);
uint32_t *TIMER1_PSR_REG = (uint32_t*) (0x40012C28);
uint32_t *TIMER1_ARR_REG = (uint32_t*) (0x40012C2C);
uint32_t *TIMER1_SR_REG = (uint32_t*) (0x40012C10);
uint32_t *TIMER1_EGR_REG = (uint32_t*) (0x40012C14);
uint32_t *TIMER1_CCER_REG = (uint32_t*) (0x40012C20);
uint32_t *TIMER1_CCMR1_REG = (uint32_t*) (0x40012C18);
uint32_t *TIMER1_CCR1_REG = (uint32_t*) (0x40012C34);
uint32_t *TIMER1_DIER_REG = (uint32_t*) (0x40012C0C);
uint32_t *NVIC_ISER0 = (uint32_t *)(0xE000E100);



void Custome_Timer(){
  //__HAL_RCC_TIM1_CLK_ENABLE();
  *RCC_APB2ENR |= (1<<11); 
  //Enable for tim2
  *TIMER1_PSR_REG = 71;
  *TIMER1_ARR_REG = 0xFFFF-1;
  //*TIMER1_EGR_REG |= (1<<0);
  //*TIMER_EGR_REG |=(1<<0);//Re-initialize the counter and generates an update of the registers.
  
  /*Capture input mode*/
  
  *TIMER1_CCMR1_REG &= ~(0xFF<<0);
  *TIMER1_CCMR1_REG |= (1<<0);//CC1 channel is configured as input, IC1 is mapped on TI1
  // *TIMER1_CCMR1_REG &= ~(0xF<<4);
  // //*TIMER1_CCMR1_REG |= (0x3<<4);//filter by write IC1F
   *TIMER1_CCMR1_REG &= ~((1 << 2 ) | (1 <<3));//no prescaler, capture is done each time an edge is detected on the capture input

  // *TIMER1_CCER_REG &= ~(0x3<<0);
  *TIMER1_CCER_REG |= (1<<1) | (1<<0);//enanble capture and capture is done when falling/rising edge IC1 state_edge
  
  //*TIMER1_SR_REG &= ~((1 << 1) | (1 << 0));

  *TIMER1_CR1_REG |= (1<<0) | (1<<7);// Counter enable and TIMx_ARR register is buffered

  //*NVIC_ISER0 |= (1 << 25);//interrupt enable timer1
  *NVIC_ISER0 |= (1 << 27);
  *TIMER1_DIER_REG |= (1<<1) | (1 << 0 );//enable capture interrupt

}


void TIM1_CC_IRQHandler(void){
  
  // stage = *TIMER1_CCR1_REG - stage;
  // if(stage < 0){
  //   stage = (0xFFFF -(0- stage));
  // }
  // if(count_bits == 2){
  //   state_edge = 1;
  // }
  //count_bits++;
  // if(stage >= 8500 && stage <= 15000){ // start and swap bit
  //   HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_SET);
  //   state_edge = 1;
  //  count_bits = 0;
  // }
  
  // if(state_edge == 1 ){
  //   if(stage > 900 &&  stage < 1500){
  //     element |= ~(1 << (31 - (countt)));
  //     countt++;
  //   }
  // }
  // count_bits++;
  //   if(stage > 1500 &&  stage < 3000){
  //     element |= 1 << (31 - (countt));
  //     countt++;
  //   }
  // }
  // if(countt == 32){
  //   buffer[buffer_index] = element;
  //   buffer_index = (buffer_index + 1) % 20;
  //   countt = 0;
  //   capture_value1 = 0;
  //   capture_value2 = 0;
  //   stage = 0;
  //   element = 0;
  //   state_edge = 0;
  //   count_bits = 0;
  // }
  if((count_bits%2)==0){
    capture_value1 = *TIMER1_CCR1_REG;    
  }
  if((count_bits%2) == 1){
    capture_value2 = *TIMER1_CCR1_REG;
  }
  if(capture_value1!=0 && capture_value2 != 0){  
    if((count_bits%2) == 1){
      if (capture_value2 >= capture_value1) {
        stage = capture_value2 - capture_value1;
      } else {
        stage = (0xFFFF - capture_value1 + capture_value2);
      }
      stage = stage ;
    }
    if((count_bits%2) == 0){
      if (capture_value1 >= capture_value2) {
        stage = capture_value1 - capture_value2;
      } else {
        stage = (0xFFFF - capture_value2 + capture_value1);
      }
      stage = stage ;
    }
  }
  // if(count_bits == 1){
  //   state_edge = 1;
  // }
  // if(count_bits == 2){
  //   state_edge = 0;
  // }
  
  if(stage >= 13000 && stage <= 14000){
    flag = 1;
  }
  if(flag ==1){
    if(stage >= 900 && stage <= 1400){
      element &= ~(1 << (31-countt));
      countt++;
    }
    if(stage >=  1700 && stage <= 2900 ){
      element |= 1 << (31-countt);
      countt++;
    }
    // else{
    //   countt = 0;
    //   element = 0;
    // }
  }
  count_bits++;
  if(countt == 32){
    buffer[buffer_index] = element;
    buffer_index = (buffer_index + 1) % 20;
    countt = 0;
    capture_value1 = 0;
    capture_value2 = 0;
    stage = 0;
    element = 0;
    count_bits = 0;
    flag = 0;
  }
  
    // if((*TIMER1_CCER_REG >> 1) & 1) {
    //   capture_value2 = *TIMER1_CCR1_REG;
    //   *TIMER1_CCER_REG |= ~(1<<1);
      
    // } 
  // *TIMER1_SR_REG &= ~(1 << 1);
  // }
}

void DELAY(uint32_t time){
  //*TIMER_CNT_REG = 0 ;
  uint32_t count = *TIMER1_CNT_REG;
   //*TIMER_CNT_REG =0;
  while((*TIMER1_CNT_REG-count)  < time);//- count
  //*TIMER_CNT_REG = 0 ;
}



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
  Custome_Timer();
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  /* USER CODE BEGIN 2 */
  
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
 /*---------Capture SFH05----------*/
  // HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,0); // Trigger for SRF sensor 
  // DELAY(10);
  // //HAL_Delay(1000);
  // HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,1);
  // //HAL_Delay(1000);
  // DELAY(10);
  // HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,0);
  // //Check_capture_value();
  // if(capture_value1 != 0 && capture_value2 != 0){
  //   if((capture_value2-capture_value1) >= 0){
  //     stage = (0.0343 * (capture_value2 - capture_value1))/2;
  //   }
  //   else{
  //     stage = (0.0343 * (capture_value2 +(65535 - capture_value1)))/2;
  //   }
  //   capture_value1 = 0 ;
  //   capture_value2 = 0 ;
  // }
  // HAL_Delay(250);

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
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
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
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);

  /*Configure GPIO pin : PC13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PA0 */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PA8 */
  GPIO_InitStruct.Pin = GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
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
