//##########################################################################
//#
//#		transition.cpp
//#
//#	Description
//#
//#-------------------------------------------------------------------------
//#
//#	File Version:	3		from: 12.08.2024
//#
//#	Bug Fix:
//#		-	when approach signal will be switched off then it should
//#			go to signal aspect SIGNAL_ASPECT_HP0
//#			change in function
//#				HandleControlGroup()
//#
//#-------------------------------------------------------------------------
//#
//#	File Version:	2		from: 09.08.2024
//#
//#	Bug Fix:
//#		-	handling of extra signals was wrong
//#			every time all signals were handled where only a part should be
//#			change in function
//#				HandleControlGroup()
//#
//#-------------------------------------------------------------------------
//#
//#	File Version:	1		from: 19.03.2023
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

#include "io_control.h"
#include "transition.h"
#include "time_values.h"
#include "sequencer_tables.h"


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
TransitionClass::TransitionClass(	uint8_t *	pusStep,
									uint16_t	uiLedMask,
									uint8_t		usStartIdx,
									uint8_t		usStopIdx	)
{
	m_pSubTransition		= NULL;
	m_ulPauseTimer			= 0L;
	m_bWaitForFadeFinished	= false;
	m_bWaitForExtraFinished	= false;

	m_uiLedMask		= uiLedMask;
	m_usLedStartIdx	= usStartIdx;
	m_usLedStopIdx	= usStopIdx;
	m_pusStep		= pusStep;
}

//******************************************************************
//	Loop
//------------------------------------------------------------------
//
bool TransitionClass::Loop( uint32_t ulMillis )
{
	uint8_t	usAspect;
	uint8_t	usControlGroup;
	bool	bIsFinished		= false;

	if( NULL != m_pSubTransition )
	{
		//------------------------------------------------------
		//	if there is a sub transition
		//	then first process the sub transition
		//
		if( m_pSubTransition->Loop( ulMillis ) )
		{
			delete m_pSubTransition;

			m_pSubTransition = NULL;
		}
	}
	else if( 0 < m_ulPauseTimer )
	{
		//------------------------------------------------------
		//	if the pause timer has elapsed
		//	then switch off the timer
		//
		if( ulMillis > m_ulPauseTimer )
		{
			m_ulPauseTimer = 0L;
		}
	}
	else if( m_bWaitForFadeFinished )
	{
		//------------------------------------------------------
		//	wait until all relevant actions are finished
		//
		if( g_clControl.IsFadeFinished( m_uiLedMask ) )
		{
			m_bWaitForFadeFinished = false;
		}
	}
	else if( m_bWaitForExtraFinished )
	{
		//------------------------------------------------------
		//	wait until all extra signals are finished
		//
		if( g_clControl.IsFadeFinished( (uint16_t)EXTRA_SIGNAL_LEDS ) )
		{
			m_bWaitForExtraFinished = false;
		}
	}
	else
	{
		//------------------------------------------------------
		//	precess the next sequence step
		//
		usAspect = pgm_read_byte( m_pusStep );

		m_pusStep++;

		usControlGroup = usAspect & SEQUENCE_CONTROL_GROUPS;

		if( SG_CONTROL_WORDS == usControlGroup )
		{
			bIsFinished = HandleControlGroup( ulMillis, usAspect );
		}
		else
		{
			HandleLedGroups( ulMillis, usControlGroup, usAspect );
		}
	}

	return( bIsFinished );
}

