/*
 * GB_MFRC522.c
 *
 *  Created on: Mar 23, 2023
 *      Author: Kunal Gupta
 *      Description: MFRC522 RFID Module driver for S32K144 MCU.
 */


#include "GB_MMFRC522.h"
#include "stdint.h"
#include "peripherals_lpspi_1.h"

void yield(void)
{
	while(1);
}

#define gb_MFRC522_CE_pin_low() LPSPI_DRV_SetPcs(0, LPSPI_PCS0, LPSPI_ACTIVE_LOW) //Slave select pin LOW

#define gb_MFRC522_CE_pin_high()    LPSPI_DRV_SetPcs(0, LPSPI_PCS0, LPSPI_ACTIVE_HIGH)//Slave select pin HIGH

// Member variables
Uid UID_t;

void GB_MFRC522_WriteRegister(PCD_Register_t GB_reg, uint8_t value)
{
	gb_MFRC522_CE_pin_low();
	GB_MA_SPI0_send_byte_conti(GB_reg);
	GB_MA_SPI0_send_byte_conti(value);
	gb_MFRC522_CE_pin_high();
}

void GB_MFRC522_WriteRegister_Multi(PCD_Register_t GB_reg, byte count, uint8_t *value)
{
	gb_MFRC522_CE_pin_low();
	GB_MA_SPI0_send_byte_conti(GB_reg);
	for (uint8_t i =0;i<count;i++){
		GB_MA_SPI0_send_byte_conti(value[i]);
	}
	gb_MFRC522_CE_pin_high();
}


uint8_t GB_MFRC522_ReadRegister(PCD_Register_t GB_reg)
{
	uint8_t gb_x = 0;
	gb_MFRC522_CE_pin_low();

	GB_MA_SPI0_exchange_byte((0x80 | GB_reg));
//	GB_MA_SPI0_send_byte_conti(0x80 | GB_reg);
    gb_x = GB_MA_SPI0_exchange_byte(0);
//	gb_x = GB_MA_SPI0_read_byte();
	gb_MFRC522_CE_pin_high();
	return gb_x;
}

void GB_MFRC522_ReadRegisterMulti(PCD_Register_t reg, byte count, byte *values, byte rxalign)
{
	if (count == 0) {
		return;
	}
	byte address = 0x80 | reg;				// MSB == 1 is for reading. LSB is not used in address. Datasheet section 8.1.2.3.
	byte index = 0;
	gb_MFRC522_CE_pin_low();
	count--;								// One read is performed outside of the loop
	GB_MA_SPI0_exchange_byte(address);      // Tell MFRC522 which address we want to read
	if (rxalign) {		// Only update bit positions rxAlign..7 in values[0]
		// Create bit mask for bit positions rxAlign..7
		byte mask = (0xFF << rxalign) & 0xFF;
		// Read value and tell that we want to read the same address again.
		byte value = GB_MA_SPI0_exchange_byte(address);
		// Apply mask to both current value of values[0] and the new data in value.
		values[0] = (values[0] & ~mask) | (value & mask);
		index++;
	}
while (index < count) {
	values[index] = GB_MA_SPI0_exchange_byte(address);	// Read value and tell that we want to read the same address again.
	index++;
}
values[index] = GB_MA_SPI0_exchange_byte(0);			// Read the final byte. Send 0 to stop reading.

	gb_MFRC522_CE_pin_high();

}

void GB_MFRC522_ClearRegisterBitMask(PCD_Register_t gb_reg, byte mask)
{
	byte tmp;
	tmp = GB_MFRC522_ReadRegister(gb_reg);
	GB_MFRC522_WriteRegister(gb_reg, tmp & (~mask));     //clear bit mask
}

