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
void LncvStorageClass::CheckEEPROM( void )
{
	uint8_t		idx;
	uint16_t	uiCheckValue = eeprom_read_word( (uint16_t *)0 );

#ifdef DEBUGGING_PRINTOUT
	g_clDebugging.PrintStorageCheck( byte0, byte1 );
#endif

//	if( (0xFFFF == uiCheckValue) || (0x0000 == uiCheckValue) )
	{
		//----------------------------------------------------------
		//	the EEPROM is empty, so write default config info ...
		//

#ifdef DEBUGGING_PRINTOUT
		g_clDebugging.PrintStorageDefault();
#endif

		WriteLNCV( LNCV_ADR_MODULE_ADDRESS,	100 );
		WriteLNCV( LNCV_ADR_ARTIKEL_NUMMER,	ARTIKEL_NUMMER );
		WriteLNCV( LNCV_ADR_SIGNAL_TYPE, SIGNAL_TYPE_DIRECT_CONTROL_MIDDLE );
		WriteLNCV( LNCV_ADR_SIGNAL_VALID_WAIT_TIME, 5000 );

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
		WriteLNCV( LNCV_ADR_TRACK_CONTACT,	0 );
		WriteLNCV( LNCV_ADR_NIGHTLIGHT,		115 );

		for( idx = 0 ; idx < NUMBER_LIGHTS ; idx++ )
		{
			WriteLNCV( LNCV_ADR_LIGHT_INTENSITY_LED_A + idx, 0xFF7F );
			WriteLNCV( LNCV_ADR_LIGHT_A + idx, 101 + idx );
		}

		//----------------------------------------------------------
		//	Clear all spare LNCVs
		//
		for( idx = LNCV_ADR_SPARE ; idx < LNCV_ADR_LIGHT_INTENSITY_LED_A ; idx++ )
		{
			WriteLNCV( idx, 0 );
		}
	}

	delay( 1000 );
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
	uint16_t	helper;
	uint8_t		idx;


#ifdef DEBUGGING_PRINTOUT
	g_clDebugging.PrintStorageRead();
#endif

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
	if( (LNCV_ADR_MODULE_ADDRESS <= Adresse) && (LNCV_ADR_LIGHT_N >= Adresse) )
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
//
void LncvStorageClass::WriteLNCV( uint16_t Adresse, uint16_t Value )
{
	//--------------------------------------------------------------
	//	because of uint16 values the address has to be shifted
	//	by '1' (this will double the address).
	//
	eeprom_write_word( (uint16_t *)(Adresse << 1), Value );
}
