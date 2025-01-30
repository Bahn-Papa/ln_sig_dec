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
//#	File version:	31		from: 30.09.2024
//#
//#	Bug Fix:
//#		-	improve calculation of new brightness level
//#			change in function
//#				ProcessBrightnessAndFade()
//#					change type of 'newValue' from 'uint32_t' to 'float'
//#
//#-------------------------------------------------------------------------
//#
//#	File version:	30		from: 14.08.2024
//#
//#	Implementation:
//#		-	improvement of special handling of approach kennlicht
//#			change in functions
//#				Init()
//#				SetSignalType()
//#
//#-------------------------------------------------------------------------
//#
//#	File Version:	29		from: 14.08.2024
//#
//#	Implementation:
//#		-	improvement of sequence handling
//#			change in functions
//#				SwitchExtraSignalsOn()
//#				SwitchExtraSignalsOff()
//#		-	unused definitions deleted
//#		
//#-------------------------------------------------------------------------
//#
//#	File Version:	28		from: 05.08.2024
//#
//#	Implementation:
//#		-	improve selection of main signal aspects. Now, only one
//#			message is needed to start a signal aspec sequence
//#			change in function
//#				HandleSignalAspectMain()
//#
//#-------------------------------------------------------------------------
//#
//#	File Version:	27		from: 05.08.2024
//#
//#	Implementation:
//#		-	add a new signal type SIGNAL_TYPE_APPROACH
//#			new variable
//#				m_bApproachKennlichtSpecialHandling
//#			changes in functions
//#				Init()
//#				SetSignalType()
//#				SwitchExtraSignalsOff()
//#
//#-------------------------------------------------------------------------
//#
//#	File Version:	26		from: 18.02.2024
//#
//#	Implementation:
//#		-	improve handling of extra signals
//#			rename variable
//#				m_usExtraSignals	=>	m_usExtraSignalsMain
//#			new variable
//#				m_usExtraSignalsApproach
//#			change in functions
//#				Init()
//#				HandleSignalAspectMain()
//#				HandleSignalAspectApproach()
//#				SwitchExtraSignalsOn()
//#
//#-------------------------------------------------------------------------
//#
//#	File Version:	25		from: 07.11.2023
//#
//#	Bug Fix:
//#		-	change handling of brightness and fade
//#			first check step, then period
//#			change in function
//#				ProcessBrightnessAndFade()
//#
//#-------------------------------------------------------------------------
//#
//#	File Version:	24		from: 11.07.2023
//#
//#	Implementation:
//#		-	improve handling when two transitions should be handled at
//#			the same time
//#			changed functions
//#				Loop()
//#				HandleSignalAspectMain()
//#				HandleSignalAspectApproach()
//#			new function
//#				ProcessTransition()
//#
//#-------------------------------------------------------------------------
//#
//#	File Version:	23		from: 08.06.2023
//#
//#	Implementation:
//#		-	add handling if no main signal is present
//#			changes in functions
//#				Init()
//#				HandleSignalAspect()
//#				HandleSignalAspectApproach()
//#
//#-------------------------------------------------------------------------
//#
//#	File Version:	22		from: 18.05.2023
//#
//#	Bug Fix:
//#		-	change input pin for track contact from PB2 to PB3
//#
//#-------------------------------------------------------------------------
//#
//#	File Version:	21		from: 30.03.2023
//#
//#	Implementation:
//#		-	add handling for a 5th rangier signal
//#			changes in functions
//#				Init()
//#				HandleSignalAspect()
//#				HandleRangierAspect()
//#
//#-------------------------------------------------------------------------
//#
//#	File Version:	20		from: 24.03.2023
//#
//#	Implementation:
//#		-	add handling for rangier signal aspects
//#			new variable
//#				m_arusRangierAspect
//#			new function
//#				HandleRangierAspect()
//#			changes in function
//#				HandleSignalAspect()
//#
//#-------------------------------------------------------------------------
//#
//#	File Version:	19		from: 23.03.2023
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
//#	File Version:	18		from: 19.03.2023
//#
//#	Implementation:
//#		-	change the way a signal aspect change is called
//#			new function
//#				HandleSignalAspect()
//#			move function from public to private
//#				HandleSignalAspectMain()
//#				HandleSignalAspectApproach()
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
#include "time_values.h"
#include "sequencer_definitions.h"
#include "sequencer_tables.h"


