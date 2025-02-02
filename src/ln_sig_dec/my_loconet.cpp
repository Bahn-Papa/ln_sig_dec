//##########################################################################
//#
//#		MyLoconetClass
//#
//#	This class will handle everything that has something to do with
//#	Loconet messages.
//#
//#-------------------------------------------------------------------------
//#
//#	File Version:	20		from: 02.02.2025
//#
//#	Implementation:
//#		-	add handling for new signal type SIGNAL_TYPE_US
//#			change in function
//#				LoconetReceived()
//#
//#-------------------------------------------------------------------------
//#
//#	File Version:	19		from: 30.09.2024
//#
//#	Bug Fix:
//#		-	improve handling of writing LNCVs
//#			change in function
//#				notifyLNCVwrite()
//#					don't write '0' or '0xFFFF' as module address
//#
//#-------------------------------------------------------------------------
//#
//#	File Version:	18		from: 18.02.2024
//#
//#	Implementation:
//#		-	improvement of notify messages
//#			don't react on notifySwitchReport and notifySwitchState
//#			to avoid missinterpretation of requests.
//#			delete functions
//#				notifySwitchReport()
//#				notifySwitchState()
//#
//#-------------------------------------------------------------------------
//#
//#	File Version:	17		from: 07.11.2023
//#
//#	Bug Fix:
//#		-	new LocoNet Lib version 1.1.13
//#			change in function
//#				notifyLNCVread()
//#		-	switch off green LED was too late
//#			change in function
//#				SetSelectedSignalAspect
//#
//#-------------------------------------------------------------------------
//#
//#	File version:	16		from: 26.07.2023
//#
//#	Bug Fix:
//#		-	by mistake no acknowledge was send when receiving a programming
//#			start with broadcast as address.
//#
//#-------------------------------------------------------------------------
//#
//#	File version:	15		from: 18.07.2023
//#
//#	Bug Fix:
//#		-	do not go into prog mode when a programming start msg
//#			with broadcast address was detected
//#			change in function
//#				notifyLNCVprogrammingStart()
//#
//#-------------------------------------------------------------------------
//#
//#	File version:	14		from: 04.06.2023
//#
//#	Bug Fix:
//#		-	do not go into prog mode when a discover msg was detected
//#			change in function
//#				notifyLNCVdiscover()
//#
//#-------------------------------------------------------------------------
//#
//#	File Version:	13		from: 30.03.2023
//#
//#	Implementation:
//#		-	add handling for a 5th rangier signal
//#			changes in function
//#				LoconetReceived()
//#
//#-------------------------------------------------------------------------
//#
//#	File version:	12		from: 17.03.2023
//#
//#	Implementation:
//#		-	change the way a signal aspect change is called
//#			change in function
//#				SetSelectedSignalAspect()
//#		-	add handling of rangier signals
//#			change in function
//#				LoconetReceived()
//#
//#-------------------------------------------------------------------------
//#
//#	File version:	11		from: 06.03.2023
//#
//#	Bug Fix:
//#		-	problem after change of module address fixed
//#			change in function
//#				notifyLNCVwrite()
//#
//#-------------------------------------------------------------------------
//#
//#	File version:	10		from: 01.02.2023
//#
//#	Bug Fix:
//#		-	could not switch off signal aspect
//#			change in function
//#				AddSignalAspect()
//#
//#-------------------------------------------------------------------------
//#
//#	File version:	9		from: 20.01.2023
//#
//#	Implementation:
//#		-	add handling of extra signals
//#			new variable
//#				uint8_t m_usExtraSignals
//#
//#-------------------------------------------------------------------------
//#
//#	File version:	8		from: 18.01.2023
//#
//#	Implementation:
//#		-	add two new devices: approach signal dark
//#			and Ersatz signal flash
//#			change in functions
//#				Init()
//#				LoconetReceived()
//#
//#-------------------------------------------------------------------------
//#
//#	File version:	7		from: 12.12.2022
//#
//#	Implementation:
//#		-	add handling of the track contact
//#			new function
//#				SendTrackContactMessage()
//#
//#-------------------------------------------------------------------------
//#
//#	File version:	6		from: 22.11.2022
//#
//#	Implementation:
//#		-	add way to mix signal aspects with direct controlled LEDs
//#			change in function: LoconetReceived()
//#
//#-------------------------------------------------------------------------
//#
//#	File version:	5		from: 17.11.2022
//#
//#	Implementation:
//#		-	streamlining lncv storage
//#			changes in function 'Init()'
//#				now read signal type direct from LNCV
//#
//#-------------------------------------------------------------------------
//#
//#	File version:	4		from: 28.09.2022
//#
//#	Implementation:
//#		-	add new signal types
//#
//#-------------------------------------------------------------------------
//#
//#	File version:	3		from: 28.09.2022
//#
//#	Implementation:
//#		-	change struct 'device_t' to save some memory space
//#			combine	bool 'm_bSensor' and bool 'm_bInvert'
//#				to	uint8_t 'm_bFlags'
//#			add definitions for
//#				DEVICE_IS_SENSOR
//#				DEVICE_IS_INVERT
//#			changes in function
//#				SetDeviceInfo()
//#				LoconetReceived()
//#
//#-------------------------------------------------------------------------
//#
//#	File version:	2		from: 19.09.2022
//#
//#	Implementation:
//#		-	add version number into EEPROM
//#			changes in function 'notifyLNCVwrite()'
//#
//#-------------------------------------------------------------------------
//#
//#	File version:	1		from: 17.09.2022
//#
//#	Implementation:
//#		-	first version
//#
//##########################################################################


