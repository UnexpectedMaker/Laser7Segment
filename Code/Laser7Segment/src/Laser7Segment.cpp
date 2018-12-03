// ---------------------------------------------------------------------------
// Created by Seon Rozenblum - seon@unexpectedmaker.com
// Copyright 2016 License: GNU GPL v3 http://www.gnu.org/licenses/gpl-3.0.html
//
// See "Laser7Segment.h" for purpose, syntax, version history, links, and more.
// ---------------------------------------------------------------------------

#include "Laser7Segment.h"
// This library is 100% entwined with the Adafruit NeoPixel library - please check their licensing terms in their library.
#include <Adafruit_NeoPixel.h>

// #define DEBUG 1
#define NUM_PIXELS_PER_BOARD 8

#define ELEMENTS(x)   (sizeof(x) / sizeof(x[0]))



// Array of pixels per segment, 7 segments with 4 pixels each
// int segmentsPixels[14][5] 	{ { 0, 1, 2, 3, 4 }, 
//                               { 5, 6, 7, 8, 9 }, 
//                               { 10, 11, 12, 13, 14 }, 
//                               { 15, 16, 17, 18, 19 }, 
//                               { 20, 21, 22, 23, 24 }, 
//                               { 25, 26, 27, 28, 29 }, 
//                               { 30, 31, 32, 33, 34 },
// 							  { 35, 36, 37, -1, -1 },
// 							  { 38, 39, 40, -1, -1 },
// 							  { 41, 42, 43, -1, -1 },
// 							  { 44, 45, 46, -1, -1 },
// 							  { 47, 48, 49, 50, 51 },
// 							  { 52, 53, 54, 55, 56 },
// 							  { 57, -1, -1, -1, -1}
// 						  	};

// Array of pixel positions in X,Y format for mapping colours in X,Y space
byte pixelsXY[8][2]		{ {1,0}, {0,1}, {2,1}, {1,2}, {0,3}, {2,3}, {1,4} ,{3,4} };

					  
// Array of segment based rainbow colour values
uint32_t segmentRainbow[7][3] {
                              { 255,0,0 },
                              { 255,124,42 },
                              { 255,255,0 },
                              { 69,223,91 },
                              { 31,189,255 },
                              { 150,11,255 },
                              { 255,30,237 }
                            };

// Available characters a 7 Segment display can show					  
const byte ARRAY_SIZE = 32;

CharacterX characters_pixel_upper[ ARRAY_SIZE ] {
	{ '0', 0b00111111 }, 
	{ '1', 0b00000110 },
	{ '2', 0b01011011 },
	{ '3', 0b01001111 },
	{ '4', 0b01100110 },
	{ '5', 0b01101101 },
	{ '6', 0b01111100 },
	{ '7', 0b00000111 },
	{ '8', 0b01111111 },
	{ '9', 0b01100111 },
	{ 'a', 0b01110111 },
	{ 'b', 0b01111111 },
	{ 'c', 0b00111001 },
	{ 'd', 0b00111111 },
	{ 'e', 0b01111001 },
	{ 'f', 0b01110001 },
	{ 'g', 0b01100111 },
	{ 'h', 0b01110110 },
	{ 'i', 0b00110000 },
	{ 'j', 0b00011110 },
	{ 'l', 0b00111000 },
	{ 'n', 0b00110111 },
	{ 'o', 0b00111111 },
	{ 'p', 0b01110011 },
	{ 'q', 0b01100111 },
	{ 'r', 0b00110001 },
	{ 's', 0b01101101 },
	{ 'u', 0b00111110 },
	{ 'x', 0b01110110 },
	{ 'y', 0b01101110 },
	{ '-', 0b01000000 },
	{ ' ', 0b00000000 }
};	
						
