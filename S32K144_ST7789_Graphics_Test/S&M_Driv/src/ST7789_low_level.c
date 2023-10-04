

#include "ST7789_low_level.h"
#include "peripherals_ST7789_Interface.h"

//#define gb_ST7789_CS_pin_low()  LPSPI_DRV_SetPcs(INST_ST7789_INTERFACE, LPSPI_PCS3, LPSPI_ACTIVE_LOW) //Slave select pin LOW
//#define gb_ST7789_CS_pin_high() LPSPI_DRV_SetPcs(INST_ST7789_INTERFACE, LPSPI_PCS3, LPSPI_ACTIVE_HIGH )

#define gb_ST7789_CS_pin_low()  PINS_DRV_WritePin(CS_PORT, CS_PIN, 0) //Slave select pin LOW
#define gb_ST7789_CS_pin_high() PINS_DRV_WritePin(CS_PORT, CS_PIN, 1 )

#define gb_ST7789_DC_pin_low()  PINS_DRV_WritePin(DC_PORT, DC_PIN, 0);
#define gb_ST7789_DC_pin_high() PINS_DRV_WritePin(DC_PORT, DC_PIN, 1);

#define gb_ST7789_reset_pin_low()  PINS_DRV_WritePin(Reset_PORT, Reset_PIN, 0)
#define gb_ST7789_reset_pin_high() PINS_DRV_WritePin(Reset_PORT, Reset_PIN, 1)

//uint8_t _rowstart =0;
//uint8_t _colstart =0;
//
//uint16_t windowWidth;
//uint16_t windowHeight;
//
//
//uint16_t _xstart =0;
//uint16_t _ystart =0;
//uint16_t _width =0;
//uint16_t _height =0;


// For ST7789 Driver IC and we are using 240*240 pixel and 1.3 inch display
#define ST7789_WIDTH 240
#define ST7789_HEIGHT 240
#define X_SHIFT 0 // starting point for X axis that is for rows. Rows will start from 0 index
#define Y_SHIFT 0 //starting point for Y axis that is for coloumns. Columns will start from 0 index.


/*
 * @brief write data to ST7789 controller
 * @param commonByte -> address of ST7789 that needs to be accessed
 * @param dataBytes  -> pointer to the data buffer that needs to send/write at that address
 * @param numDataBytes -> number of data bytes that needs to send
 */

void GB_ST7789_SendCommand(uint8_t commandByte, uint8_t *dataBytes, uint8_t numDataBytes, uint32_t timeout)
{
	//gb_ST7789_CS_pin_low();

	gb_ST7789_DC_pin_low();
	GB_MA_SPI_send_byte_conti(&commandByte,1,timeout);
	gb_ST7789_DC_pin_high();

	GB_MA_SPI_send_byte_conti(dataBytes, numDataBytes, timeout );
//	gb_ST7789_CS_pin_high();

}

