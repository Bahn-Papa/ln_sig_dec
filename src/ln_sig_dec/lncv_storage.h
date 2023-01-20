
#pragma once

//##########################################################################
//#
//#		LncvStorageClass
//#
//#	This class manages the LNCVs (L oco N et C onfiguration V ariables)
//#	These variables will be saved in the EEPROM.
//#
//#-------------------------------------------------------------------------
//#
//#	File version:	8		from: 18.01.2023
//#
//#	Implementation:
//#		-	new last LNCV address
//#		-	add Ersatz signal flash time
//#			new definitions
//#				LNCV_ADR_ERSATZ_SIGNAL_FLASH_TIME
//#				IDX_APPROACH_DARK
//#				IDX_ERSATZ_SIGNAL_FLASH
//#
//#-------------------------------------------------------------------------
//#
//#	File version:	7		from: 18.01.2023
//#
//#	Implementation:
//#		-	add dark time, flash time, approach signal dark and
//#			ersatz signal flash
//#			new definitions
//#				LNCV_ADR_DARK_TIME
//#				LNCV_ADR_FLASH_TIME
//#				LNCV_ADR_APPROACH_DARK
//#				LNCV_ADR_ERSATZ_SIGNAL_FLASH
//#				LNCV_ADR_LAST_VALID
//#
//#-------------------------------------------------------------------------
//#
//#	File version:	6		from: 14.12.2022
//#
//#	Implementation:
//#		-	add track contact delay time and re-organize LNCVs
//#			new definitions
//#				LNCV_ADR_FOUR
//#				LNCV_ADR_TRACK_CONTACT_DELAY_TIME
//#
//#-------------------------------------------------------------------------
//#
//#	File version:	5		from: 21.11.2022
//#
//#	Implementation:
//#		-	add index defines used to index a device address from the
//#			device address array
//#
//#-------------------------------------------------------------------------
//#
//#	File version:	4		from: 17.11.2022
//#
//#	Implementation:
//#		-	streamlining lncv storage
//#			remove functions
//#				GetSignalType()
//#				GetLightIntensityDay()
//#				GetLightIntensityNight()
//#			remove variable
//#				m_usSignalType
//#
//#-------------------------------------------------------------------------
//#
//#	File version:	3		from: 26.09.2022
//#
//#	Implementation:
//#		-	add new signal types
//#
//#-------------------------------------------------------------------------
//#
//#	File version:	2		from: 19.09.2022
//#
//#	Implementation:
//#		-	add version number into EEPROM
//#
//#-------------------------------------------------------------------------
//#
//#	File version:	1		from: 17.09.2022
//#
//#	Implementation:
//#		-	first version
//#
//##########################################################################


//==========================================================================
//
//		D E F I N I T I O N S
//
//==========================================================================


//----------------------------------------------------------------------
//	my artikle number
#define ARTIKEL_NUMMER			1515

#define LNCV_ADR_LAST_VALID		60


//----------------------------------------------------------------------
//	definitions for signal types
//
#define SIGNAL_TYPE_DIRECT_CONTROL_HARD		0
#define SIGNAL_TYPE_DIRECT_CONTROL_FAST		1
#define SIGNAL_TYPE_DIRECT_CONTROL_MIDDLE	2
#define SIGNAL_TYPE_DIRECT_CONTROL_SLOW		3
#define SIGNAL_TYPE_59_A_SIG				4
#define SIGNAL_TYPE_59_E_SIG				5
#define SIGNAL_TYPE_60_A_SIG				6
#define SIGNAL_TYPE_60_E_SIG				7
#define SIGNAL_TYPE_COMPACT_A_SIG			8
#define SIGNAL_TYPE_COMPACT_E_SIG			9
#define SIGNAL_TYPE_KS_A_SIG				10
#define SIGNAL_TYPE_KS_E_SIG				11


//----------------------------------------------------------------------
//	address definitions for config informations
//
#define LNCV_ADR_MODULE_ADDRESS				0
#define LNCV_ADR_ARTIKEL_NUMMER				1
#define LNCV_ADR_VERSION					2
#define LNCV_ADR_SIGNAL_TYPE				3
#define LNCV_ADR_NIGHTLIGHT					4
#define LNCV_ADR_SIGNAL_VALID_WAIT_TIME		5
#define LNCV_ADR_TRACK_CONTACT				6
#define LNCV_ADR_TRACK_CONTACT_DELAY_TIME	7
#define LNCV_ADR_DARK_TIME					8
#define LNCV_ADR_FLASH_TIME					9
#define LNCV_ADR_ERSATZ_SIGNAL_FLASH_TIME	10


//----------------------------------------------------------------------
//	light intensity values
//
//	low byte	= night intensity
//	high byte	= day intensity
//
#define LNCV_ADR_LIGHT_INTENSITY_LED_A		11
#define LNCV_ADR_LIGHT_INTENSITY_LED_B		12
#define LNCV_ADR_LIGHT_INTENSITY_LED_C		13
#define LNCV_ADR_LIGHT_INTENSITY_LED_D		14
#define LNCV_ADR_LIGHT_INTENSITY_LED_E		15
#define LNCV_ADR_LIGHT_INTENSITY_LED_F		16
#define LNCV_ADR_LIGHT_INTENSITY_LED_G		17
#define LNCV_ADR_LIGHT_INTENSITY_LED_H		18
#define LNCV_ADR_LIGHT_INTENSITY_LED_I		19
#define LNCV_ADR_LIGHT_INTENSITY_LED_J		20
#define LNCV_ADR_LIGHT_INTENSITY_LED_K		21
#define LNCV_ADR_LIGHT_INTENSITY_LED_L		22
#define LNCV_ADR_LIGHT_INTENSITY_LED_M		23
#define LNCV_ADR_LIGHT_INTENSITY_LED_N		24

