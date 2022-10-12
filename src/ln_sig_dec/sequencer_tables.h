
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
//#														re	li
//#		LED		ws	ge	gr	ge	gr	KlZ	ES	ge	ws	KL	rt	rt	gr	ZA
//#				 N	 M	 L	 K	 J	 I	 H	 G	 F	 E	 D	 C	 B	 A
//#		Bit		13	12	11	10	 9	 8	 7	 6	 5	 4	 3	 2	 1	 0
//#
//#	The first value in each sequence is the first change of
//#	a signal aspect.
//#	Each sequence will end with SEQUENCE_END.
//#
//#-------------------------------------------------------------------------
//#
//#	Signal Aspects
//#	 0	Hp0
//#	 1	Hp1
//#	 2	Hp2
//#	 3	Sh1
//#	 4	Hp1 mit 1 Zusatzanzeiger (ZS3)
//#	 5	Hp2 mit 1 Zusatzanzeiger (ZS3)
//#	 6	Kennlicht
//#	 7	Ersatzsignal (ZS1)
//#	 8	Hp1 mit 2 Zusatzanzeiger	(not implemented yet)
//#	 9	Hp1 mit 3 Zusatzanzeiger	(not implemented yet)
//#	10	Hp2 mit 2 Zusatzanzeiger	(not implemented yet)
//#	11	Hp2 mit 3 Zusatzanzeiger	(not implemented yet)
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

#define SIGNAL_ASPECTS			8

//----------------------------------------------------------------------
//	signal aspects
//
#define SIGNAL_ASPECT_HP0			0
#define SIGNAL_ASPECT_HP1			1
#define SIGNAL_ASPECT_HP2			2
#define SIGNAL_ASPECT_SH1			3
#define SIGNAL_ASPECT_HP1_ZS3		4
#define SIGNAL_ASPECT_HP2_ZS3		5
#define SIGNAL_ASPECT_KENNLICHT		6
#define SIGNAL_ASPECT_ZS1			7

//----------------------------------------------------------------------
//	definitions for sequence control
//
#define SEQUENCE_END				0xFFFF
#define SEQUENCE_APPROACH_ON		0xEFFF
#define SEQUENCE_APPROACH_OFF		0xDFFF


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
const uint16_t Main_to_Hp1[] PROGMEM =
{
	SEQUENCE_APPROACH_ON,
	0b0000000000000000,
	0b0000000000000010,
	SEQUENCE_END
};

//------------------------------------------------------------------
//	Main: from Hp0 to Hp2
//
const uint16_t Main_to_Hp2[] PROGMEM =
{
	SEQUENCE_APPROACH_ON,
	0b0000000000000000,
	0b0000000001000010,
	SEQUENCE_END
};

//------------------------------------------------------------------
//	Main: from Hp0 to Sh1
//
const uint16_t Main_to_Sh1[] PROGMEM =
{
	0b0000000000000100,
	0b0000000000100100,
	SEQUENCE_END
};

//------------------------------------------------------------------
//	Main: from Hp0 to Hp1 including ZS3
//
const uint16_t A_Sig_Main_to_Hp1_ZS3[] PROGMEM =
{
	SEQUENCE_APPROACH_ON,
	0b0000000000001101,
	0b0000000000000001,
	0b0000000000000011,
	SEQUENCE_END
};

const uint16_t E_Sig_Main_to_Hp1_ZS3[] PROGMEM =
{
	SEQUENCE_APPROACH_ON,
	0b0000000000000101,
	0b0000000000000001,
	0b0000000000000011,
	SEQUENCE_END
};

//------------------------------------------------------------------
//	Main: from Hp0 to Hp2 including ZS3
//
const uint16_t A_Sig_Main_to_Hp2_ZS3[] PROGMEM =
{
	SEQUENCE_APPROACH_ON,
	0b0000000000001101,
	0b0000000000000001,
	0b0000000001000011,
	SEQUENCE_END
};

const uint16_t E_Sig_Main_to_Hp2_ZS3[] PROGMEM =
{
	SEQUENCE_APPROACH_ON,
	0b0000000000000101,
	0b0000000000000001,
	0b0000000001000011,
	SEQUENCE_END
};

//------------------------------------------------------------------
//	Main: from Hp0 to Kennlicht
//
const uint16_t Main_to_KL[] PROGMEM =
{
	0b0000000000000000,
	0b0000000000010000,
	SEQUENCE_END
};

//------------------------------------------------------------------
//	Main: from Hp0 to Ersatzsignal (ZS1)
//
const uint16_t A_Sig_Main_to_ZS1[] PROGMEM =
{
	0b0000000000000100,
	0b0000000010000100,
	SEQUENCE_END
};

const uint16_t E_Sig_Main_to_ZS1[] PROGMEM =
{
	0b0000000000000000,
	0b0000000010001000,
	SEQUENCE_END
};

