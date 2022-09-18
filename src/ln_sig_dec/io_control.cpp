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
//#		 A			X3.2		D0		  0		Zusatzsanzeiger
//#		 B			X3.3		C5		 19		Hp1
//#		 C			X3.4		C4		 18		Hp00 Zugfahrt
//#		 D			X3.5		C3		 17		Hp00 Rangierfahrt
//#		 E			X3.6		C2		 16		Kennlicht
//#		 F			X3.7		C1		 15		SH1 Rangierfahrt
//#		 G			X3.8		C0		 14		Hp2
//#		 H			X1.2		D7		  7		Ersatzsignal
//#		 I			X1.3		D6		  6		Zusatz-Kennlicht
//#		 J			X1.4		D5		  5
//#		 K			X1.5		D4		  4
//#		 L			X1.6		D3		  3
//#		 M			X1.7		D2		  2
//#		 N			X1.8		D1		  1
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


//==========================================================================
//
//		D E F I N I T I O N S
//
//==========================================================================

//----------------------------------------------------------------------
//	Cycle time in µs
//
#define PWM_CYCLE_TIME			20400
#define PWM_STEP_TIME			(PWM_CYCLE_TIME / 255)

#define FADE_TIME_UP_INIT		1000
#define FADE_TIME_DOWN_INIT		2000

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
	m_ulLastCycleStartTime	= 0L;
	m_ulCycleTime			= 0L;
	m_ulCycleStepTime		= 0L;
	m_usSignalType			= g_clLncvStorage.GetSignalType();
	m_bNightLight			= false;

	GetFadeTimes();

	for( uint8_t idx = 0 ; idx < SIGNAL_LEDS ; idx++ )
	{
		g_arChannel[ idx ].m_usBrightnessDay	= g_clLncvStorage.GetLightIntensityDay( idx );
		g_arChannel[ idx ].m_usBrightnessNight	= g_clLncvStorage.GetLightIntensityNight( idx );
		g_arChannel[ idx ].m_fTarget = 0.0;
		g_arChannel[ idx ].m_fActual = 0.0;
	}
	
	CalculateFadeRates();
}


//******************************************************************
//	FadeUpdate
//------------------------------------------------------------------
//	This function will do the main work if a LED should be fade up
//	or down.
//
void IO_ControlClass::FadeUpdate( void )
{
	PWMChannel_t*	pChannel;
	uint8_t			idx;
	uint32_t		ulNewValue;
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
			//	... process the fade
			//
			if( pChannel->m_fActual < pChannel->m_fTarget )
			{
				ulNewValue = pChannel->m_fActual + pChannel->m_fFadeRateUp;
				
				if( pChannel->m_fActual >= pChannel->m_fTarget )
				{
					ulNewValue = pChannel->m_fTarget;
				}

				pChannel->m_fActual = ulNewValue;
			}

			if( pChannel->m_fActual > pChannel->m_fTarget )
			{
				ulNewValue = pChannel->m_fActual - pChannel->m_fFadeRateDwon;

				if( pChannel->m_fActual <= pChannel->m_fTarget )
				{
					ulNewValue = pChannel->m_fTarget;
				}

				pChannel->m_fActual = ulNewValue;
			}
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

		if( 0 == m_uiFadeTimeUp )
		{
			pChannel->m_fActual = pChannel->m_fTarget;
		}
	}
	else
	{
		pChannel->m_fTarget = 0.0;

		if( 0 == m_uiFadeTimeDown )
		{
			pChannel->m_fActual = 0.0;
		}
	}
}


//******************************************************************
//	HandleSignalAspectMain
//------------------------------------------------------------------
//
void IO_ControlClass::HandleSignalAspectMain( uint8_t usSignalAspect )
{
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
			}
			else
			{
				pChannel->m_fTarget = 0.0;
				pChannel->m_fActual = 0.0;
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
bool IO_ControlClass::IsFadeFinished( uint8_t usLedNo )
{
	if( SIGNAL_LEDS > usLedNo )
	{
		if( g_arChannel[ usLedNo ].m_fActual == g_arChannel[ usLedNo ].m_fTarget )
		{
			return( true );
		}
	}

	return( false );
}


//******************************************************************
//	GetFadeTimes
//------------------------------------------------------------------
//
void IO_ControlClass::GetFadeTimes( void )
{
	switch( m_usSignalType )
	{
		case SIGNAL_TYPE_DIRECT_CONTROL_HARD:
		case SIGNAL_TYPE_59:
		case SIGNAL_TYPE_60:
		case SIGNAL_TYPE_COMPACT:
		case SIGNAL_TYPE_KS:
		default:
			m_uiFadeTimeUp		= 0;
			m_uiFadeTimeDown	= 0;
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
