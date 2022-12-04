/*********************************************************************
 * Railuino - Hacking your Märklin
 *
 * Copyright (C) 2012 Joerg Pleumann
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * LICENSE file for more details.
 */
 
#include "TrackReporterS88_DS.h"
#include "Define.h"
// ===================================================================
// === TrackReporterS88_DS ==============================================
// ===================================================================

	
const int TIME = 50;
const int TIME_HALF = 25;

TrackReporterS88_DS::TrackReporterS88_DS(int modules) 
{
	mSize = modules;
	
	pinMode(PIN_S88_DATA, INPUT);
	
	pinMode(PIN_S88_CLOCK, OUTPUT);
	pinMode(PIN_S88_LOAD, OUTPUT);
	pinMode(PIN_S88_RESET, OUTPUT);
}

void TrackReporterS88_DS::refresh()
{
	refresh(mSize);
}

void TrackReporterS88_DS::refresh(int inMaxSize)
{
	int myByte = 0;
	int myBit = 0;
	int aCntMax = 0;
	
	if( inMaxSize > 2)
	{
		aCntMax = 32;
	}
	else
	{
		aCntMax = inMaxSize << 4;
	}

	
	for (byte i = 0; i < sizeof(mSwitches); i++) 
	{
		mSwitches[i] = 0;
	}

	digitalWrite(PIN_S88_LOAD, HIGH);
	delayMicroseconds(TIME);
	
	digitalWrite(PIN_S88_CLOCK, HIGH);
	delayMicroseconds(TIME);
	digitalWrite(PIN_S88_CLOCK, LOW);
	delayMicroseconds(TIME);
	digitalWrite(PIN_S88_RESET, HIGH);
	delayMicroseconds(TIME);
	digitalWrite(PIN_S88_RESET, LOW);
	delayMicroseconds(TIME);
	digitalWrite(PIN_S88_LOAD, LOW);

	delayMicroseconds(TIME_HALF);
	bitWrite(mSwitches[myByte], myBit++, digitalRead(PIN_S88_DATA));
	delayMicroseconds(TIME_HALF);

	
	for (int i = 1; i < aCntMax; i++) 
	{
		digitalWrite(PIN_S88_CLOCK, HIGH);
		delayMicroseconds(TIME);
		digitalWrite(PIN_S88_CLOCK, LOW);

		delayMicroseconds(TIME_HALF);
		bitWrite(mSwitches[myByte], myBit++, digitalRead(PIN_S88_DATA));

		if (myBit == 8) {
			myByte++;
			myBit = 0;
		}

		delayMicroseconds(TIME_HALF);
	}
}

boolean TrackReporterS88_DS::getValue(int index) {
	index--;
	return bitRead(mSwitches[index / 8], index % 8);
}

byte TrackReporterS88_DS::getByte(int index) {
	return mSwitches[index];
}