//==========================================================================
//
//		I N C L U D E S
//
//==========================================================================

#include <Arduino.h>
#include <LocoNet.h>

#include "my_loconet.h"
#include "io_control.h"
#include "lncv_storage.h"
#include "macros.h"


//==========================================================================
//
//		D E F I N I T I O N S
//
//==========================================================================

#define LOCONET_TX_PIN				9

#define	DIR_RED						0
#define DIR_THROWN					0
#define DIR_GREEN					1
#define DIR_CLOSED					1

#define DEVICE_IS_INVERT			0x01
#define DEVICE_IS_SENSOR			0x02


//==========================================================================
//
//		G L O B A L   V A R I A B L E S
//
//==========================================================================

MyLoconetClass	 g_clMyLoconet		= MyLoconetClass();

LocoNetCVClass	 g_clLNCV;
lnMsg			*g_pLnPacket;

uint16_t	g_uiArticleNumber;
uint16_t	g_uiModuleAddress;


//==========================================================================
//
//		C L A S S   F U N C T I O N S
//
//==========================================================================


////////////////////////////////////////////////////////////////////////
//	CLASS: MyLoconetClass
//

//**********************************************************************
//	Constructor
//----------------------------------------------------------------------
//
MyLoconetClass::MyLoconetClass()
{
}


