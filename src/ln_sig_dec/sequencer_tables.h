
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
//#		LNCV	44	43	42	41	40	39	38	37	36	35	34	33	32	31
//#
//#	The sequences for the main signal will cover the LEDs B to G
//#		where LED B is bit 0 in the sequence table
//#
//#	The sequences for the approach signal will cover the LEDs J to M
//#		where LED J is bit 0 in the sequence table
//#
//#	The format of a control word is GGLLLLLL where
//#		GG		are the two bits to build four control word groups
//#		LLLLLL	are the bits to define which LED should be switched
//#				or which command should be performed
//#	The four groups are:
//#		00	normal switching of LEDs
//#		01	flashing of LEDs
//#		10	not used
//#		11	control codes, like SEQUENCE_APPROACH_ON or SEQUENCE_END
//#
//#	The first value in each sequence is the first change of
//#		a signal aspect.
//#	Each sequence will end with SEQUENCE_END.
//#	During main signal sequences the approach signal can be switched
//#		ON / OFF (SEQUENCE_APPROACH_ON, SEQUENCE_APPROACH_OFF)
//#
//#-------------------------------------------------------------------------
//#
//#									 LNCVs
//#		Signal			main signal			approach signal
//#		Aspects			35	34	33	32		43	42	41	40
//#	 0	Hp0				 0	 0	 0	 0		 0	 0	 0	 0
//#	 1	Hp1				 0	 0	 0	 1		 0	 0	 0	 1
//#	 2	Hp2				 0	 0	 1	 0		 0	 0	 1	 0
//#	 3	Sh1				 0	 0	 1	 1		 -	 -	 -	 -
//#	 4	Kennlicht		 0	 1	 0	 0		 -	 -	 -	 -
//#
//#	if the signal is configured to react on signal aspects then
//#	the LNCVs 32 to 35 are used to build the aspect number for
//#	the main signal.
//#	LNCVs 40 to 43 are used to build the aspect number for
//#	the approach signal
//#
//#-------------------------------------------------------------------------
//#
//#	File Version:	6		from: 17.01.2023
//#
//#	Implementation:
//#		-	create sequences for E-Sig and A-Sig
//#		-	add some new control words for PAUSE etc.
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

#define SIGNAL_ASPECTS				5

//----------------------------------------------------------------------
//	signal aspects
//
#define SIGNAL_ASPECT_HP0			0
#define SIGNAL_ASPECT_HP1			1
#define SIGNAL_ASPECT_HP2			2
#define SIGNAL_ASPECT_SH1			3
#define SIGNAL_ASPECT_KENNLICHT		4

//----------------------------------------------------------------------
//	definitions for the parts in a sequence control word
//
#define SEQUENCE_CONTROL_GROUPS		0xC0
#define	SEQUENCE_LEDS				0x3F

//----------------------------------------------------------------------
//	definitions for sequencer control groups
//
#define SG_FADING_LEDS				0x00
#define SG_FLASHING_LEDS			0x40
#define SG_UNUSED					0x80
#define SG_CONTROL_WORDS			0xC0

//----------------------------------------------------------------------
//	definitions for sequence control codes
//
#define SEQUENCE_END					0xFF
#define SEQUENCE_PAUSE					0xCF
#define SEQUENCE_APPROACH_ON			0xC0
#define SEQUENCE_APPROACH_OFF			0xC1
#define SEQUENCE_SET_SIGNAL_ASPECT		0xC2
#define SEQUENCE_SET_HP0				0xC3
#define SEQUENCE_ADDITION_SIGNAL_ON		0xC4
#define SEQUENCE_ADDITION_SIGNAL_OFF	0xC5


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
const uint8_t Main_A_to_Hp1[] PROGMEM =
{
	SEQUENCE_APPROACH_ON,
	0b00000010,
	0b00000000,
	SEQUENCE_PAUSE,
	0b01010000,
	0b01000000,
	0b00100000,
	0b00100001,
	0b00000001,
	SEQUENCE_PAUSE,
	SEQUENCE_SET_SIGNAL_ASPECT,
	SEQUENCE_END
};

