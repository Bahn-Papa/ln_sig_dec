
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
//#		00	fading of LEDs
//#		01	switching of LEDs
//#		10	blinking of LEDs
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
//#	File Version:	16		from: 02.02.2025
//#
//#	Implementation:
//#		-	add sequences for US signal
//#
//#-------------------------------------------------------------------------
//#
//#	File Version:	15		from: 27.01.2025
//#
//#	Implementation:
//#		-	change sequences to reflect the change of definitions for
//#			sequencer control group
//#				Main_A_to_Hp1[]
//#				Main_A_to_Hp2[]
//#
//#-------------------------------------------------------------------------
//#
//#	File Version:	14		from: 14.08.2024
//#
//#	Implementation:
//#		-	improvement of sequence handling
//#			move all definitions into a new file 'sequencer_definitions.h'
//#
//#-------------------------------------------------------------------------
//#
//#	File Version:	13		from: 13.08.2024
//#
//#	Implementation:
//#		-	add some signal aspect definitions
//#				SIGNAL_ASPECT_VR0
//#				SIGNAL_ASPECT_VR1
//#				SIGNAL_ASPECT_VR2
//#				SIGNAL_ASPECT_UNKNOWN
//#
//#-------------------------------------------------------------------------
//#
//#	File Version:	12		from: 09.08.2024
//#
//#	Bug Fix:
//#		-	handling of extra signals was wrong
//#			every time all signals were handled where only a part should be
//#			so command codes for main and approach extra signals on and off
//#			must exist
//#			change of definitions
//#				SEQUENCE_ADDITION_SIGNAL_ON		SEQUENCE_ADDITION_SIGNAL_MAIN_ON
//#				SEQUENCE_ADDITION_SIGNAL_OFF	SEQUENCE_ADDITION_SIGNAL_MAIN_OFF
//#			new definitions
//#				SEQUENCE_ADDITION_SIGNAL_APPROACH_ON
//#				SEQUENCE_ADDITION_SIGNAL_APPROACH_OFF
//#			change of all related sequences
//#
//#-------------------------------------------------------------------------
//#
//#	File Version:	11		from: 31.07.2024
//#
//#	Bug Fix:
//#		-	extra signals were not switched during approach signal handling
//#			change in approach sequences
//#
//#-------------------------------------------------------------------------
//#
//#	File Version:	10		from: 24.03.2023
//#
//#	Implementation:
//#		-	add new definitions for rangier signal aspects
//#
//#-------------------------------------------------------------------------
//#
//#	File Version:	9		from: 23.03.2023
//#
//#	Implementation:
//#		-	add new sequences for rangier signal
//#
//#-------------------------------------------------------------------------
//#
//#	File Version:	8		from: 19.03.2023
//#
//#	Implementation:
//#		-	add new sequences for approach signal ON and OFF
//#
//#-------------------------------------------------------------------------
//#
//#	File Version:	7		from: 02.02.2023
//#
//#	Implementation:
//#		-	add new control words
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

#include "sequencer_definitions.h"


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
//	sequences to switch the approach signal ON or OFF
//
const uint8_t ApproachOn[] PROGMEM =
{
	0b00000010,
	SEQUENCE_PAUSE,
	10,
	0b00001010,
	SEQUENCE_WAIT_FOR_FADE_FINISHED,
	SEQUENCE_END
};

const uint8_t ApproachOff[] PROGMEM =
{
	0b00001000,
	SEQUENCE_WAIT_FOR_FADE_FINISHED,
	0b00000000,
	SEQUENCE_WAIT_FOR_FADE_FINISHED,
	SEQUENCE_END
};


//**********************************************************************
//	rangier signal aspect sequences
//
const uint8_t Rangier_to_Sh0[] PROGMEM =
{
	0b00000000,
	SEQUENCE_WAIT_FOR_FADE_FINISHED,
	SEQUENCE_PAUSE_DARK,
	0b00000001,
	SEQUENCE_WAIT_FOR_FADE_FINISHED,
	SEQUENCE_END
};

