//##########################################################################
//#
//#	ln_sig_dec		copyright: Michael Pfeil
//#
//#	This program will control a light signal which consists of LEDs.
//#	The LEDs will be dimmed when lighting up or down.
//#
//#-------------------------------------------------------------------------
//#
//# board:		AVR ATmega328P
//#	processor:	ATmega328P, 16 MHz
//#
//##########################################################################

#include "compile_options.h"

#define VERSION_MAIN		PLATINE_VERSION
#define	VERSION_MINOR		1
#define VERSION_BUGFIX		1

#define VERSION_NUMBER		((VERSION_MAIN * 10000) + (VERSION_MINOR * 100) + VERSION_BUGFIX)


//##########################################################################
//#
//#		Version History:
//#
//#-------------------------------------------------------------------------
//#
//#	Version:	x.01.01		from: 19.09.2022
//#
//#	Implementation:
//#		-	add version number into EEPROM
//#		-	initialize the signal with Hp0
//#		-	implement signal aspects: Hp0, Hp1 and Hp2
//#
//#-------------------------------------------------------------------------
//#
//#	Version:	x.01.00		from: 17.09.2022
//#
//#	Implementation:
//#		-	first test version
//#
//##########################################################################


//==========================================================================
//
//		I N C L U D E S
//
//==========================================================================

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


//**********************************************************************
//	setup
//----------------------------------------------------------------------
//
void setup()
{
	g_ulBlinkTimer	= 0L;
	g_bLedOn		= false;
	g_bIsProgMode	= false;

	//--------------------------------------------------------------
	//
	//	!!	A T T E N T I O N  !!
	//
	//	do not change the order of the 'Init' functions
	//
	g_clLncvStorage.CheckEEPROM( VERSION_NUMBER );
	g_clLncvStorage.Init();

	g_clControl.Init();

	g_clMyLoconet.Init();
}


//**********************************************************************
//	loop
//----------------------------------------------------------------------
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
			//--------------------------------------------------
			//	we want to leave the prog mode
			//	so performe a RESET
			//
			resetFunc();
		}
		else
		{
			//--------------------------------------------------
			//	we switched to prog mode
			g_bIsProgMode = true;
		}
	}
}
