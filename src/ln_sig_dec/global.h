
#pragma once

//##########################################################################
//#
//#		global.h
//#
//#	In this file the assignment of the individual LEDs to the various
//#	signals or signal screens is defined.
//#
//#	The basic definition comes from the hardware, which can control up to
//#	14 LEDs. So it makes sense that each bit in a uint16_t variable
//#	represents an LED.
//#	Some of these LEDs belong to a signal or signal screen. So for each
//#	signal or signal screen there will be one set of definitions.
//#	One set of definitions consists of
//#		-	the bit mask of the LEDs belonging to this set
//#		-	the first LED
//#		-	the last  LED
//#		-	the number of LEDs in this set (calculated)
//#
//#	When a signal uses only a few LEDs then it is possible that
//#	one hardware can control multiple signals.
//# Therefore there are also definitions as to which signal is selected.
//#
//#-------------------------------------------------------------------------
//#
//#	Controlling the signals or signal screens
//#	There are two different control methods:
//#		-	direct control
//#		-	signal aspects
//#
//#	Direct Control:
//#	This is very simple, for each LED there is an address to which a
//#	SWITCH Loconet message can be sent to turn the LED on or off.
//#	So there is an array of at least 14 addresses. The index number to
//#	one address in the array represents the bit number of one LED.
//#
//#	Signal Aspects:
//#	Things get a little more complicated when the signals are controlled
//#	using signal aspects. A signal can display multiple signal aspects.
//#	In order to select a signal aspect from the list of signal aspects,
//#	an index must be created. Since the index is a number, the number
//#	can be generated from a combination of its binary digits.
//#	Such a binary digit can be changed using a Loconet SWITCH message.
//#	Therefore, an assignment is required as to which address of the
//#	14 possible addresses corresponds to which bit in the index.
//#	(see the XXX_ASPECT_BIT_Y definitions below)
//#
//#-------------------------------------------------------------------------
//#
//#	File Version:	2		from: 31.01.2025
//#
//#	Implementation:
//#		-	add definitions for US signals
//#
//#-------------------------------------------------------------------------
//#
//#	File Version:	1		from: 13.08.2024
//#
//#	Implementation:
//#		-	first working version
//#
//##########################################################################


//==========================================================================
//
//		D E F I N I T I O N S
//
//==========================================================================

#define TOTAL_LEDS			16
#define	SIGNAL_LEDS			14

#define	LED_A				0
#define	LED_B				1
#define	LED_C				2
#define	LED_D				3
#define	LED_E				4
#define	LED_F				5
#define	LED_G				6
#define	LED_H				7
#define	LED_I				8
#define	LED_J				9
#define	LED_K				10
#define	LED_L				11
#define	LED_M				12
#define	LED_N				13

#define	LED_BLUE			15
#define LED_GREEN			14

#define LED_MAIN_ZUSATZANZEIGER			LED_A
#define LED_MAIN_GREEN					LED_B
#define LED_MAIN_RED_LEFT				LED_C
#define LED_MAIN_RED_RIGHT				LED_D
#define LED_MAIN_KENNLICHT				LED_E
#define LED_MAIN_SH1					LED_F
#define LED_MAIN_YELLOW					LED_G
#define LED_ERSATZSIGNAL				LED_H
#define LED_APPROACH_KENNLICHT			LED_I
#define LED_APPROACH_GREEN_TOP			LED_J
#define LED_APPROACH_YELLOW_TOP			LED_K
#define LED_APPROACH_GREEN_BOTTOM		LED_L
#define LED_APPROACH_YELLOW_BOTTOM		LED_M
#define LED_APPROACH_ZUSATZANZEIGER		LED_N
#define LED_ERSATZSIGNAL_FLASH			(LED_N + 1)

//----------------------------------------------------------------------
//	extra signal numbers
//
#define EXTRA_SIGNAL_LEDS					0x2181

#define EXTRA_SIGNAL_ZUSATZANZEIGE			0
#define EXTRA_SIGNAL_ERSATZSIGNAL			1
#define EXTRA_SIGNAL_KENNLICHT				2
#define EXTRA_SIGNAL_ANZEIGE				3
#define EXTRA_SIGNAL_ERSATZSIGNAL_FLASH		4

#define EXTRA_SIGNAL_COUNT					5

//----------------------------------------------------------------------
//	selected signals
//
#define SIGNAL_SELECTED_MAIN		7
#define SIGNAL_SELECTED_APPROACH	8

#define SIGNAL_SELECTED_RANGIER_1	0
#define SIGNAL_SELECTED_RANGIER_2	1
#define SIGNAL_SELECTED_RANGIER_3	2
#define SIGNAL_SELECTED_RANGIER_4	3
#define SIGNAL_SELECTED_RANGIER_5	4

#define RANGIER_SIGNAL_COUNT		5


#define SIGNAL_SELECTED_US_1		10
#define SIGNAL_SELECTED_US_2		11
#define SIGNAL_SELECTED_US_3		12
#define SIGNAL_SELECTED_US_4		13

#define US_SIGNAL_COUNT				4


//----------------------------------------------------------------------
//	definitions for a main signal screen
//
#define MASK_MAIN_LEDS					0x007E

#define IDX_MAIN_SCREEN_FIRST_LED		LED_MAIN_GREEN
#define IDX_MAIN_SCREEN_LAST_LED		LED_MAIN_YELLOW
#define MAIN_SCREEN_LED_COUNT			((IDX_MAIN_SCREEN_LAST_LED - IDX_MAIN_SCREEN_FIRST_LED) + 1)

