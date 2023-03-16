//##########################################################################
//#
//#		io_control.cpp
//#
//#	This class operates the ports of the Atmel chip.
//#	here the LEDs, the track contact and the micro switch are controlled
//#
//#-------------------------------------------------------------------------
//#
//#	14 LEDs are connected to the chip.
//#	See the table below for the assignment:
//#
//#		LED		Board Pin	Port Bit	Nano	Description
//#		 A			X3.2		D0		  0		Zusatzsanzeiger (ZS3)
//#		 B			X3.3		C5		 19		Hp1
//#		 C			X3.4		C4		 18		Hp00 Zugfahrt
//#		 D			X3.5		C3		 17		Hp00 Rangierfahrt
//#		 E			X3.6		C2		 16		Kennlicht
//#		 F			X3.7		C1		 15		SH1 Rangierfahrt
//#		 G			X3.8		C0		 14		Hp2
//#		 H			X1.2		D7		  7		Ersatzsignal (ZS1)
//#		 I			X1.3		D6		  6		Zusatz-Kennlicht
//#		 J			X1.4		D5		  5		grün oben
//#		 K			X1.5		D4		  4		gelb oben
//#		 L			X1.6		D3		  3		grün unten
//#		 M			X1.7		D2		  2		gelb unten
//#		 N			X1.8		D1		  1
//#
//#-------------------------------------------------------------------------
//#
//#	File Version:	17		from: 16.03.2023
//#
//#	Bug Fix:
//#		-	switching between night and day
//#			change in function
//#				SetNightLight()
//#
//#-------------------------------------------------------------------------
//#
//#	File Version:	16		from: 07.02.2023
//#
//#	Implementation:
//#		-	update the handling of Ersatzsignal flash and
//#			extra signals ON/OFF
//#			change in functions
//#				Loop()
//#				SwitchExtraSignalsOn()
//#				SwitchExtraSignalsOff()
//#
//#-------------------------------------------------------------------------
//#
//#	File Version:	15		from: 02.02.2023
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
//#			change in functions
//#				Init()
//#				Loop()
//#				ProcessTransitionMain()
//#				ProcessTransitionApproach()
//#				ProcessStateMachine()
//#
//#-------------------------------------------------------------------------
//#
//#	File Version:	14		from: 18.01.2023
//#
//#	Implementation:
//#		-	add pause times for 'dark' and 'flash'
//#			change variables
//#				from				to
//#				m_ulDarkTimer		m_ulPauseTimer
//#				m_ulDarkTime		m_uiDarkTime
//#			new variable
//#				m_uiFlashTime
//#			changes in functions
//#				Init()
//#				ProcessTransitionMain()
//#				ProcessTransitionApproach()
//#
//#-------------------------------------------------------------------------
//#
//#	File Version:	13		from: 12.12.2022
//#
//#	Implementation:
//#		-	add handling of the track contact
//#			new variable
//#				m_bContactOccupied
//#				m_ulContactTime
//#			new function
//#				IsContactOccupied()
//#			changed functions
//#				Init()
//#				Loop()
//#
//#-------------------------------------------------------------------------
//#
//#	File Version:	12		from: 29.11.2022
//#
//#	Bug Fix:
//#		-	LED handling blocked if a LED was switched to the actual
//#			state again.
//#			changes in function
//#				HandleLed()
//#
//#-------------------------------------------------------------------------
//#
//#	File Version:	11		from: 28.11.2022
//#
//#	Bug Fix:
//#		-	in function 'ProcessStateMachine()' the call to change the
//#			approach aspect to Vr0 had a parameter mismatch.
//#
//#-------------------------------------------------------------------------
//#
//#	File Version:	10		from: 25.11.2022
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
//#	File Version:	9		from: 25.11.2022
//#
//#	Implementation:
//#		-	new use of LED_GREEN and LED_BLUE for debugging:
//#				LED_GREEN:	wait time for signal aspect is active
//#							signal aspect transition is active
//#				LED_BLUE:	fading is active
//#		-	removed LED_N for debugging purpose
//#
//#-------------------------------------------------------------------------
//#
//#	File Version:	8		from: 24.11.2022
//#
//#	Bug Fix:
//#		-	switch the correct LEDs for approch signal
//#		-	correction of access to the sequences
//#
//#-------------------------------------------------------------------------
//#
//#	File Version:	7		from: 21.11.2022
//#
//#	Implementation:
//#		-	split into two sets of sequences
//#
//#-------------------------------------------------------------------------
//#
//#	File version:	6		from: 17.11.2022
//#
//#	Implementation:
//#		-	streamlining lncv storage
//#			changes in function 'Init()'
//#				now read the light intensity direct from LNCV
//#			changes in function 'SetSignalType()'
//#				now read signal type direct from LNCV
//#
//#-------------------------------------------------------------------------
//#
//#	File Version:	5		from: 12.10.2022
//#
//#	Implementation:
//#		-	add approach signal sequences
//#
//#-------------------------------------------------------------------------
//#
//#	File Version:	4		from: 11.10.2022
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
//#	Bug Fix:
//#		-	sequences so far are for A-Sig not for E-Sig
//#			so rename all tables accordingly
//#		-	only switch approach signal ON if it is OFF
//#			and vice versa
//#
//#-------------------------------------------------------------------------
//#
//#	File Version:	3		from: 23.09.2022
//#
//#	Implementation:
//#		-	add checks to avoid a crash by wrong commands or parameters
//#			changes in function HandleLed()
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

#include <Arduino.h>

#include "io_control.h"
#include "lncv_storage.h"
#include "macros.h"
#include "sequencer_tables.h"


//==========================================================================
//
//		D E F I N I T I O N S
//
//==========================================================================

#define SIGNAL_MAIN_LEDS			0x007E
#define SIGNAL_APPROACH_LEDS		0x1E00

#define MAIN_LEDS_IDX_START			IDX_SIGNAL_MAIN_BIT_0
#define MAIN_LEDS_IDX_COUNT			6
#define MAIN_LEDS_IDX_STOP			(MAIN_LEDS_IDX_START + MAIN_LEDS_IDX_COUNT)