const uint8_t Rangier_to_Sh1[] PROGMEM =
{
	0b00000000,
	SEQUENCE_WAIT_FOR_FADE_FINISHED,
	SEQUENCE_PAUSE_DARK,
	0b00000110,
	SEQUENCE_WAIT_FOR_FADE_FINISHED,
	SEQUENCE_END
};

const uint8_t Rangier_to_ShKe[] PROGMEM =
{
	0b00000000,
	SEQUENCE_WAIT_FOR_FADE_FINISHED,
	SEQUENCE_PAUSE_DARK,
	0b00000010,
	SEQUENCE_WAIT_FOR_FADE_FINISHED,
	SEQUENCE_END
};


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
	SEQUENCE_WAIT_FOR_FADE_FINISHED,
	0b00000000,
	SEQUENCE_WAIT_FOR_FADE_FINISHED,
	SEQUENCE_PAUSE_DARK,
	0b01010000,
	SEQUENCE_PAUSE_FLASH,
	0b01000000,
	SEQUENCE_PAUSE_FLASH,
	SEQUENCE_ADDITION_SIGNAL_MAIN_ON,
//	0b00100000,
//	0b00100001,
	0b00000001,
	SEQUENCE_WAIT_FOR_FADE_FINISHED,
	SEQUENCE_PAUSE,
	100,
	SEQUENCE_SET_SIGNAL_ASPECT,
	SEQUENCE_END
};

const uint8_t Main_E_to_Hp1[] PROGMEM =
{
	SEQUENCE_APPROACH_ON,
	0b00000000,
	SEQUENCE_WAIT_FOR_FADE_FINISHED,
	SEQUENCE_PAUSE_DARK,
	0b00000100,
	SEQUENCE_WAIT_FOR_FADE_FINISHED,
	0b00000000,
	SEQUENCE_WAIT_FOR_FADE_FINISHED,
	SEQUENCE_PAUSE_DARK,
	SEQUENCE_ADDITION_SIGNAL_MAIN_ON,
//	0b00100000,
//	0b00100001,
	0b00000001,
	SEQUENCE_WAIT_FOR_FADE_FINISHED,
	SEQUENCE_PAUSE,
	100,
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
	SEQUENCE_WAIT_FOR_FADE_FINISHED,
	0b00000000,
	SEQUENCE_WAIT_FOR_FADE_FINISHED,
	SEQUENCE_PAUSE_DARK,
	0b01010000,
	SEQUENCE_PAUSE_FLASH,
	0b01000000,
	SEQUENCE_PAUSE_FLASH,
	SEQUENCE_ADDITION_SIGNAL_MAIN_ON,
	0b00100000,
	SEQUENCE_WAIT_FOR_FADE_FINISHED,
	0b00100001,
	SEQUENCE_WAIT_FOR_FADE_FINISHED,
	SEQUENCE_PAUSE,
	100,
	SEQUENCE_SET_SIGNAL_ASPECT,
	SEQUENCE_END
};

const uint8_t Main_E_to_Hp2[] PROGMEM =
{
	SEQUENCE_APPROACH_ON,
	0b00000000,
	SEQUENCE_WAIT_FOR_FADE_FINISHED,
	SEQUENCE_PAUSE_DARK,
	0b00000100,
	SEQUENCE_WAIT_FOR_FADE_FINISHED,
	0b00000000,
	SEQUENCE_WAIT_FOR_FADE_FINISHED,
	SEQUENCE_PAUSE_DARK,
	SEQUENCE_ADDITION_SIGNAL_MAIN_ON,
	0b00100000,
	SEQUENCE_WAIT_FOR_FADE_FINISHED,
	0b00100001,
	SEQUENCE_WAIT_FOR_FADE_FINISHED,
	SEQUENCE_PAUSE,
	100,
	SEQUENCE_SET_SIGNAL_ASPECT,
	SEQUENCE_END
};

//------------------------------------------------------------------
//	Main: from Hp0 to Sh1
//
const uint8_t Main_A_to_Sh1[] PROGMEM =
{
	0b00000010,
	SEQUENCE_WAIT_FOR_FADE_FINISHED,
	SEQUENCE_PAUSE_DARK,
	0b00010010,
	SEQUENCE_WAIT_FOR_FADE_FINISHED,
	SEQUENCE_END
};

