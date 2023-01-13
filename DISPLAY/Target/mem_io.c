/**
  ******************************************************************************
  * File Name          : Target/mem_io.c
  * Description        : This file provides code for the configuration
  *                      of the External Memory Driver instances.
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

/* Includes ------------------------------------------------------------------*/
#include "mem_os.h"
#include "mem_io.h"

/** @addtogroup DISPLAY
  * @brief      DISPLAY Software Expansion Pack.
  * @{
  */
/** @defgroup External_Memory_Driver External Memory Driver
  * @brief    External Memory Driver API.
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Wrap Around value */
#define MEM_WRAP_AROUND_64BYTE                  MX25L6433F_WRAP_AROUND_64BYTE
/* Used Memory Commands */
#define MEM_READ_CMD                            MX25L6433F_READ_CMD
#define MEM_SET_BURST_LENGTH_CMD                MX25L6433F_SET_BURST_LENGTH_CMD

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

#define BSP_MEM_CHECK_PARAMS(Instance)

/* Exported variables --------------------------------------------------------*/
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

static void                 *MemCompObj;
static MEM_Drv_t            *MemDrv;
static MX25L6433F_IO_t      IOCtx;
static MX25L6433F_Object_t  ObjCtx;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

static int32_t MEM_Probe(uint32_t Instance);

/* Deprecated functions ------------------------------------------------------*/
__INLINE uint8_t BSP_MEM_GetTransfertStatus(uint32_t Instance)
{
  return BSP_MEM_GetTransferStatus(Instance);
}

__WEAK void BSP_MEM_SignalTransfertDone(uint32_t Instance, uint8_t Event)
{
  /* Prevent unused argument(s) compilation warning */;
  UNUSED(Event);
  BSP_MEM_SignalTransferDone(Instance);
}

/* Exported functions --------------------------------------------------------*/
/** @defgroup External_Memory_Exported_Functions External Memory Exported Functions
  * @brief    External Memory Drivers API.
  * @{
  */
/**
  * @brief  Initializes the External Memory.
  * @param  Instance:     External Memory Instance.
  * @retval int32_t:      BSP status.
  */
int32_t BSP_MEM_Init(uint32_t Instance)
{
  int32_t ret = BSP_ERROR_NONE;

  BSP_MEM_CHECK_PARAMS(Instance);

  /* MEM OS Initialize */
  if(MEM_OS_Initialize(0) == MEM_OS_ERROR_NONE)
  {
    /* Reset Handlers */
    MemCompObj = NULL;
    MemDrv = NULL;

    ret = MEM_Probe(Instance);
  }

  return ret;
}

/**
  * @brief  De-Initializes the External Memory resources
  * @param  Instance:     External Memory Instance
  * @retval int32_t:      BSP status.
  */
int32_t BSP_MEM_DeInit(uint32_t Instance)
{
  int32_t ret = BSP_ERROR_NONE;

  BSP_MEM_CHECK_PARAMS(Instance);

  if(MemDrv->DeInit != NULL)
  {
    if(MemDrv->DeInit(MemCompObj) < 0)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
  }
  else
  {
    ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }

  return ret;
}

/**
  * @brief  Erase block of the External Memory Interface.
  * @param  Instance:     External Memory Instance.
  * @param  BlockAddress: Memory block address to de erased.
  * @param  BlockSize:    Memory block size to be erased.
  * @retval int32_t:      BSP status.
  */
int32_t BSP_MEM_BlockErase(uint32_t Instance, uint32_t BlockAddress, uint32_t BlockSize)
{
  int32_t ret = BSP_ERROR_NONE;

  BSP_MEM_CHECK_PARAMS(Instance);

  if(MEM_OS_TryLock(Instance, MEM_OS_NO_WAIT) != MEM_OS_ERROR_NONE)
  {
    ret = BSP_ERROR_BUSY;
  }
  else
  {
    if(MemDrv->BlockErase != NULL)
    {
      if(MemDrv->BlockErase(MemCompObj, BlockAddress, BlockSize) < 0)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
    }
    else
    {
      ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;
    }

    MEM_OS_Unlock(Instance);
  }

  return ret;
}

/**
  * @brief  Erase the External Memory Interface.
  * @param  Instance:     External Memory Instance.
  * @retval int32_t:      BSP status.
  */
int32_t BSP_MEM_ChipErase(uint32_t Instance)
{
  int32_t ret = BSP_ERROR_NONE;

  BSP_MEM_CHECK_PARAMS(Instance);

  if(MEM_OS_TryLock(Instance, MEM_OS_NO_WAIT) != MEM_OS_ERROR_NONE)
  {
    ret = BSP_ERROR_BUSY;
  }
  else
  {
    if(MemDrv->ChipErase != NULL)
    {
      if(MemDrv->ChipErase(MemCompObj) < 0)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
    }
    else
    {
      ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;
    }

    MEM_OS_Unlock(Instance);
  }

  return ret;
}