#define APPROACH_LEDS_IDX_START		IDX_SIGNAL_APPROACH_BIT_0
#define APPROACH_LEDS_IDX_COUNT		4
#define APPROACH_LEDS_IDX_STOP		(APPROACH_LEDS_IDX_START + APPROACH_LEDS_IDX_COUNT)

#define	LEDS_PORTB					0x30
#define LEDS_PORTC					0x3F
#define LEDS_PORTD					0xFF

//----------------------------------------------------------------------
//	Track contact definitions
//
#define	TRACK_BIT					PB2
#define TRACK_PIN					PINB

#define TRACK_CONTACT_WAIT_TIME		100

#define READ_CONTACT				(0 == (TRACK_PIN & (1 << TRACK_BIT)))


//----------------------------------------------------------------------
//	State machine to switch approach signal on and off
//
#define STATE_APPROACH_OFF			1
#define	STATE_APPROACH_ON			2
#define STATE_SWITCH_OFF_LOWER		3
#define STATE_SWITCH_OFF_UPPER		4
#define STATE_SWITCH_ON_LOWER		5
#define STATE_SWITCH_ON_UPPER		6
#define STATE_SET_SIGNAL_ASPECT		7
#define STATE_SET_HP0				8

//----------------------------------------------------------------------
//	Cycle time in µs
//
#define PWM_CYCLE_TIME			20400
#define PWM_STEP_TIME			(PWM_CYCLE_TIME / 255)

#define FLASH_COUNTER_TIME		1000

//----------------------------------------------------------------------
//	Light intensity masks
//
#define MASK_INTENSITY_NIGHT		0x00FF
#define MASK_INTENSITY_DAY			0xFF00


typedef struct
{
	uint8_t	m_usBrightnessDay;
	uint8_t	m_usBrightnessNight;
	float	m_fFadeRateUp;
	float	m_fFadeRateDwon;
	float	m_fActual;
	float	m_fTarget;

} PWMChannel_t;


//==========================================================================
//
//		G L O B A L   V A R I A B L E S
//
//==========================================================================

IO_ControlClass	g_clControl	= IO_ControlClass();

//----------------------------------------------------------------------
//	LED assignment
//
//uint8_t	gusLeds[ SIGNAL_LEDS ] =
//				 { 0, 19, 18, 17, 16, 15, 14,  7,  6,  5,  4,  3,  2,  1 };
//			LED	   A   B   C   D   E   F   G   H   I   J   K   L   M   N
//	LED mask bit   0   1   2   3   4   5   6   7   8   9  10  11  12  13

//----------------------------------------------------------------------
//	this array contains the mapping	universal pin numbering to
//	pin of port
//
uint8_t	g_arPortPins[ TOTAL_LEDS ] =
{
	PD0, PC5, PC4, PC3, PC2, PC1, PC0, PD7, PD6, PD5, PD4, PD3, PD2, PD1, PB4, PB5
};

//----------------------------------------------------------------------
//	this array contains the mapping LED numbering to
//	address of port to set an output
//
volatile uint8_t * g_arPorts[ TOTAL_LEDS ] =
{
	&PORTD,
	&PORTC,
	&PORTC,
	&PORTC,
	&PORTC,
	&PORTC,
	&PORTC,
	&PORTD,
	&PORTD,
	&PORTD,
	&PORTD,
	&PORTD,
	&PORTD,
	&PORTD,
	&PORTB,
	&PORTB
};

//----------------------------------------------------------------------
//	this array contains the 'description' for each LED
//
PWMChannel_t	g_arChannel[ SIGNAL_LEDS ];

//----------------------------------------------------------------------
//	index array to get the right LED number
//
uint8_t		g_arLedIndex[] =
	{
		LED_MAIN_ZUSATZANZEIGER,
		LED_ERSATZSIGNAL,
		LED_APPROACH_KENNLICHT,
		LED_APPROACH_ZUSATZANZEIGER,
		LED_ERSATZSIGNAL_FLASH
	};

//----------------------------------------------------------------------
//	timer for one second
//
uint32_t	g_ulTimerTick;


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
//	here for all Ports the relevant I/O pins will be configured.
//	Not used I/O's will be set to Input with PullUp.
//	all connected devices (like LED's) will be set to 'off'.
//
IO_ControlClass::IO_ControlClass()
{
	DDRB	= LEDS_PORTB;
	DDRC	= LEDS_PORTC;
	DDRD	= LEDS_PORTD;

	PORTB	= ~(LEDS_PORTB);
	PORTC	= ~(LEDS_PORTC);
	PORTD	= ~(LEDS_PORTD);
}