void GB_ST7789_SendData(uint8_t *data, uint32_t data_size)
{
	//gb_ST7789_CS_pin_low();
	gb_ST7789_DC_pin_high();

	GB_MA_SPI_send_byte_conti(data, data_size, 5000);
	//gb_ST7789_CS_pin_high();
}
void GB_ST7789_Init()
{

	uint8_t ColorMod = ST7789_16_Bit_5_6_5_Input_64K_Color;
	uint8_t MADCTL_SetRotation1 = 0x08; //MY = 1, MV=0, MX = 0, BGR is set
	uint8_t MADCTL_SetRotation0 = 0xC0; // MY = 1, MX= 1, RGB is set
	uint8_t MADCTL_SetRotation2 = 0x00; //MY = 0, MX = 0, RGB is set, MH =0
	uint8_t ColAddr[4]={ 0x00,0x00,0x00,240};
	uint8_t RowAddr[4] = {0x00,0x00,320>>8,320&0xFF};
	uint8_t data;

	gb_ST7789_reset_pin_low();
	OSIF_TimeDelay(25);
	gb_ST7789_reset_pin_high();
	OSIF_TimeDelay(50);

	gb_ST7789_CS_pin_low();
		GB_ST7789_SendCommand(ST77XX_SWRESET, &data, 0, 1000 );

		GB_ST7789_SendCommand(ST77XX_SLPOUT, &data, 0, 1000 );

	GB_ST7789_SendCommand(ST77XX_COLMOD, &ColorMod, 1, 1000);   // Set color Mode

	GB_ST7789_SendCommand(ST77XX_MADCTL, &MADCTL_SetRotation1, 1, 10);  // Set display rotation


	//GB_ST7789_SendCommand(ST77XX_MADCTL, &MADCTL_SetRotation0, 1, 10);

	GB_ST7789_SendCommand(ST77XX_CASET, ColAddr, 4, 10);

	GB_ST7789_SendCommand(ST77XX_RASET, RowAddr, 4, 10);

	GB_ST7789_SendCommand(ST77XX_INVON, &data, 0, 10);

	GB_ST7789_SendCommand(ST77XX_NORON, &data, 0, 10);

	GB_ST7789_SendCommand(ST77XX_DISPON, &data, 0, 10);

 GB_ST7789_SendCommand(ST77XX_MADCTL, &MADCTL_SetRotation0, 1, 10);
	gb_ST7789_CS_pin_high();
}
void ST7789_SetAddressWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
	uint8_t data;
	gb_ST7789_CS_pin_low();

	uint16_t x_start = x0+ X_SHIFT;
	uint16_t x_end = x1 +Y_SHIFT;
	uint16_t y_start = y0+ Y_SHIFT;
	uint16_t y_end = y1 + Y_SHIFT;

	//uint8_t ColAddr[4]={ x_start >> 8,x_start & 0xFF,x_end >> 8, x_end & 0xff};
	uint8_t ColAddr[4]={ 0x00,0x00,0x00, 0xEF};

	GB_ST7789_SendCommand(ST77XX_CASET, ColAddr, 4, 10);
	//uint8_t RowAddr[4] = { y_start >> 8, y_start & 0xFF, y_end >> 8, y_start & 0xFF};
	uint8_t RowAddr[4] = { 0x00, 0x50,0x01, 0x3F};

	GB_ST7789_SendCommand(ST77XX_RASET, RowAddr, 4, 10);

	gb_ST7789_CS_pin_high();
}
uint16_t total =0;
void ST7789_Fill_Color(uint16_t color)
{

	uint16_t i,j;
	uint8_t data;

	ST7789_SetAddressWindow(0,0, ST7789_WIDTH -1, ST7789_HEIGHT-1);
	gb_ST7789_CS_pin_low();


	GB_ST7789_SendCommand(ST77XX_RAMWR, &data, 0, 10);

	for (i =0; i < ST7789_WIDTH; i++)
		for (j=0; j<ST7789_HEIGHT; j++)
		{
			uint8_t data[] = { color >>8, color & 0xFF};
			GB_ST7789_SendData(data, sizeof(data));
total ++;
		}

	gb_ST7789_CS_pin_high();
}
uint32_t length = 0;
uint16_t i,j;
void ST7789_DrawImage(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint8_t *data)
{
	uint8_t d_data;

	length = 0;;
	ST7789_SetAddressWindow(0,0, ST7789_WIDTH -1, ST7789_HEIGHT-1);
	gb_ST7789_CS_pin_low();


	GB_ST7789_SendCommand(ST77XX_RAMWR, &d_data, 0, 10);

for (j =0; j<8; j++)
{
	for(i = 0; i<14400; i++)
	{
		GB_ST7789_SendData(&data[i + (14399 *j)], 1);

	}
}
//	GB_ST7789_SendData(data, 25000);
//	for (i =0; i < ST7789_WIDTH; i++)
//		for (j=0; j<ST7789_HEIGHT; j++)
//		{
//			uint8_t data[] = { color >>8, color & 0xFF};
//			GB_ST7789_SendData(data, sizeof(data));
//total ++;
//		}
//
//
	gb_ST7789_CS_pin_high();

}
//void ST7789_DrawFilledRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color)
//{
//	//SPI start: chip select low
//   for(uint16_t i = x; i<x+w; i++)
//   {
//	   ST7789_DrawLineV
//	   //draw vertical line function
//   }
//   //SPI end: chip select high
//}
//
//void ST7789_DrawLineV()
//{
//	//SPI start: chip select low
//
//	ST7789_DrawLine();
//	//SPI end: chip select high
//}
