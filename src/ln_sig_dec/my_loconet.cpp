//##########################################################################
//#
//#		MyLoconetClass
//#
//#	This class will handle everything that has something to do with
//#	Loconet messages.
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

#define SIGNAL_SELECTED_MAIN		1
#define SIGNAL_SELECTED_APPROACH	2

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
	uint8_t		usHelpSelect;
	uint8_t		usBitNo;
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
				if( pDevice->m_bFlags & DEVICE_IS_INVERT )
				{
					bIsGreen = !bIsGreen;
				}

				//--------------------------------------------------
				//	proces message for ...
				//
				if( NUMBER_LIGHTS > idx )
				{
					if(		(SIGNAL_TYPE_59_A_SIG > m_usSignalType)
						||	(IDX_ADR_LED_A == idx)
						||	(IDX_ADR_LED_H == idx)
						||	(IDX_ADR_LED_I == idx)
						||	(IDX_ADR_LED_N == idx)		)
					{
						//------------------------------------------
						//	... a direct controlled light device
						//
						g_clControl.HandleLed( idx, bIsGreen );
					}
					else
					{
						//------------------------------------------
						//	... a signal aspect
						//
						//	the signal aspect is for ...
						//
						if( IDX_SIGNAL_APPROACH_BIT_0 > idx )
						{
							//--------------------------------------
							//	... a main signal
							//
							usHelpSelect	= SIGNAL_SELECTED_MAIN;
							usBitNo			= idx - IDX_SIGNAL_MAIN_BIT_0;
						}
						else
						{
							//--------------------------------------
							//	...	an approach signal
							//
							usHelpSelect	= SIGNAL_SELECTED_APPROACH;
							usBitNo			= idx - IDX_SIGNAL_APPROACH_BIT_0;
						}

						if( 0 < m_usSignalSelected )
						{
							//--------------------------------------
							//	there was at least one message
							//	for a signal aspect
							//	so check if this message is for
							//	the same signal aspect
							//
							if( m_usSignalSelected != usHelpSelect )
							{
								//----------------------------------
								//	NO !!
								//	so set the actual signal aspect
								//	and start to collect a new one
								//
								SetSelectedSignalAspect();
							}
						}

						if( bIsGreen )
						{
							//--------------------------------------
							//	set the bit for the signal aspect
							//
							m_usSignalAspect |= (1 << usBitNo);
						}

						m_usSignalSelected	= usHelpSelect;
						m_ulTimer			= millis() + m_uiSignalValidWaitTime;

						g_clControl.SwitchLed( LED_GREEN, true );
					}
				}
				else
				{
					//----------------------------------------------
					//	... other device
					//
					if( IDX_TRACK_CONTACT == idx )
					{
						//
						//	for the moment do nothing
						//
					}
					else if( IDX_NIGHTLIGHT == idx )
					{
						g_clControl.SetNightLight( bIsGreen );
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
	uint16_t	helper	= uiInfo;
	uint16_t	address	= helper / 10;

	helper -= (address * 10);

	m_arDevices[ idx ].m_uiAddress	= address;
	m_arDevices[ idx ].m_bFlags		= (uint8_t)helper;
}


//**********************************************************************
//	SetSelectedSignalAspect
//----------------------------------------------------------------------
//
void MyLoconetClass::SetSelectedSignalAspect( void )
{
	if( SIGNAL_SELECTED_MAIN == m_usSignalSelected )
	{
		g_clControl.HandleSignalAspectMain( m_usSignalAspect );
	}
	else
	{
		g_clControl.HandleSignalAspectApproach( m_usSignalAspect );
	}

	g_clControl.SwitchLed( LED_GREEN, false );

	m_usSignalSelected	= 0;
	m_usSignalAspect	= 0;
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
//	notifySwitchReport
//----------------------------------------------------------------------
//
void notifySwitchReport( uint16_t Address, uint8_t Output, uint8_t Direction )
{
	g_clMyLoconet.LoconetReceived( false, Address, Direction, Output );
}


//**********************************************************************
//	notifySwitchState
//----------------------------------------------------------------------
//
void notifySwitchState( uint16_t Address, uint8_t Output, uint8_t Direction )
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

	g_clMyLoconet.SetProgMode( true );

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
			g_clMyLoconet.SetProgMode( true );

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
int8_t notifyLNCVread( uint16_t ArtNr, uint16_t Address, uint16_t, uint16_t &Value )
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
			if(		(LNCV_ADR_VERSION != Address)
				&&	(LNCV_ADR_ARTIKEL_NUMMER != Address) )
			{
				g_clLncvStorage.WriteLNCV( Address, Value );
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