//******************************************************************
//	Init
//------------------------------------------------------------------
//
void IO_ControlClass::Init( void )
{
	uint16_t	uiHelper;
	bool		bIsASig;

	m_pusStepMain					= NULL;
	m_pusStepApproach				= NULL;
	m_ulLastCycleStartTime			= 0L;
	m_ulCycleTime					= 0L;
	m_ulCycleStepTime				= 0L;
	m_ulContactTime					= 0L;
	m_ulPauseTimer					= 0L;
	m_ulErsatzSignalFlashTimer		= 0L;
	m_uiErsatzSignalOffTimer		= 0;
	m_uiLedsOn						= 0x0000;
	m_uiFadeActive					= 0x0000;
	m_uiFadeTimeUp					= 0;
	m_uiFadeTimeDown				= 0;
	m_usSignalAspectMain			= SIGNAL_ASPECT_HP0;
	m_usSignalAspectApproach		= SIGNAL_ASPECT_HP0;
	m_usExtraSignals				= 0x00;
	m_usState						= STATE_APPROACH_OFF;
	m_usOldState					= m_usState;
	m_bNightLight					= false;
	m_bApproachDark					= false;
	m_bErsatzSignalFlash			= false;
	m_bApproachSignalPresent		= false;
	m_bTransitionActiveMain			= false;
	m_bTransitionActiveApproach		= false;
	m_bStateMachineActive			= false;
	m_bWaitForFadeFinished			= false;

	m_bContactOccupied				= false;

	m_uiDarkTime				= g_clLncvStorage.ReadLNCV( LNCV_ADR_DARK_TIME );
	m_uiFlashTime				= g_clLncvStorage.ReadLNCV( LNCV_ADR_FLASH_TIME );
	m_uiErsatzSignalFlashTime	= g_clLncvStorage.ReadLNCV( LNCV_ADR_ERSATZ_SIGNAL_FLASH_TIME );

	bIsASig = SetSignalType();

	for( uint8_t idx = 0 ; SIGNAL_LEDS > idx ; idx++ )
	{
		uiHelper = g_clLncvStorage.ReadLNCV( LNCV_ADR_LIGHT_INTENSITY_LED_A + idx );

		g_arChannel[ idx ].m_usBrightnessDay	= (uint8_t)((uiHelper & MASK_INTENSITY_DAY) >> 8);
		g_arChannel[ idx ].m_usBrightnessNight	= (uint8_t)( uiHelper & MASK_INTENSITY_NIGHT);
//		g_arChannel[ idx ].m_usBrightnessNight	= (uint8_t)(uiHelper);
//		uiHelper >>= 8;
//		g_arChannel[ idx ].m_usBrightnessDay	= (uint8_t)(uiHelper);
		g_arChannel[ idx ].m_fTarget = 0.0;
		g_arChannel[ idx ].m_fActual = 0.0;
	}

	CalculateFadeRates();

	if( 0 != g_clLncvStorage.ReadLNCV( LNCV_ADR_LIGHT_J ) )
	{
		m_bApproachSignalPresent = true;
	}

	//----------------------------------------------------------
	//	hello flash
	//
	SwitchLed( LED_GREEN, true  );
	delay( 300 );
	SwitchLed( LED_GREEN, false );
	SwitchLed( LED_BLUE,  true  );
	delay( 300 );
	SwitchLed( LED_BLUE,  false );
	SwitchLed( LED_GREEN, true  );
	delay( 300 );
	SwitchLed( LED_GREEN, false );
	SwitchLed( LED_BLUE,  true  );
	delay( 300 );
	SwitchLed( LED_BLUE,  false );

	delay( 100 );

	//----------------------------------------------------------
	//	read contact
	//
	m_bContactOccupied = READ_CONTACT;

	//----------------------------------------------------------
	//	set signal to show Hp0
	//
	SwitchLed( LED_C, true );

	if( bIsASig )
	{
		SwitchLed( LED_D, true );
	}
}


//******************************************************************
//	Loop
//------------------------------------------------------------------
//
void IO_ControlClass::Loop( void )
{
	ProcessBrightnessAndFade();

	//----------------------------------------------------------
	//	check track contact
	//
	if( m_ulContactTime )
	{
		//-------------------------------------------------
		//	the debounce timer is active,
		//	so check if it is elapsed...
		//
		if( millis() > m_ulContactTime )
		{
			//---------------------------------------------
			//	the timer is elapsed
			//	so check if the track contact state is
			//	still different and
			//	if so save the actual state
			//
			m_ulContactTime = 0L;
			
			if( READ_CONTACT != m_bContactOccupied )
			{
				m_bContactOccupied = READ_CONTACT;
			}
		}
	}
	else if( READ_CONTACT != m_bContactOccupied )
	{
		//------------------------------------------------------
		//	track contact state is different than before
		//	so start debounce timer
		//
		m_ulContactTime = millis() + TRACK_CONTACT_WAIT_TIME;
	}

	//----------------------------------------------------------
	//	handle Ersatzsignal flashing
	//
	if( m_bErsatzSignalFlash )
	{
		if( millis() > m_ulErsatzSignalFlashTimer )
		{
			if( IsFadeFinished( LED_ERSATZSIGNAL ) )
			{
				m_ulErsatzSignalFlashTimer = millis() + m_uiErsatzSignalFlashTime;
				
				if( m_uiLedsOn & (1 << LED_ERSATZSIGNAL) )
				{
					HandleLed( LED_ERSATZSIGNAL, false );
				}
				else
				{
					HandleLed( LED_ERSATZSIGNAL, true );
				}
			}
		}
	}

	//----------------------------------------------------------
	//	handle Ersatzsignal OFF Timer
	//
	if( (0 < g_ulTimerTick) && (millis() > g_ulTimerTick) )
	{
		g_ulTimerTick += FLASH_COUNTER_TIME;
		m_uiErsatzSignalOffTimer--;

		if( 0 == m_uiErsatzSignalOffTimer )
		{
			SetErsatzSignalFlash( false );
		}
	}

	//----------------------------------------------------------
	//	handle state machine and transitions
	//

	//----------------------------------------------------------
	//	check if we should pause the handling
	//
	if( m_ulPauseTimer > 0 )
	{
		//--------------------------------------------------
		//	if the pause time has elapsed
		//	then switch off the timer
		//
		if( millis() > m_ulPauseTimer )
		{
			m_ulPauseTimer = 0L;
		}
	}
	else if( m_bWaitForFadeFinished )
	{
		//--------------------------------------------------
		//	wait until all fade actions are finished
		//
		if( IsFadeFinished() )
		{
			m_bWaitForFadeFinished = false;
		}
	}
	else
	{
		//--------------------------------------------------
		//	now process state machine and transitions
		//
		if( m_bStateMachineActive )
		{
			ProcessStateMachine();
		}
		else if( m_bTransitionActiveApproach )
		{
			ProcessTransitionApproach();
		}
		else if( m_bTransitionActiveMain )
		{
			ProcessTransitionMain();
		}
	}
}