//**********************************************************************
//	Init
//----------------------------------------------------------------------
//
void MyLoconetClass::Init( void )
{
	uint16_t	uiHelper;


	m_ulTimer					= 0L;
	m_usSignalSelected			= 0;
	m_usSignalAspect			= 0;
	m_usExtraSignals			= 0x00;
	m_bIsProgMode				= false;
	m_usSignalType				= (uint8_t)g_clLncvStorage.ReadLNCV( LNCV_ADR_SIGNAL_TYPE );
	m_uiSignalValidWaitTime		= g_clLncvStorage.ReadLNCV( LNCV_ADR_SIGNAL_VALID_WAIT_TIME );

	g_uiArticleNumber			= g_clLncvStorage.ReadLNCV( LNCV_ADR_ARTIKEL_NUMMER );
	g_uiModuleAddress			= g_clLncvStorage.ReadLNCV( LNCV_ADR_MODULE_ADDRESS );

	//--------------------------------------------------------------
	//	setup all loconet devices
	//
	for( uint8_t idx = 0 ; idx < NUMBER_LIGHTS ; idx++ )
	{
		uiHelper = g_clLncvStorage.ReadLNCV( LNCV_ADR_LIGHT_A + idx );
		SetDeviceInfo( idx, uiHelper );
	}
	
	uiHelper = g_clLncvStorage.ReadLNCV( LNCV_ADR_TRACK_CONTACT );
	SetDeviceInfo( IDX_TRACK_CONTACT, uiHelper );

	uiHelper = g_clLncvStorage.ReadLNCV( LNCV_ADR_NIGHTLIGHT );
	SetDeviceInfo( IDX_NIGHTLIGHT, uiHelper );

	uiHelper = g_clLncvStorage.ReadLNCV( LNCV_ADR_APPROACH_DARK );
	SetDeviceInfo( IDX_APPROACH_DARK, uiHelper );

	uiHelper = g_clLncvStorage.ReadLNCV( LNCV_ADR_ERSATZ_SIGNAL_FLASH );
	SetDeviceInfo( IDX_ERSATZ_SIGNAL_FLASH, uiHelper );

	//--------------------------------------------------------------
	//	start communication
	//
	LocoNet.init( LOCONET_TX_PIN );
	
	delay( 100 );
}


//**********************************************************************
//	CheckForAndHandleMessage
//----------------------------------------------------------------------
//
void MyLoconetClass::CheckForMessageAndProcess( void )
{
	//--------------------------------------------------------------
	//	check if there is a command pending
	//
	if( 0L < m_ulTimer )
	{
		if( millis() > m_ulTimer )
		{
			SetSelectedSignalAspect();
		}
	}

	//--------------------------------------------------------------
	//	now check for new Loconet message
	//
	g_pLnPacket = LocoNet.receive();

	if( g_pLnPacket )
	{
		if( !LocoNet.processSwitchSensorMessage( g_pLnPacket ) )
		{
			g_clLNCV.processLNCVMessage( g_pLnPacket );
		}
	}
}


