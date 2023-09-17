/*!
** Copyright 2020 NXP
** @file main.c
** @brief
**         Main module.
**         This module contains user's application code.
*/
/*!
**  @addtogroup main_module main module documentation
**  @{
*/
/* MODULE main */


/* Including necessary configuration files. */
#include "sdk_project_config.h"
#include "string.h"
volatile int exit_code = 0;
uint8_t rxBuff[20];
#define welcomeMsg "Welcome to Gettobyte\n"
#define waiTing " Input the data, to print it\n"
/* User includes */

/*!
  \brief The main function for the project.
  \details The startup initialization sequence is the following:
 * - startup asm routine
 * - main()
*/
int main(void)
{
    /* Write your code here */
	  status_t error;
	  /* Configure clocks for PORT */
	  error = CLOCK_DRV_Init(&clockMan1_InitConfig0);
	  DEV_ASSERT(error == STATUS_SUCCESS);

	  error = PINS_DRV_Init(NUM_OF_CONFIGURED_PINS0, g_pin_mux_InitConfigArr0);
	  DEV_ASSERT(error == STATUS_SUCCESS);

	  error = LPUART_DRV_Init(INST_LPUART_1, &HC05_STATE, &HC05_Config);
	  DEV_ASSERT(error == STATUS_SUCCESS);

	  LPUART_DRV_SendDataPolling(INST_LPUART_1, (uint8_t *) welcomeMsg , strlen(welcomeMsg));

	  OSIF_TimeDelay(1000);

    for(;;)
    {

    	 LPUART_DRV_SendDataPolling(INST_LPUART_1, (uint8_t *) waiTing,strlen(waiTing));

    	LPUART_DRV_ReceiveDataPolling(INST_LPUART_1, &rxBuff, 5);


  	  LPUART_DRV_SendDataPolling(INST_LPUART_1, &rxBuff, 5);

  	OSIF_TimeDelay(1000);

    }
    return exit_code;
}

/* END main */
/*!
** @}
*/
