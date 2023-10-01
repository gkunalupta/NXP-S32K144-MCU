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
#include "ST7789_low_level.h"
volatile int exit_code = 0;
/* User includes */

/*!
  \brief The main function for the project.
  \details The startup initialization sequence is the following:
 * - startup asm routine
 * - main()
*/

uint8_t data = 0x55;
int main(void)
{
	/* Write your code here */
		status_t error;
		 /* Configure clocks for PORT */
		 error = CLOCK_DRV_Init(&clockMan1_InitConfig0);
		 DEV_ASSERT(error == STATUS_SUCCESS);

		 error = PINS_DRV_Init(NUM_OF_CONFIGURED_PINS0, g_pin_mux_InitConfigArr0);
		 DEV_ASSERT(error == STATUS_SUCCESS);

		 /* Initialize LPSPI0 (Send)*/
		    LPSPI_DRV_MasterInit(INST_ST7789_INTERFACE, &ST7789_InterfaceState, &ST7789_SPI_Config);


	    	GB_ST7789_Init();

		   // GB_ST7789_SendCommand(ST77XX_COLMOD, &data, 1, 100);

//		    ST7789_SetAddressWindow(0,0,240,240);
//		    ST7789_Fill_Color(ST77XX_GREEN);
//		    OSIF_TimeDelay(1000);
    for(;;)
    {

    	GB_ST7789_Init();

//	GB_ST7789_SendCommand(ST77XX_COLMOD, &data, 1, 100);
//
	 ST7789_Fill_Color(ST77XX_GREEN);
	    OSIF_TimeDelay(1000);

        if(exit_code != 0)
        {
            break;
        }
    }
    return exit_code;
}

/* END main */
/*!
** @}
*/
