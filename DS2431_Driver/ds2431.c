/*
 * ds2502.c
 *
 *  Created on: Jun 6, 2025
 *      Author: eldon
 */


#include "ds2431.h"
#include "delay_us.h"
#include <stdio.h>

static void displaydata(uint16_t address, size_t bytesToRead, uint8_t *pdataBuffer);
static void GPIOMode_Input(void);
static void GPIOMode_Output(void);


/*
 * readScratchPad[0] - TA1
 * readScratchPad[1] - TA2
 * readScratchPad[2] - E/S BYTE
 * readScratchPad[10:3] - Scratch pad Data bytes
 */
uint8_t readScratchPad[11];

void OneWire_Reset(void) {
	// Generate 1-Wire reset pulse

	//config GPIO to output for master to initiate reset pulse
	GPIOMode_Output();
	HAL_GPIO_WritePin(ONE_WIRE_PORT, ONE_WIRE_PIN, GPIO_PIN_RESET);
	delay_us(480);  // Reset pulse duration
	HAL_GPIO_WritePin(ONE_WIRE_PORT, ONE_WIRE_PIN, GPIO_PIN_SET);

	//configure GPIO as input so slave could take over the line
	GPIOMode_Input();
	delay_us(400);   // Presence pulse wait
}

void OneWire_WriteByte(uint8_t byte) {
	// Send 8 bits (LSB first)
	GPIOMode_Output();

	for (int i = 0; i < 8; i++) {

		//initiate write time slot by pulling line to gnd for 6us. 1us <= tlow1 <= 15us DS limits
		HAL_GPIO_WritePin(ONE_WIRE_PORT, ONE_WIRE_PIN, GPIO_PIN_RESET);
		delay_us(6);

		//send data bit then wait 55us for slave to sample data.
		HAL_GPIO_WritePin(ONE_WIRE_PORT, ONE_WIRE_PIN, (byte & (1 << i)) ? GPIO_PIN_SET : GPIO_PIN_RESET);
		delay_us(55);  // Timing for bit transmission

		//pull the line back to IDLE state (high) and hold for 5us to prepare for next write time slot
		//60us <= tslot <= 120us DS limits.
		HAL_GPIO_WritePin(ONE_WIRE_PORT, ONE_WIRE_PIN, GPIO_PIN_SET);
		delay_us(5);
	}
}


uint8_t OneWire_ReadByte(void) {
	uint8_t data = 0;

	// Read 8 bits (LSB first)
	for (int i = 0; i < 8; i++) {

		GPIOMode_Output();
		HAL_GPIO_WritePin(ONE_WIRE_PORT, ONE_WIRE_PIN, GPIO_PIN_RESET);
		delay_us(2); // Initiate read time slot
		HAL_GPIO_WritePin(ONE_WIRE_PORT, ONE_WIRE_PIN, GPIO_PIN_SET);
		GPIOMode_Input();
		delay_us(3); // Data acquisition timing

		if (HAL_GPIO_ReadPin(ONE_WIRE_PORT, ONE_WIRE_PIN)) {
			data |= (1 << i);
		}
		delay_us(50); // Ensure proper slot timing
	}

	return data;
}


void OneWire_WriteMemory(uint16_t address, uint8_t *pdata)
{
	uint16_t CRC16;

	OneWire_Reset();

	//write skip ROM command
	OneWire_WriteByte(0xCC);

	//write scratch pad command
	OneWire_WriteByte(0x0F);

	OneWire_WriteByte( (uint8_t) (address & 0xFF) ); // TA1

	OneWire_WriteByte( (uint8_t) ((address >> 8) & 0xFF) ); // TA2

	//write 8 bytes of data into scratch pad
	for(uint8_t i = 0 ; i < 8 ; i++)
		OneWire_WriteByte(pdata[i]); // Data Byte

	//received generated 16bit CRC by DS2431
	CRC16 = OneWire_ReadByte();
	CRC16 |= (OneWire_ReadByte() << 8);

	Read_ScratchPad(pdata);

	Copy_ScratchPad();

}