//******************************************************************
//	SetNightLight
//------------------------------------------------------------------
//
void IO_ControlClass::SetNightLight( bool bOn )
{
	if( m_bNightLight != bOn )
	{
		m_bNightLight = bOn;

		CalculateFadeRates();
		
		//------------------------------------------------------
		//	set all LEDs that are on to the new light intensity
		//
		for( uint8_t idx = 0 ; SIGNAL_LEDS > idx ; idx++ )
		{
			if( IsWordBitSet( m_uiLedsOn, idx ) )
			{
				SwitchLed( idx, true );
			}
		}

/*
		if( bOn )
		{
			//--------------------------------------------------
			//	switch from day to night
			//
			for( uint8_t idx = 0 ; SIGNAL_LEDS > idx ; idx++ )
			{
				if( IsWordBitSet( m_uiLedsOn, idx ) )
				{
					pChannel->m_fTarget = (float)PWM_STEP_TIME * pChannel->m_usBrightnessNight;
				}
			}

			CalculateFadeRates();
		}
		else
		{
			//--------------------------------------------------
			//	switch from night to day
			//
			CalculateFadeRates();
			
			for( uint8_t idx = 0 ; SIGNAL_LEDS > idx ; idx++ )
			{
				if( IsWordBitSet( m_uiLedsOn, idx ) )
				{
					pChannel->m_fTarget = (float)PWM_STEP_TIME * pChannel->m_usBrightnessDay;
				}
			}
		}
*/
	}
}


//******************************************************************
//	SetErsatzSignalFlash
//------------------------------------------------------------------
//
void IO_ControlClass::SetErsatzSignalFlash( bool bOn )
{
	if( bOn )
	{
		m_ulErsatzSignalFlashTimer	= millis() + m_uiErsatzSignalFlashTime;
	}
	else
	{
		m_ulErsatzSignalFlashTimer	= 0L;
		m_uiErsatzSignalOffTimer	= 0;
	}

	HandleLed( LED_ERSATZSIGNAL, bOn );

	m_bErsatzSignalFlash = bOn;
}


//******************************************************************
//	HandleLed
//------------------------------------------------------------------
//
void IO_ControlClass::HandleLed( uint8_t usLedNo, bool bOn )
{
	PWMChannel_t*	pChannel		= &(g_arChannel[ usLedNo ]);
	uint8_t			usBrightness;

	if( SIGNAL_LEDS > usLedNo )
	{
		if( m_bNightLight )
		{
			usBrightness = pChannel->m_usBrightnessNight;
		}
		else
		{
			usBrightness = pChannel->m_usBrightnessDay;
		}

		if( bOn )
		{
			if( IsWordBitClear( m_uiLedsOn, usLedNo ) )
			{
				SetWordBit( m_uiLedsOn, usLedNo );

				if( (LED_ERSATZSIGNAL == usLedNo) && !m_bErsatzSignalFlash )
				{
					m_uiErsatzSignalOffTimer	= g_clLncvStorage.ReadLNCV( LNCV_ADR_ERSATZ_SIGNAL_OFF_TIME );
					g_ulTimerTick				= millis() + FLASH_COUNTER_TIME;
				}

				pChannel->m_fTarget = (float)PWM_STEP_TIME * usBrightness;

				if( 0 == m_uiFadeTimeUp )
				{
					pChannel->m_fActual = pChannel->m_fTarget;
				}
				else
				{
					SetWordBit( m_uiFadeActive, usLedNo );

					SwitchLed( LED_BLUE, true );
				}
			}
		}
		else
		{
			if( IsWordBitSet( m_uiLedsOn, usLedNo ) )
			{
				ClearWordBit( m_uiLedsOn, usLedNo );

				pChannel->m_fTarget = 0.0;

				if( 0 == m_uiFadeTimeDown )
				{
					pChannel->m_fActual = 0.0;
				}
				else
				{
					SetWordBit( m_uiFadeActive, usLedNo );

					SwitchLed( LED_BLUE, true );
				}
			}
		}
	}
}


//******************************************************************
//	HandleSignalAspectMain
//------------------------------------------------------------------
//
void IO_ControlClass::HandleSignalAspectMain( uint8_t usSignalAspect, uint8_t usExtraSignals )
{
	if( !m_bTransitionActiveMain && (SIGNAL_ASPECTS > usSignalAspect) )
	{
		//------------------------------------------------------
		//	first check if the new transition is allowed
		//		allowed transitions are:
		//			from Hp0 to xxx
		//			from xxx to Hp0
		//
		if(		((SIGNAL_ASPECT_HP0 == m_usSignalAspectMain) && (SIGNAL_ASPECT_HP0 != usSignalAspect))
			||	((SIGNAL_ASPECT_HP0 != m_usSignalAspectMain) && (SIGNAL_ASPECT_HP0 == usSignalAspect)) )
		{
			//--------------------------------------------------
			//	if we get a new signal aspect then
			//		start the transition
			//		store new signal aspect
			//
			if( m_usSignalAspectMain != usSignalAspect )
			{
				m_pusStepMain			= GetFirstStepMain( m_usSignalAspectMain, usSignalAspect );
				m_usSignalAspectMain	= usSignalAspect;
				m_usExtraSignals		= usExtraSignals;
				m_bTransitionActiveMain	= true;

				SwitchLed( LED_GREEN, true );
			}
		}
	}
}


