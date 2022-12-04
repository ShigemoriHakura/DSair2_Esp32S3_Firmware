/*
BSD 3-Clause License

Copyright (c) 2016-2018, GPS_NMEA
All rights reserved.
Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


#include "Arduino.h"
#include "FlashAirSharedMem.h"
#include "Define.h"

#include <SPI.h>
uint8_t spi_trans(uint8_t dat)
{
	return SPI.transfer(dat);
}


void cs_release()
{
	//release
	digitalWrite(PIN_SPI_CS,HIGH);
	spi_trans(0xFF);
}

int8_t sd_cmd(uint8_t cmd,uint32_t arg)
{
	uint8_t val;

	spi_trans(0xFF);//END of MODE

	digitalWrite(PIN_SPI_CS,LOW);
	spi_trans(0x40 | cmd);
	spi_trans(arg>>24);
	spi_trans(arg>>16);
	spi_trans(arg>>8);
	spi_trans(arg);
	
	if(cmd == 0x00)
		spi_trans(0x95);
	else if(cmd == 0x08)
		spi_trans(0x87);
	else
		spi_trans(0x01); //CRC+STOP

	do{
		val = spi_trans(0xFF);//R1 Rcv
	}while(val == 0xFF);
	
	return val;
}

int8_t SharedMemInit(int _cs)
{
	uint8_t val;
	
	pinMode(PIN_SPI_CS, OUTPUT);//CS output
	SPI.begin(PIN_SPI_CLK, PIN_SPI_MISO, PIN_SPI_MOSI, PIN_SPI_CS);
	SPI.setClockDivider(SPI_CLOCK_DIV2);
	SPI.setDataMode(SPI_MODE0);
	SPI.setBitOrder(MSBFIRST);
	
	//power-on clock
	digitalWrite(PIN_SPI_CS,LOW);
	for(int8_t i=0;i<10;i++)
		spi_trans(0xFF);

	cs_release();

	if(sd_cmd(0,0x00000000) != 0x01){
		return -1;
	}

	cs_release();
	if(sd_cmd(8,0x000001AA) != 0x01){
		return -2;
	}
	//R7 Responce
	spi_trans(0xFF); //0
	spi_trans(0xFF); //0
	spi_trans(0xFF); //0
	if(spi_trans(0xFF) != 0xAA) //AA
	{
		return -3;
	}
	
	//--ACMD41--
	do{
		cs_release();
		val = sd_cmd(55,0x00000000);
		val = sd_cmd(41,0x40000000); //HCS=1
	}while(val != 0);
	
	cs_release();
	return 0;
}
