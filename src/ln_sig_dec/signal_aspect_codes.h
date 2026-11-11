#pragma once

//##########################################################################
//#
//#		signal_aspect_codes.h
//#
//#	The signal aspect codes defined here are used in the following cases:
//#		-	message to command a signal
//#		-	block message to send the signal state
//#
//#-------------------------------------------------------------------------
//#
//#		STOP_EMRED		Hp0, Hp00, Sh0
//#		SHUNT_GO		Sh1, Hp0 + Sh1		Rangieren erlaubt
//#		SIGNAL_DARK							Hauptschirm aus
//#		KENNLICHT		Ke					Kennlicht
//#		JOKER
//#		SUBSTITUTION	Zs1					Ersatzsignal
//#		COUNTERLINE		Zs8					Falschfahrt Auftragssignal
//#		CAUTIOUSNESS	Zs7					Vorsicht-Signal
//#
//#		STOP			Hp0, Hp00, Sh0
//#		GO_10			Hp2 + Zs3 ( 1 )
//#		GO_20			Hp2 + Zs3 ( 2 )
//#		GO_30			Hp2 + Zs3 ( 3 )
//#		GO_40			Hp2
//#		GO_50			Hp2 + Zs3 ( 5 )
//#		GO_60			Hp2 + Zs3 ( 6 )
//#		GO_70			Hp1 + Zs3 ( 7 )
//#		GO_80			Hp1 + Zs3 ( 8 )
//#		GO_90			Hp1 + Zs3 ( 9 )
//#		GO_100			Hp1 + Zs3 ( 10 )
//#		GO_110			Hp1 + Zs3 ( 11 )
//#		GO_120			Hp1 + Zs3 ( 12 )
//#		GO_130			Hp1 + Zs3 ( 13 )
//#		GO				Hp1
//#
//#-------------------------------------------------------------------------
//#
//#	File Version:	1		from: 31.12.2025
//#
//#	Implementation:
//#		-	first list of definitions
//#
//##########################################################################


//==========================================================================
//
//		D E F I N I T I O N S
//
//==========================================================================

#define SIGNAL_ASPECT_MSG_CODE					0xE4
#define SIGNAL_ASPECT_MSG_LENGTH				9

#define SIGNAL_ASPECT_CODE__UNDEFINED			0xFF

#define SIGNAL_ASPECT_CODE__SHUNT_GO			0x41
#define SIGNAL_ASPECT_CODE__SIGNAL_DARK			0x42
#define SIGNAL_ASPECT_CODE__KENNLICHT			0x43
#define SIGNAL_ASPECT_CODE__JOKER				0x44
#define SIGNAL_ASPECT_CODE__SUBSTITUTION		0x45
#define SIGNAL_ASPECT_CODE__COUNTERLINE			0x46
#define SIGNAL_ASPECT_CODE__CAUTIOUSNESS		0x47

//----	Hp0  ---------------------------------------------------
#define SIGNAL_ASPECT_CODE__STOP_EMRED			0x40
#define SIGNAL_ASPECT_CODE__STOP				0x00

//----	Hp2  ---------------------------------------------------
#define SIGNAL_ASPECT_CODE__GO_10				0x02
#define SIGNAL_ASPECT_CODE__GO_20				0x04
#define SIGNAL_ASPECT_CODE__GO_30				0x06
#define SIGNAL_ASPECT_CODE__GO_40				0x08
#define SIGNAL_ASPECT_CODE__GO_50				0x0A
#define SIGNAL_ASPECT_CODE__GO_60				0x0C

//----	Hp1  ---------------------------------------------------
#define SIGNAL_ASPECT_CODE__GO_70				0x0E
#define SIGNAL_ASPECT_CODE__GO_80				0x10
#define SIGNAL_ASPECT_CODE__GO_90				0x12
#define SIGNAL_ASPECT_CODE__GO_100				0x14
#define SIGNAL_ASPECT_CODE__GO_110				0x16
#define SIGNAL_ASPECT_CODE__GO_120				0x18
#define SIGNAL_ASPECT_CODE__GO_130				0x1A

#define SIGNAL_ASPECT_CODE__GO					0x3F


//==========================================================================
//
//		T Y P E   D E F I N I T I O N S
//
//==========================================================================

typedef struct
{
	uint8_t	command;
	uint8_t	mesg_size;
	uint8_t	addr_h;
	uint8_t addr_l;
	uint8_t	se_cmd;
	uint8_t	se_stat;
	uint8_t spd_ax;
	uint8_t spd_xa;
	uint8_t chksum;

} signal_aspect_t;