//******************************************************************
//	HandleSignalAspectApproach
//------------------------------------------------------------------
//
void IO_ControlClass::HandleSignalAspectApproach( uint8_t usSignalAspect, uint8_t usExtraSignals )
{
	if(		m_bApproachSignalPresent
		&& !m_bTransitionActiveApproach
		&& (SIGNAL_ASPECTS > usSignalAspect) )
	{
		//------------------------------------------------------
		//	first check if the new transition is allowed
		//		allowed transitions are:
		//			from Hp0 to xxx
		//			from xxx to Hp0
		//
		if(		((SIGNAL_ASPECT_HP0 == m_usSignalAspectApproach) && (SIGNAL_ASPECT_HP0 != usSignalAspect))
			||	((SIGNAL_ASPECT_HP0 != m_usSignalAspectApproach) && (SIGNAL_ASPECT_HP0 == usSignalAspect)) )
		{
			if( STATE_APPROACH_OFF == m_usState )
			{
				//----------------------------------------------
				//	the approach signal is switched OFF
				//	so just set the new signal aspect
				//
				m_usSignalAspectApproach	= usSignalAspect;
				m_usExtraSignals			= usExtraSignals;
			}
			else
			{
				//--------------------------------------------------
				//	if we got a new signal aspect then
				//		start the transition
				//		store new signal aspect
				//
				if( m_usSignalAspectApproach != usSignalAspect )
				{
					StartAspectTransitionApproach( m_usSignalAspectApproach, usSignalAspect );

					m_usSignalAspectApproach	= usSignalAspect;
					m_usExtraSignals			= usExtraSignals;
				}
			}
		}
	}
}


//******************************************************************
//	SwitchLed
//------------------------------------------------------------------
//
void IO_ControlClass::SwitchLed( uint8_t usLedNo, bool bOn )
{
	PWMChannel_t*	pChannel		= &(g_arChannel[ usLedNo ]);
	uint8_t			usBrightness;


	if( TOTAL_LEDS > usLedNo )
	{
		if( SIGNAL_LEDS > usLedNo )
		{
			if( m_bNightLight )
			{
				usBrightness = pChannel->m_usBrightnessNight;
			}
			else
			{
				usBrightness = pChannel->m_usBrightnessDay;
			}

			if( bOn )
			{
				pChannel->m_fTarget = (float)PWM_STEP_TIME * usBrightness;
				pChannel->m_fActual = pChannel->m_fTarget;

				SetWordBit( m_uiLedsOn, usLedNo );
			}
			else
			{
				pChannel->m_fTarget = 0.0;
				pChannel->m_fActual = 0.0;

				ClearWordBit( m_uiLedsOn, usLedNo );
			}
		}
		else
		{
			if( bOn )
			{
				sbi( *g_arPorts[ usLedNo ], g_arPortPins[ usLedNo ] );
			}
			else
			{
				cbi( *g_arPorts[ usLedNo ], g_arPortPins[ usLedNo ] );
			}
		}
	}
}


//******************************************************************
//	IsFadeFinished
//------------------------------------------------------------------
//
bool IO_ControlClass::IsFadeFinished( void )
{
	if( 0 == m_uiFadeActive )
	{
		return( true );
	}
	
	return( false );
}


//******************************************************************
//	IsFadeFinished
//------------------------------------------------------------------
//
bool IO_ControlClass::IsFadeFinished( uint8_t usLedNo )
{
	if( SIGNAL_LEDS > usLedNo )
	{
		if( 0 == (m_uiFadeActive & (1 << usLedNo)) )
		{
			return( true );
		}
	}

	return( false );
}


//******************************************************************
//	StartAspectTransitionApproach
//------------------------------------------------------------------
//
void IO_ControlClass::StartAspectTransitionApproach( uint8_t usAspectFrom, uint8_t usAspectTo )
{
	m_pusStepApproach = sNOP;

	if( SIGNAL_ASPECT_HP0 == usAspectFrom )
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
				m_pusStepApproach = pgm_read_word( &Approach_From_Hp0[ usAspectTo ] );
				break;

			default:
				break;
		}
	}
	else if( SIGNAL_ASPECT_HP0 == usAspectTo )
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
				m_pusStepApproach = pgm_read_word( &Approach_To_Hp0[ usAspectFrom ] );
				break;

			default:
				break;
		}
	}

	m_bTransitionActiveApproach	= true;

	SwitchLed( LED_GREEN, true );
}


//******************************************************************
//	SetSignalType
//------------------------------------------------------------------
//
bool IO_ControlClass::SetSignalType( void )
{
	bool	bIsASig	= false;


	m_usSignalType	= (uint8_t)g_clLncvStorage.ReadLNCV( LNCV_ADR_SIGNAL_TYPE );

	switch( m_usSignalType )
	{
		case SIGNAL_TYPE_59_A_SIG:
			bIsASig = true;

		case SIGNAL_TYPE_59_E_SIG:
			m_uiFadeTimeUp		= g_clLncvStorage.GetFadeTimeUp(   IDX_FADE_TIME_SLOW );
			m_uiFadeTimeDown	= g_clLncvStorage.GetFadeTimeDown( IDX_FADE_TIME_SLOW );
			break;
			
		case SIGNAL_TYPE_DIRECT_CONTROL_FAST:
			m_uiFadeTimeUp		= g_clLncvStorage.GetFadeTimeUp(   IDX_FADE_TIME_FAST );
			m_uiFadeTimeDown	= g_clLncvStorage.GetFadeTimeDown( IDX_FADE_TIME_FAST );
			break;
			
		case SIGNAL_TYPE_DIRECT_CONTROL_MIDDLE:
			m_uiFadeTimeUp		= g_clLncvStorage.GetFadeTimeUp(   IDX_FADE_TIME_MIDDLE );
			m_uiFadeTimeDown	= g_clLncvStorage.GetFadeTimeDown( IDX_FADE_TIME_MIDDLE );
			break;
			
		case SIGNAL_TYPE_DIRECT_CONTROL_SLOW:
			m_uiFadeTimeUp		= g_clLncvStorage.GetFadeTimeUp(   IDX_FADE_TIME_SLOW );
			m_uiFadeTimeDown	= g_clLncvStorage.GetFadeTimeDown( IDX_FADE_TIME_SLOW );
			break;

		case SIGNAL_TYPE_60_A_SIG:
		case SIGNAL_TYPE_COMPACT_A_SIG:
		case SIGNAL_TYPE_KS_A_SIG:
			bIsASig = true;

		case SIGNAL_TYPE_DIRECT_CONTROL_HARD:
		case SIGNAL_TYPE_60_E_SIG:
		case SIGNAL_TYPE_COMPACT_E_SIG:
		case SIGNAL_TYPE_KS_E_SIG:
		default:
			break;
	}

	return( bIsASig );
}


