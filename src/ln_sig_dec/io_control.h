
#pragma once

//##########################################################################
//#
//#		IO_ControlClass
//#
//#	This class operates the ports of the Atmel chip.
//#	here only LEDs are controlled.
//#														re	li
//#		LED		ws	ge	gr	ge	gr	KlZ	ES	ge	ws	KL	rt	rt	gr	ZA
//#				 N	 M	 L	 K	 J	 I	 H	 G	 F	 E	 D	 C	 B	 A
//#		Bit		13	12	11	10	 9	 8	 7	 6	 5	 4	 3	 2	 1	 0
//#		LNCV	44	43	42	41	40	39	38	37	36	35	34	33	32	31
//#
//#-------------------------------------------------------------------------
//#
//#	File Version:	20		from: 14.08.2024
//#
//#	Implementation:
//#		-	improvement of sequence handling
//#			change in functions
//#				SwitchExtraSignalsOn()
//#				SwitchExtraSignalsOff()
//#		
//#-------------------------------------------------------------------------
//#
//#	File Version:	19		from: 13.08.2024
//#
//#	Bug Fix:
//#		-	improvement of transition handling
//#			move all definitions to new file 'global.h'
//#			move functions to class 'TransitionClass'
//#				GetFirstStepMain()
//#				GetFirstStepApproach()
//#			move functionallity of some functions to new
//#			class 'SignalStatusClass'
//#				GetSignalAspectMain()
//#				GetSignalAspectApproach()
//#				SetSignalAspectApproach()
//#
//#
//#-------------------------------------------------------------------------
//#
//#	File Version:	18		from: 12.08.2024
//#
//#	Bug Fix:
//#		-	when approach signal will be switched off then it should
//#			go to signal aspect SIGNAL_ASPECT_HP0
//#			new function
//#				SetSignalAspectApproach()
//#
//#-------------------------------------------------------------------------
//#
//#	File Version:	17		from: 09.08.2024
//#
//#	Bug Fix:
//#		-	handling of extra signals was wrong
//#			every time all signals were handled where only a part should be
//#			change in functions
//#				SwitchExtraSignalsOn()
//#				SwitchExtraSignalsOff()
//#
//#-------------------------------------------------------------------------
//#
//#	File Version:	16		from: 05.08.2024
//#
//#	Implementation:
//#		-	add new variable for SIGNAL_TYPE_APPROACH
//#			new variable
//#				m_bApproachKennlichtSpecialHandling
//#
//#-------------------------------------------------------------------------
//#
//#	File Version:	15		from: 18.02.2024
//#
//#	Implementation:
//#		-	improve handling of extra signals
//#			rename variable
//#				m_usExtraSignals	=>	m_usExtraSignalsMain
//#			new variable
//#				m_usExtraSignalsApproach
//#
//#-------------------------------------------------------------------------
//#
//#	File Version:	14		from: 11.07.2023
//#
//#	Implementation:
//#		-	improve handling when two transitions should be handled at
//#			the same time
//#			new function
//#				ProcessTransition()
//#
//#-------------------------------------------------------------------------
//#
//#	File Version:	13		from: 30.03.2023
//#
//#	Implementation:
//#		-	add handling for a 5th rangier signal
//#
//#-------------------------------------------------------------------------
//#
//#	File Version:	12		from: 24.03.2023
//#
//#	Implementation:
//#		-	add handling for rangier signal aspects
//#			new variable
//#				m_arusRangierAspect
//#			new function
//#				HandleRangierAspect()
//#
//#-------------------------------------------------------------------------
//#
//#	File Version:	11		from: 23.03.2023
//#
//#	Implementation:
//#		-	change the handling of transitions
//#			now there is a class 'transition' to be able to handle
//#			more than one transition at the same time
//#			Lots of new functions the get access to some internal values
//#			Some functions were deleted because they moved into the new
//#			class 'transition'
//#			some local variables were delete also
//#
//#-------------------------------------------------------------------------
//#
//#	File Version:	10		from: 19.03.2023
//#
//#	Implementation:
//#		-	move definitions from 'lncv_storage.h' to here
//#				-	signal selected
//#				-	index for signal aspects
//#		-	change the way a signal aspect change is called
//#			new function
//#				HandleSignalAspect()
//#			move function from public to private
//#				HandleSignalAspectMain()
//#				HandleSignalAspectApproach()
//#
//#-------------------------------------------------------------------------
//#
//#	File Version:	9		from: 02.02.2023
//#
//#	Implementation:
//#		-	change the way of processing the sequences
//#			now you can:
//#				wait for fade finished
//#				pause for dark time
//#				pause for flash time
//#				pause for time in the next sequence byte * 10
//#			with this it is possible to overlap fades
//#			new variable
//#				m_bWaitForFadeFinished
//#
//#-------------------------------------------------------------------------
//#
//#	File Version:	8		from: 18.01.2023
//#
//#	Implementation:
//#		-	add pause times for 'dark' and 'flash'
//#			change variables
//#				from				to
//#				m_ulDarkTimer		m_ulPauseTimer
//#				m_ulDarkTime		m_uiDarkTime
//#			new variable
//#				m_uiFlashTime
//#
//#-------------------------------------------------------------------------
//#
//#	File Version:	7		from: 12.12.2022
//#
//#	Implementation:
//#		-	add handling of the track contact
//#			new variable
//#				m_bContactOccupied
//#				m_ulContactTime
//#			new function
//#				IsContactOccupied()
//#
//#-------------------------------------------------------------------------
//#
//#	File Version:	6		from: 25.11.2022
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
//#	File Version:	5		from: 18.11.2022
//#
//#	Implementation:
//#		-	split into two sets of sequences
//#
//#-------------------------------------------------------------------------
//#
//#	File Version:	4		from: 07.10.2022
//#
//#	Implementation:
//#		-	add sequencer tables for signal type 59 E-Sig
//#		-	implement signal aspects for the approach signal
//#			rename variables
//#				from						to
//#				m_bTransitionActive			m_bTransitionActiveMain
//#			new variables
//#				m_usState
//#				m_usOldState
//#				m_bTransitionActiveApproach
//#				m_bStateMachineActive
//#			rename function
//#				from						to
//#				ProcessTransition			ProcessTransitionMain
//#			new functions
//#				StartAspectTransitionApproach
//#				ProcessTransitionApproach
//#				ProcessStateMachine
//#
//#-------------------------------------------------------------------------
//#
//#	File Version:	3		from: 23.09.2022
//#
//#	Implementation:
//#		-	simplify sequencer tables
//#			delete variables
//#				m_puiSequenceMatrixMain
//#				m_puiSequenceMatrixApproach
//#			add functions
//#				GetFirstStepMain()
//#				GetFirstStepApproach()
//#
//#-------------------------------------------------------------------------
//#
//#	File Version:	2		from: 22.09.2022
//#
//#	Implementation:
//#		-	initialize the signal with Hp0
//#		-	implement signal aspects for signal type '59'
//#			add variables
//#				m_uiLedsOn
//#				m_usSignalAspectMain
//#				m_usSignalAspectMainBuffer
//#				m_usSignalAspectApproach
//#				m_usSignalAspectApproachBuffer
//#				m_uiFadeActive
//#				m_bTransitionActive
//#				m_puiSequenceMatrixMain
//#				m_puiSequenceMatrixApproach
//#				m_puiStepMain
//#				m_puiStepApproach
//#			add functions
//#				Loop()
//#				IsFadeFinished()
//#				SetSignalType()
//#				ProcessTransition()
//#			rename functions:
//#				from				to
//#				GetFadeTimes()		SetSignalType()
//#				FadeUpdate()		ProcessBrightnessAndFade()
//#
//#-------------------------------------------------------------------------
//#
//#	File Version:	1		from: 17.09.2022
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
#include "transition.h"
#include "signal_status.h"


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
//	This class operates the ports of the Atmel chip.
//	There are functions to
//		-	fade LEDs in and out
//
class IO_ControlClass
{
	public:
		IO_ControlClass();

