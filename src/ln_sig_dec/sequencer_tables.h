
#pragma once

//##########################################################################
//#
//#		sequencer tables
//#
//#	Here are all sequences defined that describe the transitions
//# from one signal aspect to another.
//#
//#-------------------------------------------------------------------------
//#
//#	Each bit of a sequence value represents one LED of the signal.
//#	If a bit is set that LED is ON.
//#
//#		LED									ge	ws	KL	rt	rt	gr	ZA
//#				 N	 M	 L	 K	 J	 I	 H	 G	 F	 E	 D	 C	 B	 A
//#		Bit		13	12	11	10	 9	 8	 7	 6	 5	 4	 3	 2	 1	 0
//#
//#	The first value in each sequence is the first change of
//#	a signal aspect.
//#	Each sequence will end with a value of '0',
//#	because at least one LED of a signal is on.
//#
//#-------------------------------------------------------------------------
//#
//#	File Version:	1		from: 21.09.2022
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

#include <avr/pgmspace.h>


//==========================================================================
//
//		D E F I N I T I O N S
//
//==========================================================================

#define SIGNAL_ASPECTS		3


//==========================================================================
//
//		C O N S T A N T S
//
//==========================================================================

//**********************************************************************
//	The no operation (NOP) sequence
//
const uint16_t sNOP PROGMEM = { 0x0000 };


//**********************************************************************
//	Signal Type '59': the different sequences
//

//------------------------------------------------------------------
//	Main: from Hp0 to Hp1
//
const uint16_t s59_Hp0_Hp1 PROGMEM =
{
	0b0000000000000010,
	0b0000000000000000
};

//------------------------------------------------------------------
//	Main: from Hp0 to Hp1
//
const uint16_t s59_Hp0_Hp2 PROGMEM =
{
	0b0000000001000000,
	0b0000000000000000
};

//------------------------------------------------------------------
//	Main: from Hp1/Hp2 to Hp0
//
const uint16_t s59_Hp1_2_Hp0 PROGMEM =
{
	0b0000000000001100,
	0b0000000000000000
};


//**********************************************************************
//	Signal Type '59': the sequence matrix
//
const uint16_t* const g_arS59Main[ SIGNAL_ASPECTS ][ SIGNAL_ASPECTS ] PROGMEM =
{
	{ sNOP,				s59_Hp0_Hp1,	s59_Hp0_Hp2	},
	{ s59_Hp1_2_Hp0,	sNOP,			sNOP		},
	{ s59_Hp1_2_Hp0,	sNOP,			sNOP		}
};