void GB_MFRC522_SetRegisterBitmask( PCD_Register_t gb_reg, byte mask)
{
	byte tmp;
	tmp = GB_MFRC522_ReadRegister(gb_reg);
	GB_MFRC522_WriteRegister(gb_reg, tmp | (mask));     //clear bit mask
}
void GB_MFRC522Version()
{
	uint8_t v =  GB_MFRC522_ReadRegister(VersionReg);
	GB_printString0("Firmware Version: 0d");
//	GB_decimel0(v);
	switch(v)
   {
		case 0x88: GB_printString0(" = (clone)\n"); break;
		case 0x90: GB_printString0(" = v0.0\n"); break;
		case 0x91: GB_printString0(" = v1.0\n"); break;
		case 0x92: GB_printString0(" = v2.0\n"); break;
		case 0x12: GB_printString0(" = counterfeit chip\n"); break;
		default: GB_printString0(" = (unknown)\n");
	}
}

void MFRC522_init()
{
	GB_MFRC522_WriteRegister(TxModeReg, 0x00);
	GB_MFRC522_WriteRegister(RxModeReg, 0x00);

	GB_MFRC522_WriteRegister(ModWidthReg, 0x26);

	// When communicating with a PICC we need a timeout if something goes wrong.
	// f_timer = 13.56 MHz / (2*TPreScaler+1) where TPreScaler = [TPrescaler_Hi:TPrescaler_Lo].
	// TPrescaler_Hi are the four low bits in TModeReg. TPrescaler_Lo is TPrescalerReg.
	GB_MFRC522_WriteRegister(TModeReg, 0x80);          // TAuto=1; timer starts automatically at the end of the transmission in all communication modes at all speeds
	GB_MFRC522_WriteRegister(TPrescalerReg, 0xA9);     // TPreScaler = TModeReg[3..0]:TPrescalerReg, ie 0x0A9 = 169 => f_timer=40kHz, ie a timer period of 25μs.
	GB_MFRC522_WriteRegister(TReloadRegH, 0x03);       // Reload timer with 0x3E8 = 1000, ie 25ms before timeout.
	GB_MFRC522_WriteRegister(TReloadRegL, 0xE8);


	GB_MFRC522_WriteRegister(TxASKReg, 0x40);// Default 0x00. Force a 100 % ASK modulation independent of the ModGsPReg register setting
	GB_MFRC522_WriteRegister(ModeReg, 0x3D); // Default 0x3F. Set the preset value for the CRC co processor for the CalcCRC command to 0x6363 (ISO 14443-3 part 6.2.4)		// Enable the antenna driver pins TX1 and TX2 (they were disabled by the reset)

	GB_MFRC522_AnteenaOn();                  //Enable the Antenna Driver pins TX1 & TX2(They are disabled by reset)

}

void GB_MFRC522_AnteenaOn()
{
	uint8_t gb_x = GB_MFRC522_ReadRegister(TxControlReg);
	if((gb_x & 0x03) != 0x03)
	{
		GB_MFRC522_WriteRegister(TxControlReg, (gb_x | 0x03));
	}
}

gb_MFRC522_statusCodes_t PCD_CalculateCRC(byte *data,  byte length,	byte *result)
{
GB_MFRC522_WriteRegister(CommandReg, PCD_Idle);		// Stop any active command.
GB_MFRC522_WriteRegister(DivIrqReg, 0x04);				// Clear the CRCIRq interrupt request bit
GB_MFRC522_WriteRegister(FIFOLevelReg, 0x80);			// FlushBuffer = 1, FIFO initialization
GB_MFRC522_WriteRegister_Multi(FIFODataReg, length, data);	// Write data to the FIFO
GB_MFRC522_WriteRegister(CommandReg, PCD_CalcCRC);		// Start the calculation

// Wait for the CRC calculation to complete. Check for the register to
// indicate that the CRC calculation is complete in a loop. If the
// calculation is not indicated as complete in ~90ms, then time out
// the operation.
const uint32_t deadline = GB_millis() + 89;

do {
	// DivIrqReg[7..0] bits are: Set2 reserved reserved MfinActIRq reserved CRCIRq reserved reserved
	byte n = GB_MFRC522_ReadRegister(DivIrqReg);
	if (n & 0x04) {									// CRCIRq bit set - calculation done
		GB_MFRC522_WriteRegister(CommandReg, PCD_Idle);	// Stop calculating CRC for new content in the FIFO.
		// Transfer the result from the registers to the result buffer
		result[0] = GB_MFRC522_ReadRegister(CRCResultRegL);
		result[1] = GB_MFRC522_ReadRegister(CRCResultRegH);
		return MFRC522_STATUS_OK;
	}
//	yield();
}
while ((GB_millis()) < deadline);

// 89ms passed and nothing happened. Communication with the MFRC522 might be down.
return STATUS_TIMEOUT;
} // End PCD_CalculateCRC()