		void Init( void );
		void Loop( void );

		void SetNightLight( bool bNightLightOn );
		void SetErsatzSignalFlash( bool bOn );

		inline void SetApproachDark( bool bDark )
		{
			m_bApproachDark = bDark;
		};

		void SwitchLed( uint8_t usLedNo, bool bOn );
		void SwitchExtraSignalsOn(	uint8_t usSelectedSignal,
									uint8_t usExtraSignals		);
		void SwitchExtraSignalsOff( uint8_t usSelectedSignal,
									uint8_t usExtraSignals		);

		void HandleLed( uint8_t usLedNo, bool bOn );
		void HandleSignalAspect(	uint8_t usSelectedSignal,
									uint8_t usSignalAspect,
									uint8_t usExtraSignals		);

		inline uint16_t GetLedsOn( void )
		{
			return( m_uiLedsOn );
		};

		inline bool IsApproachSignalPresent( void )
		{
			return( m_bApproachSignalPresent );
		};

		inline bool IsApproachDark( void )
		{
			return( m_bApproachDark );
		};

		inline bool IsFadeFinished( void )
		{
			return( 0x0000 == m_uiFadeActive );
		};

		bool IsFadeFinished( uint8_t usLedNo );

		inline bool IsFadeFinished( uint16_t uiLedMask )
		{
			return( 0x0000 == (m_uiFadeActive & uiLedMask) );
		};

		inline bool IsContactOccupied( void )
		{
			return( m_bContactOccupied );
		};

		inline bool IsOneLedOn( uint16_t uiLedMask )
		{
			return( 0x0000 != (m_uiLedsOn & uiLedMask) );
		};

		inline bool IsLedsOff( uint16_t uiLedMask )
		{
			return( 0x0000 == (m_uiLedsOn & uiLedMask) );
		};


	private:
		TransitionClass *	m_arpclTransition[ RANGIER_SIGNAL_COUNT ];

		uint32_t	m_ulLastCycleStartTime;
		uint32_t	m_ulCycleTime;
		uint32_t	m_ulCycleStepTime;
		uint32_t	m_ulContactTime;
		uint32_t	m_ulErsatzSignalFlashTimer;
		uint16_t	m_uiErsatzSignalOffTimer;
		uint16_t	m_uiLedsOn;
		uint16_t	m_uiFadeTimeUp;
		uint16_t	m_uiFadeTimeDown;
		uint16_t	m_uiFadeActive;
		uint8_t		m_usSignalType;
		uint8_t		m_usExtraSignalsMain;
		uint8_t		m_usExtraSignalsApproach;
		bool		m_bNightLight;
		bool		m_bApproachDark;
		bool		m_bErsatzSignalFlash;
		bool		m_bMainSignalPresent;
		bool		m_bApproachSignalPresent;
		bool		m_bContactOccupied;
		bool		m_bApproachKennlichtSpecialHandling;

		bool		SetSignalType( void );
		void		CalculateFadeRates( void );
		bool		ProcessTransition( uint8_t idx, uint32_t ulMillis );
		void		ProcessBrightnessAndFade( void );
};


//==========================================================================
//
//		E X T E R N   G L O B A L   V A R I A B L E S
//
//==========================================================================

extern IO_ControlClass	g_clControl;
