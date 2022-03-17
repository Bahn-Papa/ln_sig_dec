//##########################################################################
//#
//#	LocoNet2Block		copyright: Michael Pfeil
//#
//#	Dieses Programm ist für die Hardware Loconet2Block geschrieben.
//# Hier wird die Funktionalität des Streckenblocks abgebildet.
//#	Als Vorlage dienten die State-Modelle von Stefan Bormann.
//#
//##########################################################################

//----------------------------------------------------------------------
//	The main version is defined by PLATINE_VERSION (compile_options.h)
//
//#define VERSION_MAIN		PLATINE_VERSION
#define	VERSION_MINOR		1
#define VERSION_BUGFIX		0


//##########################################################################
//#
//#		Version History:
//#
//#-------------------------------------------------------------------------
//#
//#	Version:	x.01.00		vom: 26.02.2022
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

//#include "compile_options.h"

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
