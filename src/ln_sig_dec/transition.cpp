//##########################################################################
//#
//#		transition.cpp
//#
//#	Description
//#
//#-------------------------------------------------------------------------
//#
//#	File Version:	4		from: 13.08.2024
//#
//#	Bug Fix:
//#		-	improvement of transition handling
//#			new variables
//#				m_bDoInit
//#				m_usSignalType
//#				m_usSelectedSignal
//#				m_usTargetAspect
//#				m_usExtraSignals
//#			new function
//#				InitTransition()
//#			change in constructor
//#				TransitionClass()
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

#include "global.h"
#include "io_control.h"
#include "transition.h"
#include "time_values.h"
#include "lncv_storage.h"
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
TransitionClass::TransitionClass(	uint8_t usSignalType,
									uint8_t usSelectedSignal,
									uint8_t usTargetAspect,
									uint8_t usExtraSignals	)
{
	m_usSignalType		= usSignalType;
	m_usSelectedSignal	= usSelectedSignal;
	m_usTargetAspect	= usTargetAspect;
	m_usExtraSignals	= usExtraSignals;
	m_bDoInit			= true;
	m_bSetAspect		= true;

	m_pSubTransition		= NULL;
	m_pusStep				= NULL;
	m_ulPauseTimer			= 0L;
	m_uiLedMask				= 0x0000;
	m_usLedStartIdx			= 0;
	m_usLedStopIdx			= 0;
	m_bWaitForFadeFinished	= false;
	m_bWaitForExtraFinished	= false;
}


