

#include "ST7789_low_level.h"
#include "peripherals_ST7789_Interface.h"

#define gb_ST7789_CS_pin_low()  LPSPI_DRV_SetPcs(INST_ST7789_INTERFACE, LPSPI_PCS3, LPSPI_ACTIVE_LOW) //Slave select pin LOW
#define gb_ST7789_CS_pin_high() LPSPI_DRV_SetPcs(INST_ST7789_INTERFACE, LPSPI_PCS3, LPSPI_ACTIVE_HIGH)

#define gb_ST7789_DC_pin_low()  PINS_DRV_WritePin(DC_PORT, DC_PIN, 0);
#define gb_ST7789_DC_pin_high() PINS_DRV_WritePin(DC_PORT, DC_PIN, 1);

#define gb_ST7789_reset_pin_low()  PINS_DRV_WritePin(Reset_PORT, Reset_PIN, 0)
#define gb_ST7789_reset_pin_high() PINS_DRV_WritePin(Reset_PORT, Reset_PIN, 1)

/*
 * @brief write data to ST7789 controller
 * @param commonByte -> address of ST7789 that needs to be accessed
 * @param dataBytes  -> pointer to the data buffer that needs to send/write at that address
 * @param numDataBytes -> number of data bytes that needs to send
 */

void GB_ST7789_SendCommand(uint8_t commandByte, uint8_t *dataBytes, uint8_t numDataBytes, uint32_t timeout)
{
	gb_ST7789_CS_pin_low();

	gb_ST7789_DC_pin_low();
	GB_MA_SPI_send_byte_conti(&commandByte,1,timeout);
	gb_ST7789_DC_pin_high();

	GB_MA_SPI_send_byte_conti(dataBytes, numDataBytes, timeout );
	gb_ST7789_CS_pin_high();

}

void GB_ST7789_Init()
{
	uint8_t ColorMod = ST7789_16_Bit_5_6_5_Input_64K_Color;
	uint8_t MADCTL = 0x08;

	uint8_t MADCTL_SetRotation = 0xC0; // MY = 1, MX= 1, RGB is set

	uint8_t ColAddr[4]={ 0x00,0x00,0x00,240};
	uint8_t RowAddr[4] = {0x00,0x00,320>>8,320&0xFF};
	uint8_t data;

	GB_ST7789_SendCommand(ST77XX_SWRESET, &data, 0, 150 );

	GB_ST7789_SendCommand(ST77XX_SLPOUT, &data, 0, 10 );


	GB_ST7789_SendCommand(ST77XX_COLMOD, &ColorMod, 1, 10);

	GB_ST7789_SendCommand(ST77XX_MADCTL, &MADCTL, 1, 10);

	GB_ST7789_SendCommand(ST77XX_CASET, ColAddr, 4, 10);

	GB_ST7789_SendCommand(ST77XX_RASET, RowAddr, 4, 10);

	GB_ST7789_SendCommand(ST77XX_INVON, &data, 0, 10);

	GB_ST7789_SendCommand(ST77XX_NORON, &data, 0, 10);

	GB_ST7789_SendCommand(ST77XX_DISPON, &data, 0, 10);

	GB_ST7789_SendCommand(ST77XX_MADCTL, &MADCTL_SetRotation, 1, 10);

}