/**
  * @brief  Read Data from External Memory Interface.
  * @param  Instance:     External Memory Instance.
  * @param  pData:        Pointer to buffer memory to be filled.
  * @param  Address:      Memory address to read from.
  * @param  Size:         Size of the data memory to be read.
  * @retval int32_t:      BSP status.
  */
int32_t BSP_MEM_ReadData(uint32_t Instance, uint8_t *pData, uint32_t Address, uint32_t Size)
{
  int32_t ret = BSP_ERROR_NONE;

  BSP_MEM_CHECK_PARAMS(Instance);

  if(MEM_OS_TryLock(Instance, MEM_OS_NO_WAIT) != MEM_OS_ERROR_NONE)
  {
    ret = BSP_ERROR_BUSY;
  }
  else
  {
    if(MemDrv->Read != NULL)
    {
      if(MemDrv->Read(MemCompObj, MEM_INTERFACE_MODE, pData, Address, Size) < 0)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
    }
    else
    {
      ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;
    }

    MEM_OS_Unlock(Instance);
  }

  return ret;
}

/**
  * @brief  Read Data from External Memory Interface using DMA.
  * @param  Instance:     External Memory Instance.
  * @param  pData:        Pointer to buffer memory to be filled.
  * @param  Address:      Memory address to read from.
  * @param  Size:         Size of the data memory to be read.
  * @retval int32_t:      BSP status.
  */
int32_t BSP_MEM_ReadDataDMA(uint32_t Instance, uint8_t *pData, uint32_t Address, uint32_t Size)
{
  int32_t ret = BSP_ERROR_NONE;

  BSP_MEM_CHECK_PARAMS(Instance);

  if(MEM_OS_TryLock(Instance, MEM_OS_NO_WAIT) != MEM_OS_ERROR_NONE)
  {
    ret = BSP_ERROR_BUSY;
  }
  else
  {
    if(MemDrv->ReadDMA != NULL)
    {
      if(MemDrv->ReadDMA(MemCompObj, MEM_INTERFACE_MODE, pData, Address, Size) < 0)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
    }
    else
    {
      ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;
    }

    MEM_OS_Unlock(Instance);
  }

  return ret;
}

/**
  * @brief  Write Data to External Memory Interface.
  * @param  Instance:     External Memory Instance.
  * @param  pData:        Pointer to data memory to be written.
  * @param  Address:      Memory address to write to.
  * @param  Size:         Size of the data memory to be written
  * @retval int32_t:      BSP status.
  */
int32_t BSP_MEM_WriteData(uint32_t Instance, uint8_t *pData, uint32_t Address, uint32_t Size)
{
  int32_t ret = BSP_ERROR_NONE;

  BSP_MEM_CHECK_PARAMS(Instance);

  if(MEM_OS_TryLock(Instance, MEM_OS_NO_WAIT) != MEM_OS_ERROR_NONE)
  {
    ret = BSP_ERROR_BUSY;
  }
  else
  {
    if(MemDrv->Program != NULL)
    {
      if(MemDrv->Program(MemCompObj, MEM_INTERFACE_MODE, pData, Address, Size) < 0)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
    }
    else
    {
      ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;
    }

    MEM_OS_Unlock(Instance);
  }

  return ret;
}

/**
  * @brief  Write Data to External Memory using DMA.
  * @param  Instance:     External Memory Instance.
  * @param  pData:        Pointer to data memory to be written.
  * @param  Address:      Memory address to write to.
  * @param  Size:         Size of the data memory to be written
  * @retval int32_t:      BSP status.
  */
int32_t BSP_MEM_WriteDataDMA(uint32_t Instance, uint8_t *pData, uint32_t Address, uint32_t Size)
{
  int32_t ret = BSP_ERROR_NONE;

  BSP_MEM_CHECK_PARAMS(Instance);

  if(MEM_OS_TryLock(Instance, MEM_OS_NO_WAIT) != MEM_OS_ERROR_NONE)
  {
    ret = BSP_ERROR_BUSY;
  }
  else
  {
    if(MemDrv->ProgramDMA != NULL)
    {
      if(MemDrv->ProgramDMA(MemCompObj, MEM_INTERFACE_MODE, pData, Address, Size) < 0)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
    }
    else
    {
      ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;
    }

    MEM_OS_Unlock(Instance);
  }

  return ret;
}

/**
  * @brief  Enable Memory Mapped Mode.
  * @param  Instance:     External Memory Instance.
  * @retval int32_t:      BSP status.
  */
