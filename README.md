# DS2431 1Wire EEPROM Driver
This is a 1-Wire protocol driver for the DS2431 chip, a 1024-bit 1-Wire EEPROM commonly used as an ID chip in laptop power supplies. I developed this driver to read and program the DS2431, enabling it to function as a Dell laptop power supply ID chip for my laptop supply spoofer project.

The driver is designed for STM32F4 MCUs. To port it to other STM32 families, you can modify the MCU-specific HAL header file and update the peripheral addresses in the ds2431.h file accordingly.

<img width="857" height="578" alt="image" src="https://github.com/user-attachments/assets/083e8c6a-a2a7-4671-a7d8-13523d15c728" />


Just make sure your MCU has a basic timer peripheral for microsecond delay functionality that the MCU needs to communicate with the EEPROM chip. 

**HOW TO USE THE DRIVER**

**OneWire_init():** 
Configures the GPIO and TIMER for a 1-Wire communication. Call this function first before calling other 1-Wire commands.

<img width="621" height="316" alt="image" src="https://github.com/user-attachments/assets/dcd44394-15c9-435b-b5d9-1978b2487c28" />

**********************************************************************************************************
**OneWire_ReadROM():**
Reads and displays data from DS2431 EEPROM memory.
* @param: uint16_t startaddress, start address of EEPROM read.
* @param: size_t bytesToRead, number of bytes to read from start address.
* @param: uint8_t *pdataBuffer, pointer to data buffer where read data will be stored.
<img width="768" height="155" alt="image" src="https://github.com/user-attachments/assets/a546dbbe-2c8f-46cd-917d-ee26c8025621" />


**********************************************************************************************************

**OneWire_WriteMemory:**
Writes data into DS2431 EEPROM memory.
* @param: uint16_t startaddress, start address of EEPROM write.
* @param: uint8_t *pdata, pointer to data source buffer.
   

**EEPROM WRITING GUIDELINES**
1. Write in 8-byte chunks: You can only write full 8-byte rows at a time.
2. Use 8-byte aligned addresses: Writes must start at addresses divisible by 8 (e.g., 0x00, 0x08, 0x10…).
 <img width="474" height="107" alt="image" src="https://github.com/user-attachments/assets/a386769e-cade-4a4b-a397-8c0c7f263055" />
 
 <img width="534" height="29" alt="image" src="https://github.com/user-attachments/assets/220ce1a0-e2e4-405d-9b88-e168e5086099" />


**********************************************************************************************************


**OneWire_WriteByte:**
Writes a byte of data into the 1-Wire bus
* @param: uint8_t byte, the byte to write to the bus.

<img width="326" height="22" alt="image" src="https://github.com/user-attachments/assets/8c3f45d8-92ad-42b6-8474-09489ecfaf86" />

**********************************************************************************************************
**OneWire_Reset():**
MCU sends a reset pulse to ds2431

<img width="240" height="26" alt="image" src="https://github.com/user-attachments/assets/9a1224c9-ab93-4943-9512-b6fcc5cb6fdb" />

**********************************************************************************************************
**OneWire_ReadByte:**
Reads a byte of data from 1-Wire bus.

<img width="250" height="23" alt="image" src="https://github.com/user-attachments/assets/2fec2f8e-47e7-401d-95ee-6e06bf103297" />




