
#pragma once

//##########################################################################
//#
//#		global.h
//#
//#	Description
//#
//#-------------------------------------------------------------------------
//#
//#	File Version:	2		from: 29.01.2025
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
//	index for signal aspects
//

//----	main signal  -----------------------------------------------
#define MAIN_LEDS						0x007E

#define IDX_SIGNAL_MAIN_BIT_0			1
#define IDX_SIGNAL_MAIN_BIT_1			2
#define IDX_SIGNAL_MAIN_BIT_2			3
#define IDX_SIGNAL_MAIN_BIT_3			4

#define MAIN_LEDS_IDX_START				IDX_SIGNAL_MAIN_BIT_0
#define MAIN_LEDS_IDX_COUNT				6
#define MAIN_LEDS_IDX_STOP				(MAIN_LEDS_IDX_START + MAIN_LEDS_IDX_COUNT)

//----	approach signal  -------------------------------------------
#define APPROACH_LEDS					0x1E00

#define IDX_SIGNAL_APPROACH_BIT_0		9
#define IDX_SIGNAL_APPROACH_BIT_1		10
#define IDX_SIGNAL_APPROACH_BIT_2		11
#define IDX_SIGNAL_APPROACH_BIT_3		12

#define APPROACH_LEDS_IDX_START			IDX_SIGNAL_APPROACH_BIT_0
#define APPROACH_LEDS_IDX_COUNT			4
#define APPROACH_LEDS_IDX_STOP			(APPROACH_LEDS_IDX_START + APPROACH_LEDS_IDX_COUNT)


//----	rangier signal 1  ------------------------------------------
#define RANGIER_1_LEDS					0x0007

#define IDX_RANGIER_SIGNAL_1_BIT_0		0
#define IDX_RANGIER_SIGNAL_1_BIT_1		1

#define RANGIER_1_LEDS_IDX_START		LED_A
#define RANGIER_1_LEDS_IDX_COUNT		3
#define RANGIER_1_LEDS_IDX_STOP			(RANGIER_1_LEDS_IDX_START + RANGIER_1_LEDS_IDX_COUNT)

//----	rangier signal 2  ------------------------------------------
#define RANGIER_2_LEDS					0x0038

#define IDX_RANGIER_SIGNAL_2_BIT_0		2
#define IDX_RANGIER_SIGNAL_2_BIT_1		3

#define RANGIER_2_LEDS_IDX_START		LED_D
#define RANGIER_2_LEDS_IDX_COUNT		3
#define RANGIER_2_LEDS_IDX_STOP			(RANGIER_2_LEDS_IDX_START + RANGIER_2_LEDS_IDX_COUNT)

//----	rangier signal 3  ------------------------------------------
#define RANGIER_3_LEDS					0x01C0

#define IDX_RANGIER_SIGNAL_3_BIT_0		4
#define IDX_RANGIER_SIGNAL_3_BIT_1		5

#define RANGIER_3_LEDS_IDX_START		LED_G
#define RANGIER_3_LEDS_IDX_COUNT		3
#define RANGIER_3_LEDS_IDX_STOP			(RANGIER_3_LEDS_IDX_START + RANGIER_3_LEDS_IDX_COUNT)

//----	rangier signal 4  ------------------------------------------
#define RANGIER_4_LEDS					0x0E00

#define IDX_RANGIER_SIGNAL_4_BIT_0		6
#define IDX_RANGIER_SIGNAL_4_BIT_1		7

#define RANGIER_4_LEDS_IDX_START		LED_J
#define RANGIER_4_LEDS_IDX_COUNT		3
#define RANGIER_4_LEDS_IDX_STOP			(RANGIER_4_LEDS_IDX_START + RANGIER_4_LEDS_IDX_COUNT)

//----	rangier signal 5  ------------------------------------------
#define RANGIER_5_LEDS					0x3000

#define IDX_RANGIER_SIGNAL_5_BIT_0		8
#define IDX_RANGIER_SIGNAL_5_BIT_1		9

#define RANGIER_5_LEDS_IDX_START		LED_M
#define RANGIER_5_LEDS_IDX_COUNT		2
#define RANGIER_5_LEDS_IDX_STOP			(RANGIER_5_LEDS_IDX_START + RANGIER_5_LEDS_IDX_COUNT)


//----	US signal 1  -----------------------------------------------
#define US_SIGNAL_1_LEDS				0x0007

#define IDX_US_SIGNAL_1_BIT_0			0
#define IDX_US_SIGNAL_1_BIT_1			1
#define IDX_US_SIGNAL_1_OFF				2

#define US_SIGNAL_1_LEDS_IDX_START		LED_A
#define US_SIGNAL_1_LEDS_IDX_COUNT		3
#define US_SIGNAL_1_LEDS_IDX_STOP		(US_SIGNAL_1_LEDS_IDX_START + US_SIGNAL_1_LEDS_IDX_COUNT)

//----	US signal 2  -----------------------------------------------
#define US_SIGNAL_2_LEDS				0x0070

#define IDX_US_SIGNAL_2_BIT_0			3
#define IDX_US_SIGNAL_2_BIT_1			4
#define IDX_US_SIGNAL_2_OFF				5

#define US_SIGNAL_2_LEDS_IDX_START		LED_E
#define US_SIGNAL_2_LEDS_IDX_COUNT		3
#define US_SIGNAL_2_LEDS_IDX_STOP		(US_SIGNAL_2_LEDS_IDX_START + US_SIGNAL_2_LEDS_IDX_COUNT)

//----	US signal 3  -----------------------------------------------
#define US_SIGNAL_3_LEDS				0x0380

#define IDX_US_SIGNAL_3_BIT_0			6
#define IDX_US_SIGNAL_3_BIT_1			7
#define IDX_US_SIGNAL_3_OFF				8

#define US_SIGNAL_3_LEDS_IDX_START		LED_H
#define US_SIGNAL_3_LEDS_IDX_COUNT		3
#define US_SIGNAL_3_LEDS_IDX_STOP		(US_SIGNAL_3_LEDS_IDX_START + US_SIGNAL_3_LEDS_IDX_COUNT)

//----	US signal 4  -----------------------------------------------
#define US_SIGNAL_4_LEDS				0x3800

#define IDX_US_SIGNAL_4_BIT_0			9
#define IDX_US_SIGNAL_4_BIT_1			10
#define IDX_US_SIGNAL_4_OFF				11

#define US_SIGNAL_4_LEDS_IDX_START		LED_L
#define US_SIGNAL_4_LEDS_IDX_COUNT		3
#define US_SIGNAL_4_LEDS_IDX_STOP		(US_SIGNAL_4_LEDS_IDX_START + US_SIGNAL_4_LEDS_IDX_COUNT)

#define IDX_US_SIGNAL_ALL_OFF			12
