

#include "ST7789_low_level.h"
#include "peripherals_ST7789_Interface.h"

#define gb_ST7789_CS_pin_low()  LPSPI_DRV_SetPcs(INST_ST7789_INTERFACE, LPSPI_PCS3, LPSPI_ACTIVE_LOW) //Slave select pin LOW
#define gb_ST7789_CS_pin_high() LPSPI_DRV_SetPcs(INST_ST7789_INTERFACE, LPSPI_PCS3, LPSPI_ACTIVE_HIGH)

#define gb_ST7789_DC_pin_low()  PINS_DRV_WritePin(DC_PORT, DC_PIN, 0);
#define gb_ST7789_DC_pin_high() PINS_DRV_WritePin(DC_PORT, DC_PIN, 1);

#define gb_ST7789_reset_pin_low()  PINS_DRV_WritePin(Reset_PORT, Reset_PIN, 0)
#define gb_ST7789_reset_pin_high() PINS_DRV_WritePin(Reset_PORT, Reset_PIN, 1)

void GB_ST7789_SendCommand(uint8_t commandByte, uint8_t *dataBytes, uint8_t numDataBytes)
{
	gb_ST7789_CS_pin_low();

	gb_ST7789_DC_pin_low();
	GB_MA_SPI_send_byte_conti(&commandByte,1);
	gb_ST7789_DC_pin_high();
//
//	for (uint8_t i = 0; i<numDataBytes;i++)
	GB_MA_SPI_send_byte_conti(dataBytes, numDataBytes );

	gb_ST7789_CS_pin_high();

}