//------------------------------------------------------------------
//	Main: from Hp0 to Kennlicht
//
const uint8_t Main_A_to_KL[] PROGMEM =
{
	SEQUENCE_APPROACH_ON,
	0b00000010,
	SEQUENCE_WAIT_FOR_FADE_FINISHED,
	0b00000000,
	SEQUENCE_WAIT_FOR_FADE_FINISHED,
	SEQUENCE_PAUSE_DARK,
	0b00001000,
	SEQUENCE_WAIT_FOR_FADE_FINISHED,
	SEQUENCE_PAUSE,
	100,
	SEQUENCE_ADDITION_SIGNAL_MAIN_ON,
	SEQUENCE_SET_SIGNAL_ASPECT,
	SEQUENCE_END
};

const uint8_t Main_E_to_KL[] PROGMEM =
{
	SEQUENCE_APPROACH_ON,
	0b00000000,
	SEQUENCE_WAIT_FOR_FADE_FINISHED,
	SEQUENCE_PAUSE_DARK,
	0b00000100,
	SEQUENCE_WAIT_FOR_FADE_FINISHED,
	0b00000000,
	SEQUENCE_WAIT_FOR_FADE_FINISHED,
	SEQUENCE_PAUSE_DARK,
	0b00001000,
	SEQUENCE_WAIT_FOR_FADE_FINISHED,
	SEQUENCE_PAUSE,
	100,
	SEQUENCE_ADDITION_SIGNAL_MAIN_ON,
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
	SEQUENCE_ADDITION_SIGNAL_MAIN_OFF,
	SEQUENCE_APPROACH_OFF,
	SEQUENCE_END
};

const uint8_t Main_E_from_Hp1[] PROGMEM =
{
	SEQUENCE_SET_HP0,
	SEQUENCE_PAUSE_DARK,
	0b00000000,
	SEQUENCE_WAIT_FOR_FADE_FINISHED,
	SEQUENCE_PAUSE_DARK,
	0b00000100,
	SEQUENCE_WAIT_FOR_FADE_FINISHED,
	0b00000000,
	SEQUENCE_WAIT_FOR_FADE_FINISHED,
	SEQUENCE_PAUSE_DARK,
	0b00000010,
	SEQUENCE_WAIT_FOR_FADE_FINISHED,
	SEQUENCE_ADDITION_SIGNAL_MAIN_OFF,
	SEQUENCE_APPROACH_OFF,
	SEQUENCE_END
};

//------------------------------------------------------------------
//	Main: from Hp2 to Hp0
//
const uint8_t Main_A_from_Hp2[] PROGMEM =
{
	SEQUENCE_SET_HP0,
	SEQUENCE_PAUSE_DARK,
	0b00100000,
	SEQUENCE_WAIT_FOR_FADE_FINISHED,
	0b00000000,
	SEQUENCE_WAIT_FOR_FADE_FINISHED,
	SEQUENCE_PAUSE_DARK,
	0b00000010,
	SEQUENCE_WAIT_FOR_FADE_FINISHED,
	0b00000110,
	SEQUENCE_WAIT_FOR_FADE_FINISHED,
	SEQUENCE_ADDITION_SIGNAL_MAIN_OFF,
	SEQUENCE_APPROACH_OFF,
	SEQUENCE_END
};

const uint8_t Main_E_from_Hp2[] PROGMEM =
{
	SEQUENCE_SET_HP0,
	SEQUENCE_PAUSE_DARK,
	0b00100000,
	SEQUENCE_WAIT_FOR_FADE_FINISHED,
	0b00000000,
	SEQUENCE_WAIT_FOR_FADE_FINISHED,
	SEQUENCE_PAUSE_DARK,
	0b00000100,
	SEQUENCE_WAIT_FOR_FADE_FINISHED,
	0b00000000,
	SEQUENCE_WAIT_FOR_FADE_FINISHED,
	SEQUENCE_PAUSE_DARK,
	0b00000010,
	SEQUENCE_WAIT_FOR_FADE_FINISHED,
	SEQUENCE_ADDITION_SIGNAL_MAIN_OFF,
	SEQUENCE_APPROACH_OFF,
	SEQUENCE_END
};

