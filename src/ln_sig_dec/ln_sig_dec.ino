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
#define	VERSION_MINOR		4
#define VERSION_BUGFIX		6

#define VERSION_NUMBER		((VERSION_MAIN * 10000) + (VERSION_MINOR * 100) + VERSION_BUGFIX)


//##########################################################################
//#
//#		Version History:
//#
//#-------------------------------------------------------------------------
//#
//#	Version:	x.04.06		from: 12.12.2022
//#
//#	Implementation:
//#		-	add handling of the track contact
//#
//#-------------------------------------------------------------------------
//#
//#	Version:	x.04.05		from: 29.11.2022
//#
//#	Bug Fix:
//#		-	LED handling blocked if a LED was switched to the actual
//#			state again.
//#
//#-------------------------------------------------------------------------
//#
//#	Version:	x.04.04		from: 28.11.2022
//#
//#	Bug Fix:
//#		-	in function 'ProcessStateMachine()' the call to change the
//#			approach aspect to Vr0 had a parameter mismatch.
//#
//#-------------------------------------------------------------------------
//#
//#	Version:	x.04.03		from: 25.11.2022
//#
//#	Implementation:
//#		-	ignore handling of approach signal if there is no
//#			approach signal
//#			new variable	m_bApproachSignalPresent
//#		-	set the correct LEDs to Hp0 during Init()
//#			change function
//#				SetSignalType()	the function will return 'true' if the
//#								signal is a type of 'A-Sig'
//#		-	include function GetFirstStepApproach() into function
//#			StartAspectTransitionApproach()
//#
//#-------------------------------------------------------------------------
//#
//#	Version:	x.04.02		from: 25.11.2022
//#
//#	Implementation:
//#		-	new use of LED_GREEN and LED_BLUE for debugging:
//#				LED_GREEN:	wait time for signal aspect is active
//#							signal aspect transition is active
//#				LED_BLUE:	fading is active
//#		-	removed LED_N for debugging purpose
//#
//#-------------------------------------------------------------------------
//#
//#	Version:	x.04.01		from: 22.10.2022
//#
//#	Bug Fix:
//#		-	switch the correct LEDs for approch signal
//#		-	correction of access to the sequences
//#
//#-------------------------------------------------------------------------
//#
//#	Version:	x.04.00		from: 22.10.2022
//#
//#	Implementation:
//#		-	rework of aspect sequences
//#			split for main aspects and approach aspects
//#
//#-------------------------------------------------------------------------
//#
//#	Version:	x.03.00		from: 12.10.2022
//#
//#	Implementation:
//#		-	implement signal aspects for the approach signal
//#
//#-------------------------------------------------------------------------
//#
//#	Version:	x.02.00		from: 11.10.2022
//#
//#	Implementation:
//#		-	implementation of signal aspects for signal type 59 finished
//#
//#-------------------------------------------------------------------------
//#
//#	Version:	x.01.02		from: 23.09.2022
//#
//#	Implementation:
//#		-	add checks to avoid a crash by wrong commands or parameters
//#		-	simplify sequencer tables
//#		-	only write to EEPROM if the new value is different
//#			than the old one
//#
//#-------------------------------------------------------------------------
//#
//#	Version:	x.01.01		from: 22.09.2022
//#
//#	Implementation:
//#		-	add version number into EEPROM
//#		-	initialize the signal with Hp0
//#		-	implement signal aspects for signal type '59'
//#
//#	Bug Fix:
//#		-	green LED will blink now in prog mode
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
bool		g_bTrackContactOccupied;


//==========================================================================
//
//		G L O B A L   F U N C T I O N S
//
//==========================================================================

//**********************************************************************
//	resetFunc
//----------------------------------------------------------------------
//	This function will reset the hole processor
//	This is implemented by jumping to the hardware reset vector.
//
void (*resetFunc)( void ) = 0;


//**********************************************************************
//	setup
//----------------------------------------------------------------------
//
void setup()
{
	g_ulBlinkTimer			= 0L;
	g_bLedOn				= false;
	g_bIsProgMode			= false;
	g_bTrackContactOccupied	= false;

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

	g_bTrackContactOccupied = g_clControl.IsContactOccupied();
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
	//	process I/Os
	//
	g_clControl.Loop();
	
	//--------------------------------------------------------------
	//	process track contact state changes
	//
	if( g_clControl.IsContactOccupied() != g_bTrackContactOccupied )
	{
		g_bTrackContactOccupied = g_clControl.IsContactOccupied();

		g_clMyLoconet.SendTrackContactMessage( g_bTrackContactOccupied );
	}

	//--------------------------------------------------------------
	//	in prog mode the green LED should blink
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

			g_bLedOn = !g_bLedOn;
		}
	}

	//--------------------------------------------------------------
	//	handle prog mode changes
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
			//
			g_bIsProgMode = true;
		}
	}
}
