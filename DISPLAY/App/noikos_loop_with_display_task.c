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

#include "stm32g0xx_it.h"

#include <stdbool.h>
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
//#define STEP_LINES                  1
//#define USE_MEM_DMA                 1
//#define USE_LCD_DMA                 1
//#define BUTTON_USER_PRESSED_STATE   GPIO_PIN_RESET
/* USER CODE END PD */
/* USER CODE BEGIN PM */
//#define MIN(w,h)            (w < h ? w : h)
//#if (USE_MEM_DMA == 1)
//#define MEM_READ_DATA       BSP_MEM_ReadDataDMA
//#else
//#define MEM_READ_DATA       BSP_MEM_ReadData
//#endif
/* USER CODE END PM */
//#define GFX01M1_DELAY 		HAL_Delay(3) //@Phu add to wait for SPI complete
/************************************
 * PRIVATE TYPEDEFS
 ************************************/
//typedef struct image_s
//{
//  uint32_t  Width;
//  uint32_t  Height;
//  uint8_t   bpp;
//  uint8_t*  Data;
//} image_t;
//
//typedef struct orientation_s
//{
//  uint32_t  lcd;
//  uint32_t  key;
//} orientation_t;
/* USER CODE END PTD */

/* PHU CODE BEGIN PTD */
typedef enum task_display_process_state_enum {
	TASK_DISPLAY_PROCESS_STATE_GPIO_GENERIC,
	TASK_DISPLAY_PROCESS_STATE_KEY_PRESSED,
	TASK_DISPLAY_PROCESS_STATE_KEY_PRESSED_LCD_CLEAR0,
	TASK_DISPLAY_PROCESS_STATE_KEY_PRESSED_LCD_CLEAR1,
	TASK_DISPLAY_PROCESS_STATE_KEY_PRESSED_LCD_CLEAR2,
	TASK_DISPLAY_PROCESS_STATE_KEY_PRESSED_DISPLAY,
	TASK_DISPLAY_PROCESS_STATE_KEY_UP_LCD_CLEAR,
	TASK_DISPLAY_PROCESS_STATE_KEY_DOWN_LCD_CLEAR,
	TASK_DISPLAY_PROCESS_STATE_KEY_HANDLE,
	TASK_DISPLAY_PROCESS_STATE_BUTTON_USER_PRESSED_LCD_CLEAR,
	TASK_DISPLAY_PROCESS_STATE_BUTTON_USER_PRESSED_AFTER_LCD_CLEAR
}task_display_process_state_e;
/* PHU CODE END PTD */
 /************************************
 * INCLUDED EXTERN VARIABLES DEFINITION
 ************************************/

/************************************
 * STATIC VARIABLES
 ************************************/
/* USER CODE BEGIN PV */
//static uint8_t __IO TransferAllowed = 0;
//static uint8_t CacheBuffer[(320*2*BUFFER_CACHE_LINES)];
//static uint16_t posy0 = 0;
//static uint16_t posx = 0;
//static uint16_t posy = 0;
//static uint8_t key = 1;
//static uint8_t image_id = 0;
//static uint32_t LCD_Width = 0;
//static uint32_t LCD_Height = 0;
//static uint32_t LCD_Orientation = 0;
//static uint8_t orientation_id = 0;

//static image_t Images[] = { { 240, 240, 2, (uint8_t *)Image1 }
//                          , { 240, 320, 2, (uint8_t *)Image2 }
//                          , { 320, 240, 2, (uint8_t *)Image3 }
//                          , { 240, 240, 2, (uint8_t *)Image4 }
//                          , { 240, 320, 2, (uint8_t *)Image5 }
//                          , { 240, 240, 2, (uint8_t *)Image6 }
//                          , {0, 0, 0, 0} };
//
//static const orientation_t orientations[] = { { LCD_ORIENTATION_PORTRAIT, KEY_ORIENTATION_PORTRAIT }
//                                            , { LCD_ORIENTATION_LANDSCAPE, KEY_ORIENTATION_LANDSCAPE }
//                                            , { LCD_ORIENTATION_PORTRAIT_ROT180, KEY_ORIENTATION_PORTRAIT_ROT180 }
//                                            , { LCD_ORIENTATION_LANDSCAPE_ROT180, KEY_ORIENTATION_LANDSCAPE_ROT180 }} ;