//******************************************************************
//	CalculateFadeRates
//------------------------------------------------------------------
//	This function will calulate the delta values for the fade.
//
void IO_ControlClass::CalculateFadeRates( void )
{
	PWMChannel_t*	pChannel;
	float			fHelper;
	uint8_t			usBrightness;

	for( uint8_t idx = 0 ; SIGNAL_LEDS > idx ; idx++ )
	{
		pChannel = &(g_arChannel[ idx ]);

		pChannel->m_fFadeRateUp		= 0.0;
		pChannel->m_fFadeRateDwon	= 0.0;

		if( m_bNightLight )
		{
			usBrightness = pChannel->m_usBrightnessNight;
		}
		else
		{
			usBrightness = pChannel->m_usBrightnessDay;
		}

		fHelper = usBrightness * PWM_STEP_TIME;

		if( 0 < m_uiFadeTimeUp )
		{
			pChannel->m_fFadeRateUp = fHelper / m_uiFadeTimeUp / 1000 * PWM_STEP_TIME;
		}
		
		if( 0 < m_uiFadeTimeDown )
		{
			pChannel->m_fFadeRateDwon = fHelper / m_uiFadeTimeDown / 1000 * PWM_STEP_TIME;
		}
	}
}


//******************************************************************
//	GetFirstStepMain
//------------------------------------------------------------------
//	This function will return a pointer to the first step of
//	a signal aspect sequence for the main signal
//
uint8_t * IO_ControlClass::GetFirstStepMain( uint8_t usFromAspect, uint8_t usToAspect )
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
				pFirstStep = pgm_read_word( &A_Sig_Main_From_Hp0[ usToAspect ] );
				break;

			case SIGNAL_TYPE_59_E_SIG:
			case SIGNAL_TYPE_60_E_SIG:
			case SIGNAL_TYPE_COMPACT_E_SIG:
			case SIGNAL_TYPE_KS_E_SIG:
				pFirstStep = pgm_read_word( &E_Sig_Main_From_Hp0[ usToAspect ] );
				break;

			case SIGNAL_TYPE_DIRECT_CONTROL_HARD:
			case SIGNAL_TYPE_DIRECT_CONTROL_FAST:
			case SIGNAL_TYPE_DIRECT_CONTROL_MIDDLE:
			case SIGNAL_TYPE_DIRECT_CONTROL_SLOW:
			default:
				break;
		}
	}
	else if( SIGNAL_ASPECT_HP0 == usToAspect )
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
//	SwitchExtraSignalsOn
//------------------------------------------------------------------
//	This function will switch on the selected Extra signals
//
void IO_ControlClass::SwitchExtraSignalsOn( void )
{
	uint8_t		usMask	= 0x01;
	uint8_t		usLedNo;

	for( uint8_t idx = 0 ; EXTRA_SIGNAL_COUNT > idx ; idx++ )
	{
		if( m_usExtraSignals & usMask )
		{
			usLedNo = g_arLedIndex[ idx ];

			if( LED_ERSATZSIGNAL_FLASH == usLedNo )
			{
				SetErsatzSignalFlash( true );
			}
			else
			{
				HandleLed( usLedNo, true );
			}

			m_bWaitForFadeFinished = true;
		}

		usMask <<= 1;
	}
}


//******************************************************************
//	SwitchExtraSignalsOff
//------------------------------------------------------------------
//	This function will switch off all Extra signals
//
void IO_ControlClass::SwitchExtraSignalsOff( void )
{
	SetErsatzSignalFlash( false );

	for( uint8_t idx = 0 ; EXTRA_SIGNAL_ERSATZSIGNAL_FLASH > idx ; idx++ )
	{
		HandleLed( g_arLedIndex[ idx ], false );
	}

	m_bWaitForFadeFinished = true;
}


