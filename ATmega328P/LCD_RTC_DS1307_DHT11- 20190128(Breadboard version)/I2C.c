﻿/*************************************
Target MCU & clock speed: ATmega328P @ 1Mhz internal
Name    : I2C.c
Author  : Insoo Kim (insoo@hotmail.com)
Created : 2017-07-13 오후 9:26:09
Updated :

Description: 
	0) Here we code I2C functions from scratch, you can use I2C library of any sort.

	1) I2C functions to deal with TWI protocol SDA, SCL enabled devices.
		such as RTC DS1307 chip.

HEX size[Byte]:

Ref:
**************************************/

 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include <avr/io.h>
 #include <avr/interrupt.h>
 #include <avr/wdt.h>
 #include <avr/sleep.h>
 #include "externs.h"
 #include "defines.h"
 #include <util/delay.h>

//----------------------------------
void I2C_Init()
// at 16 MHz, the SCL frequency will be 16/(16+2(TWBR)), assuming prescalar of 0.
// so for 100KHz SCL, TWBR = ((F_CPU/F_SCL)-16)/2 = ((16/0.1)-16)/2 = 144/2 = 72.
{
	TWSR = 0; // set prescalar to zero
	TWBR = ((F_CPU/F_SCL)-16)/2; // set SCL frequency in TWI bit register
}//I2C_Init

//----------------------------------
byte I2C_Detect(byte addr)
// look for device at specified address; return 1=found, 0=not found
{
	TWCR = TW_START; // send start condition
	while (!TW_READY); // wait
	TWDR = addr; // load device's bus address
	TWCR = TW_SEND; // and send it
	while (!TW_READY); // wait
	return (TW_STATUS==0x18); // return 1 if found; 0 otherwise
}//I2C_Detect

//----------------------------------
byte I2C_FindDevice(byte start)
// returns with address of first device found; 0=not found
{
	for (byte addr=start;addr<0xFF;addr++) // search all 256 addresses
	{
		if (I2C_Detect(addr)) // I2C detected?
		return addr; // leave as soon as one is found
	}
	return 0; // none detected, so return 0.
}//I2C_FindDevice

//----------------------------------
void I2C_Start (byte slaveAddr)
{
	I2C_Detect(slaveAddr);
}//I2C_Start

//----------------------------------
// sends a data byte to slave
byte I2C_Write (byte data) 
{
	TWDR = data; // load data to be sent
	TWCR = TW_SEND; // and send it
	while (!TW_READY); // wait
	return (TW_STATUS!=0x28);
}//I2C_Write

//----------------------------------
byte I2C_ReadACK () // reads a data byte from slave
{
	TWCR = TW_ACK; // ack = will read more data
	while (!TW_READY); // wait
	return TWDR;
	//return (TW_STATUS!=0x28);
}//I2C_ReadACK

//----------------------------------
byte I2C_ReadNACK () // reads a data byte from slave
{
TWCR = TW_NACK; // nack = not reading more data
while (!TW_READY); // wait
return TWDR;
//return (TW_STATUS!=0x28);
}//I2C_ReadNACK
 
//----------------------------------
void I2C_WriteByte(byte busAddr, byte data)
{
	I2C_Start(busAddr); // send bus address
	I2C_Write(data); // then send the data byte
	I2C_Stop();
}//I2C_WriteByte

//----------------------------------
void I2C_WriteRegister(byte busAddr, byte deviceRegister, byte data)
{
	I2C_Start(busAddr); // send bus address
	I2C_Write(deviceRegister); // first byte = device register address
	I2C_Write(data); // second byte = data for device register
	I2C_Stop();
}//I2C_WriteRegister

//----------------------------------
byte I2C_ReadRegister(byte busAddr, byte deviceRegister)
{
	byte data = 0;
	I2C_Start(busAddr); // send device address
	I2C_Write(deviceRegister); // set register pointer
	I2C_Start(busAddr+READ); // restart as a read operation
	data = I2C_ReadNACK(); // read the register data
	I2C_Stop(); // stop
	return data;
}//I2C_ReadRegister
