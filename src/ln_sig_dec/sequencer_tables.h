
#pragma once

//##########################################################################
//#
//#		sequencer tables
//#
//#	Here are all sequences defined that describe the transitions
//# from one signal aspect to another.
//#
//#	There are two sets of sequences:
//#		-		main signal aspect sequences
//#		-	approach signal aspect sequences
//#
//#-------------------------------------------------------------------------
//#
//#	Each bit of a sequence value represents one LED of the signal.
//#	If a bit is set that LED is ON.
//#														re	li
//#		LED		ws	ge	gr	ge	gr	KlZ	ES	ge	ws	KL	rt	rt	gr	ZA
//#				 N	 M	 L	 K	 J	 I	 H	 G	 F	 E	 D	 C	 B	 A
//#		Bit		13	12	11	10	 9	 8	 7	 6	 5	 4	 3	 2	 1	 0
//#
//#	The sequences for the main signal will cover the LEDs B to G
//#		where LED B is bit 0 in the sequence table
//#
//#	The sequences for the approach signal will cover the LEDs J to M
//#		where LED J is bit 0 in the sequence table
//#
//#	The first value in each sequence is the first change of
//#		a signal aspect.
//#	Each sequence will end with SEQUENCE_END.
//#	During main signal sequences the approach signal can be switched
//#		ON / OFF (SEQUENCE_APPROACH_ON, SEQUENCE_APPROACH_OFF)
//#
//#-------------------------------------------------------------------------
//#
//#	Signal Aspects
//#	 0	Hp0
//#	 1	Hp1
//#	 2	Hp2
//#	 3	Sh1
//#	 4	Kennlicht
//#
//#-------------------------------------------------------------------------
//#
//#	File Version:	5		from: 21.11.2022
//#
//#	Implementation:
//#		-	split into two sets of sequences
//#					main signal sequences
//#				approach signal sequences
//#		-	create general sequences
//#		-	remove special sequences for signal type 59
//#
//#-------------------------------------------------------------------------
//#
//#	File Version:	4		from: 12.10.2022
//#
//#	Implementation:
//#		-	rename sequences to make it more clear what will happen
//#		-	add approach signal sequences
//#
//#-------------------------------------------------------------------------
//#
//#	File Version:	3		from: 11.10.2022
//#
//#	Implementation:
//#		-	add sequencer tables for signal type 59 E-Sig
//#
//#	Bug Fix:
//#		-	sequences so far are for A-Sig not for E-Sig
//#			so rename all tables accordingly
//#
//#-------------------------------------------------------------------------
//#
//#	File Version:	2		from: 23.09.2022
//#
//#	Implementation:
//#		-	simplify sequencer tables
//#			rename all tables, but sNOP
//#			delete table g_arS59Main
//#
//#-------------------------------------------------------------------------
//#
//#	File Version:	1		from: 22.09.2022
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

#define SIGNAL_ASPECTS			5

//----------------------------------------------------------------------
//	signal aspects
//
#define SIGNAL_ASPECT_HP0			0
#define SIGNAL_ASPECT_HP1			1
#define SIGNAL_ASPECT_HP2			2
#define SIGNAL_ASPECT_SH1			3
#define SIGNAL_ASPECT_KENNLICHT		4

//----------------------------------------------------------------------
//	definitions for sequence control
//
#define SEQUENCE_END				0xFF
#define SEQUENCE_APPROACH_ON		0xEF
#define SEQUENCE_APPROACH_OFF		0xDF


//==========================================================================
//
//		C O N S T A N T S
//
//==========================================================================

//**********************************************************************
//	The no operation (NOP) sequence
//
const uint8_t sNOP[] PROGMEM = { SEQUENCE_END };


//**********************************************************************
//	general signal aspect sequences
//
//	there are two sets of sequences
//		one set for the main signal and
//		one set for the approach signal
//	if there are different sequences needed for a specific signal type
//	the tables are behind this section
//

//------------------------------------------------------------------
//	Main: from Hp0 to Hp1
//
const uint8_t Main_to_Hp1[] PROGMEM =
{
	SEQUENCE_APPROACH_ON,
	0b00000000,
	0b00000001,
	SEQUENCE_END
};