//**********************************************************************
//	LoconetReceived
//----------------------------------------------------------------------
//
void MyLoconetClass::LoconetReceived( bool isSensor, uint16_t adr, uint8_t dir, uint8_t /* output */ )
{
	device_t*	pDevice;
	bool		bIsGreen	= (dir != DIR_RED);
	uint8_t		idx;

	for( idx = 0 ; DEVICE_COUNT > idx ; idx++ )
	{
		pDevice = &(m_arDevices[ idx ]);

		//----------------------------------------------------------
		//	first check if this is one of our addresses
		//
		if( adr == pDevice->m_uiAddress )
		{
			//------------------------------------------------------
			//	yes it is
			//	then check if we are searching for
			//	a sensor message ('isSensor' == true) or
			//	a switch message ('isSensor' == false)
			//
			if( isSensor == (pDevice->m_bFlags & DEVICE_IS_SENSOR) )
			{
				//--------------------------------------------------
				//	we get to this code area only
				//	if 'isSensor' and the expression
				//	'pDevice->m_usFlags & DEVICE_IS_SENSOR' are both
				//		true	==>		sensor message
				//		false	==>		switch message
				//--------------------------------------------------
				//	Okay, address and message type are correct
				//	so process the message ...
				//

				//--------------------------------------------------
				//	Check if 'dir' should be inverted
				//
				if( 0 == (pDevice->m_bFlags & DEVICE_IS_INVERT) )
				{
					bIsGreen = !bIsGreen;
				}

				//--------------------------------------------------
				//	proces message for ...
				//
				if( SIGNAL_TYPE_US == m_usSignalType )
				{
					//----------------------------------------------
					//	... a signal aspect for a rangier signal
					//
					switch( idx )
					{
						case IDX_US_SIGNAL_1_ASPECT_BIT_0:
							AddSignalAspect( SIGNAL_SELECTED_US_1, bIsGreen, 0x01 );
							break;

						case IDX_US_SIGNAL_1_ASPECT_BIT_1:
							AddSignalAspect( SIGNAL_SELECTED_US_1, bIsGreen, 0x02 );
							break;

						case IDX_US_SIGNAL_1_ON_OFF:
							g_clControl.SwitchSignal( SIGNAL_SELECTED_US_1, bIsGreen );
							break;

						case IDX_US_SIGNAL_2_ASPECT_BIT_0:
							AddSignalAspect( SIGNAL_SELECTED_US_2, bIsGreen, 0x01 );
							break;

						case IDX_US_SIGNAL_2_ASPECT_BIT_1:
							AddSignalAspect( SIGNAL_SELECTED_US_2, bIsGreen, 0x02 );
							break;

						case IDX_US_SIGNAL_2_ON_OFF:
							g_clControl.SwitchSignal( SIGNAL_SELECTED_US_2, bIsGreen );
							break;

						case IDX_US_SIGNAL_3_ASPECT_BIT_0:
							AddSignalAspect( SIGNAL_SELECTED_US_3, bIsGreen, 0x01 );
							break;

						case IDX_US_SIGNAL_3_ASPECT_BIT_1:
							AddSignalAspect( SIGNAL_SELECTED_US_3, bIsGreen, 0x02 );
							break;

						case IDX_US_SIGNAL_3_ON_OFF:
							g_clControl.SwitchSignal( SIGNAL_SELECTED_US_3, bIsGreen );
							break;

						case IDX_US_SIGNAL_4_ASPECT_BIT_0:
							AddSignalAspect( SIGNAL_SELECTED_US_4, bIsGreen, 0x01 );
							break;

						case IDX_US_SIGNAL_4_ASPECT_BIT_1:
							AddSignalAspect( SIGNAL_SELECTED_US_4, bIsGreen, 0x02 );
							break;

						case IDX_US_SIGNAL_4_ON_OFF:
							g_clControl.SwitchSignal( SIGNAL_SELECTED_US_4, bIsGreen );
							break;

						case IDX_US_SIGNAL_ALL_ON_OFF:
							g_clControl.SwitchSignal( SIGNAL_SELECTED_US_1, bIsGreen );
							g_clControl.SwitchSignal( SIGNAL_SELECTED_US_2, bIsGreen );
							g_clControl.SwitchSignal( SIGNAL_SELECTED_US_3, bIsGreen );
							g_clControl.SwitchSignal( SIGNAL_SELECTED_US_4, bIsGreen );
							break;
					}
				}
				else if( SIGNAL_TYPE_FOUR_RANGIER_SIG == m_usSignalType )
				{
					//----------------------------------------------
					//	... a signal aspect for a rangier signal
					//
					switch( idx )
					{
						case IDX_RANGIER_SIGNAL_1_ASPECT_BIT_0:
							AddSignalAspect( SIGNAL_SELECTED_RANGIER_1, bIsGreen, 0x01 );
							break;

						case IDX_RANGIER_SIGNAL_1_ASPECT_BIT_1:
							AddSignalAspect( SIGNAL_SELECTED_RANGIER_1, bIsGreen, 0x02 );
							break;

						case IDX_RANGIER_SIGNAL_2_ASPECT_BIT_0:
							AddSignalAspect( SIGNAL_SELECTED_RANGIER_2, bIsGreen, 0x01 );
							break;

						case IDX_RANGIER_SIGNAL_2_ASPECT_BIT_1:
							AddSignalAspect( SIGNAL_SELECTED_RANGIER_2, bIsGreen, 0x02 );
							break;

						case IDX_RANGIER_SIGNAL_3_ASPECT_BIT_0:
							AddSignalAspect( SIGNAL_SELECTED_RANGIER_3, bIsGreen, 0x01 );
							break;

						case IDX_RANGIER_SIGNAL_3_ASPECT_BIT_1:
							AddSignalAspect( SIGNAL_SELECTED_RANGIER_3, bIsGreen, 0x02 );
							break;

						case IDX_RANGIER_SIGNAL_4_ASPECT_BIT_0:
							AddSignalAspect( SIGNAL_SELECTED_RANGIER_4, bIsGreen, 0x01 );
							break;

						case IDX_RANGIER_SIGNAL_4_ASPECT_BIT_1:
							AddSignalAspect( SIGNAL_SELECTED_RANGIER_4, bIsGreen, 0x02 );
							break;

						case IDX_RANGIER_SIGNAL_5_ASPECT_BIT_0:
							AddSignalAspect( SIGNAL_SELECTED_RANGIER_5, bIsGreen, 0x01 );
							break;
					}
				}
				else if( (NUMBER_LIGHTS > idx) && (SIGNAL_TYPE_59_A_SIG > m_usSignalType) )
				{
					//----------------------------------------------
					//	... a direct controlled light device
					//
					g_clControl.HandleLed( idx, bIsGreen );
				}
				else
				{
					//----------------------------------------------
					//	... a signal aspect or other devices
					//
					switch( idx )
					{
						case IDX_MAIN_SCREEN_ASPECT_BIT_0:
						case IDX_MAIN_SCREEN_ASPECT_BIT_1:
						case IDX_MAIN_SCREEN_ASPECT_BIT_2:
						case IDX_MAIN_SCREEN_ASPECT_BIT_3:
							AddSignalAspect( SIGNAL_SELECTED_MAIN, bIsGreen,
											 (1 << (idx - IDX_MAIN_SCREEN_ASPECT_BIT_0)) );
							break;

						case IDX_APPROACH_SCREEN_ASPECT_BIT_0:
						case IDX_APPROACH_SCREEN_ASPECT_BIT_1:
						case IDX_APPROACH_SCREEN_ASPECT_BIT_2:
						case IDX_APPROACH_SCREEN_ASPECT_BIT_3:
							AddSignalAspect( SIGNAL_SELECTED_APPROACH, bIsGreen,
											 (1 << (idx - IDX_APPROACH_SCREEN_ASPECT_BIT_0)) );
							break;

						case LED_MAIN_ZUSATZANZEIGER:
						case LED_ERSATZSIGNAL:
						case LED_APPROACH_KENNLICHT:
						case LED_APPROACH_ZUSATZANZEIGER:
						case LED_ERSATZSIGNAL_FLASH:
							SetExtraSignal( idx, bIsGreen );
							break;

						case IDX_NIGHTLIGHT:
							g_clControl.SetNightLight( bIsGreen );
							break;

						case IDX_APPROACH_DARK:
							g_clControl.SetApproachDark( bIsGreen );
							break;

						case IDX_TRACK_CONTACT:
						default:
							//	do nothing
							break;
					}
				}
			}
		}
	}
}