gb_MFRC522_statusCodes_t GB_MFRC522_CommunicateWithPICC(byte command,   //The command to execute
                                                      byte waitIRQ,
													  byte *sendData,
													  byte sendLen,
													  byte *backdata,
													  byte *backlen,
													  byte *validbits,
													  byte rxalign,
													bool checkCRC
													)
{
	byte txLastBits = validbits ? *validbits :0;
	byte bitFraming = (rxalign <<4) + txLastBits;    //RxAlign = BitFramingReg[6..4]. TxLastBits = BitFramingReg[2..0]

	GB_MFRC522_WriteRegister(CommandReg, PCD_Idle);			// Stop any active command.
	GB_MFRC522_WriteRegister(ComIrqReg, 0x7F);					// Clear all seven interrupt request bits

	// For PCD to PICC Communication!!
	GB_MFRC522_WriteRegister(FIFOLevelReg, 0x80);				// FlushBuffer = 1, FIFO initialization
	GB_MFRC522_WriteRegister_Multi(FIFODataReg, sendLen, sendData);	// Write sendData to the FIFO
	GB_MFRC522_WriteRegister(BitFramingReg, bitFraming);		// Bit adjustments
	GB_MFRC522_WriteRegister(CommandReg, command);				// Execute the command
	if(command = PCD_Transceive){
		GB_MFRC522_SetRegisterBitmask(BitFramingReg, 0x80);     //StartSend=1, transmission of data starts
	}

	const uint32_t deadline = GB_millis()+36;
	bool completed = false;
	do {
		byte n = GB_MFRC522_ReadRegister(ComIrqReg);	// ComIrqReg[7..0] bits are: Set1 TxIRq RxIRq IdleIRq HiAlertIRq LoAlertIRq ErrIRq TimerIRq
		if (n & waitIRQ) {					// One of the interrupts that signal success has been set.
	//	GB_printString0("signal success\n");
			completed = true;
			break;
		}
		if (n & 0x01) {						// Timer interrupt - nothing received in 25ms
		//	GB_printString0("Timeout\n");
			return STATUS_TIMEOUT;
		}
	//yield();
	}
	while ((GB_millis()) < deadline);

	int cycles = 720000;
	    	while(cycles--);
	  //_delay_ms(30);
//36ms and nothing happened. Communication with the MFRC522 might be down.
if (!completed) {
	return STATUS_TIMEOUT;
}

//Stop now if any errors except collisions were detected.
byte errorRegValue = GB_MFRC522_ReadRegister(ErrorReg); // ErrorReg[7..0] bits are: WrErr TempErr reserved BufferOvfl CollErr CRCErr ParityErr ProtocolErr
if (errorRegValue & 0x13) {	 // BufferOvfl ParityErr ProtocolErr
	GB_printString0("Some error \n");
	return MFRC522_STATUS_ERROR;
}

byte _validbits = 0;
//If the caller wants data back, get it from the MFRC522.
if(backdata && backlen) {
	byte n = GB_MFRC522_ReadRegister(FIFOLevelReg);
// 	GB_printString0("FIFO len");
	//GB_decimel0(n);
	//GB_printString0("\n");
	if(n > *backlen){
		return MFRC522_STATUS_NO_ROOM;
	}

	*backlen = n;
	GB_MFRC522_ReadRegisterMulti(FIFODataReg, n, backdata, rxalign);
// 	GB_printString0("FIFO len");
// 	GB_decimel0(*backdata);
//	GB_printString0("\n");
	_validbits = GB_MFRC522_ReadRegister(ControlReg) & 0x07;
	if(validbits){
		*validbits = _validbits;
			}
}
// Tell about collisions
if (errorRegValue & 0x08) {		// CollErr
	return MFRC522_STATUS_COLLISION;
}

// Perform CRC_A validation if requested.
if (backdata && backlen && checkCRC) {
	// In this case a MIFARE Classic NAK is not OK.
	if (*backlen == 1 && _validbits == 4) {
		return MFRC522_STATUS_MIFARE_NACK;
	}
	// We need at least the CRC_A value and all 8 bits of the last byte must be received.
	if (*backlen < 2 || _validbits != 0) {
		return MFRC522_STATUS_CRC_WRONG;
	}
	// Verify CRC_A - do our own calculation and store the control in controlBuffer.
	byte controlBuffer[2];
	gb_MFRC522_statusCodes_t status = PCD_CalculateCRC(&backdata[0], *backlen - 2, &controlBuffer[0]);
	if (status != MFRC522_STATUS_OK) {
		return status;
	}
	if ((backdata[*backlen - 2] != controlBuffer[0]) || (backdata[*backlen - 1] != controlBuffer[1])) {
		return MFRC522_STATUS_CRC_WRONG;
	}
}

			return MFRC522_STATUS_OK;
}//return MFRC522_STATUS_OK;
gb_MFRC522_statusCodes_t GB_MFRC522_TransceiveData( byte *senddata,  //Pointer to the data to transfer to the FIFO
                                                  byte sendlen,       //Number of bytes to transfer to FIFO
												  byte *backdata,     //nullptr or pointer to buffer if data should be read back after executing the command
												  byte *backlen,      //In: Max no of bytes to write to *backdata. Out: The no of bytes returned.
												  byte *validbits,    //In/Out: The number of valid bits in the last byte
												  byte rxalign,
												  bool checkCRC       //In: True => The last 2 bytes of the response is assumed to be a CRC_A that must be validated
												  )
{
	byte waitIRQ = 0x30;
// 	GB_printString0("uy\n");
// 	GB_decimel0(rxalign);
// 	GB_printString0("\n");
// 	GB_decimel0(checkCRC);
// 	GB_printString0("\n");

	int cycles = 7200000;
	    	while(cycles--);
	//_delay_ms(100);
	return GB_MFRC522_CommunicateWithPICC(PCD_Transceive, waitIRQ, senddata, sendlen, backdata, backlen, validbits, rxalign, checkCRC);
}