//******************************************************************
//	ProcessTransitionMain
//------------------------------------------------------------------
//	This function will process the steps of
//	a signal aspect transition for the main signal
//
void IO_ControlClass::ProcessTransitionMain( void )
{
	uint16_t	uiPauseTime;
	uint8_t		usAspect;
	uint8_t		usControlGroup;
	uint8_t		usDiff;
	uint8_t		usMask		= 0x01;

	usAspect = pgm_read_byte( m_pusStepMain );

	m_pusStepMain++;

	usControlGroup = usAspect & SEQUENCE_CONTROL_GROUPS;

	if( SG_CONTROL_WORDS == usControlGroup )
	{
		switch( usAspect )
		{
			case SEQUENCE_END:
				//--------------------------------------------------
				//	the end of the sequence is reached
				//	so stop the transition
				//
				m_bTransitionActiveMain = false;

				SwitchLed( LED_GREEN, false );
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
				uiPauseTime		= pgm_read_byte( m_pusStepMain ) * 10;
				m_ulPauseTimer	= millis() + uiPauseTime;
				m_pusStepMain++;
				break;

			case SEQUENCE_PAUSE_DARK:
				//--------------------------------------------------
				//	make a pause in the sequence with the length
				//	of 'DarkTime'
				//
				m_ulPauseTimer	= millis() + m_uiDarkTime;
				break;

			case SEQUENCE_PAUSE_FLASH:
				//--------------------------------------------------
				//	make a pause in the sequence with the length
				//	of 'FlashTime'
				//
				m_ulPauseTimer	= millis() + m_uiFlashTime;
				break;

			case SEQUENCE_APPROACH_ON:
				//--------------------------------------------------
				//	the approach signal should be switched ON
				//
				if(		m_bApproachSignalPresent
					&& !m_bApproachDark
					&& (STATE_APPROACH_OFF == m_usState) )
				{
					m_usState				= STATE_SWITCH_ON_LOWER;
					m_bStateMachineActive	= true;
				}
				break;

			case SEQUENCE_SET_SIGNAL_ASPECT:
				//--------------------------------------------------
				//	check if the approach signal should show
				//	a signal aspect
				//
				if(		m_bApproachSignalPresent
					&& !m_bApproachDark
					&& (STATE_APPROACH_ON == m_usState) )
				{
					if( SIGNAL_ASPECT_HP0 != m_usSignalAspectApproach )
					{
						StartAspectTransitionApproach( SIGNAL_ASPECT_HP0, m_usSignalAspectApproach );
					}
				}
				break;

			case SEQUENCE_SET_HP0:
				//--------------------------------------------------
				//	check if the approach signal should show Hp0
				//
				if(		m_bApproachSignalPresent
					&& !m_bApproachDark
					&& (STATE_APPROACH_ON == m_usState) )
				{
					if( SIGNAL_ASPECT_HP0 != m_usSignalAspectApproach )
					{
						StartAspectTransitionApproach( m_usSignalAspectApproach, SIGNAL_ASPECT_HP0 );
					}
				}
				break;

			case SEQUENCE_APPROACH_OFF:
				//--------------------------------------------------
				//	the approach signal should be switched OFF
				//
				if(		m_bApproachSignalPresent
					&& !m_bApproachDark
					&& (STATE_APPROACH_ON == m_usState) )
				{
					m_usState				= STATE_SWITCH_OFF_UPPER;
					m_bStateMachineActive	= true;
				}

				m_bApproachDark = false;
				break;

			case SEQUENCE_ADDITION_SIGNAL_ON:
				SwitchExtraSignalsOn();
				break;

			case SEQUENCE_ADDITION_SIGNAL_OFF:
				SwitchExtraSignalsOff();
				break;
		}
	}
	else
	{
		//--------------------------------------------------
		//	check wich LEDs of the main signal
		//	should go OFF and wich should go ON
		//
		usDiff  = (uint8_t)((m_uiLedsOn & SIGNAL_MAIN_LEDS) >> MAIN_LEDS_IDX_START);
		usDiff ^= (usAspect & SEQUENCE_LEDS);

		for( uint8_t idx = MAIN_LEDS_IDX_START ; (MAIN_LEDS_IDX_STOP > idx) && (usDiff != 0) ; idx++ )
		{
			if( usDiff & usMask )
			{
				if( usAspect & usMask )
				{
					if( SG_FLASHING_LEDS == usControlGroup )
					{
						SwitchLed( idx, true );
						m_ulPauseTimer = millis() + m_uiFlashTime;
					}
					else
					{
						HandleLed( idx, true );
					}
				}
				else
				{
					if( SG_FLASHING_LEDS == usControlGroup )
					{
						SwitchLed( idx, false );
						m_ulPauseTimer = millis() + m_uiFlashTime;
					}
					else
					{
						HandleLed( idx, false );
					}
				}

				usDiff &= ~usMask;
			}

			usMask <<= 1;
		}
	}
}


//******************************************************************
//	ProcessTransitionApproach
//------------------------------------------------------------------
//	This function will process the steps of
//	a signal aspect transition for the approach signal
//
void IO_ControlClass::ProcessTransitionApproach( void )
{
	uint16_t	uiPauseTime;
	uint8_t		usAspect;
	uint8_t		usControlGroup;
	uint8_t		usDiff;
	uint8_t		usMask		= 0x01;

	usAspect = pgm_read_byte( m_pusStepApproach );

	m_pusStepApproach++;

	usControlGroup = usAspect & SEQUENCE_CONTROL_GROUPS;

	if( SG_CONTROL_WORDS == usControlGroup )
	{
		switch( usAspect )
		{
			case SEQUENCE_END:
				//----------------------------------------------
				//	the end of the sequence is reached
				//	so stop the transition
				//
				m_bTransitionActiveApproach = false;

				if( !m_bTransitionActiveMain )
				{
					SwitchLed( LED_GREEN, false );
				}
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
				uiPauseTime		= pgm_read_byte( m_pusStepApproach ) * 10;
				m_ulPauseTimer	= millis() + uiPauseTime;
				m_pusStepApproach++;
				break;

			case SEQUENCE_PAUSE_DARK:
				//--------------------------------------------------
				//	make a pause in the sequence with the length
				//	of 'DarkTime'
				//
				m_ulPauseTimer	= millis() + m_uiDarkTime;
				break;

			case SEQUENCE_PAUSE_FLASH:
				//--------------------------------------------------
				//	make a pause in the sequence with the length
				//	of 'FlashTime'
				//
				m_ulPauseTimer	= millis() + m_uiFlashTime;
				break;

			case SEQUENCE_ADDITION_SIGNAL_ON:
				SwitchExtraSignalsOn();
				break;

			case SEQUENCE_ADDITION_SIGNAL_OFF:
				SwitchExtraSignalsOff();
				break;

			default:
				//	do nothing
				break;
		}
	}
	else
	{
		//--------------------------------------------------
		//	check wich LEDs of the approach signal
		//	should go OFF and wich should go ON
		//
		usDiff  = (uint8_t)((m_uiLedsOn & SIGNAL_APPROACH_LEDS) >> APPROACH_LEDS_IDX_START);
		usDiff ^= usAspect;

		for( uint8_t idx = APPROACH_LEDS_IDX_START ; (APPROACH_LEDS_IDX_STOP > idx) && (usDiff != 0) ; idx++ )
		{
			if( usDiff & usMask )
			{
				if( usAspect & usMask )
				{
					if( SG_FLASHING_LEDS == usControlGroup )
					{
						SwitchLed( idx, true );
						m_ulPauseTimer = millis() + m_uiFlashTime;
					}
					else
					{
						HandleLed( idx, true );
					}
				}
				else
				{
					if( SG_FLASHING_LEDS == usControlGroup )
					{
						SwitchLed( idx, false );
						m_ulPauseTimer = millis() + m_uiFlashTime;
					}
					else
					{
						HandleLed( idx, false );
					}
				}

				usDiff &= ~usMask;
			}

			usMask <<= 1;
		}
	}
}