//******************************************************************
//	HandleControlGroup
//------------------------------------------------------------------
//
bool TransitionClass::HandleControlGroup( uint32_t ulMillis, uint8_t usControlCode )
{
	uint16_t	uiPauseTime;
	uint8_t		usAspect		= g_clControl.GetSignalAspectApproach();
	bool		bIsFinished		= false;

	switch( usControlCode )
	{
		case SEQUENCE_END:
			//--------------------------------------------------
			//	the end of the sequence is reached
			//	so stop the transition
			//
			bIsFinished = true;

			g_clControl.SwitchLed( LED_GREEN, false );
			break;

		case SEQUENCE_WAIT_FOR_FADE_FINISHED:
			//--------------------------------------------------
			//	wait until all fade actions are finished
			//
			m_bWaitForFadeFinished = true;
			break;

		case SEQUENCE_PAUSE:
			//--------------------------------------------------
			//	make a pause in the sequence
			//
			uiPauseTime		= pgm_read_byte( m_pusStep ) * 10;
			m_ulPauseTimer	= ulMillis + uiPauseTime;
			m_pusStep++;
			break;

		case SEQUENCE_PAUSE_DARK:
			//--------------------------------------------------
			//	make a pause in the sequence with the length
			//	of 'DarkTime'
			//
			m_ulPauseTimer	= ulMillis + g_stTimeValues.uiDarkTime;
			break;

		case SEQUENCE_PAUSE_FLASH:
			//--------------------------------------------------
			//	make a pause in the sequence with the length
			//	of 'FlashTime'
			//
			m_ulPauseTimer	= ulMillis + g_stTimeValues.uiFlashTime;
			break;

		case SEQUENCE_APPROACH_ON:
			//--------------------------------------------------
			//	the approach signal should be switched ON
			//
			if(		g_clControl.IsApproachSignalPresent()
				&& !g_clControl.IsApproachDark()
				&&  g_clControl.IsLedsOff( APPROACH_LEDS )	)
			{
				m_pSubTransition = new TransitionClass( (uint8_t *)&ApproachOn,
														APPROACH_LEDS,
														APPROACH_LEDS_IDX_START,
														APPROACH_LEDS_IDX_STOP		);
			}
			break;

		case SEQUENCE_SET_SIGNAL_ASPECT:
			//--------------------------------------------------
			//	check if the approach signal should show
			//	a signal aspect
			//
			if(		g_clControl.IsApproachSignalPresent()
				&& !g_clControl.IsApproachDark()
				&&  g_clControl.IsOneLedOn( APPROACH_LEDS )	)
			{
				if( SIGNAL_ASPECT_HP0 != usAspect )
				{
					m_pSubTransition = new TransitionClass( g_clControl.GetFirstStepApproach( SIGNAL_ASPECT_HP0, usAspect ),
															APPROACH_LEDS, APPROACH_LEDS_IDX_START, APPROACH_LEDS_IDX_STOP );
				}
			}
			break;

		case SEQUENCE_SET_HP0:
			//--------------------------------------------------
			//	check if the approach signal should show Hp0
			//
			if(		g_clControl.IsApproachSignalPresent()
				&& !g_clControl.IsApproachDark()
				&&  g_clControl.IsOneLedOn( APPROACH_LEDS )	)
			{
				if( SIGNAL_ASPECT_HP0 != usAspect )
				{
					m_pSubTransition = new TransitionClass( g_clControl.GetFirstStepApproach( usAspect, SIGNAL_ASPECT_HP0 ),
															APPROACH_LEDS, APPROACH_LEDS_IDX_START, APPROACH_LEDS_IDX_STOP );
				}
			}
			break;

		case SEQUENCE_APPROACH_OFF:
			//--------------------------------------------------
			//	the approach signal should be switched OFF
			//
			if(		g_clControl.IsApproachSignalPresent()
				&& !g_clControl.IsApproachDark()
				&&  g_clControl.IsOneLedOn( APPROACH_LEDS )	)
			{
				m_pSubTransition = new TransitionClass( (uint8_t *)&ApproachOff,
														APPROACH_LEDS,
														APPROACH_LEDS_IDX_START,
														APPROACH_LEDS_IDX_STOP		);

				g_clControl.SetSignalAspectApproach( SIGNAL_ASPECT_HP0 );
			}
			break;

		case SEQUENCE_ADDITION_SIGNAL_MAIN_ON:
			g_clControl.SwitchExtraSignalsOn( SIGNAL_SELECTED_MAIN );
			m_bWaitForExtraFinished = true;
			break;

		case SEQUENCE_ADDITION_SIGNAL_MAIN_OFF:
			g_clControl.SwitchExtraSignalsOff( SIGNAL_SELECTED_MAIN );
			m_bWaitForExtraFinished = true;
			break;

		case SEQUENCE_ADDITION_SIGNAL_APPROACH_ON:
			g_clControl.SwitchExtraSignalsOn( SIGNAL_SELECTED_APPROACH );
			m_bWaitForExtraFinished = true;
			break;

		case SEQUENCE_ADDITION_SIGNAL_APPROACH_OFF:
			g_clControl.SwitchExtraSignalsOff( SIGNAL_SELECTED_APPROACH );
			m_bWaitForExtraFinished = true;
			break;
	}

	return( bIsFinished );
}

void TransitionClass::HandleLedGroups( uint32_t ulMillis, uint8_t usControlGroup, uint8_t usAspect )
{
	uint8_t		usDiff;
	uint8_t		usMask		= 0x01;

	//----------------------------------------------------------
	//	check wich LEDs should go OFF and wich should go ON
	//
	usDiff  = (uint8_t)((g_clControl.GetLedsOn() & m_uiLedMask) >> m_usLedStartIdx);
	usDiff ^= (usAspect & SEQUENCE_LEDS);

	for( uint8_t idx = m_usLedStartIdx ; (m_usLedStopIdx > idx) && (usDiff != 0) ; idx++ )
	{
		if( usDiff & usMask )
		{
			if( usAspect & usMask )
			{
				if( SG_FLASHING_LEDS == usControlGroup )
				{
					g_clControl.SwitchLed( idx, true );
					m_ulPauseTimer = ulMillis + g_stTimeValues.uiFlashTime;
				}
				else
				{
					g_clControl.HandleLed( idx, true );
				}
			}
			else
			{
				if( SG_FLASHING_LEDS == usControlGroup )
				{
					g_clControl.SwitchLed( idx, false );
					m_ulPauseTimer = ulMillis + g_stTimeValues.uiFlashTime;
				}
				else
				{
					g_clControl.HandleLed( idx, false );
				}
			}

			usDiff &= ~usMask;
		}

		usMask <<= 1;
	}
}
