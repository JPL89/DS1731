// @File		MAIN.C		 		
// @Author		JOSIMAR PEREIRA LEITE
// @country		BRAZIL
// @Date		17/03/24
//
//
// Copyright (C) 2024 JOSIMAR PEREIRA LEITE
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
#include "utils.h"
#include "ks0066.h"
#include "wire.h"
#include "ds1731.h"

#define DS1631_SLA_W 0x90
#define DS1631_SLA_R 0x91

// REGISTERS
#define DS1631_THF_REGISTER 0x06
#define DS1631_TLF_REGISTER 0x05
#define DS1631_NVB_REGISTER 0x04
#define DS1631_R1_REGISTER 0x03
#define DS1631_R0_REGISTER 0x02
#define DS1631_POL_REGISTER 0x01
#define DS1631_1SHOT_REGISTER 0x00

// COMMANDS
#define DS1631_START_CONVERT 0x51
#define DS1631_STOP_CONVERT 0x22
#define DS1631_READ_TEMPERATURE 0xAA
#define DS1631_ACCESS_TH 0xA1
#define DS1631_ACCESS_TL 0xA2
#define DS1631_ACCESS_CONFIG 0xAC
#define DS1631_SOFTWARE_POR 0x54

void DS1631_Command(unsigned char cmd)
{
    Wire_Start();
	Wire_Write(DS1631_SLA_W);
	Wire_Write(cmd);
	Wire_Stop();
}

void DS1631_Write(unsigned char reg, unsigned char data)
{
    Wire_Start();
	Wire_Write(DS1631_SLA_W);
    Wire_Write(reg); 
    Wire_Write(data); 
	Wire_Stop();
}

unsigned int DS1631_Read(unsigned char reg)
{
    unsigned int data = 0;
    
    Wire_Start();
    Wire_Write(DS1631_SLA_W);
    Wire_Write(reg);
    Wire_Start();
    Wire_Write(DS1631_SLA_R);
    data |= Wire_Ack();
    data <<= 8;
    data |= Wire_Nack();
	Wire_Stop();
    
    return ((unsigned int) data);
}

void main(void)
{
    __delay_ms(500);   
    
    Pic18f2520_init();
    
    Wire_Init();            
    
    KS0066_Init();

    while(1)
    {
        DS1631_Command(DS1631_START_CONVERT);    
        __delay_ms(1000);   
        DS1631_Command(DS1631_STOP_CONVERT);    

        unsigned int r = DS1631_Read(DS1631_READ_TEMPERATURE);

        r >>= 8;
        
        KS0066_Clear(0);
        KS0066_Clear(1);  
        KS0066_Goto(0,0); 
        
        KS0066_Char(48 + r / 1000000 % 10);
        KS0066_Char(48 + r / 100000 % 10);
        KS0066_Char(48 + r / 10000 % 10);
        KS0066_Char(48 + r / 1000 % 10);
        KS0066_Char(48 + r / 100 % 10);
        KS0066_Char(48 + r / 10 % 10);
        KS0066_Char(48 + r / 1 % 10);
    }
    
    return;
}