#define BRIGHTNESS_MAX						255
#define BRIGHTNESS_MIN						0


//----------------------------------------------------------------------
//	fade times
//
#define LNCV_ADR_FADE_TIME_UP_FAST			25
#define LNCV_ADR_FADE_TIME_UP_MIDDLE		26
#define LNCV_ADR_FADE_TIME_UP_SLOW			27
#define LNCV_ADR_FADE_TIME_DOWN_FAST		28
#define LNCV_ADR_FADE_TIME_DOWN_MIDDLE		29
#define LNCV_ADR_FADE_TIME_DOWN_SLOW		30

#define IDX_FADE_TIME_IMMEDIATE				0
#define IDX_FADE_TIME_FAST					1
#define IDX_FADE_TIME_MIDDLE				2
#define IDX_FADE_TIME_SLOW					3

#define FADE_TIME_COUNT						4


//----------------------------------------------------------------------
//	address definitions to switch each light separately
//
#define LNCV_ADR_LIGHT_A					31
#define LNCV_ADR_LIGHT_B					32
#define LNCV_ADR_LIGHT_C					33
#define LNCV_ADR_LIGHT_D					34
#define LNCV_ADR_LIGHT_E					35
#define LNCV_ADR_LIGHT_F					36
#define LNCV_ADR_LIGHT_G					37
#define LNCV_ADR_LIGHT_H					38
#define LNCV_ADR_LIGHT_I					39
#define LNCV_ADR_LIGHT_J					40
#define LNCV_ADR_LIGHT_K					41
#define LNCV_ADR_LIGHT_L					42
#define LNCV_ADR_LIGHT_M					43
#define LNCV_ADR_LIGHT_N					44


//----------------------------------------------------------------------
//	address definitions for special purposes
//
#define LNCV_ADR_APPROACH_DARK				45
#define LNCV_ADR_ERSATZ_SIGNAL_FLASH		46


//----------------------------------------------------------------------
//	number of device addresses
//
//	14 lights to control	=>		14 addresses
//	track contact			=>		 1 address
//	select nightlight		=>		 1 address
//	approach dark			=>		 1 address
//	Ersatz signal flash		=>		 1 address
//						TOTAL OF	18 addresses
//
#define NUMBER_LIGHTS			14
#define	DEVICE_COUNT			(NUMBER_LIGHTS + 4)


//----------------------------------------------------------------------
//	index for signal aspects
//
#define IDX_SIGNAL_MAIN_BIT_0			1
#define IDX_SIGNAL_MAIN_BIT_1			2
#define IDX_SIGNAL_MAIN_BIT_2			3
#define IDX_SIGNAL_MAIN_BIT_3			4

#define IDX_SIGNAL_APPROACH_BIT_0		9
#define IDX_SIGNAL_APPROACH_BIT_1		10
#define IDX_SIGNAL_APPROACH_BIT_2		11
#define IDX_SIGNAL_APPROACH_BIT_3		12


//----------------------------------------------------------------------
//	index into IN address array
//	to use when call function GetInAddress()
//
#define IDX_ADR_LED_A				0
#define IDX_ADR_LED_B				1
#define IDX_ADR_LED_C				2
#define IDX_ADR_LED_D				3
#define IDX_ADR_LED_E				4
#define IDX_ADR_LED_F				5
#define IDX_ADR_LED_G				6
#define IDX_ADR_LED_H				7
#define IDX_ADR_LED_I				8
#define IDX_ADR_LED_J				9
#define IDX_ADR_LED_K				10
#define IDX_ADR_LED_L				11
#define IDX_ADR_LED_M				12
#define IDX_ADR_LED_N				13
#define IDX_TRACK_CONTACT			(NUMBER_LIGHTS)
#define IDX_NIGHTLIGHT				(NUMBER_LIGHTS + 1)
#define IDX_APPROACH_DARK			(NUMBER_LIGHTS + 2)
#define IDX_ERSATZ_SIGNAL_FLASH		(NUMBER_LIGHTS + 3)


////////////////////////////////////////////////////////////////////////
//	CLASS:	LncvStorageClass
//
class LncvStorageClass
{
	public:
		//----------------------------------------------------------
		//	Constructor
		//
		LncvStorageClass();

		void CheckEEPROM( uint16_t uiVersionNumber );
		void Init( void );

		uint16_t GetFadeTimeUp(   uint8_t idx );
		uint16_t GetFadeTimeDown( uint8_t idx );

		bool IsValidLNCVAddress( uint16_t uiAddress );
		uint16_t ReadLNCV(  uint16_t uiAddress );
		void	 WriteLNCV( uint16_t uiAddress, uint16_t uiValue );
};


//==========================================================================
//
//		E X T E R N   G L O B A L   V A R I A B L E S
//
//==========================================================================

extern LncvStorageClass		g_clLncvStorage;