gb_MFRC522_statusCodes_t GB_PICC_REQA_OR_WUPA(byte command, byte *gb_bufferATQA, byte *gb_buffersize)
{
	gb_MFRC522_statusCodes_t status;
	byte gb_validbits;
	if(gb_bufferATQA == 0 || *gb_buffersize < 2) { // As gb_bufferATQA is a buffer to store the Answer to request, when command REQ is send by PCD.
	 return MFRC522_STATUS_NO_ROOM;                                                   //So this buffer should be pointing to some valid pointer and not to null pointer. And buffer
	}												   //size should be greater then 2 bytes, ATQA is of 2 bytes.

	GB_MFRC522_ClearRegisterBitMask(CollReg,0x80);
	gb_validbits = 7;

	status = GB_MFRC522_TransceiveData(&command, 1, gb_bufferATQA, gb_buffersize, &gb_validbits, 0, 0);
// 	GB_printString0("status -->");
// 	GB_decimel0(status);
// 	GB_printString0("\n");
	if( status != MFRC522_STATUS_OK);
	return status;

	if(*gb_buffersize !=2 || gb_validbits != 0){
		return MFRC522_STATUS_ERROR;
	}

	return MFRC522_STATUS_OK;

}
gb_MFRC522_statusCodes_t GB_PICC_RequestA(byte * bufferATQ, byte *buffersize)
{
	return GB_PICC_REQA_OR_WUPA(PICC_CMD_REQA, bufferATQ, buffersize);
}
bool GB_PICC_IsNewCardPresent(void)
{
	byte gb_bufferATQ[2]; // We will be sending the Request command. That is in order to detect the PICCs which are in the operating field
	                     // PCD shall send repeated request commands. PCD will send REQ or WUP in any sequence to detect the PICCs.
						 //REQ commands are transmitted via short frame If PICC is in energizing field for PCD and gets powered up,
						 //it will start listening for valid REQ command. And transmits its ATQ(Answer to request).
						 //This answer to Request is stored in this buffer. ATQA is a 2 byte number that is returned by PICC.
	byte gb_buffersize = sizeof(gb_bufferATQ);
	// Reset baud rates
	GB_MFRC522_WriteRegister(TxModeReg, 0x00);
	GB_MFRC522_WriteRegister(RxModeReg, 0x00);
	// Reset ModWidthReg
	GB_MFRC522_WriteRegister(ModWidthReg, 0x26);//38 in decimal

   gb_MFRC522_statusCodes_t result = GB_PICC_RequestA(gb_bufferATQ, &gb_buffersize);
    GB_decimel0(result);
    GB_printString0("\n");
   return (result == MFRC522_STATUS_OK || result == MFRC522_STATUS_COLLISION);

}



