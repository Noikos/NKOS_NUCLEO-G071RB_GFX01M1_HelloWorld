/**
 ********************************************************************************
 * @file    unv_uart_buff_impl.c
 * @author  Phu Tran
 * @date   Jan 2023
 * @brief
 * COPYRIGHT NOTICE: (c) 2023 Noimonkosmo.  All rights reserved. 
 ********************************************************************************
 */


/************************************
 * INCLUDES
 ************************************/
#include "unv_uart_buff_impl.h"
#include "main.h"
/************************************
 * PRIVATE PREPROCESSOR DEFINES
 ************************************/
 
 /************************************
 * PRIVATE MACROS
 ************************************/

/************************************
 * PRIVATE TYPEDEFS
 ************************************/
 
 /************************************
 * EXPORTED VARIABLES DEFINITION
 ************************************/
unv_uart_buff_t uart_buff_0;
/************************************
 * PRIVATE&STATIC VARIABLES
 ************************************/

/************************************
 * GLOBAL VARIABLES
 ************************************/

/************************************
 * PRIVATE&STATIC FUNCTION PROTOTYPES
 ************************************/

/************************************
 * PRIVATE&STATIC FUNCTIONS
 ************************************/

/************************************
 * GLOBAL FUNCTIONS
 ************************************/

/*!
 * @brief Identify the larger of two 8-bit integers.
 *
 * @param[in] num1  The first number to be compared.
 * @param[in] num2  The second number to be compared.
 *
 * @return The value of the larger number.
 *
 * This function must be run before crc_fast() or the table stored in ROM.
 */
void uart_buff_transmit_tc_callback(void)
{
    printf ("\r\n ** uart_buff_transmit_tc_callback() ** \r\n");
    //g_uart0_ctrl.p_reg->SCR |= (uint8_t) (SCI_SCR_TEIE_MASK);
}

__attribute__((weak)) void uart_buff_transmit_er_callback(void)
{

}


__attribute__((weak)) void uart_buff_hardware_config(void)
{

}

void uart_buff_phy_write(uint8_t data)
{
    printf ("\r\n ** uart_buff_phy_write() ** \r\n");
	LL_USART_TransmitData8(USART1, data);
    //NIT_R_SCI_UART_Write8(&g_uart0_ctrl, data);
}

uint8_t  uart_buff_phy_read(void)
{
//    printf ("\r\n ** uart_buff_phy_read()=0x%x ** \r\n",uart_revice_char);
    return LL_USART_ReceiveData8(USART1);
}

uint8_t  uart_buff_phy_read_status(void)
{
  return 0;
}

void uart_buff_phy_enable_txe_flag(void)
{
    printf ("\r\n ** uart_buff_phy_enable_txe_flag() ** \r\n");
    /* Enable TXE interrupt */
  LL_USART_EnableIT_TXE(USART1);
}

void uart_buff_phy_disable_txe_flag(void)
{
    printf ("\r\n ** uart_buff_phy_disable_txe_flag() ** \r\n");
    //R_BSP_IrqDisable(g_uart0_ctrl.p_cfg->tei_irq);
    LL_USART_DisableIT_TXE(USART1);
    //g_uart0_ctrl.p_reg->SCR &= (uint8_t) ~(SCI_SCR_TIE_MASK);
}

////////////////////////////////////////////
void init_uart_buffer(void)
{
  uart_buff_0.uart_buff_transmit_tc_callback =  uart_buff_transmit_tc_callback;

  uart_buff_0.uart_buff_transmit_er_callback = uart_buff_transmit_er_callback;

  uart_buff_0.uart_buff_phy_write = uart_buff_phy_write;

  uart_buff_0.uart_buff_phy_read = uart_buff_phy_read;

  uart_buff_0.uart_buff_phy_read_status =  uart_buff_phy_read_status;

  uart_buff_0.uart_buff_phy_enable_txe_flag = uart_buff_phy_enable_txe_flag;

  uart_buff_0.uart_buff_phy_disable_txe_flag = uart_buff_phy_disable_txe_flag;

  unv_uart_buff_init(&uart_buff_0);

}

void nit_sci_uart_rxi_isr(void)
{
    unv_uart_buff_receive_interrupt_handler(&uart_buff_0);
}
void nit_sci_uart_txi_isr(void)
{
    unv_uart_buff_transmit_tc_interrupt_handler(&uart_buff_0);
}
 void nit_sci_uart_tei_isr(void)
{
     unv_uart_buff_transmit_txe_interrupt_handler(&uart_buff_0);
}
void nit_sci_uart_eri_isr(uint8_t err,uint16_t data, bool is9bit)
{
    unv_uart_buff_transmit_error_interrupt_handler(&uart_buff_0);
}

/*** end of file ***/
