/***********************************************************************************************************************
 * This file was generated by the S32 Configuration Tools. Any manual edits made to this file
 * will be overwritten if the respective S32 Configuration Tools is used to update this file.
 **********************************************************************************************************************/

/* clang-format off */
/* TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
!!GlobalInfo
product: Peripherals v10.0
processor: S32K144
package_id: S32K144_LQFP100
mcu_data: s32sdk_s32k1xx_rtm_403
processor_version: 0.0.0
functionalGroups:
- name: BOARD_InitPeripherals
  UUID: a6d43dcb-893a-49cd-9d9d-0bb26ff0f05b
  called_from_default_init: true
  selectedCore: core0
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS **********/
/* clang-format on */

/*******************************************************************************
 * Included files 
 ******************************************************************************/
#include "peripherals_ST7789_Interface.h"

/*******************************************************************************
 * ST7789_Interface initialization code
 ******************************************************************************/
/* clang-format off */
/* TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
instance:
- name: 'ST7789_Interface'
- type: 'lpspi_config'
- mode: 'general_soft'
- custom_name_enabled: 'true'
- type_id: 'lpspi'
- functional_group: 'BOARD_InitPeripherals'
- peripheral: 'LPSPI_1'
- config_sets:
  - lpspi:
    - lpspiMasterConfig:
      - 0:
        - name: 'ST7789_SPI_Config'
        - readOnly: 'true'
        - bitsPerSec: '1000000'
        - whichPcs: 'LPSPI_PCS3'
        - pcsPolarity: 'LPSPI_ACTIVE_HIGH'
        - isPcsContinuous: 'false'
        - bitcount: '8'
        - lpspiSrcClk: '8000000'
        - clkPhase: 'LPSPI_CLOCK_PHASE_1ST_EDGE'
        - clkPolarity: 'LPSPI_SCK_ACTIVE_HIGH'
        - lsbFirst: 'false'
        - transferType: 'LPSPI_USING_INTERRUPTS'
        - rxDMAChannel: '0'
        - txDMAChannel: '0'
        - callback: 'NULL'
        - callbackParam: 'NULL'
    - lpspiSlaveConfig: []
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS **********/
/* clang-format on */

/**
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global] 
 * Violates MISRA 2012 Required Rule 8.4, A compatible declaration shall be 
 * visible when an object or function with external linkage is defined..
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.7, External variable could be made static.
 * The external variables will be used in other source files in application code.
 *
 */

void GB_MA_SPI_send_byte_conti(uint8_t *val, uint8_t count)
{
	static uint8_t rx_value;
	status_t error;
	//LPSPI_DRV_MasterTransferBlocking(INST_ST7789_INTERFACE,val, &rx_value, count, 1); //Transfer the data from MOSI to MISO

	error = LPSPI_DRV_MasterTransfer(INST_ST7789_INTERFACE,val, &rx_value, count); //Transfer the data from MOSI to MISO
error = LPSPI_DRV_MasterGetTransferStatus(INST_ST7789_INTERFACE, &rx_value);
DEV_ASSERT(error == STATUS_SUCCESS);

}

uint8_t GB_MA_SPI_exchange_byte(uint8_t *val, uint8_t count)
{
	static uint8_t rx_value;
	LPSPI_DRV_MasterTransferBlocking(INST_ST7789_INTERFACE,val, &rx_value, count, 1); //Transfer the data from MOSI to MISO
	return rx_value;

}

/* Define state structure for current SPI instance */
lpspi_state_t ST7789_InterfaceState;

/* LPSPI Master Configurations 0 */

const lpspi_master_config_t ST7789_SPI_Config = {
  .bitsPerSec = 1000000UL,
  .whichPcs = LPSPI_PCS3,
  .pcsPolarity = LPSPI_ACTIVE_HIGH,
  .isPcsContinuous = false,
  .bitcount = 8U,
  .lpspiSrcClk = 8000000UL,
  .clkPhase = LPSPI_CLOCK_PHASE_1ST_EDGE,
  .clkPolarity = LPSPI_SCK_ACTIVE_HIGH,
  .lsbFirst = false,
  .transferType = LPSPI_USING_INTERRUPTS,
  .rxDMAChannel = 0U,
  .txDMAChannel = 0U,
  .callback = NULL,
  .callbackParam = NULL
};



