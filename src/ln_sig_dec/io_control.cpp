//##########################################################################
//#
//#		io_control.cpp
//#
//#	This class operates the ports of the Atmel chip.
//#	here only LEDs are controlled by the help of the PWM library
//#
//#-------------------------------------------------------------------------
//#
//#	14 LEDs are connected to the chip.
//#	See the table below for the assignment:
//#
//#		LED		Board Pin	Port Bit	Nano	Description
//#		 A			X3.2		D0		  0		Zusatzsanzeiger
//#		 B			X3.3		C5		 19		Hp1
//#		 C			X3.4		C4		 18		Hp00 Zugfahrt
//#		 D			X3.5		C3		 17		Hp00 Rangierfahrt
//#		 E			X3.6		C2		 16		Kennlicht
//#		 F			X3.7		C1		 15		SH1 Rangierfahrt
//#		 G			X3.8		C0		 14		Hp2
//#		 H			X1.2		D7		  7		Ersatzsignal
//#		 I			X1.3		D6		  6		Zusatz-Kennlicht
//#		 J			X1.4		D5		  5
//#		 K			X1.5		D4		  4
//#		 L			X1.6		D3		  3
//#		 M			X1.7		D2		  2
//#		 N			X1.8		D1		  1
//#
//#-------------------------------------------------------------------------
//#
//#	File Version:	1		vom: 16.03.2022
//#
//#	Implementation:
//#		-	first working version
//#
//##########################################################################


//==========================================================================
//
//		I N C L U D E S
//
//==========================================================================

#include <Arduino.h>
#include <SoftPWM.h>

#include "io_control.h"


//==========================================================================
//
//		D E F I N I T I O N S
//
//==========================================================================

#define	SIGNAL_LEDS			14

#define FADE_TIME_UP		200
#define FADE_TIME_DOWN		400


//==========================================================================
//
//		G L O B A L   V A R I A B L E S
//
//==========================================================================

IO_ControlClass	g_clControl	= IO_ControlClass();

//----------------------------------------------------------------------
//	LED assignment
//
uint8_t	gusLeds[ SIGNAL_LEDS ] =
				 { 0, 19, 18, 17, 16, 15, 14,  7,  6,  5,  4,  3,  2,  1 };
//			LED	   A   B   C   D   E   F   G   H   I   J   K   L   M   N
//	LED mask bit   0   1   2   3   4   5   6   7   8   9  10  11  12  13


//==========================================================================
//
//		C L A S S   F U N C T I O N S
//
//==========================================================================


////////////////////////////////////////////////////////////////////////
//	CLASS: IO_ControlClass
//

//******************************************************************
//	Constructor
//------------------------------------------------------------------
IO_ControlClass::IO_ControlClass()
{
}


//******************************************************************
//	Init
//------------------------------------------------------------------
//	here for all Ports the relevant I/O pins will be configured.
//	Not used I/O's will be set to Input with PullUp.
//	all connected devices (like LED's) will set to 'off'.
//
void IO_ControlClass::Init( void )
{
	SoftPWMBegin();

	for( uint8_t idx = 0 ; idx < SIGNAL_LEDS ; idx++ )
	{
		SoftPWMSet( gusLeds[ idx ], 0 );
	}
	
	SoftPWMSetFadeTime( ALL, FADE_TIME_UP, FADE_TIME_DOWN );
}


//******************************************************************
//	FadeLed
//------------------------------------------------------------------
//
void IO_ControlClass::FadeLeds( uint8_t usLeds, uint8_t usBrightness )
{
	uint8_t	usMask = 0x01;

	for( uint8_t idx = 0 ; idx < SIGNAL_LEDS ; idx++ )
	{
		if( usMask & usLeds )
		{
			SoftPWMSet( gusLeds[ idx ], usBrightness );
		}
		
		usMask <<= 1;
	}
}
