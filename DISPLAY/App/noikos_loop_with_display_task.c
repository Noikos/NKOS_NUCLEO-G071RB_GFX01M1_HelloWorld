/**
 ********************************************************************************
 * @file    noikos_loop_with_display_task.c
 * @author  Phu Tran
 * @date    Jan 29, 2023
 * @brief
 ********************************************************************************
 */

/************************************
 * INCLUDES
 ************************************/
#include "noikos_loop_with_display_task.h"

#include "app_display.h"
#include "main.h"

/* Private includes ----------------------------------------------------------*/
#include "key_io.h"
#include "mem_io.h"
#include "lcd_io.h"
/* USER CODE BEGIN Includes */
#include "string.h"
#include "Image1.h"
#include "Image2.h"
#include "Image3.h"
#include "Image4.h"
#include "Image5.h"
#include "Image6.h"
/* USER CODE END Includes */
/************************************
 * PRIVATE MACROS AND DEFINES
 ************************************/
/* USER CODE BEGIN PD */
#define STEP_LINES                  1
#define USE_MEM_DMA                 1
#define USE_LCD_DMA                 1
#define BUTTON_USER_PRESSED_STATE   GPIO_PIN_RESET
/* USER CODE END PD */
/* USER CODE BEGIN PM */
#define MIN(w,h)            (w < h ? w : h)
#if (USE_MEM_DMA == 1)
#define MEM_READ_DATA       BSP_MEM_ReadDataDMA
#else
#define MEM_READ_DATA       BSP_MEM_ReadData
#endif
/* USER CODE END PM */
#define GFX01M1_DELAY 		HAL_Delay(3) //@Phu add to wait for SPI complete
/************************************
 * PRIVATE TYPEDEFS
 ************************************/
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
/* USER CODE END PTD */
 /************************************
 * INCLUDED EXTERN VARIABLES DEFINITION
 ************************************/

/************************************
 * STATIC VARIABLES
 ************************************/
/* USER CODE BEGIN PV */
uint8_t __IO TransferAllowed = 0;
static uint8_t CacheBuffer[(320*2*BUFFER_CACHE_LINES)];
static uint16_t posy0 = 0;
static uint16_t posx = 0;
static uint16_t posy = 0;
static uint8_t key = 1;
static uint8_t image_id = 0;
static uint32_t LCD_Width = 0;
static uint32_t LCD_Height = 0;
static uint32_t LCD_Orientation = 0;
static uint8_t orientation_id = 0;

static image_t Images[] = { { 240, 240, 2, (uint8_t *)Image1 }
                          , { 240, 320, 2, (uint8_t *)Image2 }
                          , { 320, 240, 2, (uint8_t *)Image3 }
                          , { 240, 240, 2, (uint8_t *)Image4 }
                          , { 240, 320, 2, (uint8_t *)Image5 }
                          , { 240, 240, 2, (uint8_t *)Image6 }
                          , {0, 0, 0, 0} };

static const orientation_t orientations[] = { { LCD_ORIENTATION_PORTRAIT, KEY_ORIENTATION_PORTRAIT }
                                            , { LCD_ORIENTATION_LANDSCAPE, KEY_ORIENTATION_LANDSCAPE }
                                            , { LCD_ORIENTATION_PORTRAIT_ROT180, KEY_ORIENTATION_PORTRAIT_ROT180 }
                                            , { LCD_ORIENTATION_LANDSCAPE_ROT180, KEY_ORIENTATION_LANDSCAPE_ROT180 }} ;
/* USER CODE END PV */
/************************************
 * GLOBAL VARIABLES
 ************************************/

/************************************
 * STATIC FUNCTION PROTOTYPES
 ************************************/

/************************************
 * STATIC FUNCTIONS
 ************************************/

/************************************
 * GLOBAL FUNCTIONS
 ************************************/

/*!
 * @brief Identify the larger of two 8-bit integers.
 *
 * @param[in] none.
 *
 * @return delay time to wait until next action.
 *
 * This function must be registered as loop_with_delay_task .
 */
unsigned int noikos_loop_with_delay_task_display_process(void)
{
    return 0;
}

/*** end of file ***/