//**********************************************************************
//	SendTrackContactMessage
//----------------------------------------------------------------------
//
void MyLoconetClass::SendTrackContactMessage( bool bOccupied )
{
	uint16_t	uiAddress	= m_arDevices[ IDX_TRACK_CONTACT ].m_uiAddress;
	uint8_t		usFlags		= m_arDevices[ IDX_TRACK_CONTACT ].m_bFlags;

	if( DEVICE_IS_INVERT & usFlags )
	{
		bOccupied = !bOccupied;
	}

	if( DEVICE_IS_SENSOR & usFlags )
	{
		LocoNet.reportSensor( uiAddress, bOccupied );
	}
	else
	{
		LocoNet.requestSwitch( uiAddress, 1, bOccupied );
		delay( 10 );
		LocoNet.requestSwitch( uiAddress, 0, bOccupied );
	}
}


//**********************************************************************
//	SetDeviceInfo
//----------------------------------------------------------------------
//
void MyLoconetClass::SetDeviceInfo( uint8_t idx, uint16_t uiInfo )
{
	uint16_t	address	= uiInfo / 10;

	uiInfo -= (address * 10);

	m_arDevices[ idx ].m_uiAddress	= address;
	m_arDevices[ idx ].m_bFlags		= (uint8_t)uiInfo;
}


