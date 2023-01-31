/**
 ********************************************************************************
 * @file    unv_uart_buff_impl.h
 * @author  Phu Tran
 * @date   Jan 2023
 * @brief
 * COPYRIGHT NOTICE: (c) 2023 Noimonkosmo.  All rights reserved. 
 ********************************************************************************
 */

#ifndef _UNV_UART_BUFF_IMPL_H_
#define _UNV_UART_BUFF_IMPL_H_

/************************************
 * INCLUDES
 ************************************/
#include "unv_uart_buff.h"
/************************************
 * EXPORTED PREPROCESSOR DEFINES
 ************************************/
 
 /************************************
 * EXPORTED MACROS
 ************************************/

/************************************
 * EXPORTED TYPEDEFS
 ************************************/

/************************************
 * EXPORTED VARIABLES
 ************************************/
extern unv_uart_buff_t uart_buff_0;
/************************************
 * EXPORTED  FUNCTION PROTOTYPES
 ************************************/
//fsp_err_t nit_uart_initialize(void);
void uart_buff_0_transmit_tc_callback(void);
void uart_buff_0_transmit_er_callback(void);
void uart_buff_0_phy_write(uint8_t data);
uint8_t  uart_buff_0_phy_read(void);
uint8_t  uart_buff_0_phy_read_status(void);
void uart_buff_0_phy_enable_txe_flag(void);
void uart_buff_0_phy_disable_txe_flag(void);
void init_uart_buffer(void);
//void nit_uart_deinit(void);


#endif /* _UNV_UART_BUFF_IMPL_H_ */

/*** end of file ***/