CharacterX characters_pixel[ ARRAY_SIZE ] {

	{ '0', 0b00111111 }, 
	{ '1', 0b00000110 },
	{ '2', 0b01011011 },
	{ '3', 0b01001111 },
	{ '4', 0b01100110 },
	{ '5', 0b01101101 },
	{ '6', 0b01111100 },
	{ '7', 0b00000111 },
	{ '8', 0b01111111 },
	{ '9', 0b01100111 },
	{ 'a', 0b01110111 },
	{ 'b', 0b01111111 },
	{ 'c', 0b00111001 },
	{ 'd', 0b00111111 },
	{ 'e', 0b01111001 },
	{ 'f', 0b01110001 },
	{ 'g', 0b01100111 },
	{ 'h', 0b01110110 },
	{ 'i', 0b00110000 },
	{ 'j', 0b00011110 },
	{ 'l', 0b00111000 },
	{ 'n', 0b00110111 },
	{ 'o', 0b00111111 },
	{ 'p', 0b01110011 },
	{ 'q', 0b01100111 },
	{ 'r', 0b00110001 },
	{ 's', 0b01101101 },
	{ 'u', 0b00111110 },
	{ 'x', 0b01110110 },
	{ 'y', 0b01101110 },
	{ '-', 0b01000000 },
	{ ' ', 0b00000000 }
};

													
Laser7Segment::Laser7Segment( uint8_t displayCount, uint8_t dPin )
{
	dispCount = displayCount;
	dispPin = dPin;
	pixels = Adafruit_NeoPixel ();
	pixels.updateType( NEO_GRB + NEO_KHZ800 );
  	pixels.updateLength( dispCount * NUM_PIXELS_PER_BOARD );
  	pixels.setPin(dispPin);
	isReady = false;
}

Laser7Segment::~Laser7Segment()
{
	isReady = false;
}

bool Laser7Segment::IsReady()
{
	return isReady;
}

void Laser7Segment::Begin( uint8_t brightness )
{

	pixels.begin(); // This initializes the NeoPixel library.
	pixels.show();
	pixels.setBrightness( brightness );

	cachedString = "";
	cachedCharacters = ( CharacterX *) malloc(dispCount * sizeof(CharacterX) );
	cachedBytes = ( uint8_t *) malloc(dispCount * sizeof(uint8_t) );
	
	// Set all cached bytes to 0
	for ( int i = 0; i < dispCount; i++ )
		cachedBytes[i] = 0;

	#ifdef DEBUG
		Serial.print("Brightness: ");
		Serial.println(brightness);
		Serial.println("Ready!");
	#endif

	//Digits are initialised and ready
	isReady = true;
	isForcedUpper = false;
}

void Laser7Segment::Clear( uint8_t display )
{
	if ( isReady )
	{
		for ( int p = 0; p < NUM_PIXELS_PER_BOARD; p++ )
			pixels.setPixelColor( p + ( display * NUM_PIXELS_PER_BOARD ), Color(0,0,0) );

		pixels.show();
	}
}

void Laser7Segment::SetBrightness( uint8_t brightness )
{
	if ( isReady )
	{
		pixels.setBrightness( brightness );
	}
}


byte Laser7Segment::GetArraySize()
{
	return ARRAY_SIZE;
}

void Laser7Segment::ForceUppercase( bool force )
{
	isForcedUpper = force;
}

// Character Laser7Segment::GetCharacterByByte( uint16_t id )
// {
// 	for ( int i = 0; i < ELEMENTS( available_characters ); i++ )
// 	{
// 		if ( available_characters[i].segments == id )
// 			return available_characters[i];
// 	}

// 	return available_characters[0];
// }

// Character Laser7Segment::FindCharacter( byte c )
// {
// 	for ( int i = 0; i < ELEMENTS( available_characters ); i++ )
// 	{
// 		if ( available_characters[i].id == c )
// 			return available_characters[i];
// 	}

// 	return available_characters[0];
// }

CharacterX Laser7Segment::FindCharacterX( char c )
{

	for ( int i = 0; i < ELEMENTS( characters_pixel ); i++ )
	{
		Serial.println( String(i) +" " + String(characters_pixel[i].id) +" " + String(c) );
		if ( characters_pixel[i].id == c )
		{
			if ( isForcedUpper )
				return characters_pixel_upper[i];
			else
				return characters_pixel[i];
		}
	}

	return characters_pixel[0];
}



String Laser7Segment::GetCharacterAtArrayIndex( int index )
{
	return ( (String)(char)characters_pixel[ index ].id );
}

// Convert separate R,G,B into packed 32-bit RGB color.
// Packed format is always RGB, regardless of LED strand color order.
uint32_t Laser7Segment::Color(uint8_t r, uint8_t g, uint8_t b)
{
  return ((uint32_t)r << 16) | ((uint32_t)g <<  8) | b;
}