//------------------------------------------------------------------
//	Main: from Hp0 to Hp2
//
const uint8_t Main_to_Hp2[] PROGMEM =
{
	SEQUENCE_APPROACH_ON,
	0b00000000,
	0b00100001,
	SEQUENCE_END
};

//------------------------------------------------------------------
//	Main: from Hp0 to Sh1
//
const uint8_t Main_A_to_Sh1[] PROGMEM =
{
	0b00000010,
	0b00010010,
	SEQUENCE_END
};

const uint8_t Main_E_to_Sh1[] PROGMEM =
{
	0b00000000,
	0b00010100,
	SEQUENCE_END
};

//------------------------------------------------------------------
//	Main: from Hp0 to Kennlicht
//
const uint8_t Main_to_KL[] PROGMEM =
{
	0b00000000,
	0b00001000,
	SEQUENCE_END
};



//------------------------------------------------------------------
//	Main: from xxx to Hp0
//
const uint8_t Main_A_to_Hp0[] PROGMEM =
{
	0b00000000,
	0b00000110,
	SEQUENCE_APPROACH_OFF,
	SEQUENCE_END
};

const uint8_t Main_E_to_Hp0[] PROGMEM =
{
	0b00000000,
	0b00000010,
	SEQUENCE_APPROACH_OFF,
	SEQUENCE_END
};

//------------------------------------------------------------------
//	Main: from Sh1 or ZS1 to Hp0
//
const uint8_t Main_A_from_Sh1[] PROGMEM =
{
	0b00000010,
	0b00000110,
	SEQUENCE_END
};


//------------------------------------------------------------------
//------------------------------------------------------------------
//	Approach: from Hp0 to Hp1
//
const uint8_t Appr_to_Hp1[] PROGMEM =
{
	0b00000000,
	0b00000101,
	SEQUENCE_END
};

//------------------------------------------------------------------
//	Approach: from Hp0 to Hp2
//
const uint8_t Appr_to_Hp2[] PROGMEM =
{
	0b00000000,
	0b00001001,
	SEQUENCE_END
};

//------------------------------------------------------------------
//	Approach: from xxx to Hp0
//
const uint8_t Appr_to_Hp0[] PROGMEM =
{
	0b00000000,
	0b00001010,
	SEQUENCE_END
};


//**********************************************************************
//	General Signal Type
//

//------------------------------------------------------------------
//	A-Sig from Hp0
//
const uint8_t* const A_Sig_Main_From_Hp0[] PROGMEM =
{
	sNOP,
	Main_to_Hp1,
	Main_to_Hp2,
	Main_A_to_Sh1,
	Main_to_KL
};

//------------------------------------------------------------------
//	A-Sig to Hp0
//
const uint8_t* const A_Sig_Main_To_Hp0[] PROGMEM =
{
	sNOP,
	Main_A_to_Hp0,
	Main_A_to_Hp0,
	Main_A_from_Sh1,
	Main_A_to_Hp0
};

//------------------------------------------------------------------
//	E-Sig from Hp0
//
const uint8_t* const E_Sig_Main_From_Hp0[] PROGMEM =
{
	sNOP,
	Main_to_Hp1,
	Main_to_Hp2,
	Main_E_to_Sh1,
	Main_to_KL
} ;

//------------------------------------------------------------------
//	E-Sig to Hp0
//
const uint8_t* const E_Sig_Main_To_Hp0[] PROGMEM =
{
	sNOP,
	Main_E_to_Hp0,
	Main_E_to_Hp0,
	Main_E_to_Hp0,
	Main_E_to_Hp0
};


//------------------------------------------------------------------
//	Approach Signal from Hp0
//
const uint8_t* const Approach_From_Hp0[] PROGMEM =
{
	sNOP, Appr_to_Hp1, Appr_to_Hp2, sNOP, sNOP
} ;

//------------------------------------------------------------------
//	Approach Signal to Hp0
//
const uint8_t* const Approach_To_Hp0[] PROGMEM =
{
	sNOP, Appr_to_Hp0, Appr_to_Hp0, sNOP, sNOP
} ;