int32_t BSP_MEM_EnableMemoryMappedMode(uint32_t Instance)
{
  int32_t ret = BSP_ERROR_NONE;

  BSP_MEM_CHECK_PARAMS(Instance);

  if(MEM_OS_TryLock(Instance, MEM_OS_NO_WAIT) != MEM_OS_ERROR_NONE)
  {
    ret = BSP_ERROR_BUSY;
  }
  else
  {
    if(MemDrv->EnableMemoryMappedMode != NULL)
    {
      if(MemDrv->EnableMemoryMappedMode(MemCompObj, MEM_INTERFACE_MODE) < 0)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
    }
    else
    {
      ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;
    }

    MEM_OS_Unlock(Instance);
  }

  return ret;
}

/**
  * @brief  Get the status of the SPI Transfer.
  * @param  Instance:     External Memory Instance.
  * @retval uint8_t:      Zero if no Transfer, Transfer Operation code otherwise.
  */
uint8_t BSP_MEM_GetTransferStatus(uint32_t Instance)
{
  BSP_MEM_CHECK_PARAMS(Instance);

  return MEM_OS_IsLocked(Instance);
}

/**
  * @brief  Wait for until complete SPI Transfer.
  * @param  Instance:     External Memory Instance.
  */
void BSP_MEM_WaitForTransferToBeDone(uint32_t Instance)
{
  if (Instance < MEM_INSTANCES_NBR)
  {
    MEM_OS_WaitForTransferToBeDone(Instance);
  }
}

/**
  * @brief  Signal Transfer Event Done.
  * @param  Instance:     External Memory Instance.
  */
__WEAK void BSP_MEM_SignalTransferDone(uint32_t Instance)
{
  /* This is the user's Callback to be implemented at the application level */
}
/**
  * @}
  */

/* Private functions ---------------------------------------------------------*/
/* USER CODE BEGIN PF */

/* USER CODE END PF */

#if (USE_HAL_SPI_REGISTER_CALLBACKS == 1U)
  /**
  * @brief  Transfer Read Complete CallBack
  * @param  hspi SPI Handler
  */
static void SPI_RxCpltCallback(SPI_HandleTypeDef *hspi)
{
  if(hspi == ObjCtx.handle)
  {
    /* Set the nCS */
    MEM_CS_HIGH();

    /* Unlock MEM ressources */
    MEM_OS_UnlockFromISR(0);

    /* Signal Transfer Done Event */
    BSP_MEM_SignalTransferDone(0);
  }
}
#endif /* USE_HAL_SPI_REGISTER_CALLBACKS */

/**
  * @brief  Set Burst Read length
  * @param  burstconfig:  Burst length
  * @retval int32_t:      BSP status.
  */
static int32_t MEM_SetBurstReadLength(uint8_t burstconfig)
{
  int32_t ret = BSP_ERROR_NONE;

  uint8_t cmd[2] = { MEM_SET_BURST_LENGTH_CMD
                   , burstconfig
                   };

  // Chip select go low to start a flash command
  MEM_CS_LOW();

  // Send SBL command and config data
  ret = MEM_SPI_Send( cmd, 2 );

  // Chip select go high to end a flash command
  MEM_CS_HIGH();

  return ret;
}

/**
  * @brief  Software Reset
  * @param  None
  * @retval int32_t:      BSP status.
  */
static int32_t MEM_SoftwareReset(void)
{
  int32_t ret = BSP_ERROR_NONE;
  uint8_t cmd;

  MEM_CS_LOW();
  cmd = MX25L6433F_RESET_ENABLE_CMD;
  ret = MEM_SPI_Send( &cmd, 1 );
  MEM_CS_HIGH();

  if(ret == BSP_ERROR_NONE)
  {
    MEM_CS_LOW();
    cmd = MX25L6433F_RESET_MEMORY_CMD;
    ret = MEM_SPI_Send( &cmd, 1 );
    MEM_CS_HIGH();
    HAL_Delay(20);
  }

  return ret;
}

/**
  * @brief  Initializes MEM low level.
  * @param  None
  * @retval int32_t:      BSP status.
  */
static int32_t MEM_IO_Init(void)
{
  int32_t ret = BSP_ERROR_NONE;

  ret = MEM_SPI_Init();

  if(ret == BSP_ERROR_NONE)
  {
    ret = MEM_SoftwareReset();
  }

  if(ret == BSP_ERROR_NONE)
  {
    /* MEM Initialization */
    MemCompObj = &ObjCtx;
    MemDrv = (MEM_Drv_t *)&MX25L6433F_MEM_Driver;
    ObjCtx.IsInitialized = 0;

    if(MemDrv->Init(MemCompObj) < 0)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
  }

  if(ret == BSP_ERROR_NONE)
  {
#if (USE_HAL_SPI_REGISTER_CALLBACKS == 1U)
    HAL_SPI_RegisterCallback((SPI_HandleTypeDef *)ObjCtx.handle, HAL_SPI_RX_COMPLETE_CB_ID, SPI_RxCpltCallback);
#endif /* USE_HAL_SPI_REGISTER_CALLBACKS */

    /* Enable Burst Read - 64-bytes */
    ret = MEM_SetBurstReadLength(MEM_WRAP_AROUND_64BYTE);
  }

  return ret;
}