#define IDX_MAIN_SCREEN_ASPECT_BIT_0	1
#define IDX_MAIN_SCREEN_ASPECT_BIT_1	2
#define IDX_MAIN_SCREEN_ASPECT_BIT_2	3
#define IDX_MAIN_SCREEN_ASPECT_BIT_3	4


//----------------------------------------------------------------------
//	definitions for a approach signal screen
//
#define MASK_APPROACH_LEDS					0x1E00

#define IDX_APPROACH_SCREEN_FIRST_LED		LED_APPROACH_GREEN_TOP
#define IDX_APPROACH_SCREEN_LAST_LED		LED_APPROACH_YELLOW_BOTTOM
#define APPROACH_SCREEN_LED_COUNT			((IDX_APPROACH_SCREEN_LAST_LED - IDX_APPROACH_SCREEN_FIRST_LED) + 1)

#define IDX_APPROACH_SCREEN_ASPECT_BIT_0	9
#define IDX_APPROACH_SCREEN_ASPECT_BIT_1	10
#define IDX_APPROACH_SCREEN_ASPECT_BIT_2	11
#define IDX_APPROACH_SCREEN_ASPECT_BIT_3	12


//----------------------------------------------------------------------
//	definitions for a set of rangier signals
//
//	the LED count for each rangier signal is the same except 
//	for rangier signal 5
//	so we need only the first LED of each signal
//

#define	RANGIER_SIGNAL_LED_COUNT			3
#define	RANGIER_SIGNAL_5_LED_COUNT			2

//----	rangier signal 1  ------------------------------------------

#define MASK_RANGIER_SIGNAL_1_LEDS			0x0007

#define IDX_RANGIER_SIGNAL_1_FIRST_LED		LED_A

#define IDX_RANGIER_SIGNAL_1_ASPECT_BIT_0	0
#define IDX_RANGIER_SIGNAL_1_ASPECT_BIT_1	1

//----	rangier signal 2  ------------------------------------------

#define MASK_RANGIER_SIGNAL_2_LEDS			0x0038

#define IDX_RANGIER_SIGNAL_2_FIRST_LED		LED_D

#define IDX_RANGIER_SIGNAL_2_ASPECT_BIT_0	2
#define IDX_RANGIER_SIGNAL_2_ASPECT_BIT_1	3

//----	rangier signal 3  ------------------------------------------

#define MASK_RANGIER_SIGNAL_3_LEDS			0x01C0

#define IDX_RANGIER_SIGNAL_3_FIRST_LED		LED_G

#define IDX_RANGIER_SIGNAL_3_ASPECT_BIT_0	4
#define IDX_RANGIER_SIGNAL_3_ASPECT_BIT_1	5

//----	rangier signal 4  ------------------------------------------

#define MASK_RANGIER_SIGNAL_4_LEDS			0x0E00

#define IDX_RANGIER_SIGNAL_4_FIRST_LED		LED_J

#define IDX_RANGIER_SIGNAL_4_ASPECT_BIT_0	6
#define IDX_RANGIER_SIGNAL_4_ASPECT_BIT_1	7

//----	rangier signal 5  ------------------------------------------

#define MASK_RANGIER_SIGNAL_5_LEDS			0x3000

#define IDX_RANGIER_SIGNAL_5_FIRST_LED		LED_M

#define IDX_RANGIER_SIGNAL_5_ASPECT_BIT_0	8
#define IDX_RANGIER_SIGNAL_5_ASPECT_BIT_1	9


//----------------------------------------------------------------------
//	definitions for a set of US signals
//
//	the LED count for each US signal is the same
//	so we need only the first LED of each signal
//

#define	US_SIGNAL_LED_COUNT		3

//----	US signal 1  -----------------------------------------------

#define MASK_US_SIGNAL_1_LEDS			0x0007

#define IDX_US_SIGNAL_1_FIRST_LED		LED_A

#define IDX_US_SIGNAL_1_ASPECT_BIT_0	0
#define IDX_US_SIGNAL_1_ASPECT_BIT_1	1
#define IDX_US_SIGNAL_1_ON_OFF			2

//----	US signal 2  -----------------------------------------------

#define MASK_US_SIGNAL_2_LEDS			0x0070

#define IDX_US_SIGNAL_2_FIRST_LED		LED_E

#define IDX_US_SIGNAL_2_ASPECT_BIT_0	3
#define IDX_US_SIGNAL_2_ASPECT_BIT_1	4
#define IDX_US_SIGNAL_2_ON_OFF			5

//----	US signal 3  -----------------------------------------------

#define US_SIGNAL_3_LEDS				0x0380

#define IDX_US_SIGNAL_3_FIRST_LED		LED_H

#define IDX_US_SIGNAL_3_ASPECT_BIT_0	6
#define IDX_US_SIGNAL_3_ASPECT_BIT_1	7
#define IDX_US_SIGNAL_3_ON_OFF			8

//----	US signal 4  -----------------------------------------------
#define US_SIGNAL_4_LEDS				0x3800

#define IDX_US_SIGNAL_4_FIRST_LED		LED_L

#define IDX_US_SIGNAL_4_ASPECT_BIT_0	9
#define IDX_US_SIGNAL_4_ASPECT_BIT_1	10
#define IDX_US_SIGNAL_4_ON_OFF			11

#define IDX_US_SIGNAL_ALL_ON_OFF		12