//==========================================================================
//
//		D E F I N I T I O N S
//
//==========================================================================

#define	LEDS_PORTB		0x30
#define LEDS_PORTC		0x3F
#define LEDS_PORTD		0xFF

//----------------------------------------------------------------------
//	Track contact definitions
//
#define	TRACK_BIT					PB3
#define TRACK_PIN					PINB

#define TRACK_CONTACT_WAIT_TIME		100

#define READ_CONTACT				(0 == (TRACK_PIN & (1 << TRACK_BIT)))


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

time_values_t	g_stTimeValues;


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

	for( uint8_t idx = 0 ; RANGIER_SIGNAL_COUNT > idx ; idx++ )
	{
		m_arpclTransition[ idx ]		= NULL;
	}

	m_ulLastCycleStartTime				= 0L;
	m_ulCycleTime						= 0L;
	m_ulCycleStepTime					= 0L;
	m_ulContactTime						= 0L;
	m_ulErsatzSignalFlashTimer			= 0L;
	m_uiErsatzSignalOffTimer			= 0;
	m_uiLedsOn							= 0x0000;
	m_uiFadeActive						= 0x0000;
	m_uiFadeTimeUp						= 0;
	m_uiFadeTimeDown					= 0;
	m_usExtraSignalsMain				= 0x00;
	m_usExtraSignalsApproach			= 0x00;
	m_bNightLight						= false;
	m_bApproachDark						= false;
	m_bErsatzSignalFlash				= false;
	m_bMainSignalPresent				= false;
	m_bApproachSignalPresent			= false;
	m_bApproachKennlichtSpecialHandling	= false;

	m_bContactOccupied					= false;

	g_stTimeValues.uiDarkTime				= g_clLncvStorage.ReadLNCV( LNCV_ADR_DARK_TIME );
	g_stTimeValues.uiFlashTime				= g_clLncvStorage.ReadLNCV( LNCV_ADR_FLASH_TIME );
	g_stTimeValues.uiErsatzSignalFlashTime	= g_clLncvStorage.ReadLNCV( LNCV_ADR_ERSATZ_SIGNAL_FLASH_TIME );

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

	if( 0 != g_clLncvStorage.ReadLNCV( LNCV_ADR_LIGHT_B ) )
	{
		m_bMainSignalPresent = true;
	}

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
	//	set signal(s) to show Hp0
	//
	if( SIGNAL_TYPE_FOUR_RANGIER_SIG == m_usSignalType )
	{
		SwitchLed( LED_A, true );
		SwitchLed( LED_D, true );
		SwitchLed( LED_G, true );
		SwitchLed( LED_J, true );
		SwitchLed( LED_M, true );
	}
	else if( SIGNAL_TYPE_US == m_usSignalType )
	{
		for( uint8_t idx = 0 ; US_SIGNAL_COUNT > idx ; idx++ )
		{
			uint8_t	sigIdx	= SIGNAL_SELECTED_US_1 + idx;

			g_clStatus.SetSignalAspect( sigIdx, US_SIGNAL_ASPECT_HP0 );
			g_clStatus.SetSignalOn( sigIdx, true );
		}

		SwitchLed( LED_A, true );
		SwitchLed( LED_G, true );
		SwitchLed( LED_H, true );
		SwitchLed( LED_N, true );
	}
	else if( m_bMainSignalPresent )
	{
		SwitchLed( LED_MAIN_RED_LEFT, true );

		if( bIsASig )
		{
			SwitchLed( LED_MAIN_RED_RIGHT, true );
		}
	}
	else
	{
		SwitchLed( LED_APPROACH_YELLOW_TOP,		true );
		SwitchLed( LED_APPROACH_YELLOW_BOTTOM,	true );
	}
	
	//----------------------------------------------------------
	//	configuration options
	//		approach signal kennlicht special handling
	//
	uiHelper = g_clLncvStorage.ReadLNCV( LNCV_ADR_CONFIG );

	if( uiHelper & APPROACH_KENNLICHT_SPECIAL_HANDLING )
	{
		m_bApproachKennlichtSpecialHandling = true;
		
		if( uiHelper & APPROACH_KENNLICHT_DEFAULT_ON )
		{
			SwitchLed( LED_APPROACH_KENNLICHT, true );
		}
	}
}


