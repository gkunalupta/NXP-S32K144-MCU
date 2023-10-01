/*
 * ST7789_low_level.h
 *
 *  Created on: 17-Sep-2023
 *      Author: gettobyte_kunal
 */

#ifndef INC_ST7789_LOW_LEVEL_H_
#define INC_ST7789_LOW_LEVEL_H_
#include "peripherals_ST7789_Interface.h"
#include "pin_mux.h"
#include "stdint.h"
#include "stdbool.h"
typedef uint8_t byte;

typedef enum
{
	ST7789_CMD_DELAY = 0x80

}ST7789_TimeOut_Macros;

typedef enum
{
	ST7789_12_Bit_4_4_4_Input_4K_Color = 0x03,//0b00000011
	ST7789_16_Bit_5_6_5_Input_64K_Color = 0x55,//0b01010101
	ST7789_18_Bit_6_6_6_Input_262K_Color = 0x66,//0b01100110


}ST7789_Color_mode_values;

#define ST7735_TFTWIDTH_128 128  // for 1.44 and mini
#define ST7735_TFTWIDTH_80 80    // for mini
#define ST7735_TFTHEIGHT_128 128 // for 1.44" display
#define ST7735_TFTHEIGHT_160 160 // for 1.8" and mini display

#define ST_CMD_DELAY 0x80 // special signifier for command lists

#define ST77XX_NOP 0x00
#define ST77XX_SWRESET 0x01
#define ST77XX_RDDID 0x04
#define ST77XX_RDDST 0x09

#define ST77XX_SLPIN 0x10
#define ST77XX_SLPOUT 0x11
#define ST77XX_PTLON 0x12
#define ST77XX_NORON 0x13

#define ST77XX_INVOFF 0x20
#define ST77XX_INVON 0x21
#define ST77XX_DISPOFF 0x28
#define ST77XX_DISPON 0x29
#define ST77XX_CASET 0x2A
#define ST77XX_RASET 0x2B
#define ST77XX_RAMWR 0x2C
#define ST77XX_RAMRD 0x2E

#define ST77XX_PTLAR 0x30
#define ST77XX_TEOFF 0x34
#define ST77XX_TEON 0x35
#define ST77XX_MADCTL 0x36
#define ST77XX_COLMOD 0x3A

#define ST77XX_MADCTL_MY 0x80
#define ST77XX_MADCTL_MX 0x40
#define ST77XX_MADCTL_MV 0x20
#define ST77XX_MADCTL_ML 0x10
#define ST77XX_MADCTL_RGB 0x00

#define ST77XX_RDID1 0xDA
#define ST77XX_RDID2 0xDB
#define ST77XX_RDID3 0xDC
#define ST77XX_RDID4 0xDD

// Some ready-made 16-bit ('565') color settings:
#define ST77XX_BLACK 0x0000
#define ST77XX_WHITE 0xFFFF
#define ST77XX_RED 0xF800
#define ST77XX_GREEN 0x07E0
#define ST77XX_BLUE 0x001F
#define ST77XX_CYAN 0x07FF
#define ST77XX_MAGENTA 0xF81F
#define ST77XX_YELLOW 0xFFE0
#define ST77XX_ORANGE 0xFC00

typedef struct {
    const uint8_t width;
    uint8_t height;
    const uint16_t *data;
} FontDef;


/*
 * @brief write data to ST7789 controller
 * @param commonByte -> address of ST7789 that needs to be accessed
 * @param dataBytes  -> pointer to the data buffer that needs to send/write at that address
 * @param numDataBytes -> number of data bytes that needs to send
 */

void GB_ST7789_SendCommand(uint8_t commandByte, uint8_t *dataBytes, uint8_t numDataBytes, uint32_t timeout);

void GB_ST7789_Init(void);
/*
 * Reference from: https://github.com/Floyd-Fish/ST7789-STM32/blob/master/ST7789/st7789.c
 */
void GB_ST7789_SetRotation(uint8_t m);
void ST7789_SetAddressWindow(uint16_t x0, uint16_t yo, uint16_t x1, uint16_t y1);
void ST7789_Fill_Color(uint16_t color);
void ST7789DrawPixel(uint16_t x, uint16_t y, uint16_t color);
void ST7789_Fill(uint16_t xSta, uint16_t ySta, uint16_t xEnd, uint16_t yEnd, uint16_t color);
void ST7789_Draw_Pixel_4px( uint16_t x, uint16_t y, uint16_t color);
void ST7789_DrawLine( uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
void ST7789_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void ST7789_DrawCircle(uint16_t x0, uint16_t y0, uint8_t r, uint16_t color);
void ST7789_DrawImage(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint16_t *data);
void ST7789_InvertColors(uint8_t invert);
void ST7789_WriteChar(uint16_t x, uint16_t y, char ch, FontDef font, uint16_t color, uint16_t bgcolor);
void ST7789_WriteString(uint16_t x, uint16_t y, const char *str, FontDef font, uint16_t color, uint16_t bgcolor);
void ST7789_DrawFilledRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
void ST7789_DrawTriangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint16_t color);
void ST7789_DrawFilledTriangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint16_t color);
void ST7789_DrawFilledCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
void ST7789_TearEffect(uint8_t tear);
void ST7789_Test(void);
#endif /* INC_ST7789_LOW_LEVEL_H_ */
