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
//#		 J			X1.4		D5		  5
//#		 K			X1.5		D4		  4
//#		 L			X1.6		D3		  3
//#		 M			X1.7		D2		  2
//#		 N			X1.8		D1		  1
//#
//#-------------------------------------------------------------------------
//#
//#	File Version:	4		from: 23.09.2022
//#
//#	Implementation:
//#		-	implement signal aspects for the approach signal
//#			rename variables
//#				from						to
//#				m_bTransitionActive			m_bTransitionActiveMain
//#			new variables
//#				m_bTransitionActiveApproach
//#				m_bStateMachineActive
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

#define SIGNAL_MAIN_LEDS		0x21FF


//----------------------------------------------------------------------
//	Cycle time in µs
//
#define PWM_CYCLE_TIME			20400
#define PWM_STEP_TIME			(PWM_CYCLE_TIME / 255)

#define	LEDS_PORTB				0x30
#define LEDS_PORTC				0x3F
#define LEDS_PORTD				0xFF

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
	m_puiStepMain					= 0x0000;
	m_puiStepApproach				= 0x0000;
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
	m_bNightLight					= false;
	m_bTransitionActive				= false;

	SetSignalType();

	for( uint8_t idx = 0 ; idx < SIGNAL_LEDS ; idx++ )
	{
		g_arChannel[ idx ].m_usBrightnessDay	= g_clLncvStorage.GetLightIntensityDay( idx );
		g_arChannel[ idx ].m_usBrightnessNight	= g_clLncvStorage.GetLightIntensityNight( idx );
		g_arChannel[ idx ].m_fTarget = 0.0;
		g_arChannel[ idx ].m_fActual = 0.0;
	}

	CalculateFadeRates();

	delay( 100 );

	SwitchLed( LED_C, true );
	SwitchLed( LED_D, true );
}