/**
  * @brief  DeInitializes MEM low level
  * @param  None
  * @retval int32_t:      BSP status.
  */
static int32_t MEM_IO_DeInit(void)
{
  int32_t ret = BSP_ERROR_NONE;

  BSP_MEM_CHECK_PARAMS(Instance);

  ret = MEM_SPI_DeInit();

  return ret;
}

/**
  * @brief  Send data to the selected Memory at specified Address.
  * @param  pData:        Pointer to data to write to the the specified address memory.
  * @param  Address:      Memory address to be filled
  * @param  Size:         Length of data to write to the the specified address memory
  * @retval int32_t:      BSP status.
  */
static int32_t MEM_IO_SendData(uint8_t *pData, uint32_t Address, uint32_t Size)
{
  return BSP_ERROR_FEATURE_NOT_SUPPORTED;
}

/**
  * @brief  Send data using DMA to the selected Memory at specified Address.
  * @param  pData:        Pointer to data to write to the the specified address memory.
  * @param  Address:      Memory address to be filled
  * @param  Size:         Length of data to write to the the specified address memory
  * @retval int32_t:      BSP status.
  */
static int32_t MEM_IO_SendDataDMA(uint8_t *pData, uint32_t Address, uint32_t Size)
{
  return BSP_ERROR_FEATURE_NOT_SUPPORTED;
}

/**
  * @brief  Read Data from Serial Periphiral Memory Interface.
  * @param  pData:        Pointer to Data Memory to be filled
  * @param  Address:      Memory address to read from
  * @param  Size:         Size of Memory to be read
  * @retval int32_t:      BSP status.
  */
static int32_t MEM_IO_RecvData(uint8_t *pData, uint32_t Address, uint32_t Size)
{
  int32_t ret = BSP_ERROR_NONE;
  uint8_t cmd[4] = { MEM_READ_CMD
                   , ((Address >> 16) & 0xFF)
                   , ((Address >>  8) & 0xFF)
                   , (Address & 0xFF)
                   };

  /* Reset the nCS pin */
  MEM_CS_LOW();

  ret = MEM_SPI_Send(cmd, 4);
  if(ret == BSP_ERROR_NONE)
  {
    ret = MEM_SPI_Recv(pData, Size);
  }

  /* Set the nCS */
  MEM_CS_HIGH();

  return ret;
}

/**
  * @brief  Read Data from Serial Periphiral Memory Interface using DMA.
  * @param  pData:        Pointer to Data Memory to be filled
  * @param  Address:      Memory address to read from
  * @param  Size:         Size of Memory to be read
  * @retval int32_t:      BSP status.
  */
static int32_t MEM_IO_RecvDataDMA(uint8_t *pData, uint32_t Address, uint32_t Size)
{
  int32_t ret = BSP_ERROR_NONE;
  uint8_t cmd[4] = { MEM_READ_CMD
                   , ((Address >> 16) & 0xFF)
                   , ((Address >>  8) & 0xFF)
                   , (Address & 0xFF)
                   };

  /* Reset the nCS pin */
  MEM_CS_LOW();

  ret = MEM_SPI_Send(cmd, 4);
  if(ret == BSP_ERROR_NONE)
  {
    ret = MEM_SPI_Recv_DMA(pData, Size);
  }

  return ret;
}

/**
  * @brief  Register Bus IOs for instance 0 if MX25L6433F ID is OK
  * @param  Instance:     External Memory Instance.
  * @retval int32_t:      BSP status.
  */
static int32_t MEM_Probe(uint32_t Instance)
{
  int32_t ret = BSP_ERROR_NONE;

  /* Configure the lcd driver : map to MEM_IO function*/
  IOCtx.Init              = MEM_IO_Init;
  IOCtx.DeInit            = MEM_IO_DeInit;
  IOCtx.SendData          = MEM_IO_SendData;
  IOCtx.SendDataDMA       = MEM_IO_SendDataDMA;
  IOCtx.RecvData          = MEM_IO_RecvData;
  IOCtx.RecvDataDMA       = MEM_IO_RecvDataDMA;

  /* Set Base Address */
  IOCtx.Address           = MEM_BASE_ADDRESS;

  /* Register OSPI handle */
  ObjCtx.handle           = &hMEMSPI;

  if(MX25L6433F_RegisterBusIO(&ObjCtx, &IOCtx) != MX25L6433F_OK)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }

  return ret;
}
/**
  * @}
  */
/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
