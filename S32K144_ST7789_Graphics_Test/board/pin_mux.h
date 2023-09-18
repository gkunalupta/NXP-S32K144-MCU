#ifndef _PIN_MUX_H_
#define _PIN_MUX_H_

#include "pins_driver.h"

/***********************************************************************************************************************
 * Definitions
 **********************************************************************************************************************/

/*!
 * @addtogroup pin_mux
 * @{
 */

/***********************************************************************************************************************
 * API
 **********************************************************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif


/*! @brief Definitions/Declarations for BOARD_InitPins Functional Group */
/*! @brief User definition pins */
#define DIN_PORT    PTB
#define DIN_PIN     16U
#define CLK_PORT    PTB
#define CLK_PIN     14U
#define CS_PORT    PTB
#define CS_PIN     17U
#define DC_PORT    PTB
#define DC_PIN     13U
#define Reset_PORT    PTB
#define Reset_PIN     12U
/*! @brief User number of configured pins */
#define NUM_OF_CONFIGURED_PINS0 5
/*! @brief User configuration structure */
extern pin_settings_config_t g_pin_mux_InitConfigArr0[NUM_OF_CONFIGURED_PINS0];


#if defined(__cplusplus)
}
#endif

/*!
 * @}
 */
#endif /* _PIN_MUX_H_ */

/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/