//******************************************************************
//	Loop
//------------------------------------------------------------------
//
void IO_ControlClass::Loop( void )
{
	//----------------------------------------------------------
	//	process signal aspect transition
	//
	if( m_bTransitionActive )
	{
		ProcessTransition();
	}

	//----------------------------------------------------------
	//	process fading
	//
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
			SetWordBit( m_uiLedsOn, usLedNo );

			pChannel->m_fTarget = (float)PWM_STEP_TIME * usBrightness;

			if( 0 == m_uiFadeTimeUp )
			{
				pChannel->m_fActual = pChannel->m_fTarget;
			}
			else
			{
				SetWordBit( m_uiFadeActive, usLedNo );
			}
		}
		else
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
	uint16_t	uiFirstStep;


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
			
			//------------------------------------------------------
			//	if no signal aspect transition is active then
			//		start the transition
			//		store new signal aspect
			//
			if(		!m_bTransitionActive
				&&	(m_usSignalAspectMain != usSignalAspect) )
			{
				m_puiStepMain	= GetFirstStepMain( m_usSignalAspectMain, usSignalAspect );
				uiFirstStep		= pgm_read_word( m_puiStepMain );

				//--------------------------------------------------
				//	if the first step is SEQUENCE_END
				//	then we have nothing to do and stay at the
				//	actual signal aspect.
				//	In the other case we start a transition.
				//
				if( SEQUENCE_END == uiFirstStep )
				{
					m_usSignalAspectMainBuffer = m_usSignalAspectMain;
				}
				else
				{
					m_bTransitionActive		= true;
					m_usSignalAspectMain	= usSignalAspect;

					SwitchLed( LED_BLUE, true );
				}
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
//	SetSignalType
//------------------------------------------------------------------
//
void IO_ControlClass::SetSignalType( void )
{
	m_usSignalType	= g_clLncvStorage.GetSignalType();

	switch( m_usSignalType )
	{
		case SIGNAL_TYPE_59:
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

		case SIGNAL_TYPE_DIRECT_CONTROL_HARD:
		case SIGNAL_TYPE_60:
		case SIGNAL_TYPE_COMPACT:
		case SIGNAL_TYPE_KS:
		default:
			break;
	}
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
uint16_t * IO_ControlClass::GetFirstStepMain( uint8_t usFromAspect, uint8_t usToAspect )
{
	uint16_t *	pFirstStep	= 0x0000;

	if( SIGNAL_ASPECT_HP0 == usFromAspect )
	{
		switch( usToAspect )
		{
			case SIGNAL_ASPECT_HP1:
				pFirstStep = (uint16_t *)Main_Hp0_Hp1;
				break;

			case SIGNAL_ASPECT_HP2:
				pFirstStep = (uint16_t *)Main_Hp0_Hp2;
				break;

			case SIGNAL_ASPECT_HP1_ZS3:
				pFirstStep = (uint16_t *)Main_Hp0_Hp1_ZS3;
				break;

			case SIGNAL_ASPECT_HP2_ZS3:
				pFirstStep = (uint16_t *)Main_Hp0_Hp2_ZS3;
				break;

			case SIGNAL_ASPECT_SH1:
				pFirstStep = (uint16_t *)Main_Hp0_Sh1;
				break;

			case SIGNAL_ASPECT_KENNLICHT:
				pFirstStep = (uint16_t *)Main_Hp0_KL;
				break;

			case SIGNAL_ASPECT_ZS1:
				pFirstStep = (uint16_t *)Main_Hp0_ZS1;
				break;

			default:
				break;
		}
	}
	else if( SIGNAL_ASPECT_HP0 == usToAspect )
	{
		switch( usFromAspect )
		{
			case SIGNAL_ASPECT_HP1:
			case SIGNAL_ASPECT_HP2:
			case SIGNAL_ASPECT_KENNLICHT:
				pFirstStep = (uint16_t *)Main_to_Hp0;
				break;

			case SIGNAL_ASPECT_HP1_ZS3:
			case SIGNAL_ASPECT_HP2_ZS3:
				pFirstStep = (uint16_t *)Main_ZS3_Hp0;
				break;

			case SIGNAL_ASPECT_SH1:
			case SIGNAL_ASPECT_ZS1:
				pFirstStep = (uint16_t *)Main_Sh1_ZS1_Hp0;
				break;

			default:
				break;
		}
	}

	return( pFirstStep );
}


//******************************************************************
//	GetFirstStepApproach
//------------------------------------------------------------------
//	This function will return a pointer to the first step of
//	a signal aspect sequence for the approach signal
//
uint16_t * IO_ControlClass::GetFirstStepApproach( uint8_t usFromAspect, uint8_t usToAspect )
{
	uint16_t *	pFirstStep	= 0x0000;

	if( SIGNAL_ASPECT_HP0 == usFromAspect )
	{
		switch( usToAspect )
		{
			case SIGNAL_ASPECT_HP1:
//				pFirstStep = (uint16_t *)Main_Hp0_Hp1;
				break;

			case SIGNAL_ASPECT_HP2:
//				pFirstStep = (uint16_t *)Main_Hp0_Hp2;
				break;

			case SIGNAL_ASPECT_HP1_ZS3:
//				pFirstStep = (uint16_t *)Main_Hp0_Hp1_ZS3;
				break;

			case SIGNAL_ASPECT_HP2_ZS3:
//				pFirstStep = (uint16_t *)Main_Hp0_Hp2_ZS3;
				break;

			case SIGNAL_ASPECT_SH1:
//				pFirstStep = (uint16_t *)Main_Hp0_Sh1;
				break;

			case SIGNAL_ASPECT_KENNLICHT:
//				pFirstStep = (uint16_t *)Main_Hp0_KL;
				break;

			case SIGNAL_ASPECT_ZS1:
//				pFirstStep = (uint16_t *)Main_Hp0_ZS1;
				break;

			default:
				break;
		}
	}
	else if( SIGNAL_ASPECT_HP0 == usToAspect )
	{
		switch( usFromAspect )
		{
			case SIGNAL_ASPECT_HP1:
			case SIGNAL_ASPECT_HP2:
			case SIGNAL_ASPECT_KENNLICHT:
//				pFirstStep = (uint16_t *)Main_to_Hp0;
				break;

			case SIGNAL_ASPECT_HP1_ZS3:
			case SIGNAL_ASPECT_HP2_ZS3:
//				pFirstStep = (uint16_t *)Main_ZS3_Hp0;
				break;

			case SIGNAL_ASPECT_SH1:
			case SIGNAL_ASPECT_ZS1:
//				pFirstStep = (uint16_t *)Main_Sh1_ZS1_Hp0;
				break;

			default:
				break;
		}
	}

	return( pFirstStep );
}


//******************************************************************
//	ProcessTransition
//------------------------------------------------------------------
//	This function will process the steps of
//	a signal aspect transition
//
void IO_ControlClass::ProcessTransition( void )
{
	uint16_t	uiAspect;
	uint16_t	uiDiff;
	uint16_t	uiMask	= 0x0001;

	if( IsFadeFinished() )
	{
		uiAspect = pgm_read_word( m_puiStepMain );

		if( SEQUENCE_END == uiAspect )
		{
			//--------------------------------------------------
			//	the end of the sequence is reached
			//	so stop the transition
			//
			m_bTransitionActive = false;

			SwitchLed( LED_BLUE, false );

			//--------------------------------------------------
			//	check if we got a new signal aspect in between
			//	if so, start a new transition
			//
			if( m_usSignalAspectMain != m_usSignalAspectMainBuffer )
			{
				HandleSignalAspectMain( m_usSignalAspectMainBuffer );
			}
		}
		else
		{
			//--------------------------------------------------
			//	check wich LEDs of the main signal
			//	should go OFF and wich should go ON
			//
			uiDiff  = m_uiLedsOn & SIGNAL_MAIN_LEDS;
			uiDiff ^= uiAspect;

			for( uint8_t idx = 0 ; SIGNAL_LEDS > idx ; idx++ )
			{
				if( uiDiff & uiMask )
				{
					if( uiAspect & uiMask )
					{
						HandleLed( idx, true );
					}
					else
					{
						HandleLed( idx, false );
					}
				}

				uiMask <<= 1;
			}

			m_puiStepMain++;
		}
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
				}

				pChannel->m_fActual = ulNewValue;
			}

			uiMask <<= 1;
		}
	}
}
