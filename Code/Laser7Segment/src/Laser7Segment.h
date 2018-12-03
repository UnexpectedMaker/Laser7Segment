// ---------------------------------------------------------------------------
// Laser7Segment Library - v1.0.0 - 05/12/2018
//
// AUTHOR/LICENSE:
// Created by Seon Rozenblum - seon@unexpectedmaker.com
// Copyright 2016 License: GNU GPL v3 http://www.gnu.org/licenses/gpl-3.0.html
//
// LINKS:
// Project home: XXX <--
// Blog: XXX <--
//
// DISCLAIMER:
// This software is furnished "as is", without technical support, and with no 
// warranty, express or implied, as to its usefulness for any purpose.
//
// PURPOSE:
// Seven Segment Library for the Neo7Segment display boards, or for use with strips of NeoPixels arranged as 7 Segment displays
//
// SYNTAX:
//   Laser7Segment( digits, pin ) - Initialise the array of displays
//     Parameters:
//		* digits		- The number of digits you will be displaying on
//		* pin			- Pin speaker is wired to (other wire to ground, be sure to add an inline 100 ohm resistor).
//
// HISTORY:

//
// 05/12/2018 v1.0 - Initial release.
//
// ---------------------------------------------------------------------------

#ifndef Laser7Segment_h
	#define Laser7Segment_h

	#include <Adafruit_NeoPixel.h>

  	#if defined(ARDUINO) && ARDUINO >= 100
    	#include <Arduino.h>
  	#else
    	#include <WProgram.h>
		#include <pins_arduino.h>
	#endif

	#ifdef __AVR__
	  #include <avr/power.h>
	#endif
	

typedef struct 
{
	char id;
	uint8_t pixels;
} CharacterX;

class Laser7Segment
{
	public:
		Laser7Segment( uint8_t displayCount, uint8_t dPin  );
		~Laser7Segment();
		
		void Begin( uint8_t brightness );
		void SetBrightness( uint8_t brightness );
		void Clear( uint8_t display );
		  
		void DisplayTextVerticalRainbow( String text, uint32_t colorA, uint32_t colorB );
		void DisplayTextHorizontalRainbow( String text, uint32_t colorA, uint32_t colorB );
		void DisplayTextColor( String text, uint32_t color );
		void DisplayTextColorCycle( String text, uint8_t index );
		void DisplayTextMarquee( String text, uint8_t index, uint32_t color );
		void DisplayTextChaser( String text, uint8_t index, uint32_t color );
		void DisplayKnightRider( uint8_t index, uint32_t color );
		void DisplayBorderAnimation( uint8_t index, uint32_t color );
		void DisplayTime( uint8_t hours, uint8_t mins, uint8_t secs, uint32_t color, uint32_t colorB );

		void SetPixel( uint8_t pixel, uint32_t color );

		String SpaceToDisplay( String data );
		void ForceUppercase( bool force );
		
	    static uint32_t Color(uint8_t r, uint8_t g, uint8_t b);
		
		uint32_t Wheel( byte WheelPos );

		bool IsReady( void );

		byte GetArraySize();
		String GetCharacterAtArrayIndex( int index );
		
	protected:

		
	private:
		Adafruit_NeoPixel pixels;
		uint8_t dispCount;
		uint8_t dispPin;
		CharacterX FindCharacterX( char );

		void CheckToCacheBytes( String s );
		String PadTimeData( int8_t data );
		String SpaceTimeData( int8_t data );
		uint8_t Red( uint32_t col );
		uint8_t Green( uint32_t col );
		uint8_t Blue( uint32_t col );
		uint8_t *cachedBytes;
		CharacterX *cachedCharacters;
		String cachedString;
		bool isReady;
		bool isForcedUpper;
};



#endif