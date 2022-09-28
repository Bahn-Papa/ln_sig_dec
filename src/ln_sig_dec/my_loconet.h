
#pragma once

//##########################################################################
//#
//#		MyLoconetClass
//#
//#	This class will handle everything that has something to do with
//#	Loconet messages.
//#
//#-------------------------------------------------------------------------
//#
//#	File version:	2		from: 28.09.2022
//#
//#	Implementation:
//#		-	change struct 'device_t' to save some memory space
//#			combine	bool 'm_bSensor' and bool 'm_bInvert'
//#				to	uint8_t 'm_bFlags'
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

#include <stdint.h>

#include "lncv_storage.h"


//==========================================================================
//
//		T Y P E   D E F I N I T I O N S
//
//==========================================================================

typedef struct
{
	uint16_t	m_uiAddress;
	uint8_t		m_bFlags;

} device_t;


//==========================================================================
//
//		C L A S S   D E F I N I T I O N S
//
//==========================================================================


///////////////////////////////////////////////////////////////////////
//	CLASS:	MyLoconetClass
//
//	This class will handle everything that has something to do with
//	Loconet messages.
//
class MyLoconetClass
{
	public:
		MyLoconetClass();

		void Init( void );
		void CheckForMessageAndProcess( void );
		void LoconetReceived( bool isSensor, uint16_t adr, uint8_t dir, uint8_t output );

		inline void SetProgMode( bool bProgMode )
		{
			m_bIsProgMode = bProgMode;
		};

		inline bool IsProgMode( void )
		{
			return( m_bIsProgMode );
		};


	private:
		device_t	m_arDevices[ DEVICE_COUNT ];
		uint16_t	m_uiSignalValidWaitTime;
		uint32_t	m_ulTimer;
		uint8_t		m_usSignalType;
		uint8_t		m_usSignalSelected;
		uint8_t		m_usSignalAspect;
		bool		m_bIsProgMode;

		void SetDeviceInfo( uint8_t idx, uint16_t uiInfo );
		void SetSelectedSignalAspect( void );
};


//==========================================================================
//
//		E X T E R N   G L O B A L   V A R I A B L E S
//
//==========================================================================

extern MyLoconetClass		g_clMyLoconet;
