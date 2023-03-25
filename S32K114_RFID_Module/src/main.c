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
#include "stdint.h"
#include "GB_MMFRC522.h"

volatile int exit_code = 0;
/* User includes */

uint8_t tx_buff[10];
uint8_t rx_buff[100];

/*!
 * @brief: LPTMR IRQ handler.
 */

//uint32_t counter_v =0;
//void LPTMR0_IRQHandler(void)
//{
//	counter_v++;
//    /* Clear compare flag */
//    LPTMR_DRV_ClearCompareFlag(INST_LPTMR_1);
//    /* Toggle LED0 */
// //   PINS_DRV_TogglePins(LED_GPIO_PORT, (1 << LED0_INDEX));
//}
/*!
  \brief The main function for the project.
  \details The startup initialization sequence is the following:
 * - startup asm routine
 * - main()
*/
int main(void)
{
    /* Write your code here */
	tx_buff[0]= 0xEE;

	// Member variables
	Uid uid_value;;
	//Configure clock
	CLOCK_DRV_Init(&clockMan1_InitConfig0);

	//configure gpio pins(SPI , uart, output/input)
   PINS_DRV_Init(NUM_OF_CONFIGURED_PINS0, g_pin_mux_InitConfigArr0);

   //Configure UART peripheral
   LPUART_DRV_Init(0, &lpUartState0, &lpuart_0_InitConfig0);

   //Configure Timer peripheral
   LPTMR_DRV_Init(INST_LPTMR_1, &lptmr_1_config0, false);
   /* Enable IRQ for LPTMR */
   INT_SYS_EnableIRQ(LPTMR0_IRQn);
   /* Start LPTMR counter */
   LPTMR_DRV_StartCounter(INST_LPTMR_1);

   //Start SPI Master communication
   LPSPI_DRV_MasterInit(0, &lpspi_1State,  &lpspi_0_MasterConfig0);
   LPSPI_DRV_SetPcs(INST_LPSPI_1, LPSPI_PCS0, LPSPI_ACTIVE_LOW); //Slave select pin LOW
   LPSPI_DRV_MasterTransferBlocking(INST_LPSPI_1,tx_buff, rx_buff, 1, 100); //Transfer the data from MOSI to MISO
   LPSPI_DRV_SetPcs(INST_LPSPI_1, LPSPI_PCS0, LPSPI_ACTIVE_HIGH);//Slave select pin HIGH

   MFRC522_init();

   GB_MFRC522Version();
     for(;;)
    {


    	 GB_printString0("Kunal\n");
//
	   GB_MFRC522Version();
//
	   if(GB_PICC_IsNewCardPresent())
	   {
		   GB_PICC_ReadCardSerial();
//
		   GB_PICCDetails(&uid_value);
		   }
    	int cycles = 7200000;
    	while(cycles--);
//    	LPSPI_DRV_SetPcs(0, LPSPI_PCS0, LPSPI_ACTIVE_LOW);
////    	int cycles = 720;
////    	while(cycles--);
//    	LPSPI_DRV_MasterTransferBlocking(0,tx_buff, rx_buff, 1, 100);
//    	LPSPI_DRV_SetPcs(0, LPSPI_PCS0, LPSPI_ACTIVE_HIGH);


        if(exit_code != 0)
        {

//        	int cycles = 720000;
//        	while(cycles--);
//        	LPSPI_DRV_SetPcs(0, LPSPI_PCS0, LPSPI_ACTIVE_LOW);
//        	LPSPI_DRV_MasterTransferBlocking(0,tx_buff, rx_buff, 1, 100);
//        	LPSPI_DRV_SetPcs(0, LPSPI_PCS0, LPSPI_ACTIVE_HIGH);
//

            break;
        }
    }
    return exit_code;
}

/* END main */
/*!
** @}
*/
