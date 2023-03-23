
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

#include "transition.h"


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

#define LED_MAIN_ZUSATZANZEIGER			LED_A
#define LED_MAIN_GREEN					LED_B
#define LED_MAIN_RED_LEFT				LED_C
#define LED_MAIN_RED_RIGHT				LED_D
#define LED_MAIN_KENNLICHT				LED_E
#define LED_MAIN_SH1					LED_F
#define LED_MAIN_YELLOW					LED_G
#define LED_ERSATZSIGNAL				LED_H
#define LED_APPROACH_KENNLICHT			LED_I
#define LED_APPROACH_GREEN_TOP			LED_J
#define LED_APPROACH_YELLOW_TOP			LED_K
#define LED_APPROACH_GREEN_BOTTOM		LED_L
#define LED_APPROACH_YELLOW_BOTTOM		LED_M
#define LED_APPROACH_ZUSATZANZEIGER		LED_N
#define LED_ERSATZSIGNAL_FLASH			(LED_N + 1)

//----------------------------------------------------------------------
//	extra signal numbers
//
#define EXTRA_SIGNAL_LEDS					0x2181

#define EXTRA_SIGNAL_ZUSATZANZEIGE			0
#define EXTRA_SIGNAL_ERSATZSIGNAL			1
#define EXTRA_SIGNAL_KENNLICHT				2
#define EXTRA_SIGNAL_ANZEIGE				3
#define EXTRA_SIGNAL_ERSATZSIGNAL_FLASH		4

#define EXTRA_SIGNAL_COUNT					5

//----------------------------------------------------------------------
//	selected signals
//
#define SIGNAL_SELECTED_MAIN		7
#define SIGNAL_SELECTED_APPROACH	8

#define SIGNAL_SELECTED_RANGIER_1	0
#define SIGNAL_SELECTED_RANGIER_2	1
#define SIGNAL_SELECTED_RANGIER_3	2
#define SIGNAL_SELECTED_RANGIER_4	3

#define RANGIER_SIGNAL_COUNT		4

//----------------------------------------------------------------------
//	index for signal aspects
//

//----	main signal  -----------------------------------------------
#define MAIN_LEDS						0x007E

#define IDX_SIGNAL_MAIN_BIT_0			1
#define IDX_SIGNAL_MAIN_BIT_1			2
#define IDX_SIGNAL_MAIN_BIT_2			3
#define IDX_SIGNAL_MAIN_BIT_3			4

#define MAIN_LEDS_IDX_START				IDX_SIGNAL_MAIN_BIT_0
#define MAIN_LEDS_IDX_COUNT				6
#define MAIN_LEDS_IDX_STOP				(MAIN_LEDS_IDX_START + MAIN_LEDS_IDX_COUNT)

//----	approach signal  -------------------------------------------
#define APPROACH_LEDS					0x1E00

#define IDX_SIGNAL_APPROACH_BIT_0		9
#define IDX_SIGNAL_APPROACH_BIT_1		10
#define IDX_SIGNAL_APPROACH_BIT_2		11
#define IDX_SIGNAL_APPROACH_BIT_3		12

#define APPROACH_LEDS_IDX_START			IDX_SIGNAL_APPROACH_BIT_0
#define APPROACH_LEDS_IDX_COUNT			4
#define APPROACH_LEDS_IDX_STOP			(APPROACH_LEDS_IDX_START + APPROACH_LEDS_IDX_COUNT)

//----	rangier signal 1  ------------------------------------------
#define RANGIER_1_LEDS					0x0007

#define IDX_RANGIER_SIGNAL_1_BIT_0		0
#define IDX_RANGIER_SIGNAL_1_BIT_1		1

#define RANGIER_1_LEDS_IDX_START		IDX_RANGIER_SIGNAL_1_BIT_0
#define RANGIER_1_LEDS_IDX_COUNT		3
#define RANGIER_1_LEDS_IDX_STOP			(RANGIER_1_LEDS_IDX_START + RANGIER_1_LEDS_IDX_COUNT)

//----	rangier signal 2  ------------------------------------------
#define RANGIER_2_LEDS					0x0038

#define IDX_RANGIER_SIGNAL_2_BIT_0		2
#define IDX_RANGIER_SIGNAL_2_BIT_1		3

#define RANGIER_2_LEDS_IDX_START		IDX_RANGIER_SIGNAL_2_BIT_0
#define RANGIER_2_LEDS_IDX_COUNT		3
#define RANGIER_2_LEDS_IDX_STOP			(RANGIER_2_LEDS_IDX_START + RANGIER_2_LEDS_IDX_COUNT)

//----	rangier signal 3  ------------------------------------------
#define RANGIER_3_LEDS					0x01C0

#define IDX_RANGIER_SIGNAL_3_BIT_0		4
#define IDX_RANGIER_SIGNAL_3_BIT_1		5

#define RANGIER_3_LEDS_IDX_START		IDX_RANGIER_SIGNAL_3_BIT_0
#define RANGIER_3_LEDS_IDX_COUNT		3
#define RANGIER_3_LEDS_IDX_STOP			(RANGIER_3_LEDS_IDX_START + RANGIER_3_LEDS_IDX_COUNT)

//----	rangier signal 4  ------------------------------------------
#define RANGIER_4_LEDS					0x0E00

#define IDX_RANGIER_SIGNAL_4_BIT_0		6
#define IDX_RANGIER_SIGNAL_4_BIT_1		7

#define RANGIER_4_LEDS_IDX_START		IDX_RANGIER_SIGNAL_4_BIT_0
#define RANGIER_4_LEDS_IDX_COUNT		3
#define RANGIER_4_LEDS_IDX_STOP			(RANGIER_4_LEDS_IDX_START + RANGIER_4_LEDS_IDX_COUNT)


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
		void SwitchExtraSignalsOn( void );
		void SwitchExtraSignalsOff( void );

		void HandleLed( uint8_t usLedNo, bool bOn );
		void HandleSignalAspect(	uint8_t usSelectedSignal,
									uint8_t usSignalAspect,
									uint8_t usExtraSignals		);

		inline uint16_t GetLedsOn( void )
		{
			return( m_uiLedsOn );
		};

		inline uint8_t GetSignalAspectMain( void )
		{
			return( m_usSignalAspectMain );
		};

		inline uint8_t GetSignalAspectApproach( void )
		{
			return( m_usSignalAspectApproach );
		};

		uint8_t *	GetFirstStepMain( uint8_t usFromAspect, uint8_t usToAspect );
		uint8_t *	GetFirstStepApproach( uint8_t usAspectFrom, uint8_t usAspectTo );

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
		uint8_t		m_usSignalAspectMain;
		uint8_t		m_usSignalAspectApproach;
		uint8_t		m_usExtraSignals;
		bool		m_bNightLight;
		bool		m_bApproachDark;
		bool		m_bErsatzSignalFlash;
		bool		m_bMainSignalPresent;
		bool		m_bApproachSignalPresent;
		bool		m_bContactOccupied;

		bool		SetSignalType( void );
		void		CalculateFadeRates( void );
		void		HandleSignalAspectMain( uint8_t usSignalAspect, uint8_t usExtraSignals );
		void		HandleSignalAspectApproach( uint8_t usSignalAspect, uint8_t usExtraSignals );
		void		ProcessBrightnessAndFade( void );
};


//==========================================================================
//
//		E X T E R N   G L O B A L   V A R I A B L E S
//
//==========================================================================

extern IO_ControlClass	g_clControl;