//------------------------------------------------------------------
//	Main: from Sh1 to Hp0
//
const uint8_t Main_A_from_Sh1[] PROGMEM =
{
	0b00000010,
	SEQUENCE_WAIT_FOR_FADE_FINISHED,
	SEQUENCE_PAUSE_DARK,
	0b00000110,
	SEQUENCE_WAIT_FOR_FADE_FINISHED,
	SEQUENCE_END
};

//------------------------------------------------------------------
//	Main: from KL to Hp0
//
const uint8_t Main_A_from_KL[] PROGMEM =
{
	SEQUENCE_SET_HP0,
	SEQUENCE_PAUSE_DARK,
	0b00000000,
	SEQUENCE_WAIT_FOR_FADE_FINISHED,
	SEQUENCE_PAUSE_DARK,
	0b00000010,
	SEQUENCE_WAIT_FOR_FADE_FINISHED,
	0b00000110,
	SEQUENCE_WAIT_FOR_FADE_FINISHED,
	SEQUENCE_ADDITION_SIGNAL_MAIN_OFF,
	SEQUENCE_APPROACH_OFF,
	SEQUENCE_END
};

const uint8_t Main_E_from_KL[] PROGMEM =
{
	SEQUENCE_SET_HP0,
	SEQUENCE_PAUSE_DARK,
	0b00000000,
	SEQUENCE_WAIT_FOR_FADE_FINISHED,
	SEQUENCE_PAUSE_DARK,
	0b00000100,
	SEQUENCE_WAIT_FOR_FADE_FINISHED,
	0b00000000,
	SEQUENCE_WAIT_FOR_FADE_FINISHED,
	SEQUENCE_PAUSE_DARK,
	0b00000010,
	SEQUENCE_WAIT_FOR_FADE_FINISHED,
	SEQUENCE_ADDITION_SIGNAL_MAIN_OFF,
	SEQUENCE_APPROACH_OFF,
	SEQUENCE_END
};


//------------------------------------------------------------------
//------------------------------------------------------------------
//	Approach: from Hp0 to Hp1
//
const uint8_t Appr_to_Hp1[] PROGMEM =
{
	SEQUENCE_ADDITION_SIGNAL_APPROACH_ON,
	0b00001000,
	SEQUENCE_WAIT_FOR_FADE_FINISHED,
	SEQUENCE_PAUSE_DARK,
	0b00001001,
	SEQUENCE_WAIT_FOR_FADE_FINISHED,
	SEQUENCE_PAUSE_DARK,
	0b00000001,
	SEQUENCE_WAIT_FOR_FADE_FINISHED,
	SEQUENCE_PAUSE_DARK,
	0b00000101,
	SEQUENCE_WAIT_FOR_FADE_FINISHED,
	SEQUENCE_END
};

//------------------------------------------------------------------
//	Approach: from Hp0 to Hp2
//
const uint8_t Appr_to_Hp2[] PROGMEM =
{
	SEQUENCE_ADDITION_SIGNAL_APPROACH_ON,
	0b00001000,
	SEQUENCE_WAIT_FOR_FADE_FINISHED,
	SEQUENCE_PAUSE_DARK,
	0b00001001,
	SEQUENCE_WAIT_FOR_FADE_FINISHED,
	SEQUENCE_END
};