//******************************************************************
//	Loop
//------------------------------------------------------------------
//
void IO_ControlClass::Loop( void )
{
	uint32_t	ulMillis = millis();


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
		if( ulMillis > m_ulContactTime )
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
		m_ulContactTime = ulMillis + TRACK_CONTACT_WAIT_TIME;
	}

	//----------------------------------------------------------
	//	handle Ersatzsignal flashing
	//
	if( m_bErsatzSignalFlash )
	{
		if( ulMillis > m_ulErsatzSignalFlashTimer )
		{
			if( IsFadeFinished( (uint8_t)LED_ERSATZSIGNAL ) )
			{
				m_ulErsatzSignalFlashTimer = ulMillis + g_stTimeValues.uiErsatzSignalFlashTime;
				
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
	if( (0 < g_ulTimerTick) && (ulMillis > g_ulTimerTick) )
	{
		g_ulTimerTick += FLASH_COUNTER_TIME;
		m_uiErsatzSignalOffTimer--;

		if( 0 == m_uiErsatzSignalOffTimer )
		{
			SetErsatzSignalFlash( false );
		}
	}

	//----------------------------------------------------------
	//	handle transitions
	//
	if( SIGNAL_TYPE_FOUR_RANGIER_SIG == m_usSignalType )
	{
		for( uint8_t idx = 0 ; RANGIER_SIGNAL_COUNT > idx ; idx++ )
		{
			ProcessTransition( idx, ulMillis );
		}
	}
	else if( SIGNAL_TYPE_US == m_usSignalType )
	{
		for( uint8_t idx = 0 ; US_SIGNAL_COUNT > idx ; idx++ )
		{
			ProcessTransition( idx, ulMillis );
		}
	}
	else if( ProcessTransition( 0, ulMillis ) )
	{
		m_arpclTransition[ 0 ]	= m_arpclTransition[ 1 ];
		m_arpclTransition[ 1 ]	= NULL;
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
		m_ulErsatzSignalFlashTimer	= millis() + g_stTimeValues.uiErsatzSignalFlashTime;
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
//	HandleSignalAspect
//------------------------------------------------------------------
//
void IO_ControlClass::HandleSignalAspect(	uint8_t usSelectedSignal,
											uint8_t usSignalAspect,
											uint8_t usExtraSignals		)
{
	TransitionClass	*	pclTransition;
	uint8_t				idx;


	//---------------------------------------------------------
	//	temporary "downgrade" usSignalAspect to valid number
	//	this is done to have no gaps in the sequencer tables
	//
	if( 4 == usSignalAspect )
	{
		usSignalAspect = 3;
	}
	else if( 8 == usSignalAspect )
	{
		usSignalAspect = 4;
	}
	//
	//---------------------------------------------------------

	switch( usSelectedSignal )
	{
		case SIGNAL_SELECTED_MAIN:
			if( m_bMainSignalPresent && (SIGNAL_ASPECTS > usSignalAspect) )
			{
				if(		(NULL == m_arpclTransition[ 0 ])
					||	(NULL == m_arpclTransition[ 1 ]) )
				{
					pclTransition = new TransitionClass(	m_usSignalType,
															usSelectedSignal,
															usSignalAspect,
															usExtraSignals		);

					if( NULL == m_arpclTransition[ 0 ] )
					{
						m_arpclTransition[ 0 ]	= pclTransition;
					}
					else
					{
						m_arpclTransition[ 1 ]	= pclTransition;
					}

					SwitchLed( LED_GREEN, true );
				}
			}
			break;

		case SIGNAL_SELECTED_APPROACH:
			if( m_bApproachSignalPresent && (SIGNAL_ASPECTS > usSignalAspect) )
			{
				if( IsLedsOff( APPROACH_LEDS ) )
				{
					//----------------------------------------------
					//	the approach signal is switched OFF
					//	so just store the new signal aspect
					//	as the actual one
					//
					g_clStatus.SetSignalAspect( usSelectedSignal, usSignalAspect );
					m_usExtraSignalsApproach = usExtraSignals;
				}
				else if(	(NULL == m_arpclTransition[ 0 ])
						||	(NULL == m_arpclTransition[ 1 ]) )
				{
					pclTransition = new TransitionClass(	m_usSignalType,
															usSelectedSignal,
															usSignalAspect,
															usExtraSignals		);

					if( NULL == m_arpclTransition[ 0 ] )
					{
						m_arpclTransition[ 0 ]	= pclTransition;
					}
					else
					{
						m_arpclTransition[ 1 ]	= pclTransition;
					}

					SwitchLed( LED_GREEN, true );
				}
			}
			break;

		case SIGNAL_SELECTED_RANGIER_1:
		case SIGNAL_SELECTED_RANGIER_2:
		case SIGNAL_SELECTED_RANGIER_3:
		case SIGNAL_SELECTED_RANGIER_4:
		case SIGNAL_SELECTED_RANGIER_5:
			if(		(RANGIER_ASPECTS > usSignalAspect)
				&&	(NULL == m_arpclTransition[ usSelectedSignal ]) )
			{
				m_arpclTransition[ usSelectedSignal ] =
					new TransitionClass(	m_usSignalType,
											usSelectedSignal,
											usSignalAspect,
											0x00				);

				SwitchLed( LED_GREEN, true );
			}
			break;

		case SIGNAL_SELECTED_US_1:
		case SIGNAL_SELECTED_US_2:
		case SIGNAL_SELECTED_US_3:
		case SIGNAL_SELECTED_US_4:
			if( US_SIGNAL_ASPECTS > usSignalAspect )
			{
				idx = usSelectedSignal - SIGNAL_SELECTED_US_1;

				if( !g_clStatus.GetSignalOn( usSelectedSignal ) )
				{
					g_clStatus.SetSignalAspect( usSelectedSignal, usSignalAspect );
				}
				else if( NULL == m_arpclTransition[ idx ] )
				{
					m_arpclTransition[ idx ] =
						new TransitionClass(	m_usSignalType,
												usSelectedSignal,
												usSignalAspect,
												0x00				);

					SwitchLed( LED_GREEN, true );
				}
			}
			break;
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
//	SwitchSignal
//------------------------------------------------------------------
//
void IO_ControlClass::SwitchSignal( uint8_t usSelectedSignal, bool bOn )
{
	uint8_t		usTargetAspect		= g_clStatus.GetSignalAspect( usSelectedSignal );
	uint16_t	uiMask;
	uint8_t		usStartIdx;
	uint8_t		usStopIdx;
	uint8_t *	pusStep				= (uint8_t *)sNOP;
	uint8_t		usIdx;
	bool		bStartTransition	= true;


	if( g_clStatus.GetSignalOn( usSelectedSignal ) != bOn )
	{
		switch( usSelectedSignal )
		{
			case SIGNAL_SELECTED_US_1:
				usStartIdx	= US_SIGNAL_1_LEDS_IDX_START;
				usStopIdx	= US_SIGNAL_1_LEDS_IDX_STOP;
				uiMask		= US_SIGNAL_1_LEDS;
				usIdx		= 0;

				if( m_bContactOccupied )
				{
					//-----------------------------------------
					//	hier die alten Signale schalten
					//
					if( bOn )
					{
						pusStep = pgm_read_word( &US_FADE_On[ usTargetAspect ] );
					}
					else
					{
						pusStep	= (uint8_t *)&US_FADE_to_Off;
					}
				}
				else
				{
					//-----------------------------------------
					//	und hier entsprechend die Neuen
					//
					if( bOn )
					{
						pusStep = pgm_read_word( &US_SW_On[ usTargetAspect ] );
					}
					else
					{
						pusStep	= (uint8_t *)&US_SW_to_Off;
					}
				}
				break;

			case SIGNAL_SELECTED_US_2:
				usStartIdx	= US_SIGNAL_2_LEDS_IDX_START;
				usStopIdx	= US_SIGNAL_2_LEDS_IDX_STOP;
				uiMask		= US_SIGNAL_2_LEDS;
				usIdx		= 1;
				
				if( m_bContactOccupied )
				{
					if( bOn )
					{
						pusStep = pgm_read_word( &US_FADE_INV_On[ usTargetAspect ] );
					}
					else
					{
						pusStep	= (uint8_t *)&US_FADE_to_Off;
					}
				}
				else
				{
					if( bOn )
					{
						pusStep = pgm_read_word( &US_SW_INV_On[ usTargetAspect ] );
					}
					else
					{
						pusStep	= (uint8_t *)&US_SW_to_Off;
					}
				}
				break;

			case SIGNAL_SELECTED_US_3:
				usStartIdx	= US_SIGNAL_3_LEDS_IDX_START;
				usStopIdx	= US_SIGNAL_3_LEDS_IDX_STOP;
				uiMask		= US_SIGNAL_3_LEDS;
				usIdx		= 2;
				
				if( m_bContactOccupied )
				{
					if( bOn )
					{
						pusStep = pgm_read_word( &US_FADE_On[ usTargetAspect ] );
					}
					else
					{
						pusStep	= (uint8_t *)&US_FADE_to_Off;
					}
				}
				else
				{
					if( bOn )
					{
						pusStep = pgm_read_word( &US_SW_On[ usTargetAspect ] );
					}
					else
					{
						pusStep	= (uint8_t *)&US_SW_to_Off;
					}
				}
				break;

			case SIGNAL_SELECTED_US_4:
				usStartIdx	= US_SIGNAL_4_LEDS_IDX_START;
				usStopIdx	= US_SIGNAL_4_LEDS_IDX_STOP;
				uiMask		= US_SIGNAL_4_LEDS;
				usIdx		= 3;
				
				if( m_bContactOccupied )
				{
					if( bOn )
					{
						pusStep = pgm_read_word( &US_FADE_INV_On[ usTargetAspect ] );
					}
					else
					{
						pusStep	= (uint8_t *)&US_FADE_to_Off;
					}
				}
				else
				{
					if( bOn )
					{
						pusStep = pgm_read_word( &US_SW_INV_On[ usTargetAspect ] );
					}
					else
					{
						pusStep	= (uint8_t *)&US_SW_to_Off;
					}
				}
				break;

			default:
				bStartTransition = false;
				break;
		}
		
		if( bStartTransition )
		{
			if( NULL == m_arpclTransition[ usIdx ] )
			{
				m_arpclTransition[ usIdx ] =
					new TransitionClass(	pusStep,
											uiMask,
											usStartIdx,
											usStopIdx		);

				g_clStatus.SetSignalOn( usSelectedSignal, bOn );

				SwitchLed( LED_GREEN, true );
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

		case SIGNAL_TYPE_US:
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
//	SwitchExtraSignalsOn
//------------------------------------------------------------------
//	This function will switch on the selected Extra signals
//
void IO_ControlClass::SwitchExtraSignalsOn(	uint8_t usSelectedSignal,
											uint8_t usExtraSignals		)
{
	uint8_t		usLedNo;
	uint8_t		usMask		= 0x01;

	if( SIGNAL_SELECTED_MAIN == usSelectedSignal )
	{
		m_usExtraSignalsMain = usExtraSignals;
	}
	else
	{
		m_usExtraSignalsApproach = usExtraSignals;
	}

	for( uint8_t idx = 0 ; EXTRA_SIGNAL_COUNT > idx ; idx++ )
	{
		if( usExtraSignals & usMask )
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
		}

		usMask <<= 1;
	}
}


//******************************************************************
//	SwitchExtraSignalsOff
//------------------------------------------------------------------
//	This function will switch off Extra signals, depending on
//	which signal type is given
//		SIGNAL_SELECTED_MAIN		all extra signals off
//		SIGNAL_SELECTED_APPROACH	switch off only approach signal
//									related extra signals
//
void IO_ControlClass::SwitchExtraSignalsOff( uint8_t usSelectedSignal,
											 uint8_t /* usExtraSignals */	)
{
	uint8_t	usLedNo;
	uint8_t	idx		= 2;	//	start of approach extra signals


	if( SIGNAL_SELECTED_MAIN == usSelectedSignal )
	{
		SetErsatzSignalFlash( false );
		idx = 0;	//	all approach extra signals
	}

	while( EXTRA_SIGNAL_ERSATZSIGNAL_FLASH > idx )
	{
		usLedNo = g_arLedIndex[ idx++ ];

		//-----------------------------------------------------
		//	do not switch off LED_APPROACH_KENNLICHT when
		//	m_bApproachKennlichtSpecialHandling is true
		//	in all other cases switch of the extra signals
		//
		if(		!m_bApproachKennlichtSpecialHandling
			|| (LED_APPROACH_KENNLICHT != usLedNo)		)
		{
			HandleLed( usLedNo, false );
		}
	}
}


//******************************************************************
//	ProcessTransition
//------------------------------------------------------------------
//	This function will process the selected transition
//
bool IO_ControlClass::ProcessTransition( uint8_t idx, uint32_t ulMillis )
{
	TransitionClass	*	pclTransition	= m_arpclTransition[ idx ];
	bool				bFinished		= false;


	if( NULL != pclTransition )
	{
		if( pclTransition->Loop( ulMillis ) )
		{
			delete pclTransition;

			m_arpclTransition[ idx ]	= NULL;
			bFinished					= true;
		}
	}

	return( bFinished );
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
	float			fNewValue;
	uint16_t		uiMask			= 0x0001;
	uint32_t		ulActualTime	= micros();

	
	//----------------------------------------------------------
	//	first check if a cycle step should be handled
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
				fNewValue = pChannel->m_fActual + pChannel->m_fFadeRateUp;
				
				if( fNewValue >= pChannel->m_fTarget )
				{
					//----------------------------------
					//	the target is reached
					//	so stop fading
					//
					fNewValue		= pChannel->m_fTarget;
					m_uiFadeActive &= ~uiMask;

					SwitchLed( LED_BLUE, false );
				}

				pChannel->m_fActual = fNewValue;
			}

			if( pChannel->m_fActual > pChannel->m_fTarget )
			{
				//------------------------------------------
				//	... to darker light
				//
				fNewValue = pChannel->m_fActual - pChannel->m_fFadeRateDwon;

				if( fNewValue <= pChannel->m_fTarget )
				{
					//----------------------------------
					//	the target is reached
					//	so stop fading
					//
					fNewValue		= pChannel->m_fTarget;
					m_uiFadeActive &= ~uiMask;

					SwitchLed( LED_BLUE, false );
				}

				pChannel->m_fActual = fNewValue;
			}

			uiMask <<= 1;
		}
	}

	//----------------------------------------------------------
	//	next check if a new cycle should start
	//
	if( (ulActualTime > m_ulCycleTime) || (ulActualTime < m_ulLastCycleStartTime) )
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
}
