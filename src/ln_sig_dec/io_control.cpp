//##########################################################################
//#
//#		io_control.cpp
//#
//#	This class operates the ports of the Atmel chip.
//#	here only LEDs are controlled by the help of the PWM library
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

#define MAIN_LEDS_IDX_START			1
#define MAIN_LEDS_IDX_COUNT			6
#define MAIN_LEDS_IDX_STOP			(MAIN_LEDS_IDX_START + MAIN_LEDS_IDX_COUNT)

#define APPROACH_LEDS_IDX_START		9
#define APPROACH_LEDS_IDX_COUNT		4
#define APPROACH_LEDS_IDX_STOP		(APPROACH_LEDS_IDX_START + APPROACH_LEDS_IDX_COUNT)

#define	LEDS_PORTB					0x30
#define LEDS_PORTC					0x3F
#define LEDS_PORTD					0xFF

//----------------------------------------------------------------------
//	State machine to switch approach signal on and off
//
#define STATE_APPROACH_OFF			1
#define	STATE_APPROACH_ON			2
#define STATE_SWITCH_OFF			3
#define STATE_SWITCH_ON				4
#define STATE_SET_SIGNAL_ASPECT		5
#define STATE_SET_HP0				6

//----------------------------------------------------------------------
//	Cycle time in µs
//
#define PWM_CYCLE_TIME			20400
#define PWM_STEP_TIME			(PWM_CYCLE_TIME / 255)

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
//	all connected devices (like LED's) will set to 'off'.
//
IO_ControlClass::IO_ControlClass()
{
	DDRB	|= LEDS_PORTB;
	DDRC	|= LEDS_PORTC;
	DDRD	|= LEDS_PORTD;

	PORTB	&= ~(LEDS_PORTB);
	PORTC	&= ~(LEDS_PORTC);
	PORTD	&= ~(LEDS_PORTD);
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
	m_uiLedsOn						= 0x0000;
	m_uiFadeActive					= 0x0000;
	m_uiFadeTimeUp					= 0;
	m_uiFadeTimeDown				= 0;
	m_usSignalAspectMain			= SIGNAL_ASPECT_HP0;
	m_usSignalAspectMainBuffer		= SIGNAL_ASPECT_HP0;
	m_usSignalAspectApproach		= SIGNAL_ASPECT_HP0;
	m_usSignalAspectApproachBuffer	= SIGNAL_ASPECT_HP0;
	m_usState						= STATE_APPROACH_OFF;
	m_usOldState					= m_usState;
	m_bNightLight					= false;
	m_bApproachSignalPresent		= false;
	m_bTransitionActiveMain			= false;
	m_bTransitionActiveApproach		= false;
	m_bStateMachineActive			= false;

	bIsASig = SetSignalType();

	for( uint8_t idx = 0 ; SIGNAL_LEDS > idx ; idx++ )
	{
		uiHelper = g_clLncvStorage.ReadLNCV( LNCV_ADR_LIGHT_INTENSITY_LED_A + idx );

		g_arChannel[ idx ].m_usBrightnessDay	= (uint8_t)((uiHelper & MASK_INTENSITY_DAY) >> 8);
		g_arChannel[ idx ].m_usBrightnessNight	= (uint8_t)( uiHelper & MASK_INTENSITY_NIGHT);
		g_arChannel[ idx ].m_fTarget = 0.0;
		g_arChannel[ idx ].m_fActual = 0.0;
	}

	CalculateFadeRates();

	if( 0 != g_clLncvStorage.ReadLNCV( LNCV_ADR_LIGHT_J ) )
	{
		m_bApproachSignalPresent = true;
	}

	delay( 100 );

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
	if( m_bStateMachineActive )
	{
		ProcessStateMachine();
	}

	if( m_bTransitionActiveApproach )
	{
		ProcessTransitionApproach();
	}

	if( !m_bStateMachineActive && m_bTransitionActiveMain )
	{
		ProcessTransitionMain();
	}

	ProcessBrightnessAndFade();
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
		
		for( uint8_t idx = 0 ; SIGNAL_LEDS > idx ; idx++ )
		{
			if( 0.0 != g_arChannel[ idx ].m_fTarget )
			{
				HandleLed( idx, true );
			}
		}
	}
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
void IO_ControlClass::HandleSignalAspectMain( uint8_t usSignalAspect )
{
	if( SIGNAL_ASPECTS > usSignalAspect )
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
			//	store new signal aspect in a buffer just in
			//	case a signal aspect transition is active
			//
			if( m_usSignalAspectMainBuffer != usSignalAspect )
			{
				m_usSignalAspectMainBuffer = usSignalAspect;
			}
			
			//--------------------------------------------------
			//	if no signal aspect transition is active then
			//		start the transition
			//		store new signal aspect
			//
			if(		!m_bTransitionActiveMain
				&&	(m_usSignalAspectMain != usSignalAspect) )
			{
				m_pusStepMain			= GetFirstStepMain( m_usSignalAspectMain, usSignalAspect );
				m_usSignalAspectMain	= usSignalAspect;
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
void IO_ControlClass::HandleSignalAspectApproach( uint8_t usSignalAspect )
{
	if( m_bApproachSignalPresent && (SIGNAL_ASPECTS > usSignalAspect) )
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
				m_usSignalAspectApproach		= usSignalAspect;
				m_usSignalAspectApproachBuffer	= usSignalAspect;
			}
			else
			{
				//--------------------------------------------------
				//	store new signal aspect in a buffer just in
				//	case a signal aspect transition is active
				//
				if( m_usSignalAspectApproachBuffer != usSignalAspect )
				{
					m_usSignalAspectApproachBuffer = usSignalAspect;
				}
				
				//--------------------------------------------------
				//	if no signal aspect transition is active then
				//		start the transition
				//		store new signal aspect
				//
				if(		!m_bTransitionActiveApproach
					&&	(m_usSignalAspectApproach != usSignalAspect) )
				{
					StartAspectTransitionApproach( m_usSignalAspectApproach, usSignalAspect );

					m_usSignalAspectApproach = usSignalAspect;
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
//	ProcessTransitionMain
//------------------------------------------------------------------
//	This function will process the steps of
//	a signal aspect transition for the main signal
//
void IO_ControlClass::ProcessTransitionMain( void )
{
	uint8_t	usAspect;
	uint8_t	usDiff;
	uint8_t	usMask	= 0x01;

	if( IsFadeFinished() )
	{
		usAspect = pgm_read_byte( m_pusStepMain );

		m_pusStepMain++;

		if( SEQUENCE_END == usAspect )
		{
			//--------------------------------------------------
			//	the end of the sequence is reached
			//	so stop the transition
			//
			m_bTransitionActiveMain = false;

			SwitchLed( LED_GREEN, false );

			//--------------------------------------------------
			//	check if we got a new signal aspect in between
			//	if so, start a new transition
			//
			if( m_usSignalAspectMain != m_usSignalAspectMainBuffer )
			{
				HandleSignalAspectMain( m_usSignalAspectMainBuffer );
			}
		}
		else if( SEQUENCE_APPROACH_ON == usAspect )
		{
			//--------------------------------------------------
			//	the approach signal should be switched ON
			//
			if( m_bApproachSignalPresent && (STATE_APPROACH_OFF == m_usState) )
			{
				m_usState				= STATE_SWITCH_ON;
				m_bStateMachineActive	= true;
			}
		}
		else if( SEQUENCE_APPROACH_OFF == usAspect )
		{
			//--------------------------------------------------
			//	the approach signal should be switched OFF
			//
			if( m_bApproachSignalPresent && (STATE_APPROACH_ON == m_usState) )
			{
				m_usState				= STATE_SET_HP0;
				m_bStateMachineActive	= true;
			}
		}
		else
		{
			//--------------------------------------------------
			//	check wich LEDs of the main signal
			//	should go OFF and wich should go ON
			//
			usDiff  = (uint8_t)((m_uiLedsOn & SIGNAL_MAIN_LEDS) >> MAIN_LEDS_IDX_START);
			usDiff ^= usAspect;

			for( uint8_t idx = MAIN_LEDS_IDX_START ; (MAIN_LEDS_IDX_STOP > idx) && (usDiff != 0) ; idx++ )
			{
				if( usDiff & usMask )
				{
					if( usAspect & usMask )
					{
						HandleLed( idx, true );
					}
					else
					{
						HandleLed( idx, false );
					}

					usDiff &= ~usMask;
				}

				usMask <<= 1;
			}
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
	uint16_t	usAspect;
	uint16_t	usDiff;
	uint16_t	usMask	= 0x01;

	if( IsFadeFinished() )
	{
		usAspect = pgm_read_byte( m_pusStepApproach );

		m_pusStepApproach++;

		if( SEQUENCE_END == usAspect )
		{
			//--------------------------------------------------
			//	the end of the sequence is reached
			//	so stop the transition
			//
			m_bTransitionActiveApproach = false;

			if( !m_bTransitionActiveMain )
			{
				SwitchLed( LED_GREEN, false );
			}

			//--------------------------------------------------
			//	check if we got a new signal aspect in between
			//	if so, start a new transition
			//
			if( m_usSignalAspectApproach != m_usSignalAspectApproachBuffer )
			{
				StartAspectTransitionApproach( m_usSignalAspectApproach, m_usSignalAspectApproachBuffer );
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
						HandleLed( idx, true );
					}
					else
					{
						HandleLed( idx, false );
					}

					usDiff &= ~usMask;
				}

				usMask <<= 1;
			}
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
		case STATE_APPROACH_OFF:
			if( m_usState != m_usOldState )
			{
				m_usOldState			= m_usState;
				m_bStateMachineActive	= false;
			}
			break;

		//------------------------------------------------------
		//
		case STATE_SWITCH_ON:
			if( m_usState != m_usOldState )
			{
				m_usOldState = m_usState;

				//------------------------------------------
				//	set signal aspect Hp0
				//
				HandleLed( LED_K, true );
				HandleLed( LED_M, true );
			}
			else if( IsFadeFinished() )
			{
				if( SIGNAL_ASPECT_HP0 == m_usSignalAspectApproach )
				{
					m_usState = STATE_APPROACH_ON;
				}
				else
				{
					m_usState = STATE_SET_SIGNAL_ASPECT;
				}
			}
			break;

		//------------------------------------------------------
		//
		case STATE_SET_SIGNAL_ASPECT:
			if( m_usState != m_usOldState )
			{
				m_usOldState = m_usState;

				StartAspectTransitionApproach( SIGNAL_ASPECT_HP0, m_usSignalAspectApproach );
			}
			else if( !m_bTransitionActiveApproach )
			{
				m_usState = STATE_APPROACH_ON;
			}
			break;

		//------------------------------------------------------
		//
		case STATE_APPROACH_ON:
			if( m_usState != m_usOldState )
			{
				m_usOldState			= m_usState;
				m_bStateMachineActive	= false;
			}
			break;

		//------------------------------------------------------
		//
		case STATE_SET_HP0:
			if( m_usState != m_usOldState )
			{
				m_usOldState = m_usState;

				if( SIGNAL_ASPECT_HP0 != m_usSignalAspectApproach )
				{
					StartAspectTransitionApproach( m_usSignalAspectApproach, SIGNAL_ASPECT_HP0 );
				}
			}
			else if( !m_bTransitionActiveApproach )
			{
				m_usState = STATE_SWITCH_OFF;
			}
			break;

		//------------------------------------------------------
		//
		case STATE_SWITCH_OFF:
			if( m_usState != m_usOldState )
			{
				m_usOldState = m_usState;

				HandleLed( LED_K, false );
				HandleLed( LED_M, false );
			}
			else if( IsFadeFinished() )
			{
				m_usState = STATE_APPROACH_OFF;
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