static uint32_t LCD_Clear_Instance;
static uint32_t LCD_Clear_Xpos;
static uint32_t LCD_Clear_Ypos;
static uint32_t LCD_Clear_Width;
static uint32_t LCD_Clear_Height;
static bool LCD_Clear_Finish;

static image_t *display_img_image;
static uint16_t display_img_posx;
static uint16_t display_img_posy;
static bool display_finish;
/* USER CODE END PV */
/************************************
 * GLOBAL VARIABLES
 ************************************/

/************************************
 * STATIC FUNCTION PROTOTYPES
 ************************************/
static uint32_t BSP_LCD_Clear();
static uint32_t Display_Image();
/************************************
 * STATIC FUNCTIONS
 ************************************/
static uint32_t BSP_LCD_Clear()
{
  static volatile uint8_t state = 0;

  static uint32_t size;
  static uint32_t CacheLinesCnt, CacheLinesSz;
  static uint32_t offset , line_cnt;



  while(1)
  {
	switch(state) {
	case 0:
		offset = 0;
		line_cnt = LCD_Clear_Ypos;
		LCD_Clear_Finish = false;

		size = (2*LCD_Clear_Width*LCD_Clear_Height);
		CacheLinesCnt = (LCD_Clear_Height > BUFFER_CACHE_LINES ? BUFFER_CACHE_LINES : LCD_Clear_Height);
		CacheLinesSz = (2*LCD_Clear_Width*CacheLinesCnt);
		memset(CacheBuffer, 0, CacheLinesSz);
		state = 1;
	case 1:
		if(BSP_LCD_FillRGBRect(LCD_Clear_Instance, USE_LCD_DMA, CacheBuffer, LCD_Clear_Xpos, line_cnt, LCD_Clear_Width, CacheLinesCnt) != BSP_ERROR_NONE)
		{
		  Error_Handler();
		}

		//GFX01M1_DELAY;
		//break;
		printf("\r\n LCD_CLEAR move to state=%d \r\n",state);
		state = 2;
		return 5;
	case 2:
		BSP_LCD_WaitForTransferToBeDone(0);
		line_cnt += CacheLinesCnt;
		offset += CacheLinesSz;
		/* Check remaining data size */
		if(offset == size)
		{
			state = 0;
			return 0;
		}
		else if((offset + CacheLinesSz) > size)
		{
		  /* Transfer last block and exit */
		  CacheLinesCnt = ((size - offset)/ (2*LCD_Clear_Width));
		  if(BSP_LCD_FillRGBRect(LCD_Clear_Instance, USE_LCD_DMA, CacheBuffer, LCD_Clear_Xpos, line_cnt, LCD_Clear_Width, CacheLinesCnt) != BSP_ERROR_NONE)
		  {
			Error_Handler();

		  }
//		  GFX01M1_DELAY;
//		  BSP_LCD_WaitForTransferToBeDone(0);
//		  break;
		  printf("\r\n LCD_CLEAR move to state=%d \r\n",state);
		  state = 3;
		  return 5;
		}
		state = 1;
		break;
	case 3:
		BSP_LCD_WaitForTransferToBeDone(0);
		LCD_Clear_Finish = true;
		state = 0;
		return 0;
	}
  }
}

/**
  * @brief  Refresh the display.
  * @param  None.
  * @retval None.
  */
