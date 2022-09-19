
#pragma once

//##########################################################################
//#
//#		macros.h
//#
//#	This file contains some usefull macros.
//#
//#-------------------------------------------------------------------------
//#
//#	File Version:	2		from: 19.09.2022
//#
//#	Implementation:
//#		-	add macros for set and clear a bit in a word
//#				ClearWordBit()
//#				SetWordBit()
//#
//#-------------------------------------------------------------------------
//#
//#	File Version:	1		from: 23.05.2022
//#
//#	Implementation:
//#		-	first working version
//#
//##########################################################################


//==========================================================================
//
//		M A C R O S
//
//==========================================================================

#ifndef cbi
#define cbi(sfr, bit)				(_SFR_BYTE(sfr) &= ~_BV( bit ))
#endif

#ifndef sbi
#define sbi(sfr, bit)				(_SFR_BYTE(sfr) |= _BV( bit ))
#endif

#ifndef ClearBit
#define ClearBit( theByte, bit )	(theByte &= ~_BV( bit ))
#endif

#ifndef SetBit
#define SetBit( theByte, bit )		(theByte |= _BV( bit ))
#endif

#ifndef ClearWordBit
#define ClearWordBit( theWord, bit )	(theWord &= ~(1 << bit))
#endif

#ifndef SetWordBit
#define SetWordBit( theWord, bit )		(theWord |= (1 << bit))
#endif
