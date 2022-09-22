
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
//#	Signal Aspects
//#	 0	Hp0
//#	 1	Hp1
//#	 2	Hp2
//#	 3	Sh1
//#	 4	Hp1 mit 1 Zusatzanzeiger
//#	 5	Hp2 mit 1 Zusatzanzeiger
//#	 6	Kennlicht
//#	 7	Ersatzsignal
//#	 8	Hp1 mit 2 Zusatzanzeiger	(not implemented yet)
//#	 9	Hp1 mit 3 Zusatzanzeiger	(not implemented yet)
//#	10	Hp2 mit 2 Zusatzanzeiger	(not implemented yet)
//#	11	Hp2 mit 3 Zusatzanzeiger	(not implemented yet)
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

#define SIGNAL_ASPECTS_MAIN			8
#define SIGNAL_ASPECTS_APPROACH		0

#define SEQUENCE_END				0xFFFF


//==========================================================================
//
//		C O N S T A N T S
//
//==========================================================================

//**********************************************************************
//	The no operation (NOP) sequence
//
const uint16_t sNOP[] PROGMEM = { SEQUENCE_END };


//**********************************************************************
//	Signal Type '59': the different sequences
//

//------------------------------------------------------------------
//	Main: from Hp0 to Hp1
//
const uint16_t s59_Hp0_Hp1[] PROGMEM =
{
	0b0000000000000000,
	0b0000000000000010,
	SEQUENCE_END
};

//------------------------------------------------------------------
//	Main: from Hp0 to Hp2
//
const uint16_t s59_Hp0_Hp2[] PROGMEM =
{
	0b0000000000000000,
	0b0000000001000010,
	SEQUENCE_END
};

//------------------------------------------------------------------
//	Main: from Hp0 to Sh1
//
const uint16_t s59_Hp0_Sh1[] PROGMEM =
{
	0b0000000000000100,
	0b0000000000100100,
	SEQUENCE_END
};

//------------------------------------------------------------------
//	Main: from Hp0 to Hp1 including 1 Zusatzanzeiger
//
const uint16_t s59_Hp0_Hp1_ZS3[] PROGMEM =
{
	0b0000000000001101,
	0b0000000000000001,
	0b0000000000000011,
	SEQUENCE_END
};

//------------------------------------------------------------------
//	Main: from Hp0 to Hp2 including 1 Zusatzanzeiger
//
const uint16_t s59_Hp0_Hp2_ZS3[] PROGMEM =
{
	0b0000000000001101,
	0b0000000000000001,
	0b0000000001000011,
	SEQUENCE_END
};

//------------------------------------------------------------------
//	Main: from Hp0 to Kennlicht
//
const uint16_t s59_Hp0_KL[] PROGMEM =
{
	0b0000000000000000,
	0b0000000000010000,
	SEQUENCE_END
};

//------------------------------------------------------------------
//	Main: from Hp0 to Ersatzsignal
//
const uint16_t s59_Hp0_ZS1[] PROGMEM =
{
	0b0000000000000100,
	0b0000000010000100,
	SEQUENCE_END
};

//------------------------------------------------------------------
//	Main: from xxx to Hp0
//
const uint16_t s59_to_Hp0[] PROGMEM =
{
	0b0000000000000000,
	0b0000000000001100,
	SEQUENCE_END
};

//------------------------------------------------------------------
//	Main: from 1 Zusatzanzeiger to Hp0
//
const uint16_t s59_ZS3_Hp0[] PROGMEM =
{
	0b0000000000000001,
	0b0000000000001101,
	0b0000000000001100,
	SEQUENCE_END
};

//------------------------------------------------------------------
//	Main: from Sh1 to Hp0
//
const uint16_t s59_Sh1_Hp0[] PROGMEM =
{
	0b0000000000000100,
	0b0000000000001100,
	SEQUENCE_END
};

//------------------------------------------------------------------
//	Main: from Hp0 to Ersatzsignal
//
const uint16_t s59_ZS1_Hp0[] PROGMEM =
{
	0b0000000000000100,
	0b0000000000001100,
	SEQUENCE_END
};


//**********************************************************************
//	Signal Type '59': the sequence matrix
//
const uint16_t* const g_arS59Main[ SIGNAL_ASPECTS_MAIN ][ SIGNAL_ASPECTS_MAIN ] PROGMEM =
{
	{ sNOP,			s59_Hp0_Hp1,	s59_Hp0_Hp2,	s59_Hp0_Sh1,	s59_Hp0_Hp1_ZS3,	s59_Hp0_Hp2_ZS3,	s59_Hp0_KL,	s59_Hp0_ZS1	},
	{ s59_to_Hp0,	sNOP,			sNOP,			sNOP,			sNOP,				sNOP,				sNOP,		sNOP		},
	{ s59_to_Hp0,	sNOP,			sNOP,			sNOP,			sNOP,				sNOP,				sNOP,		sNOP		},
	{ s59_Sh1_Hp0,	sNOP,			sNOP,			sNOP,			sNOP,				sNOP,				sNOP,		sNOP		},
	{ s59_ZS3_Hp0,	sNOP,			sNOP,			sNOP,			sNOP,				sNOP,				sNOP,		sNOP		},
	{ s59_ZS3_Hp0,	sNOP,			sNOP,			sNOP,			sNOP,				sNOP,				sNOP,		sNOP		},
	{ s59_to_Hp0,	sNOP,			sNOP,			sNOP,			sNOP,				sNOP,				sNOP,		sNOP		},
	{ s59_ZS1_Hp0,	sNOP,			sNOP,			sNOP,			sNOP,				sNOP,				sNOP,		sNOP		},
};