void Laser7Segment::CheckToCacheBytes( String str )
{
	if ( str != cachedString )
	{
		cachedString = str;

		int lengthOfLoop = min( dispCount, (uint8_t)str.length() );
		
		int index = 0;
		for ( int s = 0; s < lengthOfLoop; s++ )
		{
			#ifdef DEBUG
			Serial.print( (String)cachedString.charAt(s) );
			Serial.print( " .. " );
			#endif

			String temp = cachedString;
			temp.toLowerCase();
			char c = temp.charAt(s);
			

			if ( (String)c != "." )
			{ 
				cachedCharacters[index] = FindCharacterX( c );
				cachedBytes[index] = cachedCharacters[index].pixels;
				#ifdef DEBUG
				Serial.println( "1" );
				#endif
				index++;
			}
			else if ( s > 0 && bitRead( cachedCharacters[index-1].pixels, 63 ) != 1 )
			{
				cachedBytes[index-1] = cachedBytes[index-1] | 0b10000000;
				#ifdef DEBUG
				Serial.println( "2" );
				#endif
			}
			else
			{
				cachedCharacters[index] = FindCharacterX( c );
				cachedBytes[index] = 0b10000000;
				#ifdef DEBUG
				Serial.println( "3" );
				#endif
				index++;
			}
		}
	}
}

void Laser7Segment::DisplayTextVerticalRainbow( String text, uint32_t colorA, uint32_t colorB )
{
	if ( !isReady )
		return;

	CheckToCacheBytes( text );	

	// Clamp the length, so text longer than the display count is ignored
	int lengthOfLoop = min( dispCount, (uint8_t)text.length() );

	// Grab the byte (bits) for the segmens for the character passed in
	for ( int s = 0; s < lengthOfLoop; s++ )
	{
		for ( int i = 0; i < NUM_PIXELS_PER_BOARD; i++ )
		{

			int y = pixelsXY[ i ][1];

			uint8_t red = ((Red(colorA) * (4 - y)) + (Red(colorB) * y)) * 0.25;
			uint8_t green = ((Green(colorA) * (4 - y)) + (Green(colorB) * y)) * 0.25;
			uint8_t blue = ((Blue(colorA) * (4 - y)) + (Blue(colorB) * y)) * 0.25;

			uint32_t color = Color(red, green, blue );

			bool on = ( bitRead( cachedBytes[s], i) == 1 );
			pixels.setPixelColor( i + ( s * NUM_PIXELS_PER_BOARD ), on ? color : Color(0,0,0) );

		}

		Serial.println(" ");
	}
		
	// we have finished setting all of the colors on each segment for this Laser7Segment, so lets turn on the pixels
	pixels.show();
		
	// CheckToCacheBytes( text );

	// // Clamp the length, so text longer than the display count is ignored
	// int lengthOfLoop = min( dispCount, (uint8_t)text.length() );

	// uint32_t color;
	
	// // Grab the byte (bits) for the segmens for the character passed in
	// for ( int s = 0; s < lengthOfLoop; s++ )
	// {
	// 	byte code = cachedBytes[s];
	// 	if(code > -1)
	// 	{
	// 		int pixelIndex = 0; // current pixel index
		
	// 		for( int segment = 0; segment < 7; segment++ )
	// 		{
	// 			bool on = ( bitRead( code, segment) == 1 );
	// 			for ( int p = 0; p < 4; p++ )
	// 			{
	// 				// we want the Y position (row) so we can use that as the colour index 
	// 				int y = pixelsXY[ pixelIndex ][1];

	// 				uint8_t red = ((Red(colorA) * (10 - y)) + (Red(colorB) * y)) * 0.1;
	// 				uint8_t green = ((Green(colorA) * (10 - y)) + (Green(colorB) * y)) * 0.1;
	// 				uint8_t blue = ((Blue(colorA) * (10 - y)) + (Blue(colorB) * y)) * 0.1;

	// 				color = Color(red, green, blue );
				
	// 				pixels.setPixelColor( segmentsPixels[ segment ][ p ] + ( s * NUM_PIXELS_PER_BOARD ), on ? color : Color(0,0,0) );
	// 				pixelIndex++;
	// 			}
	// 		}

	// 		#ifdef USEDP

	// 			bool on = ( bitRead( code, 7) == 1 );
	// 			pixels.setPixelColor( segmentsPixels[ 7 ][ 0 ], on ? color : Color(0,0,0));

	// 		#endif
	// 	}
	// }
		
	// // we have finished setting all of the colors on each segment for this Laser7Segment, so lets turn on the pixels
	// pixels.show(); 
}