//**********************************************************************
//	AddSignalAspect
//----------------------------------------------------------------------
//
void MyLoconetClass::AddSignalAspect( uint8_t usSelected, bool bIsGreen, uint8_t usAspectMask )
{
	if( 0 < m_usSignalSelected )
	{
		//----------------------------------------------------------
		//	there was at least one message for a signal aspect
		//	so check if this message is for the same signal aspect
		//
		if( m_usSignalSelected != usSelected )
		{
			//--------------------------------------------------
			//	NO !!
			//	so set the actual signal aspect
			//	and start to collect a new one
			//
			SetSelectedSignalAspect();
		}
	}

	//----------------------------------------------------------
	//	'add' the bit for the signal aspect
	//
	if( bIsGreen )
	{
		m_usSignalAspect	|= usAspectMask;
	}

	m_usSignalSelected	 = usSelected;
	m_ulTimer			 = millis() + m_uiSignalValidWaitTime;

	g_clControl.SwitchLed( LED_GREEN, true );
}


//**********************************************************************
//	SetExtraSignal
//----------------------------------------------------------------------
//
void MyLoconetClass::SetExtraSignal( uint8_t usLED, bool bIsGreen )
{
	if( 0 == m_ulTimer )
	{
		//----------------------------------------------------------
		//	there is no signal aspect pending,
		//	so handle extra signal direct
		//
		if( LED_ERSATZSIGNAL_FLASH == usLED )
		{
			g_clControl.SetErsatzSignalFlash( bIsGreen );
		}
		else
		{
			g_clControl.HandleLed( usLED, bIsGreen );
		}
	}
	else if( bIsGreen )
	{
		//----------------------------------------------------------
		//	there is a signal aspect pending,
		//	so remember which extra signals should be switched on
		//	during the handling of the signal aspect
		//
		switch( usLED )
		{
			case LED_MAIN_ZUSATZANZEIGER:
				m_usExtraSignals |= (1 << EXTRA_SIGNAL_ZUSATZANZEIGE);
				break;

			case LED_ERSATZSIGNAL:
				m_usExtraSignals |= (1 << EXTRA_SIGNAL_ERSATZSIGNAL);
				break;

			case LED_APPROACH_KENNLICHT:
				m_usExtraSignals |= (1 << EXTRA_SIGNAL_KENNLICHT);
				break;

			case LED_APPROACH_ZUSATZANZEIGER:
				m_usExtraSignals |= (1 << EXTRA_SIGNAL_ANZEIGE);
				break;

			case LED_ERSATZSIGNAL_FLASH:
				m_usExtraSignals |= (1 << EXTRA_SIGNAL_ERSATZSIGNAL_FLASH);
				break;
		}
	}
}


//**********************************************************************
//	SetSelectedSignalAspect
//----------------------------------------------------------------------
//
void MyLoconetClass::SetSelectedSignalAspect( void )
{
	g_clControl.SwitchLed( LED_GREEN, false );
	
	g_clControl.HandleSignalAspect(	m_usSignalSelected,
									m_usSignalAspect,
									m_usExtraSignals	);

	m_usSignalSelected	= 0;
	m_usSignalAspect	= 0;
	m_usExtraSignals	= 0;
	m_ulTimer			= 0L;
}


//==========================================================================
//
//		L O C O N E T   C A L L B A C K   F U N C T I O N S
//
//==========================================================================


//**********************************************************************
//	notifySensor
//----------------------------------------------------------------------
//
void notifySensor( uint16_t Address, uint8_t State )
{
	g_clMyLoconet.LoconetReceived( true, Address, State, 0 );
}


//**********************************************************************
//	notifySwitchRequest
//----------------------------------------------------------------------
//
void notifySwitchRequest( uint16_t Address, uint8_t Output, uint8_t Direction )
{
	g_clMyLoconet.LoconetReceived( false, Address, Direction, Output );
}