const uint8_t Main_E_to_Hp1[] PROGMEM =
{
	SEQUENCE_APPROACH_ON,
	0b00000000,
	SEQUENCE_PAUSE,
	0b00000100,
	0b00000000,
	SEQUENCE_PAUSE,
	0b00100000,
	0b00100001,
	0b00000001,
	SEQUENCE_PAUSE,
	SEQUENCE_SET_SIGNAL_ASPECT,
	SEQUENCE_END
};

//------------------------------------------------------------------
//	Main: from Hp0 to Hp2
//
const uint8_t Main_A_to_Hp2[] PROGMEM =
{
	SEQUENCE_APPROACH_ON,
	0b00000010,
	0b00000000,
	SEQUENCE_PAUSE,
	0b01010000,
	0b01000000,
	0b00100000,
	0b00100001,
	SEQUENCE_PAUSE,
	SEQUENCE_SET_SIGNAL_ASPECT,
	SEQUENCE_END
};

const uint8_t Main_E_to_Hp2[] PROGMEM =
{
	SEQUENCE_APPROACH_ON,
	0b00000000,
	SEQUENCE_PAUSE,
	0b00000100,
	0b00000000,
	SEQUENCE_PAUSE,
	0b00100000,
	0b00100001,
	SEQUENCE_PAUSE,
	SEQUENCE_SET_SIGNAL_ASPECT,
	SEQUENCE_END
};

//------------------------------------------------------------------
//	Main: from Hp0 to Sh1
//
const uint8_t Main_A_to_Sh1[] PROGMEM =
{
	0b00000010,
	SEQUENCE_PAUSE,
	0b00010010,
	SEQUENCE_END
};

//------------------------------------------------------------------
//	Main: from Hp0 to Kennlicht
//
const uint8_t Main_A_to_KL[] PROGMEM =
{
	SEQUENCE_APPROACH_ON,
	0b00000010,
	0b00000000,
	SEQUENCE_PAUSE,
	0b00001000,
	SEQUENCE_PAUSE,
	SEQUENCE_SET_SIGNAL_ASPECT,
	SEQUENCE_END
};

const uint8_t Main_E_to_KL[] PROGMEM =
{
	SEQUENCE_APPROACH_ON,
	0b00000000,
	SEQUENCE_PAUSE,
	0b00000100,
	0b00000000,
	SEQUENCE_PAUSE,
	0b00001000,
	SEQUENCE_PAUSE,
	SEQUENCE_SET_SIGNAL_ASPECT,
	SEQUENCE_END
};



//------------------------------------------------------------------
//	Main: from Hp1 to Hp0
//
const uint8_t Main_A_from_Hp1[] PROGMEM =
{
	SEQUENCE_SET_HP0,
	SEQUENCE_PAUSE,
	0b00000000,
	SEQUENCE_PAUSE,
	0b00000010,
	0b00000110,
	SEQUENCE_APPROACH_OFF,
	SEQUENCE_END
};

const uint8_t Main_E_from_Hp1[] PROGMEM =
{
	SEQUENCE_SET_HP0,
	SEQUENCE_PAUSE,
	0b00000000,
	SEQUENCE_PAUSE,
	0b00000100,
	0b00000000,
	SEQUENCE_PAUSE,
	0b00000010,
	SEQUENCE_APPROACH_OFF,
	SEQUENCE_END
};

//------------------------------------------------------------------
//	Main: from Hp2 to Hp0
//
const uint8_t Main_A_from_Hp2[] PROGMEM =
{
	SEQUENCE_SET_HP0,
	SEQUENCE_PAUSE,
	0b00100000,
	0b00000000,
	SEQUENCE_PAUSE,
	0b00000010,
	0b00000110,
	SEQUENCE_APPROACH_OFF,
	SEQUENCE_END
};