void Laser7Segment::DisplayTextHorizontalRainbow( String text, uint32_t colorA, uint32_t colorB )
{
	if ( !isReady )
		return;	
	
	CheckToCacheBytes( text );

	int numPixelsPerColumn = 3;
	int numColumns = dispCount * numPixelsPerColumn;

	uint32_t color;

	// Clamp the length, so text longer than the display count is ignored
	int lengthOfLoop = min( dispCount, (uint8_t)text.length() );
	
	// Grab the byte (bits) for the segmens for the character passed in
	for ( int s = 0; s < lengthOfLoop; s++ )
	{
		for ( int i = 0; i < NUM_PIXELS_PER_BOARD; i++ )
		{
				bool on = ( bitRead( cachedBytes[s], i) == 1 );
				// we want the Y position (row) so we can use that as the colour index 
				int x = pixelsXY[ i ][0] + ( numPixelsPerColumn * s );

				uint8_t red = ((Red(colorA) * (numColumns - x)) + (Red(colorB) * x)) / numColumns;
				uint8_t green = ((Green(colorA) * (numColumns - x)) + (Green(colorB) * x)) / numColumns;
				uint8_t blue = ((Blue(colorA) * (numColumns - x)) + (Blue(colorB) * x)) / numColumns;

				color = Color(red, green, blue );
			
				pixels.setPixelColor( i + ( s * NUM_PIXELS_PER_BOARD ), on ? color : Color(0,0,0) );
				//pixelIndex++;
	
		}

		bool on = ( bitRead( cachedBytes[s], 7) == 1 );
		pixels.setPixelColor( 7 + ( s * NUM_PIXELS_PER_BOARD ), on ? color : Color(0,0,0));
	}
		
	// we have finished setting all of the colors on each segment for this Laser7Segment, so lets turn on the pixels
	pixels.show(); 
}

void Laser7Segment::DisplayTextMarquee( String text, uint8_t index, uint32_t color )
{
	if ( !isReady )
		return;
		

	CheckToCacheBytes( text );	

	// Clamp the length, so text longer than the display count is ignored
	int lengthOfLoop = min( dispCount, (uint8_t)text.length() );

	// Grab the byte (bits) for the segmens for the character passed in
	for ( int s = 0; s < lengthOfLoop; s++ )
	{
		for ( int i = 0; i < NUM_PIXELS_PER_BOARD; i++ )
		{
			bool on = ( bitRead( cachedBytes[s], i) == 1 );

			if ( index == 0 && ( i == 1 || i == 3 || i == 5 || i == 7 ) )
				on = false;
			else if ( index == 1 && ( i == 0 || i == 2 || i == 4 || i == 6 ) )
				on = false;

			pixels.setPixelColor( i + ( s * NUM_PIXELS_PER_BOARD ), on ? color : Color(0,0,0) );

		}
	}

	// we have finished setting all of the colors on each segment for this Laser7Segment, so lets turn on the pixels
	pixels.show();
}

void Laser7Segment::DisplayTextChaser( String text, uint8_t index, uint32_t color )
{
	if ( !isReady )
		return;
	
	// CheckToCacheBytes( text );
	
	// // Clamp the length, so text longer than the display count is ignored
	// int lengthOfLoop = min( dispCount, (uint8_t)text.length() );

	// // Grab the byte (bits) for the segmens for the character passed in
	// for ( int s = 0; s < lengthOfLoop; s++ )
	// {
	// 	byte code = cachedBytes[s];
		
	// 	if(code > -1)
	// 	{
	// 		for( int segment = 0; segment < 7; segment++ )
	// 		{
	// 			for ( int p = 0; p < 4; p++ )
	// 			{
	// 				bool on = ( bitRead( code, segment) == 1 );
									
	// 				if ( p != index )
	// 					on = false;

	// 				pixels.setPixelColor( segmentsPixels[ segment ][ p ] + ( s * NUM_PIXELS_PER_BOARD ), on ? color : Color(0,0,0) );
	// 			}
	// 		}

	// 		#ifdef USEDP

	// 			bool on = ( bitRead( code, 7) == 1 );
	// 			pixels.setPixelColor( segmentsPixels[ 7 ][ 0 ] + ( s * NUM_PIXELS_PER_BOARD ), on ? color : Color(0,0,0));

	// 		#endif
	// 	}
	// }
	// // we have finished setting all of the colors on each segment for this Laser7Segment, so lets turn on the pixels
	// pixels.show();
}

