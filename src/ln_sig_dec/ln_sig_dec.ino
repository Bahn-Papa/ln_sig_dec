//##########################################################################
//#
//#	ln_sig_dec		copyright: Michael Pfeil
//#
//#	This program will control a light signal which consists of LEDs.
//#	The LEDs will be dimmed when lighting up or down.
//#
//#-------------------------------------------------------------------------
//#
//#	File version:	1		vom: 01.04.2026
//#
//#	Implementation:
//#		-	change handling of version numbers
//#			move definitions and project/change history to version_info.h
//#
//##########################################################################


//==========================================================================
//
//		I N C L U D E S
//
//==========================================================================

#include <Arduino.h>

#include "version_info.h"
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
uint32_t	g_ulContactOffDelayTimer;
uint16_t	g_uiDelayTime;
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
	g_ulBlinkTimer				= 0L;
	g_ulContactOffDelayTimer	= 0L;
	g_bLedOn					= false;
	g_bIsProgMode				= false;
	g_bTrackContactOccupied		= false;

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

	g_bTrackContactOccupied	= g_clControl.IsContactOccupied();
	g_uiDelayTime			= g_clLncvStorage.ReadLNCV( LNCV_ADR_TRACK_CONTACT_DELAY_TIME );
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
	if( g_ulContactOffDelayTimer )
	{
		//----------------------------------------------------------
		//	the off delay timer is aktive
		//
		if( millis() > g_ulContactOffDelayTimer )
		{
			//--------------------------------------------------
			//	timer elapsed, so
			//	-	switch timer off
			//	-	send occupied state to Loconet
			//
			g_ulContactOffDelayTimer	= 0L;
			g_bTrackContactOccupied		= g_clControl.IsContactOccupied();

			g_clMyLoconet.SendTrackContactMessage( g_bTrackContactOccupied );
		}
		else if( g_clControl.IsContactOccupied() == g_bTrackContactOccupied )
		{
			//--------------------------------------------------
			//	the contact is occupied again and before
			//	the timer elapsed, so stop the timer
			//
			g_ulContactOffDelayTimer = 0L;
		}
	}
	else if( g_clControl.IsContactOccupied() != g_bTrackContactOccupied )
	{
		//----------------------------------------------------------
		//	track contact changed state
		//	-	new state is occupied	=> send LN message direct
		//	-	new state is free		=> start delay timer
		//
		if( g_clControl.IsContactOccupied() )
		{
			g_bTrackContactOccupied = g_clControl.IsContactOccupied();

			g_clMyLoconet.SendTrackContactMessage( g_bTrackContactOccupied );
		}
		else if( g_uiDelayTime )
		{
			g_ulContactOffDelayTimer = millis() + g_uiDelayTime;
		}
		else
		{
			//------------------------------------------------------
			//	no delay time configured, so send free message
			//	direct
			g_bTrackContactOccupied = g_clControl.IsContactOccupied();

			g_clMyLoconet.SendTrackContactMessage( g_bTrackContactOccupied );
		}
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
