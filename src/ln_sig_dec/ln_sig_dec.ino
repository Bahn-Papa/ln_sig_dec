//##########################################################################
//#
//#	ln_sig_dec		copyright: Michael Pfeil
//#
//#	This program will control a light signal which consists of LEDs.
//#	The LEDs will be dimmed when lighting up or down.
//#
//##########################################################################

//----------------------------------------------------------------------
//	The main version is defined by PLATINE_VERSION (compile_options.h)
//
//#define VERSION_MAIN		PLATINE_VERSION
#define	VERSION_MINOR		0
#define VERSION_BUGFIX		1


//##########################################################################
//#
//#		Version History:
//#
//#-------------------------------------------------------------------------
//#
//#	Version:	x.00.01		vom: 18.03.2022
//#
//#	Implementation:
//#		-	first test version
//#
//##########################################################################


//==========================================================================
//
//		I N C L U D E S
//
//==========================================================================

#include "compile_options.h"

#include "io_control.h"


//**************************************************************************
//	setup
//--------------------------------------------------------------------------
//
void setup()
{
	g_clControl.Init();
}


//**************************************************************************
//	loop
//--------------------------------------------------------------------------
//
void loop()
{
	uint8_t		usMask = 0x01;
	
	for( uint8_t idx = 0 ; idx < 14 ; idx++ )
	{
		g_clControl.FadeLeds( usMask, 255 );
		
		delay( 1000 );
		
		g_clControl.FadeLeds( usMask, 0 );
		
		delay( 1000 );
		
		usMask <<= 1;
	}
}
