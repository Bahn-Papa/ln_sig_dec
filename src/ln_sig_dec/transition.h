
#pragma once

//##########################################################################
//#
//#		transition.h
//#
//#	Description
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
		TransitionClass(	uint8_t *pusStep, uint16_t uiLedMask,
							uint8_t usStartIdx, uint8_t usStopIdx );

		bool Loop( uint32_t	ulMillis );

	protected:
		bool HandleControlGroup( uint32_t ulMillis, uint8_t usAspect );
		void HandleLedGroups( uint32_t ulMillis, uint8_t usControlGroup, uint8_t usAspect );

	private:
		TransitionClass *	m_pSubTransition;
		uint32_t			m_ulPauseTimer;
		uint16_t			m_uiLedMask;
		uint8_t				m_usLedStartIdx;
		uint8_t				m_usLedStopIdx;
		uint8_t *			m_pusStep;
		bool				m_bWaitForFadeFinished;
		bool				m_bWaitForExtraFinished;
};
