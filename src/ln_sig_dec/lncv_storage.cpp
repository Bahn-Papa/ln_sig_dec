//##########################################################################
//#
//#		LncvStorageClass
//#
//#	This class manages the LNCVs (L oco N et C onfiguration V ariables)
//#	These variables will be saved in the EEPROM.
//#
//#	To simplify access, the variables were arranged one after the other
//#	without gaps. This makes it very easy to read and write the variables
//#	with a tool.
//#
//#-------------------------------------------------------------------------
//#
//#	File Version:	3		from: 22.09.2022
//#
//#	Implementation:
//#		-	only write to EEPROM if the new value is different
//#			than the old one
//#
//#	Bug Fix:
//#		-	correct default addresses for test case
//#
//#-------------------------------------------------------------------------
//#
//#	File Version:	2		from: 19.09.2022
//#
//#	Implementation:
//#		-	add version number into EEPROM
//#		-	add define to write test values into EEPROM
//#
//#-------------------------------------------------------------------------
//#
//#	File Version:	1		from: 17.09.2022
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

#include "lncv_storage.h"


//==========================================================================
//
//		D E F I N I T I O N S
//
//==========================================================================

//#define WRITE_TEST_VALUES		1


//==========================================================================
//
//		G L O B A L   V A R I A B L E S
//
//==========================================================================

LncvStorageClass	g_clLncvStorage = LncvStorageClass();


////////////////////////////////////////////////////////////////////////
//	CLASS: LncvStorageClass
//

//**********************************************************************
//	Constructor
//----------------------------------------------------------------------
//
LncvStorageClass::LncvStorageClass()
{
}


//**********************************************************************
//	CheckEEPROM
//----------------------------------------------------------------------
//	This function checks if the EEPROM is empty (0xFF in the cells).
//	If so, then the EEPROM will be filled with default config infos
//	and all addresses will be set to zero.
//
void LncvStorageClass::CheckEEPROM( uint16_t uiVersionNumber )
{
	uint8_t		idx;
	uint16_t	uiVersion;


#ifdef WRITE_TEST_VALUES
	if( true )
#else
	uint16_t	uiCheckValue = eeprom_read_word( (uint16_t *)0 );


	if( (0xFFFF == uiCheckValue) || (0x0000 == uiCheckValue) )
#endif
	{
		//----------------------------------------------------------
		//	the EEPROM is empty, so write default config info ...
		//
		WriteLNCV( LNCV_ADR_ARTIKEL_NUMMER,	ARTIKEL_NUMMER );
		WriteLNCV( LNCV_ADR_VERSION, uiVersionNumber );

#ifdef WRITE_TEST_VALUES
		WriteLNCV( LNCV_ADR_MODULE_ADDRESS,	100 );
		WriteLNCV( LNCV_ADR_SIGNAL_TYPE, SIGNAL_TYPE_59 );
		WriteLNCV( LNCV_ADR_SIGNAL_VALID_WAIT_TIME, 5000 );
#else
		WriteLNCV( LNCV_ADR_MODULE_ADDRESS,	1 );
		WriteLNCV( LNCV_ADR_SIGNAL_TYPE, SIGNAL_TYPE_DIRECT_CONTROL_MIDDLE );
		WriteLNCV( LNCV_ADR_SIGNAL_VALID_WAIT_TIME, 100 );
#endif

		//----------------------------------------------------------
		//	... default fade times ...
		//
		WriteLNCV( LNCV_ADR_FADE_TIME_UP_FAST,		300 );
		WriteLNCV( LNCV_ADR_FADE_TIME_UP_MIDDLE,	500 );
		WriteLNCV( LNCV_ADR_FADE_TIME_UP_SLOW,		800 );
		WriteLNCV( LNCV_ADR_FADE_TIME_DOWN_FAST,	300 );
		WriteLNCV( LNCV_ADR_FADE_TIME_DOWN_MIDDLE,	500 );
		WriteLNCV( LNCV_ADR_FADE_TIME_DOWN_SLOW,	800 );

		//----------------------------------------------------------
		//	... and default light intensities
		//		and address values into EEPROM
		//
#ifdef WRITE_TEST_VALUES
		WriteLNCV( LNCV_ADR_TRACK_CONTACT,	1160 );
		WriteLNCV( LNCV_ADR_NIGHTLIGHT,		1150 );
#else
		WriteLNCV( LNCV_ADR_TRACK_CONTACT,	0 );
		WriteLNCV( LNCV_ADR_NIGHTLIGHT,		0 );
#endif

		for( idx = 0 ; idx < NUMBER_LIGHTS ; idx++ )
		{
#ifdef WRITE_TEST_VALUES
			WriteLNCV( LNCV_ADR_LIGHT_INTENSITY_LED_A + idx, 0xFF7F );
			WriteLNCV( LNCV_ADR_LIGHT_A + idx, 1010 + (10 * idx) );
#else
			WriteLNCV( LNCV_ADR_LIGHT_INTENSITY_LED_A + idx, 0xFF7F );
			WriteLNCV( LNCV_ADR_LIGHT_A + idx, 0 );
#endif
		}

		//----------------------------------------------------------
		//	Clear all spare LNCVs
		//
		for( idx = LNCV_ADR_SPARE ; idx < LNCV_ADR_LIGHT_INTENSITY_LED_A ; idx++ )
		{
			WriteLNCV( idx, 0 );
		}
	}
	else
	{
		uiVersion = ReadLNCV( LNCV_ADR_VERSION );
		
		if( uiVersion != uiVersionNumber )
		{
			WriteLNCV( LNCV_ADR_VERSION, uiVersionNumber );
		}
	}
	delay( 500 );
}