//******************************************************************
//	ProcessStateMachine
//------------------------------------------------------------------
//	This function will process the state machine to switch the
//	approach signal ON or OFF
//
void IO_ControlClass::ProcessStateMachine( void )
{
	switch( m_usState )
	{
		//------------------------------------------------------
		//
		case STATE_APPROACH_ON:
		case STATE_APPROACH_OFF:
			if( m_usState != m_usOldState )
			{
				m_usOldState			= m_usState;
				m_bStateMachineActive	= false;
			}
			break;

		//------------------------------------------------------
		//
		case STATE_SWITCH_ON_LOWER:
			if( m_usState != m_usOldState )
			{
				m_usOldState = m_usState;

				//------------------------------------------
				//	set signal aspect Hp0 lower light
				//
				HandleLed( LED_M, true );

				m_ulPauseTimer = millis() + 100;

				m_usState = STATE_SWITCH_ON_UPPER;
			}
			break;

		//------------------------------------------------------
		//
		case STATE_SWITCH_ON_UPPER:
			if( m_usState != m_usOldState )
			{
				m_usOldState = m_usState;

				//------------------------------------------
				//	set signal aspect Hp0 upper light
				//
				HandleLed( LED_K, true );

				m_bWaitForFadeFinished	= true;
				m_usState				= STATE_APPROACH_ON;
			}
			break;

		//------------------------------------------------------
		//
		case STATE_SWITCH_OFF_UPPER:
			if( m_usState != m_usOldState )
			{
				m_usOldState = m_usState;

				HandleLed( LED_K, false );

				m_bWaitForFadeFinished	= true;
				m_usState				= STATE_SWITCH_OFF_LOWER;
			}
			break;

		//------------------------------------------------------
		//
		case STATE_SWITCH_OFF_LOWER:
			if( m_usState != m_usOldState )
			{
				m_usOldState = m_usState;

				HandleLed( LED_M, false );

				m_bWaitForFadeFinished	= true;
				m_usState				= STATE_APPROACH_OFF;
			}
			break;

		//------------------------------------------------------
		//
		default:
			break;
	}
}


//******************************************************************
//	ProcessBrightnessAndFade
//------------------------------------------------------------------
//	This function will do the main work if a LED should be fade up
//	or down.
//
void IO_ControlClass::ProcessBrightnessAndFade( void )
{
	PWMChannel_t*	pChannel;
	uint8_t			idx;
	uint32_t		ulNewValue;
	uint16_t		uiMask			= 0x0001;
	uint32_t		ulActualTime	= micros();

	
	//----------------------------------------------------------
	//	first check if a new cycle should start
	//
	if( ulActualTime > m_ulCycleTime )
	{
		//------------------------------------------------------
		//	yes, we should start a new cycle
		//	so store the cycle start time,
		//	set the time when the next cycle should start
		//	and the time when the next cycle step should start
		//
		m_ulLastCycleStartTime	= ulActualTime;
		m_ulCycleTime			= ulActualTime + PWM_CYCLE_TIME;
		m_ulCycleStepTime		= ulActualTime + PWM_STEP_TIME;

		//------------------------------------------------------
		//	then switch on each LED where a fade is active
		//	or the target brightness is not '0'
		//	and
		//	for each active fade process the fade
		//
		for( idx = 0 ; idx < SIGNAL_LEDS ; idx++ )
		{
			pChannel = &(g_arChannel[ idx ]);

			if(		(pChannel->m_fTarget > 0.0)
				||	(pChannel->m_fActual > 0.0) )
			{
				sbi( *g_arPorts[ idx ], g_arPortPins[ idx ] );
			}

			if( pChannel->m_fActual < pChannel->m_fTarget )
			{
				pChannel->m_fActual += pChannel->m_fFadeRateUp;
			}

			if( pChannel->m_fActual > pChannel->m_fTarget )
			{
				pChannel->m_fActual -= pChannel->m_fFadeRateDwon;
			}
		}
	}
	
	//----------------------------------------------------------
	//	next check if a cycle step should be handled
	//
	if( ulActualTime > m_ulCycleStepTime )
	{
		//------------------------------------------------------
		//	yes, we should handle a new cycle step
		//	so set the time when the next cycle step should start
		//
		m_ulCycleStepTime = ulActualTime + PWM_STEP_TIME;

		//------------------------------------------------------
		//	next, for each LED ...
		//
		for( idx = 0 ; idx < SIGNAL_LEDS ; idx++ )
		{
			pChannel = &(g_arChannel[ idx ]);

			//--------------------------------------------------
			//	... when we have reached the brightness level
			//		then switch off the LED
			//
			if(	ulActualTime > (pChannel->m_fActual + m_ulLastCycleStartTime) )
			{
				cbi( *g_arPorts[ idx ], g_arPortPins[ idx ] );
			}

			//--------------------------------------------------
			//	... process the fade ...
			//
			if( pChannel->m_fActual < pChannel->m_fTarget )
			{
				//------------------------------------------
				//	... to brighter light
				//
				ulNewValue = pChannel->m_fActual + pChannel->m_fFadeRateUp;
				
				if( ulNewValue >= pChannel->m_fTarget )
				{
					//----------------------------------
					//	the target is reached
					//	so stop fading
					//
					ulNewValue		= pChannel->m_fTarget;
					m_uiFadeActive &= ~uiMask;

					SwitchLed( LED_BLUE, false );
				}

				pChannel->m_fActual = ulNewValue;
			}

			if( pChannel->m_fActual > pChannel->m_fTarget )
			{
				//------------------------------------------
				//	... to darker light
				//
				ulNewValue = pChannel->m_fActual - pChannel->m_fFadeRateDwon;

				if( ulNewValue <= pChannel->m_fTarget )
				{
					//----------------------------------
					//	the target is reached
					//	so stop fading
					//
					ulNewValue		= pChannel->m_fTarget;
					m_uiFadeActive &= ~uiMask;

					SwitchLed( LED_BLUE, false );
				}

				pChannel->m_fActual = ulNewValue;
			}

			uiMask <<= 1;
		}
	}
}
