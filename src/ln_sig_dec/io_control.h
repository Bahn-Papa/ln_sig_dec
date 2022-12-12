
#pragma once

//##########################################################################
//#
//#		IO_ControlClass
//#
//#	This class operates the ports of the Atmel chip.
//#	here only LEDs are controlled by the help of the PWM library
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


//==========================================================================
//
//		D E F I N I T I O N S
//
//==========================================================================

#define TOTAL_LEDS			16
#define	SIGNAL_LEDS			14

#define	LED_A				0
#define	LED_B				1
#define	LED_C				2
#define	LED_D				3
#define	LED_E				4
#define	LED_F				5
#define	LED_G				6
#define	LED_H				7
#define	LED_I				8
#define	LED_J				9
#define	LED_K				10
#define	LED_L				11
#define	LED_M				12
#define	LED_N				13

#define	LED_BLUE			15
#define LED_GREEN			14


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

		void HandleLed( uint8_t usLedNo, bool bOn );
		void HandleSignalAspectMain( uint8_t usSignalAspect );
		void HandleSignalAspectApproach( uint8_t usSignalAspect );

		void SwitchLed( uint8_t usLedNo, bool bOn );

		bool IsFadeFinished( void );
		bool IsFadeFinished( uint8_t usLedNo );

		inline bool IsContactOccupied( void )
		{
			return( m_bContactOccupied );
		}


	private:
		uint8_t *	m_pusStepMain;
		uint8_t *	m_pusStepApproach;

		uint32_t	m_ulLastCycleStartTime;
		uint32_t	m_ulCycleTime;
		uint32_t	m_ulCycleStepTime;
		uint32_t	m_ulContactTime;
		uint16_t	m_uiLedsOn;
		uint16_t	m_uiFadeTimeUp;
		uint16_t	m_uiFadeTimeDown;
		uint16_t	m_uiFadeActive;
		uint8_t		m_usSignalType;
		uint8_t		m_usSignalAspectMain;
		uint8_t		m_usSignalAspectMainBuffer;
		uint8_t		m_usSignalAspectApproach;
		uint8_t		m_usSignalAspectApproachBuffer;
		uint8_t		m_usState;
		uint8_t		m_usOldState;
		bool		m_bNightLight;
		bool		m_bApproachSignalPresent;
		bool		m_bTransitionActiveMain;
		bool		m_bTransitionActiveApproach;
		bool		m_bStateMachineActive;
		bool		m_bContactOccupied;

		bool		SetSignalType( void );
		void		CalculateFadeRates( void );
		uint8_t *	GetFirstStepMain( uint8_t usFromAspect, uint8_t usToAspect );
		void		StartAspectTransitionApproach( uint8_t usAspectFrom, uint8_t usAspectTo );
		void		ProcessTransitionMain( void );
		void		ProcessTransitionApproach( void );
		void		ProcessStateMachine( void );
		void		ProcessBrightnessAndFade( void );
};


//==========================================================================
//
//		E X T E R N   G L O B A L   V A R I A B L E S
//
//==========================================================================

extern IO_ControlClass	g_clControl;
