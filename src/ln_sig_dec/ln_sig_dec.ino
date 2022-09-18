//##########################################################################
//#
//#	Ln_Sig_Dec		copyright: Michael Pfeil
//#
//#	This programm will control a light signal.
//#
//#-------------------------------------------------------------------------
//#
//# board:		AVR ATmega328P
//#	processor:	ATmega328P, 16 MHz
//#
//##########################################################################

//----------------------------------------------------------------------
//	The main version is defined by PLATINE_VERSION (compile_options.h)
//
#define VERSION_MAIN		0
#define	VERSION_MINOR		1
#define VERSION_BUGFIX		0


//##########################################################################
//#
//#		Version History:
//#
//#-------------------------------------------------------------------------
//#
//#	Version:	0.01.00		from: 17.09.2022
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

//#include "compile_options.h"

#include <Arduino.h>

#include "io_control.h"
#include "lncv_storage.h"
#include "my_loconet.h"


//==========================================================================
//
//		I N C L U D E S
//
//==========================================================================

#define BLINK_TIME_ON		500
#define BLINK_TIME_OFF		500


//==========================================================================
//
//		G L O B A L   V A R I A B L E S
//
//==========================================================================

uint32_t	g_ulBlinkTimer;
bool		g_bLedOn;
bool		g_bIsProgMode;


//==========================================================================
//
//		G L O B A L   F U N C T I O N S
//
//==========================================================================

//**********************************************************************
//	resetFunc
//----------------------------------------------------------------------
//	This function will reset the hole processor because by calling the
//	function the program will jump to the hardware reset vector.
//
void (*resetFunc)( void ) = 0;


//**************************************************************************
//	setup
//--------------------------------------------------------------------------
//
void setup()
{
	g_ulBlinkTimer	= 0L;
	g_bLedOn		= false;
	g_bIsProgMode	= false;

	//------------------------------------------------------------------
	//
	//	!!	A T T E N T I O N  !!
	//
	//	do not change the order of the 'Init' functions
	//
	g_clLncvStorage.CheckEEPROM();
	g_clLncvStorage.Init();

	g_clControl.Init();

	g_clMyLoconet.Init();
}


//**************************************************************************
//	loop
//--------------------------------------------------------------------------
//
void loop()
{
	//--------------------------------------------------------------
	//	check for Loconet messages
	//
	g_clMyLoconet.CheckForMessageAndProcess();

	//--------------------------------------------------------------
	//	process fade
	//
	g_clControl.FadeUpdate();
	
	//--------------------------------------------------------------
	//	in prog mode green LED should blink
	//
	if( g_bIsProgMode )
	{
		if( millis() > g_ulBlinkTimer )
		{
			if( g_bLedOn )
			{
				g_clControl.SwitchLed( LED_GREEN, false );

				g_ulBlinkTimer = millis() + BLINK_TIME_OFF;
			}
			else
			{
				g_clControl.SwitchLed( LED_GREEN, true );

				g_ulBlinkTimer = millis() + BLINK_TIME_ON;
			}
		}
	}

	//--------------------------------------------------------------
	//	Programmier-Modus
	//
	if( g_clMyLoconet.IsProgMode() != g_bIsProgMode )
	{
		if( g_bIsProgMode )
		{
			resetFunc();
		}
		else
		{
			g_bIsProgMode = true;
		}
	}

/*
	uint8_t		idx;
	uint16_t	usMask = 0x0001;
	
	g_clControl.SwitchLed( LED_BLUE, true );

	for( idx = 0 ; idx < SIGNAL_LEDS ; idx++ )
	{
		if( 3 == idx )
		{
			g_clControl.FadeLeds( usMask, 255 );
		}
		else
		{
			g_clControl.FadeLeds( usMask, 128 );
		}

		delay( 500 );

		usMask <<= 1;
	}

	delay( 1000 );

	usMask = 0x0001;
	g_clControl.SwitchLed( LED_BLUE, false );

	for( idx = 0 ; idx < SIGNAL_LEDS ; idx++ )
	{
		g_clControl.FadeLeds( usMask, 0 );

		delay( 500 );

		usMask <<= 1;
	}

	delay( 1000 );

	uint8_t	idx;

	g_clControl.SwitchLed( LED_BLUE, true );

	for( idx = 0 ; idx < SIGNAL_LEDS ; idx++ )
	{
		g_clControl.SwitchLed( idx, true );
		
		delay( 500 );
	}

	g_clControl.SwitchLed( LED_BLUE, false );

	for( idx = 0 ; idx < SIGNAL_LEDS ; idx++ )
	{
		g_clControl.SwitchLed( idx, false );
		
		delay( 500 );
	}
*/
}
