//##########################################################################
//#
//#		signal_status.cpp
//#
//#	Description
//#
//#-------------------------------------------------------------------------
//#
//#	File Version:	2		from: 27.01.2025
//#
//#	Implementation:
//#		-	add support for new signal type SIGNAL_TYPE_US
//#			changes in constructor and all functions
//#			new variable
//#				m_arusUsSignalAspect[]
//#
//#-------------------------------------------------------------------------
//#
//#	File Version:	1		from: 14.08.2024
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

#include "global.h"
#include "signal_status.h"
#include "sequencer_definitions.h"


//==========================================================================
//
//		D E F I N I T I O N S
//
//==========================================================================


//==========================================================================
//
//		G L O B A L   V A R I A B L E S
//
//==========================================================================

SignalStatusClass	g_clStatus	= SignalStatusClass();


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
//
SignalStatusClass::SignalStatusClass( void )
{
	m_usSignalAspectMain		= SIGNAL_ASPECT_HP0;
	m_usSignalAspectApproach	= SIGNAL_ASPECT_HP0;

	for( uint8_t idx = 0 ; RANGIER_SIGNAL_COUNT > idx ; idx++ )
	{
		m_arusRangierAspect[ idx ]	= RANGIER_ASPECT_SH0;
	}

	for( uint8_t idx = 0 ; US_SIGNAL_COUNT > idx ; idx++ )
	{
		m_arusUsSignalAspect[ idx ]	= US_SIGNAL_ASPECT_HP0;
	}
}


//******************************************************************
//	GetSignalAspect
//------------------------------------------------------------------
//
uint8_t SignalStatusClass::GetSignalAspect( uint8_t usSelectedSignal )
{
	uint8_t	usAspect;

	switch( usSelectedSignal )
	{
		case SIGNAL_SELECTED_MAIN:
			usAspect = m_usSignalAspectMain;
			break;

		case SIGNAL_SELECTED_APPROACH:
			usAspect = m_usSignalAspectApproach;
			break;

		case SIGNAL_SELECTED_RANGIER_1:
		case SIGNAL_SELECTED_RANGIER_2:
		case SIGNAL_SELECTED_RANGIER_3:
		case SIGNAL_SELECTED_RANGIER_4:
		case SIGNAL_SELECTED_RANGIER_5:
			usAspect = m_arusRangierAspect[ usSelectedSignal ];
			break;

		case SIGNAL_SELECTED_US_1:
		case SIGNAL_SELECTED_US_2:
		case SIGNAL_SELECTED_US_3:
		case SIGNAL_SELECTED_US_4:
			usAspect = m_arusUsSignalAspect[ usSelectedSignal - SIGNAL_SELECTED_US_1 ];
			break;

		default:
			usAspect = SIGNAL_ASPECT_UNKNOWN;
			break;
	}

	return( usAspect );
}


//******************************************************************
//	SetSignalAspect
//------------------------------------------------------------------
//
void SignalStatusClass::SetSignalAspect( uint8_t usSelectedSignal,
										 uint8_t usAspect			)
{
	switch( usSelectedSignal )
	{
		case SIGNAL_SELECTED_MAIN:
			m_usSignalAspectMain = usAspect;
			break;

		case SIGNAL_SELECTED_APPROACH:
			m_usSignalAspectApproach = usAspect;
			break;

		case SIGNAL_SELECTED_RANGIER_1:
		case SIGNAL_SELECTED_RANGIER_2:
		case SIGNAL_SELECTED_RANGIER_3:
		case SIGNAL_SELECTED_RANGIER_4:
		case SIGNAL_SELECTED_RANGIER_5:
			m_arusRangierAspect[ usSelectedSignal ] = usAspect;
			break;

		case SIGNAL_SELECTED_US_1:
		case SIGNAL_SELECTED_US_2:
		case SIGNAL_SELECTED_US_3:
		case SIGNAL_SELECTED_US_4:
			m_arusUsSignalAspect[ usSelectedSignal - SIGNAL_SELECTED_US_1 ] = usAspect;
			break;

		default:
			break;
	}
}