//------------------------------------------------------------------
//	Main: from xxx to Hp0
//
const uint16_t A_Sig_Main_to_Hp0[] PROGMEM =
{
	0b0000000000000000,
	0b0000000000001100,
	SEQUENCE_APPROACH_OFF,
	SEQUENCE_END
};

const uint16_t E_Sig_Main_to_Hp0[] PROGMEM =
{
	0b0000000000000000,
	0b0000000000000100,
	SEQUENCE_APPROACH_OFF,
	SEQUENCE_END
};

//------------------------------------------------------------------
//	Main: from ZS3 to Hp0
//
const uint16_t A_Sig_Main_from_ZS3[] PROGMEM =
{
	0b0000000000000001,
	0b0000000000001101,
	0b0000000000001100,
	SEQUENCE_APPROACH_OFF,
	SEQUENCE_END
};

const uint16_t E_Sig_Main_from_ZS3[] PROGMEM =
{
	0b0000000000000001,
	0b0000000000000101,
	0b0000000000000100,
	SEQUENCE_APPROACH_OFF,
	SEQUENCE_END
};

//------------------------------------------------------------------
//	Main: from Sh1 or ZS1 to Hp0
//
const uint16_t A_Sig_Main_from_Sh1_ZS1[] PROGMEM =
{
	0b0000000000000100,
	0b0000000000001100,
	SEQUENCE_END
};


//------------------------------------------------------------------
//------------------------------------------------------------------
//	Approach: from Hp0 to Hp1
//
const uint16_t Appr_to_Hp1[] PROGMEM =
{
	0b0000000000000000,
	0b0000101000000000,
	SEQUENCE_END
};

//------------------------------------------------------------------
//	Approach: from Hp0 to Hp2
//
const uint16_t Appr_to_Hp2[] PROGMEM =
{
	0b0000000000000000,
	0b0001001000000000,
	SEQUENCE_END
};

//------------------------------------------------------------------
//	Approach: from xxx to Hp0
//
const uint16_t Appr_to_Hp0[] PROGMEM =
{
	0b0000000000000000,
	0b0001010000000000,
	SEQUENCE_END
};


//**********************************************************************
//	General Signal Type
//

//------------------------------------------------------------------
//	Approach Signal from Hp0
//
const uint16_t* const Approach_From_Hp0[] PROGMEM =
{
	sNOP, Appr_to_Hp1, Appr_to_Hp2, sNOP, Appr_to_Hp1, Appr_to_Hp2, sNOP, sNOP
} ;

//------------------------------------------------------------------
//	Approach Signal to Hp0
//
const uint16_t* const Approach_To_Hp0[] PROGMEM =
{
	sNOP, Appr_to_Hp0, Appr_to_Hp0, sNOP, Appr_to_Hp0, Appr_to_Hp0, sNOP, sNOP
} ;


//**********************************************************************
//	Signal Type 59
//

//------------------------------------------------------------------
//	A-Sig from Hp0
//
const uint16_t* const Sig_Type_59_A_Sig_Main_From_Hp0[] PROGMEM =
{
	sNOP, Main_to_Hp1, Main_to_Hp2, Main_to_Sh1,
	A_Sig_Main_to_Hp1_ZS3, A_Sig_Main_to_Hp2_ZS3, Main_to_KL, A_Sig_Main_to_ZS1
} ;

//------------------------------------------------------------------
//	A-Sig to Hp0
//
const uint16_t* const Sig_Type_59_A_Sig_Main_To_Hp0[] PROGMEM =
{
	sNOP, A_Sig_Main_to_Hp0, A_Sig_Main_to_Hp0, A_Sig_Main_from_Sh1_ZS1,
	A_Sig_Main_from_ZS3, A_Sig_Main_from_ZS3, A_Sig_Main_to_Hp0, A_Sig_Main_from_Sh1_ZS1
};

//------------------------------------------------------------------
//	E-Sig from Hp0
//
const uint16_t* const Sig_Type_59_E_Sig_Main_From_Hp0[] PROGMEM =
{
	sNOP, Main_to_Hp1, Main_to_Hp2, sNOP,
	E_Sig_Main_to_Hp1_ZS3, E_Sig_Main_to_Hp2_ZS3, Main_to_KL, E_Sig_Main_to_ZS1
} ;

//------------------------------------------------------------------
//	E-Sig to Hp0
//
const uint16_t* const Sig_Type_59_E_Sig_Main_To_Hp0[] PROGMEM =
{
	sNOP, E_Sig_Main_to_Hp0, E_Sig_Main_to_Hp0, E_Sig_Main_to_Hp0,
	E_Sig_Main_from_ZS3, E_Sig_Main_from_ZS3, E_Sig_Main_to_Hp0, E_Sig_Main_to_Hp0
};
