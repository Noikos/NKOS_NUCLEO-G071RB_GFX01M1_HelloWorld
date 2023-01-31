/**
  ******************************************************************************
  * File Name          : app_display.h
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef APP_DISPLAY_H
#define APP_DISPLAY_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
/* Exported types ------------------------------------------------------------*/
 typedef struct image_s
 {
   uint32_t  Width;
   uint32_t  Height;
   uint8_t   bpp;
   uint8_t*  Data;
 } image_t;

 typedef struct orientation_s
 {
   uint32_t  lcd;
   uint32_t  key;
 } orientation_t;
 /* Exported define ------------------------------------------------------------*/
 /* USER CODE BEGIN PD */
#define STEP_LINES                  1
#define USE_MEM_DMA                 1
#define USE_LCD_DMA                 1
#define BUTTON_USER_PRESSED_STATE   GPIO_PIN_RESET
 /* USER CODE END PD */
 /* Exported macro -------------------------------------------------------------*/
 /* USER CODE BEGIN PM */
#define MIN(w,h)            (w < h ? w : h)
#if (USE_MEM_DMA == 1)
#define MEM_READ_DATA       BSP_MEM_ReadDataDMA
#else
#define MEM_READ_DATA       BSP_MEM_ReadData
#endif
 ///* USER CODE END PM */
#define GFX01M1_DELAY 		HAL_Delay(3) //@Phu add to wait for SPI complete
/* Exported variables ---------------------------------------------------------*/
/* USER CODE BEGIN ET */


 extern uint8_t CacheBuffer[];
 extern uint16_t posy0;
 extern uint16_t posx;
 extern uint16_t posy;
 extern uint8_t key;
 extern uint8_t image_id;
 extern uint32_t LCD_Width;
 extern uint32_t LCD_Height;
 extern uint32_t LCD_Orientation;
 extern uint8_t orientation_id;

 extern image_t Images[];

 extern const orientation_t orientations[] ;
/* USER CODE END ET */


/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void MX_DISPLAY_Init(void);
void MX_DISPLAY_Process(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

#ifdef __cplusplus
}
#endif
#endif /* APP_DISPLAY_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
