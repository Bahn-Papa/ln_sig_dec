
#pragma once

//##########################################################################
//#
//#		transition.h
//#
//#	Description
//#
//#-------------------------------------------------------------------------
//#
//#	File Version:	2		from: 13.08.2024
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

#include <stdint.h>


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
//	CLASS:	IO_ControlClass
//
class TransitionClass
{
	public:
		TransitionClass(	uint8_t usSignalType,
							uint8_t usSelectedSignal,
							uint8_t usTargetAspect,
							uint8_t usExtraSignals	);

		TransitionClass(	uint8_t *	pusStep,
							uint16_t	uiLedMask,
							uint8_t		usLedStartIdx,
							uint8_t		usLedStopIdx	);

		bool Loop( uint32_t	ulMillis );

	protected:
		bool InitTransition( void );
		bool HandleControlGroup( uint32_t ulMillis, uint8_t usAspect );
		void HandleLedGroups( uint32_t ulMillis, uint8_t usControlGroup, uint8_t usAspect );

		uint8_t * GetFirstStepMain( uint8_t usFromAspect );
		uint8_t * GetFirstStepApproach( uint8_t usFromAspect );

	private:
		uint8_t				m_usSignalType;
		uint8_t				m_usSelectedSignal;
		uint8_t				m_usTargetAspect;
		uint8_t				m_usExtraSignals;
		TransitionClass *	m_pSubTransition;
		uint32_t			m_ulPauseTimer;
		uint16_t			m_uiLedMask;
		uint8_t				m_usLedStartIdx;
		uint8_t				m_usLedStopIdx;
		uint8_t *			m_pusStep;
		bool				m_bDoInit;
		bool				m_bSetAspect;
		bool				m_bWaitForFadeFinished;
		bool				m_bWaitForExtraFinished;
};
