// @File		 		
// @Author		JOSIMAR PEREIRA LEITE
// @country		Brazil
// @Date		11/10/22
//
//
// Copyright (C) 2021  JOSIMAR PEREIRA LEITE
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
#include "wire.h"

void Wire_Start(void)
{
    SCL = 1;
    __asm__ __volatile__("nop");
    SDA = 1;
    __asm__ __volatile__("nop");
    SDA = 0;
    __asm__ __volatile__("nop");
    SCL = 0;
    __asm__ __volatile__("nop");
}

void Wire_Stop(void)
{
    SCL = 0;
    __asm__ __volatile__("nop");
    SDA = 0;
    __asm__ __volatile__("nop");
    SCL = 1;
    __asm__ __volatile__("nop");
    SDA = 1;
    
}

unsigned char Wire_Ack(void)
{
    unsigned char byte = 0;

    SDA_TRIS = 1;
    
    SDA = 1;
    
    for (unsigned char mask = 0x80; mask; mask >>= 1)
    {                
	if(SDA == 1) byte |= mask; 
	
        SCL = 1;
	__asm__ __volatile__("nop");
	SCL = 0;
        __asm__ __volatile__("nop");
    }
	
    SDA_TRIS = 0;

    SDA = 0;
    __asm__ __volatile__("nop");
    SCL = 1;
    __asm__ __volatile__("nop");
    SCL = 0;
    __asm__ __volatile__("nop");
    
    return byte;
}

unsigned char Wire_Nack(void)
{
    unsigned char byte = 0;
    
    SDA_TRIS = 1;
    
    SDA = 1;
    
    for (unsigned char mask = 0x80; mask; mask >>= 1)
    {                
	if(SDA== 1) byte |= mask; 
	
        SCL = 1;
	__asm__ __volatile__("nop");
	SCL = 0;
        __asm__ __volatile__("nop");
    }
	
     SDA_TRIS = 0;

    SDA = 1;
    __asm__ __volatile__("nop");
    SCL = 1;
    __asm__ __volatile__("nop");
    SCL = 0;
    __asm__ __volatile__("nop");
	
    return byte;
}

void Wire_Write(unsigned char data)
{
    for(unsigned char mask = 0x80; mask; mask >>= 1)
    {
	if(data & mask) SDA = 1; else SDA = 0;
	
        SCL = 1;
	__asm__ __volatile__("nop");
	SCL = 0;
	__asm__ __volatile__("nop");
    }
    
    SDA = 0;
    __asm__ __volatile__("nop");
    SCL = 1;
    __asm__ __volatile__("nop");
    SCL = 0;
    __asm__ __volatile__("nop");
}

unsigned char Wire_Polling(unsigned char Slave)
{
    unsigned char ack = 0;
	
    Wire_Start();
    
    for(unsigned char mask = 0x80; mask; mask >>= 1)
    {
	if(Slave & mask) SDA = 1; else SDA = 0;
	
        SCL = 1;
	__asm__ __volatile__("nop");
	SCL = 0;
	__asm__ __volatile__("nop");
    }
        
    // Read ACK
    SDA_TRIS = 1;
    
    SDA = 1;
    __asm__ __volatile__("nop");
    SCL = 1;
    __asm__ __volatile__("nop");
    if(SDA == 1) ack = 1; 
    SCL = 0;
    __asm__ __volatile__("nop");
    
    SDA_TRIS = 0;
    
    Wire_Stop();
    
    return ack;
}

void Wire_Init(void)
{
    SCL_TRIS = 0;
    SDA_TRIS = 0;
}

