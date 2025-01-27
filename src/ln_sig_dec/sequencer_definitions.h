
#pragma once

//##########################################################################
//#
//#		sequencer_definitions.h
//#
//#	Here are all definitions defined to handle the sequences
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
//#	 0	Hp0/Vr0			 0	 0	 0	 0		 0	 0	 0	 0
//#	 1	Hp1/Vr1			 0	 0	 0	 1		 0	 0	 0	 1
//#	 2	Hp2/Vr2			 0	 0	 1	 0		 0	 0	 1	 0
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
//#	File Version:	2		from: 26.01.2025
//#
//#	Implementation:
//#		-	add definitions for new signal type SIGNAL_TYPE_US
//#
//#-------------------------------------------------------------------------
//#
//#	File Version:	1		from: 14.08.2024
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


//==========================================================================
//
//		D E F I N I T I O N S
//
//==========================================================================

#define SIGNAL_ASPECTS				5
#define RANGIER_ASPECTS				3
#define US_SIGNAL_ASPECTS			4

//----------------------------------------------------------------------
//	signal aspects
//
#define SIGNAL_ASPECT_UNKNOWN		100

#define SIGNAL_ASPECT_HP0			0
#define SIGNAL_ASPECT_HP1			1
#define SIGNAL_ASPECT_HP2			2
#define SIGNAL_ASPECT_SH1			3
#define SIGNAL_ASPECT_KENNLICHT		4

//----------------------------------------------------------------------
//	signal aspects
//
#define SIGNAL_ASPECT_VR0			0
#define SIGNAL_ASPECT_VR1			1
#define SIGNAL_ASPECT_VR2			2

//----------------------------------------------------------------------
//	signal aspects
//
#define RANGIER_ASPECT_SH0			0
#define RANGIER_ASPECT_SH1			1
#define RANGIER_ASPECT_SHKE			2

//----------------------------------------------------------------------
//	signal aspects for US signals
//
#define US_SIGNAL_ASPECT_HP0			0
#define US_SIGNAL_ASPECT_HP2			1
#define US_SIGNAL_ASPECT_HP2_BLINK		2
#define US_SIGNAL_ASPECT_HP1			3

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
#define SEQUENCE_END							0xFF
#define SEQUENCE_WAIT_FOR_FADE_FINISHED			0xCF
#define SEQUENCE_PAUSE							0xC0
#define SEQUENCE_PAUSE_DARK						0xC1
#define SEQUENCE_PAUSE_FLASH					0xC2
#define SEQUENCE_APPROACH_ON					0xC3
#define SEQUENCE_APPROACH_OFF					0xC4
#define SEQUENCE_SET_SIGNAL_ASPECT				0xC5
#define SEQUENCE_SET_HP0						0xC6
#define SEQUENCE_ADDITION_SIGNAL_MAIN_ON		0xC7
#define SEQUENCE_ADDITION_SIGNAL_MAIN_OFF		0xC8
#define SEQUENCE_ADDITION_SIGNAL_APPROACH_ON	0xC9
#define SEQUENCE_ADDITION_SIGNAL_APPROACH_OFF	0xCA