const uint8_t Main_E_from_Hp2[] PROGMEM =
{
	SEQUENCE_SET_HP0,
	SEQUENCE_PAUSE,
	0b00100000,
	0b00000000,
	SEQUENCE_PAUSE,
	0b00000100,
	0b00000000,
	SEQUENCE_PAUSE,
	0b00000010,
	SEQUENCE_APPROACH_OFF,
	SEQUENCE_END
};

//------------------------------------------------------------------
//	Main: from Sh1 to Hp0
//
const uint8_t Main_A_from_Sh1[] PROGMEM =
{
	0b00000010,
	SEQUENCE_PAUSE,
	0b00000110,
	SEQUENCE_END
};

//------------------------------------------------------------------
//	Main: from KL to Hp0
//
const uint8_t Main_A_from_KL[] PROGMEM =
{
	SEQUENCE_SET_HP0,
	SEQUENCE_PAUSE,
	0b00000000,
	SEQUENCE_PAUSE,
	0b00000010,
	0b00000110,
	SEQUENCE_APPROACH_OFF,
	SEQUENCE_END
};

const uint8_t Main_E_from_KL[] PROGMEM =
{
	SEQUENCE_SET_HP0,
	SEQUENCE_PAUSE,
	0b00000000,
	SEQUENCE_PAUSE,
	0b00000100,
	0b00000000,
	SEQUENCE_PAUSE,
	0b00000010,
	SEQUENCE_APPROACH_OFF,
	SEQUENCE_END
};


//------------------------------------------------------------------
//------------------------------------------------------------------
//	Approach: from Hp0 to Hp1
//
const uint8_t Appr_to_Hp1[] PROGMEM =
{
	0b00001000,
	SEQUENCE_PAUSE,
	0b00001001,
	SEQUENCE_PAUSE,
	0b00000001,
	SEQUENCE_PAUSE,
	0b00000101,
	SEQUENCE_END
};

//------------------------------------------------------------------
//	Approach: from Hp0 to Hp2
//
const uint8_t Appr_to_Hp2[] PROGMEM =
{
	0b00001000,
	SEQUENCE_PAUSE,
	0b00001001,
	SEQUENCE_END
};

//------------------------------------------------------------------
//	Approach: from xxx to Hp0
//
const uint8_t Appr_to_Hp0[] PROGMEM =
{
	0b00000001,
	0b00000000,
	SEQUENCE_PAUSE,
	0b00001000,
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
	Main_A_to_Hp1,
	Main_A_to_Hp2,
	Main_A_to_Sh1,
	Main_A_to_KL
};

//------------------------------------------------------------------
//	A-Sig to Hp0
//
const uint8_t* const A_Sig_Main_To_Hp0[] PROGMEM =
{
	sNOP,
	Main_A_from_Hp1,
	Main_A_from_Hp2,
	Main_A_from_Sh1,
	Main_A_from_KL
};

//------------------------------------------------------------------
//	E-Sig from Hp0
//
const uint8_t* const E_Sig_Main_From_Hp0[] PROGMEM =
{
	sNOP,
	Main_E_to_Hp1,
	Main_E_to_Hp2,
	sNOP,
	Main_E_to_KL
};

//------------------------------------------------------------------
//	E-Sig to Hp0
//
const uint8_t* const E_Sig_Main_To_Hp0[] PROGMEM =
{
	sNOP,
	Main_E_from_Hp1,
	Main_E_from_Hp2,
	sNOP,
	Main_E_from_KL
};


//------------------------------------------------------------------
//	Approach Signal from Hp0
//
const uint8_t* const Approach_From_Hp0[] PROGMEM =
{
	sNOP,
	Appr_to_Hp1,
	Appr_to_Hp2,
	sNOP,
	sNOP
};

//------------------------------------------------------------------
//	Approach Signal to Hp0
//
const uint8_t* const Approach_To_Hp0[] PROGMEM =
{
	sNOP,
	Appr_to_Hp0,
	Appr_to_Hp0,
	sNOP,
	sNOP
} ;
