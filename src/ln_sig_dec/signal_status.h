
#pragma once

//##########################################################################
//#
//#		signal_status.h
//#
//#	Description
//#
//#-------------------------------------------------------------------------
//#
//#	File Version:	2		from: 29.01.2025
//#
//#	Implementation:
//#		-	add support for new signal type SIGNAL_TYPE_US
//#			new variables
//#				m_arbUsSignalOn[]
//#				m_arusUsSignalAspect[]
//#			new functions
//#				GetSignalOn()
//#				SetSignalOn()
//#
//#-------------------------------------------------------------------------
//#
//#	File Version:	1		from: 13.08.2024
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

#include <stdint.h>

#include "global.h"


//==========================================================================
//
//		D E F I N I T I O N S
//
//==========================================================================


//==========================================================================
//
//		C L A S S   D E F I N I T I O N S
//
//==========================================================================

////////////////////////////////////////////////////////////////////////
//	CLASS:	SignalStatusClass
//
//	This class will hold the state of the signal(s) and transitions
//
class SignalStatusClass
{
	public:
		SignalStatusClass( void );

		uint8_t	GetSignalAspect( uint8_t usSelectedSignal );
		void	SetSignalAspect( uint8_t usSelectedSignal, uint8_t usAspect );

		bool	GetSignalOn( uint8_t usSelectedSignal );
		void	SetSignalOn( uint8_t usSelectedSignal, bool bIsOn );

	private:
		uint8_t		m_arusRangierAspect[ RANGIER_SIGNAL_COUNT ];
		uint8_t		m_arusUsSignalAspect[ US_SIGNAL_COUNT ];
		bool		m_arbUsSignalOn[ US_SIGNAL_COUNT ];
		uint8_t		m_usSignalAspectMain;
		uint8_t		m_usSignalAspectApproach;
};


//==========================================================================
//
//		E X T E R N   G L O B A L   V A R I A B L E S
//
//==========================================================================

extern SignalStatusClass	g_clStatus;