/**
 * Transmits SELECT/ANTICOLLISION commands to select a single PICC.
 * Before calling this function the PICCs must be placed in the READY(*) state by calling PICC_RequestA() or PICC_WakeupA().
 * On success:
 * 		- The chosen PICC is in state ACTIVE(*) and all other PICCs have returned to state IDLE/HALT. (Figure 7 of the ISO/IEC 14443-3 draft.)
 * 		- The UID size and value of the chosen PICC is returned in *uid along with the SAK.
 *
 * A PICC UID consists of 4, 7 or 10 bytes.
 * Only 4 bytes can be specified in a SELECT command, so for the longer UIDs two or three iterations are used:
 * 		UID size	Number of UID bytes		Cascade levels		Example of PICC
 * 		========	===================		==============		===============
 * 		single				 4						1				MIFARE Classic
 * 		double				 7						2				MIFARE Ultralight
 * 		triple				10						3				Not currently in use?
 *
 * @return MFRC522_STATUS_OK on success, STATUS_??? otherwise.
 */

gb_MFRC522_statusCodes_t GB_PICC_Select(	Uid *uid,			///< Pointer to Uid struct. Normally output, but can also be used to supply a known UID.
											byte validBits		///< The number of known UID bits supplied in *uid. Normally 0. If set you must also supply uid->size.
										 ) {
	bool uidComplete;
	bool selectDone;
	bool useCascadeTag;
	byte cascadeLevel = 1;
	gb_MFRC522_statusCodes_t result;
	byte count;
	byte checkBit;
	byte index;
	byte uidIndex;					// The first index in uid->uidByte[] that is used in the current Cascade Level.
	int8_t currentLevelKnownBits;		// The number of known UID bits in the current Cascade Level.
	byte buffer[9];					// The SELECT/ANTICOLLISION commands uses a 7 byte standard frame + 2 bytes CRC_A
	byte bufferUsed;				// The number of bytes used in the buffer, ie the number of bytes to transfer to the FIFO.
	byte rxAlign;					// Used in BitFramingReg. Defines the bit position for the first bit received.
	byte txLastBits;				// Used in BitFramingReg. The number of valid bits in the last transmitted byte.
	byte *responseBuffer;
	byte responseLength;

	// Description of buffer structure:
	//		Byte 0: SEL 				Indicates the Cascade Level: PICC_CMD_SEL_CL1, PICC_CMD_SEL_CL2 or PICC_CMD_SEL_CL3
	//		Byte 1: NVB					Number of Valid Bits (in complete command, not just the UID): High nibble: complete bytes, Low nibble: Extra bits.
	//		Byte 2: UID-data or CT		See explanation below. CT means Cascade Tag.
	//		Byte 3: UID-data
	//		Byte 4: UID-data
	//		Byte 5: UID-data
	//		Byte 6: BCC					Block Check Character - XOR of bytes 2-5
	//		Byte 7: CRC_A
	//		Byte 8: CRC_A
	// The BCC and CRC_A are only transmitted if we know all the UID bits of the current Cascade Level.
	//
	// Description of bytes 2-5: (Section 6.5.4 of the ISO/IEC 14443-3 draft: UID contents and cascade levels)
	//		UID size	Cascade level	Byte2	Byte3	Byte4	Byte5
	//		========	=============	=====	=====	=====	=====
	//		 4 bytes		1			uid0	uid1	uid2	uid3
	//		 7 bytes		1			CT		uid0	uid1	uid2
	//						2			uid3	uid4	uid5	uid6
	//		10 bytes		1			CT		uid0	uid1	uid2
	//						2			CT		uid3	uid4	uid5
	//						3			uid6	uid7	uid8	uid9

	// Sanity checks
	if (validBits > 80) {
		return MFRC522_STATUS_INVALID;
	}

	// Prepare MFRC522
	GB_MFRC522_ClearRegisterBitMask(CollReg, 0x80);		// ValuesAfterColl=1 => Bits received after collision are cleared.

	// Repeat Cascade Level loop until we have a complete UID.
	uidComplete = false;
	while (!uidComplete) {
		// Set the Cascade Level in the SEL byte, find out if we need to use the Cascade Tag in byte 2.
		switch (cascadeLevel) {
			case 1:
				buffer[0] = PICC_CMD_SEL_CL1;
				uidIndex = 0;
				useCascadeTag = validBits && uid->size > 4;	// When we know that the UID has more than 4 bytes
				break;

			case 2:
				buffer[0] = PICC_CMD_SEL_CL2;
				uidIndex = 3;
				useCascadeTag = validBits && uid->size > 7;	// When we know that the UID has more than 7 bytes
				break;

			case 3:
				buffer[0] = PICC_CMD_SEL_CL3;
				uidIndex = 6;
				useCascadeTag = false;						// Never used in CL3.
				break;

			default:
				return MFRC522_STATUS_INTERNAL_ERROR;
				break;
		}

// 		GB_decimel0(validBits);
// 		GB_printString0("\n");
// 		GB_decimel0(uidIndex);
// 		GB_printString0("\n");
// 		GB_decimel0(useCascadeTag);
// 		GB_printString0("\n");
		// How many UID bits are known in this Cascade Level?
		currentLevelKnownBits = validBits - (8 * uidIndex);
		if (currentLevelKnownBits < 0) {
			currentLevelKnownBits = 0;
		}
		// Copy the known bits from uid->uidByte[] to buffer[]
		index = 2; // destination index in buffer[]
		if (useCascadeTag) {
			buffer[index++] = PICC_CMD_CT; // buffer[3]
		}
		byte bytesToCopy = currentLevelKnownBits / 8 + (currentLevelKnownBits % 8 ? 1 : 0); // The number of bytes needed to represent the known bits for this level.
// 		GB_decimel0(bytesToCopy);
// 		GB_printString0("\n");
		if (bytesToCopy) {
			byte maxBytes = useCascadeTag ? 3 : 4; // Max 4 bytes in each Cascade Level. Only 3 left if we use the Cascade Tag
			if (bytesToCopy > maxBytes) {
				bytesToCopy = maxBytes;
			}
			for (count = 0; count < bytesToCopy; count++) {
				buffer[index++] = uid->uidByte[uidIndex + count];
			}
		}
		// Now that the data has been copied we need to include the 8 bits in CT in currentLevelKnownBits
		if (useCascadeTag) {
			currentLevelKnownBits += 8;
		}

		// Repeat anti collision loop until we can transmit all UID bits + BCC and receive a SAK - max 32 iterations.
		selectDone = false;
		while (!selectDone) {
			// Find out how many bits and bytes to send and receive.
			if (currentLevelKnownBits >= 32) { // All UID bits in this Cascade Level are known. This is a SELECT.
				//Serial.print(F("SELECT: currentLevelKnownBits=")); Serial.println(currentLevelKnownBits, DEC);
// 						GB_decimel0(buffer[1]);
// 						GB_printString0("o\n");
				buffer[1] = 0x70; // NVB - Number of Valid Bits: Seven whole bytes

// 					GB_decimel0(buffer[1]);
// 					GB_printString0("\n");
				// Calculate BCC - Block Check Character
				buffer[6] = buffer[2] ^ buffer[3] ^ buffer[4] ^ buffer[5];
// 						GB_decimel0(buffer[6]);
// 						GB_printString0("o\n");
				// Calculate CRC_A
				result = PCD_CalculateCRC(buffer, 7, &buffer[7]);
 						GB_decimel0(result);
 						GB_printString0("\n");
				if (result != MFRC522_STATUS_OK) {
					return result;
				}
			//	GB_printString0("z\n");
				txLastBits		= 0; // 0 => All 8 bits are valid.
				bufferUsed		= 9;
				// Store response in the last 3 bytes of buffer (BCC and CRC_A - not needed after tx)
				responseBuffer	= &buffer[6];
				responseLength	= 3;
			}
			else { // This is an ANTICOLLISION/SELECT
				//Serial.print(F("ANTICOLLISION: currentLevelKnownBits=")); Serial.println(currentLevelKnownBits, DEC);
				txLastBits		= currentLevelKnownBits % 8;
				count			= currentLevelKnownBits / 8;	// Number of whole bytes in the UID part.
				index			= 2 + count;					// Number of whole bytes: SEL + NVB + UIDs
				buffer[1]		= (index << 4) + txLastBits;	// NVB - Number of Valid Bits
// 				GB_decimel0(buffer[1]);
// 				GB_printString0("\n");
				bufferUsed		= index + (txLastBits ? 1 : 0);
				// Store response in the unused part of buffer
				responseBuffer	= &buffer[index];
				responseLength	= sizeof(buffer) - index;
			}

			// Set bit adjustments
			rxAlign = txLastBits;											// Having a separate variable is overkill. But it makes the next line easier to read.
			GB_MFRC522_WriteRegister(BitFramingReg, (rxAlign << 4) + txLastBits);	// RxAlign = BitFramingReg[6..4]. TxLastBits = BitFramingReg[2..0]

// 			for (int i =0; i<bufferUsed; i++)
// 	{	GB_decimel0(buffer[i]);
// 		GB_printString0(" ");
// 	}
	//	GB_printString0("\n");
// 			GB_decimel0(bufferUsed);
// 			GB_printString0("\n");
			// Transmit the buffer and receive the response.
			result = GB_MFRC522_TransceiveData(buffer, bufferUsed, responseBuffer, &responseLength, &txLastBits, rxAlign, 0);
// 			GB_decimel0(result);
// 			GB_printString0("\n");
			//GB_decimel0(selectDone);
// 						for (int i =0; i<responseLength; i++)
// 						{
// 							GB_decimel0(responseBuffer[i]);
// 							GB_printString0(" ");
// 						}
			if (result == MFRC522_STATUS_COLLISION) { // More than one PICC in the field => collision.
				byte valueOfCollReg = GB_MFRC522_ReadRegister(CollReg); // CollReg[7..0] bits are: ValuesAfterColl reserved CollPosNotValid CollPos[4:0]
				if (valueOfCollReg & 0x20) { // CollPosNotValid
					return MFRC522_STATUS_COLLISION; // Without a valid collision position we cannot continue
				}
				byte collisionPos = valueOfCollReg & 0x1F; // Values 0-31, 0 means bit 32.
				if (collisionPos == 0) {
					collisionPos = 32;
				}
				if (collisionPos <= currentLevelKnownBits) { // No progress - should not happen
					return MFRC522_STATUS_INTERNAL_ERROR;
				}
				// Choose the PICC with the bit set.
				currentLevelKnownBits	= collisionPos;
				count			= currentLevelKnownBits % 8; // The bit to modify
				checkBit		= (currentLevelKnownBits - 1) % 8;
				index			= 1 + (currentLevelKnownBits / 8) + (count ? 1 : 0); // First byte is index 0.
				buffer[index]	|= (1 << checkBit);
			}
			else if (result != MFRC522_STATUS_OK) {
				return result;
			}
			else { // MFRC522_STATUS_OK
				if (currentLevelKnownBits >= 32) { // This was a SELECT.
					selectDone = true; // No more anticollision
// 					GB_decimel0(currentLevelKnownBits);
// 					GB_printString0("2)");
					// We continue below outside the while.
				}
				else { // This was an ANTICOLLISION.
					// We now have all 32 bits of the UID in this Cascade Level
// 					GB_printString0("\n");
// 								GB_decimel0(currentLevelKnownBits);
// 								GB_printString0("\n");
// 					GB_printString0("1)");
					currentLevelKnownBits = 32;
					// Run loop again to do the SELECT.
				}
			}
		} // End of while (!selectDone)

		// We do not check the CBB - it was constructed by us above.

//		GB_printString0("3");
		// Copy the found UID bytes from buffer[] to uid->uidByte[]
		index			= (buffer[2] == PICC_CMD_CT) ? 3 : 2; // source index in buffer[]
		bytesToCopy		= (buffer[2] == PICC_CMD_CT) ? 3 : 4;
		for (count = 0; count < bytesToCopy; count++) {
			uid->uidByte[uidIndex + count] = buffer[index++];
		}

		// Check response SAK (Select Acknowledge)
		if (responseLength != 3 || txLastBits != 0) { // SAK must be exactly 24 bits (1 byte + CRC_A).
			return MFRC522_STATUS_ERROR;
		}
		// Verify CRC_A - do our own calculation and store the control in buffer[2..3] - those bytes are not needed anymore.
		result = PCD_CalculateCRC(responseBuffer, 1, &buffer[2]);
		if (result != MFRC522_STATUS_OK) {
			return result;
		}
		if ((buffer[2] != responseBuffer[1]) || (buffer[3] != responseBuffer[2])) {
			return MFRC522_STATUS_CRC_WRONG;
		}
		if (responseBuffer[0] & 0x04) { // Cascade bit set - UID not complete yes
			cascadeLevel++;
		}
		else {
			uidComplete = true;
			uid->sak = responseBuffer[0];
		}
	} // End of while (!uidComplete)

	// Set correct uid->size
	uid->size = 3 * cascadeLevel + 1;

	return MFRC522_STATUS_OK;
} // End PICC_Select()

bool GB_PICC_ReadCardSerial(void)
{
	gb_MFRC522_statusCodes_t result = GB_PICC_Select(&UID_t, 0);
	return (result == MFRC522_STATUS_OK);
}


void GB_PICCDetails(Uid *uid)
{
	// UID
	GB_printString0("Card UID:\n");
	for (byte i = 0; i < uid->size; i++) {
		if(uid->uidByte[i] < 0x10)
		GB_printString0("0");
		else
		GB_printString0(" ");
		GB_decimel0(uid->uidByte[i]);
	}
	GB_printString0("\n");

	// SAK
	GB_printString0("Card SAK: \n");
	if(uid->sak < 0x10)
	GB_printString0("0");
	GB_decimel0(uid->sak);

	// (suggested) PICC type
// 	PICC_Type piccType = PICC_GetType(uid->sak);
// 	Serial.print(F("PICC type: "));
// 	Serial.println(PICC_GetTypeName(piccType));
} // End PICC_DumpDetailsToSerial()