//**********************************************************************
//	Init
//----------------------------------------------------------------------
//	This function will read the saved
//		-	configuration information
//	from the EEPROM.
//
void LncvStorageClass::Init( void )
{
	//--------------------------------------------------------------
	//	read config information
	//
	m_usSignalType = (uint8_t)ReadLNCV( LNCV_ADR_SIGNAL_TYPE );


	delay( 100 );
}


//**********************************************************************
//	GetFadeTimeUp
//----------------------------------------------------------------------
//
uint16_t LncvStorageClass::GetFadeTimeUp( uint8_t idx )
{
	uint16_t	uiFadeTime	= 0;

	if( (FADE_TIME_COUNT > idx) && (0 < idx) )
	{
		idx--;

		uiFadeTime = ReadLNCV( LNCV_ADR_FADE_TIME_UP_FAST + idx );
	}

	return( uiFadeTime );
}


//**********************************************************************
//	GetFadeTimeDown
//----------------------------------------------------------------------
//
uint16_t LncvStorageClass::GetFadeTimeDown( uint8_t idx )
{
	uint16_t	uiFadeTime	= 0;

	if( (FADE_TIME_COUNT > idx) && (0 < idx) )
	{
		idx--;

		uiFadeTime = ReadLNCV( LNCV_ADR_FADE_TIME_DOWN_FAST + idx );
	}

	return( uiFadeTime );
}


//**********************************************************************
//	GetLightIntensityDay
//----------------------------------------------------------------------
//
uint8_t LncvStorageClass::GetLightIntensityDay( uint8_t usLedNo )
{
	uint8_t		usLightIntensity = BRIGHTNESS_MAX;
	uint16_t	uiHelper;

	if( NUMBER_LIGHTS > usLedNo  )
	{
		uiHelper			= ReadLNCV( LNCV_ADR_LIGHT_INTENSITY_LED_A + usLedNo );
		usLightIntensity	= (uint8_t)((uiHelper & MASK_INTENSITIY_DAY) >> 8);
	}
	
	return( usLightIntensity );
}


//**********************************************************************
//	GetLightIntensityNight
//----------------------------------------------------------------------
//
uint8_t LncvStorageClass::GetLightIntensityNight( uint8_t usLedNo )
{
	uint8_t		usLightIntensity = BRIGHTNESS_MAX;
	uint16_t	uiHelper;

	if( NUMBER_LIGHTS > usLedNo  )
	{
		uiHelper			= ReadLNCV( LNCV_ADR_LIGHT_INTENSITY_LED_A + usLedNo );
		usLightIntensity	= (uint8_t)( uiHelper & MASK_INTENSITIY_NIGHT);
	}
	
	return( usLightIntensity );
}


//**********************************************************************
//	IsValidLNCVAdress
//----------------------------------------------------------------------
//
bool LncvStorageClass::IsValidLNCVAddress( uint16_t Adresse )
{
//	if( (LNCV_ADR_MODULE_ADDRESS <= Adresse) && (LNCV_ADR_LIGHT_N >= Adresse) )
	if( LNCV_ADR_LIGHT_N >= Adresse )
	{
		return( true );
	}

	return( false );
}


//**********************************************************************
//	ReadLNCV
//----------------------------------------------------------------------
//
uint16_t LncvStorageClass::ReadLNCV( uint16_t Adresse )
{
	uint16_t	value;

	//--------------------------------------------------------------
	//	because of uint16 values the address has to be shifted
	//	by '1' (this will double the address).
	//
	value = eeprom_read_word( (uint16_t *)(Adresse << 1) );

	return( value );
}


//**********************************************************************
//	WriteLNCV
//----------------------------------------------------------------------
//	The function will write the given 'Value' to the given 'Address'
//	in EEPROM, but only if the new value is different than the old one.
//	This is done, because the EEPROM has a limited number of
//	write cycles.
//
void LncvStorageClass::WriteLNCV( uint16_t Address, uint16_t Value )
{
	//--------------------------------------------------------------
	//	because of uint16 values the address has to be shifted
	//	by '1' (this will double the address).
	//
	uint16_t *	puiAdr	= (uint16_t *)(Address << 1);
	uint16_t	uiValue	= eeprom_read_word( uiAdr );
	
	if( uiValue != Value )
	{
		eeprom_write_word( puiAdr, Value );
	}
}