static uint32_t Display_Image()
{
	enum {
		DISPLAY_IMAGE_STATE_BEGIN,
		DISPLAY_IMAGE_STATE_SEND_NFRAME_REACH_FLASH_BASE,
		DISPLAY_IMAGE_STATE_SEND_NFRAME_REACH_FLASH_BASE1,
		DISPLAY_IMAGE_STATE_SEND_FRAME,
		DISPLAY_IMAGE_STATE_SEND_FRAME_SIZE_LESS_CACHELINESSZ,
		DISPLAY_IMAGE_STATE_SEND_FRAME_SIZE_LESS_CACHELINESSZ1,
		DISPLAY_IMAGE_STATE_SEND_FRAME_SIZE_LESS_CACHELINESSZ2,
		DISPLAY_IMAGE_STATE_SEND_FRAME_SIZE_NOT_LESS_CACHELINESSZ,
		DISPLAY_IMAGE_STATE_SEND_FRAME_SIZE_NOT_LESS_CACHELINESSZ1,
		DISPLAY_IMAGE_STATE_SEND_FRAME_SIZE_NOT_LESS_CACHELINESSZ2,
		DISPLAY_IMAGE_STATE_SEND_FRAME_SIZE_NOT_LESS_CACHELINESSZ3,
		DISPLAY_IMAGE_STATE_SEND_FRAME_SIZE_NOT_LESS_CACHELINESSZ4,
		DISPLAY_IMAGE_STATE_SEND_FRAME_SIZE_NOT_LESS_CACHELINESSZ5
	};
	static volatile uint8_t state = 0;
    static uint8_t *pData;
    static uint32_t Height;

    static uint32_t size;
    static uint32_t CacheLinesSz;
    static uint32_t CacheLinesCnt;
    static uint32_t line_cnt;
    static uint32_t offset;

    uint32_t sleep;

    switch(state){
    case DISPLAY_IMAGE_STATE_BEGIN:
    	pData = (uint8_t *)display_img_image->Data;
    	Height = display_img_image->Height;

    	display_finish = false;

    	if(display_img_image->Height > LCD_Height)
		{
		  pData += (display_img_image->bpp * display_img_image->Width * ((display_img_image->Height - LCD_Height)/2));
		  Height = LCD_Height;
		}

    	// Send the frambuffer n times
		if(((uint32_t )pData & 0xFF000000) == FLASH_BASE)
		{
			state = DISPLAY_IMAGE_STATE_SEND_NFRAME_REACH_FLASH_BASE;
			break;
		}
		else
		{
			state = DISPLAY_IMAGE_STATE_SEND_FRAME;
			break;
		}
    case DISPLAY_IMAGE_STATE_SEND_NFRAME_REACH_FLASH_BASE:
    	if(BSP_LCD_FillRGBRect(0, USE_LCD_DMA, pData, display_img_posx, display_img_posy, display_img_image->Width, Height) != BSP_ERROR_NONE)
		{
    		Error_Handler();
		}
		//		  GFX01M1_DELAY;
		//		  BSP_LCD_WaitForTransferToBeDone(0);
		printf("\r\n Display_Image move to state=%d \r\n",state);
		state = DISPLAY_IMAGE_STATE_SEND_NFRAME_REACH_FLASH_BASE1;
		sleep = 5;
		break;
    case DISPLAY_IMAGE_STATE_SEND_NFRAME_REACH_FLASH_BASE1:
    	BSP_LCD_WaitForTransferToBeDone(0);
    	state = DISPLAY_IMAGE_STATE_BEGIN;
    	return 0;
    case DISPLAY_IMAGE_STATE_SEND_FRAME:
    	size = (display_img_image->bpp*display_img_image->Width*Height);
		CacheLinesSz = (2*MIN(LCD_Width,LCD_Height)*BUFFER_CACHE_LINES);
		CacheLinesCnt = (CacheLinesSz/(display_img_image->bpp*display_img_image->Width));
		line_cnt = 0;
		offset = 0;

		/* One block read */
		if(size < CacheLinesSz)
		{
			state = DISPLAY_IMAGE_STATE_SEND_FRAME_SIZE_LESS_CACHELINESSZ;
			sleep = 0;
		}
		else
		{
			state = DISPLAY_IMAGE_STATE_SEND_FRAME_SIZE_NOT_LESS_CACHELINESSZ;
		}
		break;
    case DISPLAY_IMAGE_STATE_SEND_FRAME_SIZE_LESS_CACHELINESSZ:
    	if(MEM_READ_DATA(0, CacheBuffer, (uint32_t )pData, size) != BSP_ERROR_NONE)
		{
		  Error_Handler();
		}
		//			GFX01M1_DELAY;
		//			BSP_MEM_WaitForTransferToBeDone(0);
		//			if(BSP_LCD_FillRGBRect(0, USE_LCD_DMA, CacheBuffer, display_img_posx, display_img_posy, display_img_image->Width, Height) != BSP_ERROR_NONE)
		//			{
		//			  Error_Handler();
		//			}
		//			GFX01M1_DELAY;
		//			BSP_LCD_WaitForTransferToBeDone(0);
		printf("\r\n Display_Image move to state=%d \r\n",state);
		state = DISPLAY_IMAGE_STATE_SEND_FRAME_SIZE_LESS_CACHELINESSZ1;
		sleep = 5;
    	break;

    case DISPLAY_IMAGE_STATE_SEND_FRAME_SIZE_LESS_CACHELINESSZ1:
		BSP_MEM_WaitForTransferToBeDone(0);
		if(BSP_LCD_FillRGBRect(0, USE_LCD_DMA, CacheBuffer, display_img_posx, display_img_posy, display_img_image->Width, Height) != BSP_ERROR_NONE)
		{
		  Error_Handler();
		}

		//GFX01M1_DELAY;
		//BSP_LCD_WaitForTransferToBeDone(0);
		printf("\r\n Display_Image move to state=%d \r\n",state);
		state = DISPLAY_IMAGE_STATE_SEND_FRAME_SIZE_LESS_CACHELINESSZ2;
		sleep = 5;
		break;
    case DISPLAY_IMAGE_STATE_SEND_FRAME_SIZE_LESS_CACHELINESSZ2:
    	BSP_LCD_WaitForTransferToBeDone(0);

    	display_finish = true;
		state = DISPLAY_IMAGE_STATE_BEGIN;
		sleep = 0;
		break;
    case DISPLAY_IMAGE_STATE_SEND_FRAME_SIZE_NOT_LESS_CACHELINESSZ:
		state = DISPLAY_IMAGE_STATE_SEND_FRAME_SIZE_NOT_LESS_CACHELINESSZ1;
		sleep = 0;
		break;
    case DISPLAY_IMAGE_STATE_SEND_FRAME_SIZE_NOT_LESS_CACHELINESSZ1:
    	/* Multi-block read/write */
		if(MEM_READ_DATA(0, CacheBuffer, (uint32_t )(pData+offset), CacheLinesSz) != BSP_ERROR_NONE)
		{
		Error_Handler();
		}
		state = DISPLAY_IMAGE_STATE_SEND_FRAME_SIZE_NOT_LESS_CACHELINESSZ2;
		sleep = 5;
		break;
    case DISPLAY_IMAGE_STATE_SEND_FRAME_SIZE_NOT_LESS_CACHELINESSZ2:
		BSP_MEM_WaitForTransferToBeDone(0);
		if(BSP_LCD_FillRGBRect(0, USE_LCD_DMA, CacheBuffer, display_img_posx, display_img_posy+line_cnt, display_img_image->Width, CacheLinesCnt) != BSP_ERROR_NONE)
		{
			Error_Handler();
		}
		printf("\r\n Display_Image move to state=%d \r\n",state);
		state = DISPLAY_IMAGE_STATE_SEND_FRAME_SIZE_NOT_LESS_CACHELINESSZ3;
		sleep = 5;
		break;
    case DISPLAY_IMAGE_STATE_SEND_FRAME_SIZE_NOT_LESS_CACHELINESSZ3:
    	BSP_LCD_WaitForTransferToBeDone(0);
		line_cnt += CacheLinesCnt;
		offset += CacheLinesSz;
		/* Check remaining data size */
		if(offset == size)
		{
			/* last block transfer was done, so exit */

			display_finish = true;
			state = DISPLAY_IMAGE_STATE_BEGIN;
			sleep = 0;
			break;
		}
		else if((offset + CacheLinesSz) > size)
		{
			/* Transfer last block and exit */
			if(MEM_READ_DATA(0, CacheBuffer, (uint32_t )(pData+offset), (size - offset)) != BSP_ERROR_NONE)
			{
			  Error_Handler();
			}
			printf("\r\n Display_Image move to state=%d \r\n",state);
			state = DISPLAY_IMAGE_STATE_SEND_FRAME_SIZE_NOT_LESS_CACHELINESSZ4;
			sleep = 5;
			break;
		}
		/*Loop back to DISPLAY_IMAGE_STATE_SEND_FRAME_SIZE_NOT_LESS_CACHELINESSZ*/
		state = DISPLAY_IMAGE_STATE_SEND_FRAME_SIZE_NOT_LESS_CACHELINESSZ;
		sleep = 0;
		break;
    case DISPLAY_IMAGE_STATE_SEND_FRAME_SIZE_NOT_LESS_CACHELINESSZ4:
		BSP_MEM_WaitForTransferToBeDone(0);
		CacheLinesCnt = ((size - offset)/ (display_img_image->bpp*display_img_image->Width));
		if(BSP_LCD_FillRGBRect(0, USE_LCD_DMA, CacheBuffer, display_img_posx, display_img_posy+line_cnt, display_img_image->Width, CacheLinesCnt) != BSP_ERROR_NONE)
		{
		  Error_Handler();
		}
		printf("\r\n Display_Image move to state=%d \r\n",state);
		state = DISPLAY_IMAGE_STATE_SEND_FRAME_SIZE_NOT_LESS_CACHELINESSZ5;
		sleep = 5;
		break;
    case DISPLAY_IMAGE_STATE_SEND_FRAME_SIZE_NOT_LESS_CACHELINESSZ5:
		BSP_LCD_WaitForTransferToBeDone(0);
		display_finish = true;
		state = DISPLAY_IMAGE_STATE_BEGIN;
		sleep = 0;
		break;
    default:
    	printf("\r\n Display_Image: Error state=%d ",state);
    }
    return sleep;
}
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
uint32_t noikos_loop_with_delay_task_display_process(void)
{
	/* USER CODE BEGIN MX_DISPLAY_Process */
	int i;
	static __IO uint8_t can_move = 0;
	static __IO task_display_process_state_e display_state = TASK_DISPLAY_PROCESS_STATE_KEY_PRESSED;//Because @key is 1 at initialization phase, so it must be at this state//TASK_DISPLAY_PROCESS_STATE_GPIO_GENERIC;

	uint32_t sleep_time;
	/* Wait for TE */
	if (TransferAllowed)
	{
		switch(display_state)
		{
		case TASK_DISPLAY_PROCESS_STATE_GPIO_GENERIC:
			if(BSP_KEY_GetState(0, &key) == BSP_ERROR_NONE)
			{
			  switch(key)
			  {
				case BSP_KEY_CENTER:
				  if(posy != posy0)
				  {
					posy = posy0;
					//HAL_Delay(200);
					printf("\r\n task_display_process move to state=%d \r\n",display_state);
					display_state = TASK_DISPLAY_PROCESS_STATE_KEY_PRESSED;
					return 200;
				  }
				  else
				  {
					/* Ignore this key */
					key = 0;
				  }
				  break;
				case BSP_KEY_UP:
				  if (can_move && (posy > STEP_LINES))
				  {
//					  BSP_LCD_Clear(0, 0, (posy + Images[image_id].Height), LCD_Width, STEP_LINES);//ToDo: conversion
//					  posy -= STEP_LINES;
					  LCD_Clear_Instance = 0;
						LCD_Clear_Xpos = 0;
						LCD_Clear_Ypos = (posy + Images[image_id].Height);
						LCD_Clear_Width = LCD_Width;
						LCD_Clear_Height = STEP_LINES;
					  display_state = TASK_DISPLAY_PROCESS_STATE_KEY_UP_LCD_CLEAR;
					  printf("\r\n task_display_process move to state=%d \r\n",display_state);
				  }
				  else
				  {
					/* Ignore this key */
					key = 0;
				  }
				  break;
				case BSP_KEY_DOWN:
				  if (can_move && (posy <(LCD_Height-Images[image_id].Height-STEP_LINES)))
				  {
//					BSP_LCD_Clear(0, 0, posy, LCD_Width, STEP_LINES);//ToDo: conversion
//					posy += STEP_LINES;
					  LCD_Clear_Instance = 0;
						LCD_Clear_Xpos = 0;
						LCD_Clear_Ypos = posy;
						LCD_Clear_Width = LCD_Width;
						LCD_Clear_Height = STEP_LINES;
					  display_state = TASK_DISPLAY_PROCESS_STATE_KEY_DOWN_LCD_CLEAR;
					  printf("\r\n task_display_process move to state=%d \r\n",display_state);
				  }
				  else
				  {
					/* Ignore this key */
					key = 0;
				  }
				  break;
				case BSP_KEY_LEFT:
				  i = image_id-1;
				  while ((i >= 0) && (Images[i].Width > LCD_Width)) { i--; }
				  if (image_id > 0)
				  {
					can_move = 0;
					image_id = i;
					posy = 0;
					//HAL_Delay(200);
					display_state = TASK_DISPLAY_PROCESS_STATE_KEY_PRESSED;
					printf("\r\n task_display_process move to state=%d \r\n",display_state);
					return 200;
				  }
				  else
				  {
					/* Ignore this key */
					key = 0;
				  }
				  break;
				case BSP_KEY_RIGHT:
				  i = image_id+1;
				  while ((Images[i].Height > 0) && (Images[i].Width > LCD_Width)) { i++; }
				  if (Images[image_id+1].Height > 0)
				  {
					can_move = 0;
					image_id = i;
					posy = 0;
					//HAL_Delay(200);
					display_state = TASK_DISPLAY_PROCESS_STATE_KEY_PRESSED;
					printf("\r\n task_display_process move to state=%d \r\n",display_state);
					return 200;
				  }
				  else
				  {
					/* Ignore this key */
					key = 0;
				  }
				  break;
				default:
				  break;
			  }
			}
			else if (HAL_GPIO_ReadPin(BUTTON_USER_GPIO_Port, BUTTON_USER_Pin) == BUTTON_USER_PRESSED_STATE)
			{
			  /* Read User Button */
			  orientation_id++;
			  if(orientation_id == 4)
			  {
				orientation_id = 0;
			  }
			  LCD_Clear_Instance = 0;
				LCD_Clear_Xpos = 0;
				LCD_Clear_Ypos = 0;
				LCD_Clear_Width = LCD_Width;
				LCD_Clear_Height = LCD_Height;
			  display_state = TASK_DISPLAY_PROCESS_STATE_BUTTON_USER_PRESSED_LCD_CLEAR;
			  printf("\r\n task_display_process move to state=%d \r\n",display_state);
			  break;
//			  BSP_LCD_Clear(0, 0, 0, LCD_Width, LCD_Height);//ToDo: conversion
//			  if(BSP_LCD_SetOrientation(0, orientations[orientation_id].lcd) != BSP_ERROR_NONE)
//			  {
//				Error_Handler();
//			  }
//			  if(BSP_KEY_SetOrientation(0, orientations[orientation_id].key) != BSP_ERROR_NONE)
//			  {
//				Error_Handler();
//			  }
//			  if((BSP_LCD_GetXSize(0, &LCD_Width) != BSP_ERROR_NONE) \
//			  || (BSP_LCD_GetYSize(0, &LCD_Height) != BSP_ERROR_NONE) \
//			  || (BSP_LCD_GetOrientation(0, &LCD_Orientation) != BSP_ERROR_NONE) )
//			  {
//				Error_Handler();
//			  }
//			  i = image_id;
//			  while ((i >= 0) && (Images[i].Width > LCD_Width)) { i--; }
//			  while ((Images[i].Height > 0) && (Images[i].Width > LCD_Width)) { i++; }
//			  image_id = i;
//			  key = 255;
//			  posy = 0;
//			  can_move = 0;
//			  //HAL_Delay(200);
//			  display_state = TASK_DISPLAY_PROCESS_STATE_KEY_PRESSED;
//			  return 200;
			}
			//break;
			return 0;
		case TASK_DISPLAY_PROCESS_STATE_KEY_PRESSED:
			/* Display something */
			if(key)
			{
			  /* Reset key value */
			  key = 0;

			  /* Check if we can allow scrolling up/down the picture */
			  if ((can_move == 0) && (Images[image_id].Height < LCD_Height) \
			  && ((LCD_Orientation == LCD_ORIENTATION_PORTRAIT) || (LCD_Orientation == KEY_ORIENTATION_PORTRAIT_ROT180)))
			  {
				/* Allow moving the picture on screen */
				can_move = 1;
			  }
			  if(posy == 0) /* reload new image */
			  {
//				BSP_LCD_Clear(0, 0, 0, LCD_Width, LCD_Height);
//				posx = ((LCD_Width - Images[image_id].Width)/2);
//				if(Images[image_id].Height < LCD_Height)
//				{
//				  posy = ((LCD_Height - Images[image_id].Height)/2);
//				}
//				else
//				{
//				  posy = 0;
//				}
//				posy0 = posy;
				  LCD_Clear_Instance = 0;
				  LCD_Clear_Xpos = 0;
				  LCD_Clear_Ypos = 0;
				  LCD_Clear_Width = LCD_Width;
				  LCD_Clear_Height = LCD_Height;
				  display_state = TASK_DISPLAY_PROCESS_STATE_KEY_PRESSED_LCD_CLEAR0;
				  printf("\r\n task_display_process move to state=%d \r\n",display_state);
				  break;
			  }
			  else if(posy == posy0) /* center current image */
			  {
				posy = ((LCD_Height - Images[image_id].Height)/2);
				LCD_Clear_Instance = 0;
				LCD_Clear_Xpos = 0;
				LCD_Clear_Ypos = 0;
				LCD_Clear_Width = LCD_Width;
				LCD_Clear_Height = posy;
				display_state = TASK_DISPLAY_PROCESS_STATE_KEY_PRESSED_LCD_CLEAR1;
				printf("\r\n task_display_process move to state=%d \r\n",display_state);
				break;
//				BSP_LCD_Clear(0, 0, 0, LCD_Width, posy); //ToDo: conversion
//				BSP_LCD_Clear(0, 0, posy+Images[image_id].Height, LCD_Width, posy); //ToDo: conversion
			  }
//			  Display_Image(&Images[image_id], posx, posy);//ToDo: conversion
			  display_img_image = &Images[image_id];
				display_img_posx = posx;
				display_img_posy = posy;
				display_state = TASK_DISPLAY_PROCESS_STATE_KEY_PRESSED_DISPLAY;
				printf("\r\n task_display_process move to state=%d \r\n",display_state);
				break;
			}
			display_state = TASK_DISPLAY_PROCESS_STATE_GPIO_GENERIC;
			break;
		case TASK_DISPLAY_PROCESS_STATE_KEY_PRESSED_LCD_CLEAR0:
			sleep_time = BSP_LCD_Clear();
			if(LCD_Clear_Finish)
			{
				posx = ((LCD_Width - Images[image_id].Width)/2);
				if(Images[image_id].Height < LCD_Height)
				{
				  posy = ((LCD_Height - Images[image_id].Height)/2);
				}
				else
				{
				  posy = 0;
				}
				posy0 = posy;
				display_state = TASK_DISPLAY_PROCESS_STATE_KEY_PRESSED_DISPLAY;
				printf("\r\n task_display_process move to state=%d \r\n",display_state);
				break;
			}
			if(sleep_time > 0)
			{
				return sleep_time;
			}
			//break;
			return 0;
		case TASK_DISPLAY_PROCESS_STATE_KEY_PRESSED_LCD_CLEAR1:
			sleep_time = BSP_LCD_Clear();
			if(LCD_Clear_Finish)
			{
				LCD_Clear_Instance = 0;
				LCD_Clear_Xpos = 0;
				LCD_Clear_Ypos = posy+Images[image_id].Height;
				LCD_Clear_Width =LCD_Width;
				LCD_Clear_Height = posy;
				display_state = TASK_DISPLAY_PROCESS_STATE_KEY_PRESSED_LCD_CLEAR2;
				printf("\r\n task_display_process move to state=%d \r\n",display_state);
				break;
			}
			if(sleep_time > 0)
			{
				return sleep_time;
			}
			//break;
			return 0;
		case TASK_DISPLAY_PROCESS_STATE_KEY_PRESSED_LCD_CLEAR2:
			sleep_time = BSP_LCD_Clear();
			if(LCD_Clear_Finish)
			{
				display_img_image = &Images[image_id];
				display_img_posx = posx;
				display_img_posy = posy;
				display_state = TASK_DISPLAY_PROCESS_STATE_KEY_PRESSED_DISPLAY;
				printf("\r\n task_display_process move to state=%d \r\n",display_state);
				break;
			}
			if(sleep_time > 0)
			{
				return sleep_time;
			}
			//break;
			return 0;;
		case TASK_DISPLAY_PROCESS_STATE_KEY_PRESSED_DISPLAY:
			sleep_time = Display_Image();
			if(display_finish)
			{
				display_state = TASK_DISPLAY_PROCESS_STATE_GPIO_GENERIC;
				printf("\r\n task_display_process move to state=%d \r\n",display_state);
				break;
			}
			if(sleep_time > 0)
			{
				return sleep_time;
			}
			//break;
			return 0;
		case TASK_DISPLAY_PROCESS_STATE_KEY_UP_LCD_CLEAR:
			//BSP_LCD_Clear(0, 0, (posy + Images[image_id].Height), LCD_Width, STEP_LINES);//ToDo: conversion
			sleep_time = BSP_LCD_Clear();
			if(LCD_Clear_Finish)
			{
				posy -= STEP_LINES;
				display_state = TASK_DISPLAY_PROCESS_STATE_KEY_PRESSED;
				printf("\r\n task_display_process move to state=%d \r\n",display_state);
				break;
			}
			if(sleep_time > 0)
			{
				return sleep_time;
			}

			//break;
			return 0;
		case TASK_DISPLAY_PROCESS_STATE_KEY_DOWN_LCD_CLEAR:
//			BSP_LCD_Clear(0, 0, posy, LCD_Width, STEP_LINES);//ToDo: conversion
//			posy += STEP_LINES;
			sleep_time = BSP_LCD_Clear();
			if(LCD_Clear_Finish)
			{
				posy += STEP_LINES;
				display_state = TASK_DISPLAY_PROCESS_STATE_KEY_PRESSED;
				printf("\r\n task_display_process move to state=%d \r\n",display_state);
				break;
			}
			if(sleep_time > 0)
			{
				return sleep_time;
			}
			return 0;
		case TASK_DISPLAY_PROCESS_STATE_KEY_HANDLE:
			break;
		case TASK_DISPLAY_PROCESS_STATE_BUTTON_USER_PRESSED_LCD_CLEAR:
			sleep_time = BSP_LCD_Clear();
			if(LCD_Clear_Finish)
			{
				display_state = TASK_DISPLAY_PROCESS_STATE_BUTTON_USER_PRESSED_AFTER_LCD_CLEAR;
				printf("\r\n task_display_process move to state=%d \r\n",display_state);
				break;
			}
			if(sleep_time > 0)
			{
				return sleep_time;
			}
			return 0;
			//			  BSP_LCD_Clear(0, 0, 0, LCD_Width, LCD_Height);//ToDo: conversion
			//			  if(BSP_LCD_SetOrientation(0, orientations[orientation_id].lcd) != BSP_ERROR_NONE)
			//			  {
			//				Error_Handler();
			//			  }
			//			  if(BSP_KEY_SetOrientation(0, orientations[orientation_id].key) != BSP_ERROR_NONE)
			//			  {
			//				Error_Handler();
			//			  }
			//			  if((BSP_LCD_GetXSize(0, &LCD_Width) != BSP_ERROR_NONE) \
			//			  || (BSP_LCD_GetYSize(0, &LCD_Height) != BSP_ERROR_NONE) \
			//			  || (BSP_LCD_GetOrientation(0, &LCD_Orientation) != BSP_ERROR_NONE) )
			//			  {
			//				Error_Handler();
			//			  }
			//			  i = image_id;
			//			  while ((i >= 0) && (Images[i].Width > LCD_Width)) { i--; }
			//			  while ((Images[i].Height > 0) && (Images[i].Width > LCD_Width)) { i++; }
			//			  image_id = i;
			//			  key = 255;
			//			  posy = 0;
			//			  can_move = 0;
			//			  //HAL_Delay(200);
			//			  display_state = TASK_DISPLAY_PROCESS_STATE_KEY_PRESSED;
			//			  return 200;
		case TASK_DISPLAY_PROCESS_STATE_BUTTON_USER_PRESSED_AFTER_LCD_CLEAR:
			if(BSP_LCD_SetOrientation(0, orientations[orientation_id].lcd) != BSP_ERROR_NONE)
			{
				Error_Handler();
			}
			if(BSP_KEY_SetOrientation(0, orientations[orientation_id].key) != BSP_ERROR_NONE)
			{
				Error_Handler();
			}
			if((BSP_LCD_GetXSize(0, &LCD_Width) != BSP_ERROR_NONE) \
			|| (BSP_LCD_GetYSize(0, &LCD_Height) != BSP_ERROR_NONE) \
			|| (BSP_LCD_GetOrientation(0, &LCD_Orientation) != BSP_ERROR_NONE) )
			{
				Error_Handler();
			}
			i = image_id;
			while ((i >= 0) && (Images[i].Width > LCD_Width)) { i--; }
			while ((Images[i].Height > 0) && (Images[i].Width > LCD_Width)) { i++; }
			image_id = i;
			key = 255;
			posy = 0;
			can_move = 0;
			display_state = TASK_DISPLAY_PROCESS_STATE_KEY_PRESSED;
			printf("\r\n task_display_process move to state=%d \r\n",display_state);
			return 200;
		}
	}
	/* USER CODE END MX_DISPLAY_Process */
    return 0;
}

/*** end of file ***/