//------------------------------------------------------------------
//	Approach: from xxx to Hp0
//
const uint8_t Appr_to_Hp0[] PROGMEM =
{
	0b00000001,
	SEQUENCE_WAIT_FOR_FADE_FINISHED,
	0b00000000,
	SEQUENCE_WAIT_FOR_FADE_FINISHED,
	SEQUENCE_PAUSE_DARK,
	0b00001000,
	SEQUENCE_WAIT_FOR_FADE_FINISHED,
	0b00001010,
	SEQUENCE_WAIT_FOR_FADE_FINISHED,
	SEQUENCE_ADDITION_SIGNAL_APPROACH_OFF,
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


//**********************************************************************
//**********************************************************************
//
//	signal aspect sequences for US signals
//
//	there are two sets of sequences
//		one for switching the LEDS
//		the second for fading the LEDS
//

//**********************************************************************
//
//	sequences for switching the LEDs
//

//------------------------------------------------------------------
//	US Signal switch off
//
const uint8_t US_SW_to_Off[] PROGMEM =
{
	0b10000000,
	0b01000000,
	SEQUENCE_END
};


//------------------------------------------------------------------
//	US Signal to Hp0, inv to Hp1
//
const uint8_t US_SW_to_Hp0_INV_Hp1[] PROGMEM =
{
	0b10000000,
	0b01000000,
	0b01000001,
	SEQUENCE_END
};

//------------------------------------------------------------------
//	US Signal to Hp1, inv to Hp0
//
const uint8_t US_SW_to_Hp1_INV_Hp0[] PROGMEM =
{
	0b10000000,
	0b01000000,
	0b01000100,
	SEQUENCE_END
};

//------------------------------------------------------------------
//	US Signal to Hp2
//
const uint8_t US_SW_to_Hp2[] PROGMEM =
{
	0b10000000,
	0b01000000,
	0b01000010,
	SEQUENCE_END
};

//------------------------------------------------------------------
//	US Signal to Hp2 blinking
//
const uint8_t US_SW_to_Hp2_blink[] PROGMEM =
{
	0b01000000,
	0b10000010,
	SEQUENCE_END
};


//**********************************************************************
//
//	sequences for fading the LEDs
//

//------------------------------------------------------------------
//	US Signal switch off
//
const uint8_t US_FADE_to_Off[] PROGMEM =
{
	0b10000000,
	0b00000000,
	SEQUENCE_WAIT_FOR_FADE_FINISHED,
	SEQUENCE_END
};


//------------------------------------------------------------------
//	US Signal from Hp1/Hp2 to Hp0, inv from Hp0 to Hp1
//
const uint8_t US_FADE_to_Hp0_INV_Hp0_Hp1[] PROGMEM =
{
	0b00000001,
	SEQUENCE_WAIT_FOR_FADE_FINISHED,
	SEQUENCE_END
};

//------------------------------------------------------------------
//	US Signal from Hp0 to Hp1, inv from Hp1/Hp2 to Hp0
//
const uint8_t US_FADE_Hp0_Hp1_INV_to_Hp0[] PROGMEM =
{
	0b00000100,
	SEQUENCE_WAIT_FOR_FADE_FINISHED,
	SEQUENCE_END
};

//------------------------------------------------------------------
//	US Signal from Hp0 to Hp2, inv dito
//
const uint8_t US_FADE_Hp0_Hp2[] PROGMEM =
{
	0b00000010,
	SEQUENCE_WAIT_FOR_FADE_FINISHED,
	SEQUENCE_END
};

//------------------------------------------------------------------
//	US Signal from Hp0 to Hp2 blinking, inv dito
//
const uint8_t US_FADE_Hp0_Hp2B[] PROGMEM =
{
	0b00000000,
	0b10000010,
	SEQUENCE_END
};

//------------------------------------------------------------------
//	US Signal from Hp1 to Hp2, inv N/A
//
const uint8_t US_FADE_Hp1_Hp2[] PROGMEM =
{
	0b00000001,
	SEQUENCE_WAIT_FOR_FADE_FINISHED,
	SEQUENCE_PAUSE,
	50,
	0b00000010,
	SEQUENCE_WAIT_FOR_FADE_FINISHED,
	SEQUENCE_END
};

//------------------------------------------------------------------
//	US Signal from Hp1 to Hp2B, inv N/A
//
const uint8_t US_FADE_Hp1_Hp2B[] PROGMEM =
{
	0b00000001,
	SEQUENCE_WAIT_FOR_FADE_FINISHED,
	SEQUENCE_PAUSE,
	50,
	0b00000000,
	0b10000010,
	SEQUENCE_END
};

//------------------------------------------------------------------
//	US Signal from Hp2 to Hp1, inv N/A
//
const uint8_t US_FADE_Hp2_Hp1[] PROGMEM =
{
	0b00000001,
	SEQUENCE_WAIT_FOR_FADE_FINISHED,
	SEQUENCE_PAUSE,
	50,
	0b00000100,
	SEQUENCE_WAIT_FOR_FADE_FINISHED,
	SEQUENCE_END
};

//------------------------------------------------------------------
//	US Signal from Hp2 to Hp2B, inv dito
//
const uint8_t US_FADE_Hp2_Hp2B[] PROGMEM =
{
	0b00000000,
	SEQUENCE_WAIT_FOR_FADE_FINISHED,
	SEQUENCE_PAUSE_DARK,
	0b10000010,
	SEQUENCE_END
};

//------------------------------------------------------------------
//	US Signal from Hp2B to Hp0, inv N/A
//
const uint8_t US_FADE_Hp2B_Hp0[] PROGMEM =
{
	0b10000000,
	0b00000001,
	SEQUENCE_WAIT_FOR_FADE_FINISHED,
	SEQUENCE_END
};

//------------------------------------------------------------------
//	US Signal from Hp2B to Hp1, inv N/A
//
const uint8_t US_FADE_Hp2B_Hp1[] PROGMEM =
{
	0b10000000,
	0b00000001,
	SEQUENCE_WAIT_FOR_FADE_FINISHED,
	SEQUENCE_PAUSE,
	50,
	0b00000100,
	SEQUENCE_WAIT_FOR_FADE_FINISHED,
	SEQUENCE_END
};

//------------------------------------------------------------------
//	US Signal from Hp2B to Hp2, inv dito
//
const uint8_t US_FADE_Hp2B_Hp2[] PROGMEM =
{
	0b10000000,
	SEQUENCE_WAIT_FOR_FADE_FINISHED,
	0b00000010,
	SEQUENCE_WAIT_FOR_FADE_FINISHED,
	SEQUENCE_END
};


//------------------------------------------------------------------
//	US Signal inverse from Hp1 to Hp2
//
const uint8_t US_FADE_INV_Hp1_Hp2[] PROGMEM =
{
	0b00000100,
	SEQUENCE_WAIT_FOR_FADE_FINISHED,
	SEQUENCE_PAUSE,
	50,
	0b00000010,
	SEQUENCE_WAIT_FOR_FADE_FINISHED,
	SEQUENCE_END
};

//------------------------------------------------------------------
//	US Signal inverse from Hp1 to Hp2B
//
const uint8_t US_FADE_INV_Hp1_Hp2B[] PROGMEM =
{
	0b00000100,
	SEQUENCE_WAIT_FOR_FADE_FINISHED,
	SEQUENCE_PAUSE,
	50,
	0b00000000,
	0b10000010,
	SEQUENCE_END
};

//------------------------------------------------------------------
//	US Signal inverse from Hp2 to Hp1
//
const uint8_t US_FADE_INV_Hp2_Hp1[] PROGMEM =
{
	0b00000100,
	SEQUENCE_WAIT_FOR_FADE_FINISHED,
	SEQUENCE_PAUSE,
	50,
	0b00000001,
	SEQUENCE_WAIT_FOR_FADE_FINISHED,
	SEQUENCE_END
};

//------------------------------------------------------------------
//	US Signal inverse from Hp2B to Hp1
//
const uint8_t US_FADE_INV_Hp2B_Hp0[] PROGMEM =
{
	0b10000000,
	0b00000100,
	SEQUENCE_WAIT_FOR_FADE_FINISHED,
	SEQUENCE_END
};

//------------------------------------------------------------------
//	US Signal inverse from Hp2B to Hp1
//
const uint8_t US_FADE_INV_Hp2B_Hp1[] PROGMEM =
{
	0b10000000,
	0b00000100,
	SEQUENCE_WAIT_FOR_FADE_FINISHED,
	SEQUENCE_PAUSE,
	50,
	0b00000001,
	SEQUENCE_WAIT_FOR_FADE_FINISHED,
	SEQUENCE_END
};


//------------------------------------------------------------------
//	US Signal sequence matrix for switched signals
//
const uint8_t* const US_SW_Matrix[ US_SIGNAL_COUNT ][ US_SIGNAL_COUNT ] PROGMEM =
{
	{	sNOP,					US_SW_to_Hp1_INV_Hp0,	US_SW_to_Hp2,	US_SW_to_Hp2_blink	},
	{	US_SW_to_Hp0_INV_Hp1,	sNOP,					US_SW_to_Hp2,	US_SW_to_Hp2_blink	},
	{	US_SW_to_Hp0_INV_Hp1,	US_SW_to_Hp1_INV_Hp0,	sNOP,			US_SW_to_Hp2_blink	},
	{	US_SW_to_Hp0_INV_Hp1,	US_SW_to_Hp1_INV_Hp0,	US_SW_to_Hp2,	sNOP				}
};

const uint8_t* const US_SW_INV_Matrix[ US_SIGNAL_COUNT ][ US_SIGNAL_COUNT ] PROGMEM =
{
	{	sNOP,					US_SW_to_Hp0_INV_Hp1,	US_SW_to_Hp2,	US_SW_to_Hp2_blink	},
	{	US_SW_to_Hp1_INV_Hp0,	sNOP,					US_SW_to_Hp2,	US_SW_to_Hp2_blink	},
	{	US_SW_to_Hp1_INV_Hp0,	US_SW_to_Hp0_INV_Hp1,	sNOP,			US_SW_to_Hp2_blink	},
	{	US_SW_to_Hp1_INV_Hp0,	US_SW_to_Hp0_INV_Hp1,	US_SW_to_Hp2,	sNOP				}
};

const uint8_t* const US_SW_On[] PROGMEM =
{
	US_SW_to_Hp0_INV_Hp1,
	US_SW_to_Hp1_INV_Hp0,
	US_SW_to_Hp2,
	US_SW_to_Hp2_blink
} ;

const uint8_t* const US_SW_INV_On[] PROGMEM =
{
	US_SW_to_Hp1_INV_Hp0,
	US_SW_to_Hp0_INV_Hp1,
	US_SW_to_Hp2,
	US_SW_to_Hp2_blink
} ;


//------------------------------------------------------------------
//	US Signal sequence matrix for fading signals
//
const uint8_t* const US_FADE_Matrix[ US_SIGNAL_COUNT ][ US_SIGNAL_COUNT ] PROGMEM =
{
	{	sNOP,						US_FADE_Hp0_Hp1_INV_to_Hp0,	US_FADE_Hp0_Hp2,	US_FADE_Hp0_Hp2B	},
	{	US_FADE_to_Hp0_INV_Hp0_Hp1,	sNOP,						US_FADE_Hp1_Hp2,	US_FADE_Hp1_Hp2B	},
	{	US_FADE_to_Hp0_INV_Hp0_Hp1,	US_FADE_Hp2_Hp1,			sNOP,				US_FADE_Hp2_Hp2B	},
	{	US_FADE_Hp2B_Hp0,			US_FADE_Hp2B_Hp1,			US_FADE_Hp2B_Hp2,	sNOP				}
};

const uint8_t* const US_FADE_INV_Matrix[ US_SIGNAL_COUNT ][ US_SIGNAL_COUNT ] PROGMEM =
{
	{	sNOP,						US_FADE_to_Hp0_INV_Hp0_Hp1,	US_FADE_Hp0_Hp2,		US_FADE_Hp0_Hp2B		},
	{	US_FADE_Hp0_Hp1_INV_to_Hp0,	sNOP,						US_FADE_INV_Hp1_Hp2,	US_FADE_INV_Hp1_Hp2B	},
	{	US_FADE_Hp0_Hp1_INV_to_Hp0,	US_FADE_INV_Hp2_Hp1,		sNOP,					US_FADE_Hp2_Hp2B		},
	{	US_FADE_INV_Hp2B_Hp0,		US_FADE_INV_Hp2B_Hp1,		US_FADE_Hp2B_Hp2,		sNOP					}
};

const uint8_t* const US_FADE_On[] PROGMEM =
{
	US_FADE_to_Hp0_INV_Hp0_Hp1,
	US_FADE_Hp0_Hp1_INV_to_Hp0,
	US_FADE_Hp0_Hp2,
	US_FADE_Hp0_Hp2B
} ;

const uint8_t* const US_FADE_INV_On[] PROGMEM =
{
	US_FADE_Hp0_Hp1_INV_to_Hp0,
	US_FADE_to_Hp0_INV_Hp0_Hp1,
	US_FADE_Hp0_Hp2,
	US_FADE_Hp0_Hp2B
} ;