void OneWire_ReadMemory(uint8_t address, size_t bytesToRead, uint8_t *pdataBuffer)
{

	uint16_t endaddress = (address + bytesToRead) - 1;

	if(endaddress > boundaryaddress)
	{
		printf("error! Read exceed boundary address");
		return;
	}

	OneWire_Reset();

	//skip ROM command
	OneWire_WriteByte(0xCC);

	//Read memory command
	OneWire_WriteByte(0xF0);

	OneWire_WriteByte( (uint8_t) (address & 0xFF) ); // TA1

	OneWire_WriteByte( (uint8_t) ((address >> 8) & 0xFF) ); // TA2

	for(uint32_t i=0 ; i < bytesToRead ; i++)
	{
		//Save read byte to rxbuffer
		pdataBuffer[i] = OneWire_ReadByte();

	}

	displaydata(address, bytesToRead, pdataBuffer);
}




static void displaydata(uint16_t address, size_t bytesToRead, uint8_t *pdataBuffer)
{
	uint8_t newlineflag = 0;
	printf(" 0x%2X: ", address);

	for(uint32_t i=0 ; i < bytesToRead ; i++)
	{
		if(newlineflag == 8)
		{
			address +=8;
			printf("\n 0x%2X: ", address);
			newlineflag = 0;
		}

		printf("%c ", pdataBuffer[i]);

		newlineflag++;
	}
}


static void GPIOMode_Input(void)
{
	// Reconfigure the pin as input to allow slave to pull the bus low
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	GPIO_InitStruct.Pin = ONE_WIRE_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;  // Set pin to input mode
	GPIO_InitStruct.Pull = GPIO_NOPULL;     // No pull-up or pull-down
	HAL_GPIO_Init(ONE_WIRE_PORT, &GPIO_InitStruct);
}

static void GPIOMode_Output(void)
{
	// Reconfigure the pin as input to allow slave to pull the bus low
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	GPIO_InitStruct.Pin = ONE_WIRE_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;  // Set pin to input mode
	GPIO_InitStruct.Pull = GPIO_NOPULL;     // No pull-up or pull-down
	HAL_GPIO_Init(ONE_WIRE_PORT, &GPIO_InitStruct);
}


void Read_ScratchPad(uint8_t *pdata)
{
	uint16_t CRC16;

	OneWire_Reset();

	//ROM skip command
	OneWire_WriteByte(0xCC);

	//read scratch pad data
	OneWire_WriteByte(0xAA);

	//receive TA1, TA2, E/S BYTE & 8 BYTES OF DATA
	for(uint32_t i=0 ; i << 11 ; i++)
		readScratchPad[i] = OneWire_ReadByte();


	//received generated 16bit CRC by DS2431
	CRC16 = OneWire_ReadByte();
	CRC16 |= (OneWire_ReadByte() << 8);

	for(uint32_t i=0 ; i<8 ; i++)
	{
		if(readScratchPad[i+3] != pdata[i])
		{
			printf("Error! scratch pad data does not match with written data!!");
			OneWire_Reset();
			return;
		}
	}

	OneWire_Reset();

}


void Copy_ScratchPad()
{

	OneWire_Reset();

	//ROM skip command
	OneWire_WriteByte(0xCC);

	//copy scratch pad data
	OneWire_WriteByte(0x55);

	//send TA1
	OneWire_WriteByte(readScratchPad[0]);

	//send TA2
	OneWire_WriteByte(readScratchPad[1]);

	//send E/S BYTE
	OneWire_WriteByte(readScratchPad[2]);

	//check if copy data to memory address is success
	if(OneWire_ReadByte() == 0)
	{
		printf("Successfully written data at memory address: 0x%X", ( readScratchPad[0] | (readScratchPad[1] << 8) ) );
		OneWire_Reset();
	}
	else
	{
		printf("Error writing at memory address: 0x%X", ( readScratchPad[0] | (readScratchPad[1] << 8) ) );
		OneWire_Reset();
	}

}