//**********************************************************************
//	notifyLNCVdiscover
//----------------------------------------------------------------------
//
int8_t notifyLNCVdiscover( uint16_t &ArtNr, uint16_t &ModuleAddress )
{
	ArtNr			 = g_uiArticleNumber;
	ModuleAddress	 = g_uiModuleAddress;

//	g_clMyLoconet.SetProgMode( true );

	return( LNCV_LACK_OK );
}


//**********************************************************************
//	notifyLNCVprogrammingStart
//----------------------------------------------------------------------
//
int8_t notifyLNCVprogrammingStart( uint16_t &ArtNr, uint16_t &ModuleAddress )
{
	int8_t retval = -1;		//	default: ignore request
	
	if( g_uiArticleNumber == ArtNr )
	{
		if( 0xFFFF == ModuleAddress )
		{
			//----	broadcast, so give Module Address back  -------
//			g_clMyLoconet.SetProgMode( true );

			ModuleAddress	= g_uiModuleAddress;
			retval			= LNCV_LACK_OK;
		}
		else if( g_uiModuleAddress == ModuleAddress )
		{
			//----  das ist für mich  -----------------------------
			g_clMyLoconet.SetProgMode( true );

			retval	= LNCV_LACK_OK;
		}
	}

	return( retval );
}


//**********************************************************************
//	notifyLNCVprogrammingStop
//----------------------------------------------------------------------
//
void notifyLNCVprogrammingStop( uint16_t ArtNr, uint16_t ModuleAddress )
{
	if( g_clMyLoconet.IsProgMode() )
	{
		if(		(g_uiArticleNumber == ArtNr)
			&&	(g_uiModuleAddress == ModuleAddress) )
		{
			//----	für mich, also Prog Mode aus  ------------------
			g_clMyLoconet.SetProgMode( false );
		}
	}
}


//**********************************************************************
//	notifyLNCVread
//----------------------------------------------------------------------
//
int8_t notifyLNCVread( uint16_t ArtNr, uint16_t Address, uint16_t &Value )
{
	int8_t retval = -1;		//	default: ignore request

	if(	g_clMyLoconet.IsProgMode() && (g_uiArticleNumber == ArtNr) )
	{
		if( g_clLncvStorage.IsValidLNCVAddress( Address ) )
		{
			Value	= g_clLncvStorage.ReadLNCV( Address );
			retval	= LNCV_LACK_OK;
		}
		else
		{
			retval = LNCV_LACK_ERROR_UNSUPPORTED;
		}
	}

	return( retval );
}


//**********************************************************************
//	notifyLNCVwrite
//----------------------------------------------------------------------
//	write the 'Value' into EEPROM, but only if the 'Address' is
//	NOT the article number address and NOT the version address
//
int8_t notifyLNCVwrite( uint16_t ArtNr, uint16_t Address, uint16_t Value )
{
	int8_t retval = -1;		//	default: ignore request

	if(	g_clMyLoconet.IsProgMode() && (g_uiArticleNumber == ArtNr) )
	{
		if( g_clLncvStorage.IsValidLNCVAddress( Address ) )
		{
			if(		(LNCV_ADR_VERSION		 != Address)
				&&	(LNCV_ADR_ARTIKEL_NUMMER != Address) )
			{
				if( LNCV_ADR_MODULE_ADDRESS == Address )
				{
					//-----------------------------------------
					//	make sure that the module address
					//	 -	is not '0'
					//	 -	and not '0xFFFF' (broadcast)
					//
					if( (0 < Value) && (0xFFFF > Value) )
					{
						g_clLncvStorage.WriteLNCV( Address, Value );

						g_uiModuleAddress = Value;
					}
				}
				else
				{
					g_clLncvStorage.WriteLNCV( Address, Value );
				}
			}

			retval = LNCV_LACK_OK;
		}
		else
		{
			retval = LNCV_LACK_ERROR_UNSUPPORTED;
		}
	}

	return( retval );
}