void Laser7Segment::DisplayTextColorCycle( String text, uint8_t index )
{
	if ( !isReady )
		return;
		
	CheckToCacheBytes( text );	

	// Clamp the length, so text longer than the display count is ignored
	int lengthOfLoop = min( dispCount, (uint8_t)text.length() );

	// Grab the byte (bits) for the segmens for the character passed in
	for ( int s = 0; s < lengthOfLoop; s++ )
	{
		int colorStart = index;
		for ( int i = 0; i < NUM_PIXELS_PER_BOARD; i++ )
		{
			// twirly the colours
			uint32_t color = Wheel( colorStart & 255 );
			colorStart+=(255/28);

			bool on = ( bitRead( cachedBytes[s], i) == 1 );
			pixels.setPixelColor( i + ( s * NUM_PIXELS_PER_BOARD ), on ? color : Color(0,0,0) );

		}
	}
		
	// we have finished setting all of the colors on each segment for this Laser7Segment, so lets turn on the pixels
	pixels.show();
}


void Laser7Segment::DisplayTextColor( String text, uint32_t color )
{
	if ( !isReady )
		return;
		
	CheckToCacheBytes( text );
	
	// Clamp the length, so text longer than the display count is ignored
	int lengthOfLoop = min( dispCount, (uint8_t)text.length() );

	// Grab the byte (bits) for the segmens for the character passed in
	for ( int s = 0; s < lengthOfLoop; s++ )
	{
		for ( int i = 0; i < NUM_PIXELS_PER_BOARD; i++ )
		{
			bool on = ( bitRead( cachedBytes[s], i) == 1 );
			pixels.setPixelColor( i + ( s * NUM_PIXELS_PER_BOARD ), on ? color : Color(0,0,0) );
		}
	}
	// we have finished setting all of the colors on each segment for this Laser7Segment, so lets turn on the pixels
	pixels.show();
}


void Laser7Segment::DisplayTime( uint8_t hours, uint8_t mins, uint8_t secs, uint32_t color, uint32_t colorB )
{
	if ( !isReady )
		return;
		
	String text = SpaceTimeData( hours ) + PadTimeData( mins );


	CheckToCacheBytes( text );
	
	// Clamp the length, so text longer than the display count is ignored
	int lengthOfLoop = min( dispCount, (uint8_t)text.length() );

	// Grab the byte (bits) for the segmens for the character passed in
	for ( int s = 0; s < lengthOfLoop; s++ )
	{
		for ( int i = 0; i < NUM_PIXELS_PER_BOARD; i++ )
		{
			bool on = ( bitRead( cachedBytes[s], i) == 1 );
			if ( s < 2 )
				pixels.setPixelColor( i + ( s * NUM_PIXELS_PER_BOARD ), on ? color : Color(0,0,0) );
			else
				pixels.setPixelColor( i + ( s * NUM_PIXELS_PER_BOARD ), on ? colorB : Color(0,0,0) );
		}
	}

	// we have finished setting all of the colors on each segment for this Laser7Segment, so lets turn on the pixels
	pixels.show();
}

void Laser7Segment::SetPixel( uint8_t pixel, uint32_t color )
{
	pixels.setPixelColor( pixel, color );
	pixels.show();
}




String Laser7Segment::PadTimeData( int8_t data )
{
  if( data < 10 )
	return String("0") + String(data);

  return String(data);
}

String Laser7Segment::SpaceTimeData( int8_t data )
{
  if( data < 10 )
	return String(" ") + String(data);

  return String(data);
}

String Laser7Segment::SpaceToDisplay( String data )
{
	int num = dispCount - (uint8_t)data.length();
	for ( int i = 0; i < num; i++ )
	{
		data = " "+data;
	}
	return data;
}


// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Laser7Segment::Wheel(byte WheelPos )
{
	WheelPos = 255 - WheelPos;
	if(WheelPos < 85)
		return Color(255 - WheelPos * 3, 0, WheelPos * 3);

	if(WheelPos < 170)
	{
		WheelPos -= 85;
		return Color(0, WheelPos * 3, 255 - WheelPos * 3);
	}
	
	WheelPos -= 170;
	return Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

uint8_t Laser7Segment::Red( uint32_t col )
{
	return col >> 16;
}

uint8_t Laser7Segment::Green( uint32_t col )
{
	return col >> 8;
}


uint8_t Laser7Segment::Blue( uint32_t col )
{
	return col;
}