//******************************************************************
//	Constructor
//------------------------------------------------------------------
//
TransitionClass::TransitionClass(	uint8_t *	pusStep,
									uint16_t	uiLedMask,
									uint8_t		usLedStartIdx,
									uint8_t		usLedStopIdx	)
{
	m_pusStep				= pusStep;
	m_uiLedMask				= uiLedMask;
	m_usLedStartIdx			= usLedStartIdx;
	m_usLedStopIdx			= usLedStopIdx;
	m_bDoInit				= false;
	m_bSetAspect			= false;

	m_usSignalType			= 0;
	m_usSelectedSignal		= SIGNAL_SELECTED_APPROACH;
	m_usTargetAspect		= SIGNAL_ASPECT_VR0;
	m_usExtraSignals		= 0x00;

	m_pSubTransition		= NULL;
	m_ulPauseTimer			= 0L;
	m_bWaitForFadeFinished	= false;
	m_bWaitForExtraFinished	= false;
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

	if( m_bDoInit )
	{
		bIsFinished = InitTransition();
		
		if( bIsFinished )
		{
			return( bIsFinished );
		}
	}

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
//	InitTransition
//------------------------------------------------------------------
//
bool TransitionClass::InitTransition( void )
{
	uint8_t	usActualAspect	= g_clStatus.GetSignalAspect( m_usSelectedSignal );
	bool	bDoNotStart		= true;

	m_bDoInit = false;

	if(		(	(SIGNAL_ASPECT_HP0 == usActualAspect)
			 &&	(SIGNAL_ASPECT_HP0 != m_usTargetAspect))
		||	(	(SIGNAL_ASPECT_HP0 != usActualAspect)
			 &&	(SIGNAL_ASPECT_HP0 == m_usTargetAspect))	)
	{
		bDoNotStart = false;	//	transition should start

		switch( m_usSelectedSignal )
		{
			case SIGNAL_SELECTED_MAIN:
				m_uiLedMask		= MAIN_LEDS;
				m_usLedStartIdx	= MAIN_LEDS_IDX_START;
				m_usLedStopIdx	= MAIN_LEDS_IDX_STOP;
				m_pusStep		= GetFirstStepMain( usActualAspect );
				break;
			
			case SIGNAL_SELECTED_APPROACH:
				m_uiLedMask		= APPROACH_LEDS;
				m_usLedStartIdx	= APPROACH_LEDS_IDX_START;
				m_usLedStopIdx	= APPROACH_LEDS_IDX_STOP;
				m_pusStep		= GetFirstStepApproach( usActualAspect );
				break;

			case SIGNAL_SELECTED_RANGIER_1:
			case SIGNAL_SELECTED_RANGIER_2:
			case SIGNAL_SELECTED_RANGIER_3:
			case SIGNAL_SELECTED_RANGIER_4:
			case SIGNAL_SELECTED_RANGIER_5:
				if( SIGNAL_SELECTED_RANGIER_5 == m_usSelectedSignal )
				{
					m_usLedStartIdx	= RANGIER_5_LEDS_IDX_START;
					m_usLedStopIdx	= RANGIER_5_LEDS_IDX_STOP;
					m_uiLedMask		= RANGIER_5_LEDS;
				}
				else
				{
					m_usLedStartIdx	= RANGIER_1_LEDS_IDX_COUNT * m_usSelectedSignal;
					m_usLedStopIdx	= m_usLedStartIdx + RANGIER_1_LEDS_IDX_COUNT;
					m_uiLedMask		= RANGIER_1_LEDS << m_usLedStartIdx;
				}
				
				if( RANGIER_ASPECT_SH0 != usActualAspect )
				{
					m_pusStep	= (uint8_t *)&Rangier_to_Sh0;
				}
				else if( RANGIER_ASPECT_SH1 == m_usTargetAspect )
				{
					m_pusStep	= (uint8_t *)&Rangier_to_Sh1;
				}
				else
				{
					m_pusStep	= (uint8_t *)&Rangier_to_ShKe;
				}
				break;

			default:
				bDoNotStart = true;
				break;
		}
	}

	return( bDoNotStart );
}


//******************************************************************
//	HandleControlGroup
//------------------------------------------------------------------
//
bool TransitionClass::HandleControlGroup( uint32_t ulMillis, uint8_t usControlCode )
{
	uint16_t	uiPauseTime;
	bool		bIsFinished		= false;

	switch( usControlCode )
	{
		case SEQUENCE_END:
			//--------------------------------------------------
			//	the end of the sequence is reached
			//	so stop the transition
			//
			bIsFinished = true;

			if( m_bSetAspect )
			{
				g_clStatus.SetSignalAspect( m_usSelectedSignal, m_usTargetAspect );
			}

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
				m_pSubTransition = new TransitionClass( m_usSignalType,
														SIGNAL_SELECTED_APPROACH,
														g_clStatus.GetSignalAspect( SIGNAL_SELECTED_APPROACH ),
														0	);
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
				m_pSubTransition = new TransitionClass( m_usSignalType,
														SIGNAL_SELECTED_APPROACH,
														SIGNAL_ASPECT_HP0, 0	);
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

//******************************************************************
//	HandleLedGroups
//------------------------------------------------------------------
//
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


//******************************************************************
//	GetFirstStepMain
//------------------------------------------------------------------
//	This function will return a pointer to the first step of
//	a signal aspect sequence for the main signal
//
uint8_t * TransitionClass::GetFirstStepMain( uint8_t usFromAspect )
{
	uint8_t *	pFirstStep	= sNOP;

	if( SIGNAL_ASPECT_HP0 == usFromAspect )
	{
		switch( m_usSignalType )
		{
			case SIGNAL_TYPE_59_A_SIG:
			case SIGNAL_TYPE_60_A_SIG:
			case SIGNAL_TYPE_COMPACT_A_SIG:
			case SIGNAL_TYPE_KS_A_SIG:
				pFirstStep = pgm_read_word( &A_Sig_Main_From_Hp0[ m_usTargetAspect ] );
				break;

			case SIGNAL_TYPE_59_E_SIG:
			case SIGNAL_TYPE_60_E_SIG:
			case SIGNAL_TYPE_COMPACT_E_SIG:
			case SIGNAL_TYPE_KS_E_SIG:
				pFirstStep = pgm_read_word( &E_Sig_Main_From_Hp0[ m_usTargetAspect ] );
				break;

			case SIGNAL_TYPE_DIRECT_CONTROL_HARD:
			case SIGNAL_TYPE_DIRECT_CONTROL_FAST:
			case SIGNAL_TYPE_DIRECT_CONTROL_MIDDLE:
			case SIGNAL_TYPE_DIRECT_CONTROL_SLOW:
			default:
				break;
		}
	}
	else if( SIGNAL_ASPECT_HP0 == m_usTargetAspect )
	{
		switch( m_usSignalType )
		{
			case SIGNAL_TYPE_59_A_SIG:
			case SIGNAL_TYPE_60_A_SIG:
			case SIGNAL_TYPE_COMPACT_A_SIG:
			case SIGNAL_TYPE_KS_A_SIG:
				pFirstStep = pgm_read_word( &A_Sig_Main_To_Hp0[ usFromAspect ] );
				break;

			case SIGNAL_TYPE_59_E_SIG:
			case SIGNAL_TYPE_60_E_SIG:
			case SIGNAL_TYPE_COMPACT_E_SIG:
			case SIGNAL_TYPE_KS_E_SIG:
				pFirstStep = pgm_read_word( &E_Sig_Main_To_Hp0[ usFromAspect ] );
				break;

			case SIGNAL_TYPE_DIRECT_CONTROL_HARD:
			case SIGNAL_TYPE_DIRECT_CONTROL_FAST:
			case SIGNAL_TYPE_DIRECT_CONTROL_MIDDLE:
			case SIGNAL_TYPE_DIRECT_CONTROL_SLOW:
			default:
				break;
		}
	}

	return( pFirstStep );
}


//******************************************************************
//	GetFirstStepApproach
//------------------------------------------------------------------
//
uint8_t * TransitionClass::GetFirstStepApproach( uint8_t usFromAspect )
{
	uint8_t *	pFirstStep	= sNOP;

	if( SIGNAL_ASPECT_HP0 == usFromAspect )
	{
		switch( m_usSignalType )
		{
			case SIGNAL_TYPE_DIRECT_CONTROL_HARD:
			case SIGNAL_TYPE_DIRECT_CONTROL_FAST:
			case SIGNAL_TYPE_DIRECT_CONTROL_MIDDLE:
			case SIGNAL_TYPE_DIRECT_CONTROL_SLOW:
			case SIGNAL_TYPE_59_A_SIG:
			case SIGNAL_TYPE_59_E_SIG:
			case SIGNAL_TYPE_60_A_SIG:
			case SIGNAL_TYPE_60_E_SIG:
			case SIGNAL_TYPE_COMPACT_A_SIG:
			case SIGNAL_TYPE_COMPACT_E_SIG:
			case SIGNAL_TYPE_KS_A_SIG:
			case SIGNAL_TYPE_KS_E_SIG:
				pFirstStep = pgm_read_word( &Approach_From_Hp0[ m_usTargetAspect ] );
				break;

			default:
				break;
		}
	}
	else if( SIGNAL_ASPECT_HP0 == m_usTargetAspect )
	{
		switch( m_usSignalType )
		{
			case SIGNAL_TYPE_DIRECT_CONTROL_HARD:
			case SIGNAL_TYPE_DIRECT_CONTROL_FAST:
			case SIGNAL_TYPE_DIRECT_CONTROL_MIDDLE:
			case SIGNAL_TYPE_DIRECT_CONTROL_SLOW:
			case SIGNAL_TYPE_59_A_SIG:
			case SIGNAL_TYPE_59_E_SIG:
			case SIGNAL_TYPE_60_A_SIG:
			case SIGNAL_TYPE_60_E_SIG:
			case SIGNAL_TYPE_COMPACT_A_SIG:
			case SIGNAL_TYPE_COMPACT_E_SIG:
			case SIGNAL_TYPE_KS_A_SIG:
			case SIGNAL_TYPE_KS_E_SIG:
				pFirstStep = pgm_read_word( &Approach_To_Hp0[ usFromAspect ] );
				break;

			default:
				break;
		}
	}

	return( pFirstStep );
}
