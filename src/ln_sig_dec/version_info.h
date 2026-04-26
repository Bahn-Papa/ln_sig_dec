
#pragma once

//##########################################################################
//#
//#		version_info.h
//#
//#	This file contains the project/change history and some definitions
//#	used for compile options:
//#
//#		-	ARTICLE_NUMBER
//#			defines for which board version the software will be compiled.
//#			(differences in the I/O assignment)
//#			normally the definition will come from the IDE
//#			(selecting the right board)
//#			article number		processor
//#				1515			ATmega328P, 16 MHz
//#
//#		-	VERSION_MAIN
//#			the main software version
//#			will change when new development or bigger changes were made
//#
//#		-	VERSION_MINOR
//#			the sub software version
//#			will change by bug fixes and smaller changes in the software
//#
//#		-	VERSION_NUMBER
//#			is calculated from VERSION_MAIN and VERSION_MINOR and
//#			stored in LNCV #3 as a decimal number (to make it readable)
//#
//#-------------------------------------------------------------------------
//#
//#	File version:	1		vom: 01.04.2026
//#
//#	Implementation:
//#		-	initial version
//#
//##########################################################################


//==========================================================================
//
//		C O D E   S E L E C T I O N   D E F I N I T I O N S
//
//==========================================================================

//#define WRITE_TEST_VALUES		1

//----------------------------------------------------------------------
//	my artikle number
#ifndef ARTICLE_NUMBER
	#define ARTICLE_NUMBER		1515
#endif

#define VERSION_MAIN			14
#define	VERSION_MINOR			1

#define VERSION_NUMBER			((VERSION_MAIN * 100) + VERSION_MINOR)


//##########################################################################
//#
//#		Version History:
//#
//#-------------------------------------------------------------------------
//#
//#	Version:	14.01		from: 01.04.2026
//#
//#	Implementation:
//#		-	change the handling of different hardware (platine) versions
//#			before it was a fixed definition (PLATINE_VERSION)
//#			now it is a definition set by the IDE during compile time
//#			(ARTICLE_NUMBER)
//#		-	change the format of the software version number
//#			removed the hardware part form the version number, so now
//#			the format is VVVmm with
//#				VVV = VERSION_MAIN and
//#				mm  = VERSION_MINOR
//#		-	renamed file compile_options.h to version_info.h
//#		-	moved the project/change history from ln_sig_dec.ino
//#			to version_info.h
//#		-	moved version definitions from ln_sig_dec.ino to version_info.h
//#		-	changes in files
//#				ln_sig_dec.ino
//#				lncv_storage.cpp, lncv_storage.h
//#				my_loconet.cpp
//#
//#-------------------------------------------------------------------------
//#
//#	Version:	x.14.00		from: 18.03.2026
//#
//#	Implementation:
//#		-	switch to firmware update over loconet
//#			change in file
//#				my_loconet.cpp
//#			new files
//#				FirmwareUpdateMessage.cpp, FirmwareUpdateMessage.h
//#
//#-------------------------------------------------------------------------
//#
//#	Version:	x.13.09		from: 31.10.2025
//#
//#	Bug Fix:
//#		-	to witch off Ersatzsignal when signal is in Hp0 a sequence to
//#			do that was missing. Add the sequence Main_Hp0_to_Hp0
//#			changes in files
//#				sequencer_tables.h
//#				transition.cpp
//#
//#-------------------------------------------------------------------------
//#
//#	Version:	x.13.08		from: 09.10.2025
//#
//#	Bug Fix:
//#		-	Ersatzsignal will be switched off now.
//#			change in file
//#				transition.cpp
//#		-	the green LED indicating a running sequence will be switched
//#			off at correct moment/place now
//#			change in files
//#				transition.cpp
//#				io_control.cpp
//#
//#-------------------------------------------------------------------------
//#
//#	Version:	x.13.07		from: 28.05.2025
//#
//#	Bug Fix:
//#		-	extra signals will now be handled correct during 'ON'
//#			and 'OFF' sequences
//#			change in file
//#				my_loconet.cpp
//#
//#-------------------------------------------------------------------------
//#
//#	Version:	x.13.06		from: 18.05.2025
//#
//#	Implementation:
//#		-	add a watchdog to force the fade of the LEDs to stop
//#			change in files
//#				io_control.h
//#				io_control.cpp
//#
//#-------------------------------------------------------------------------
//#
//#	Version:	x.13.05		from: 18.05.2025
//#
//#	Implementation:
//#		-	add function to check if a main signal is present
//#		-	add switch off some extra signals if the target signal aspect
//#			is Hp0
//#			change in files
//#				io_control.h
//#				transition.cpp
//#
//#-------------------------------------------------------------------------
//#
//#	Version:	x.13.04		from: 18.05.2025
//#
//#	Bug Fix:
//#		-	do not start transitions for US signal types if the target
//#			signal aspect is the same as the actual signal aspect
//#
//#-------------------------------------------------------------------------
//#
//#	Version:	x.13.03		from: 07.02.2025
//#
//#	Implementation:
//#		-	add the possibility to delete a bit of a signal aspect
//#
//#-------------------------------------------------------------------------
//#
//#	Version:	x.13.02		from: 06.02.2025
//#
//#	Implementation:
//#		-	add saving of the actual US signal mode
//#
//#-------------------------------------------------------------------------
//#
//#	Version:	x.13.01		from: 04.02.2025
//#
//#	Implementation:
//#		-	change handling of switching US signal types from track contact
//#			to button pressing
//#			change in function
//#				loop()
//#
//#-------------------------------------------------------------------------
//#
//#	Version:	x.13.00		from: 02.02.2025
//#
//#	Implementation:
//#		-	add support for US signals
//#
//#-------------------------------------------------------------------------
//#
//#	Version:	x.12.02		from: 30.09.2024
//#
//#	Bug Fix:
//#		-	improve handling of writing LNCVs
//#				don't write '0' or '0xFFFF' as module address
//#
//#-------------------------------------------------------------------------
//#
//#	Version:	x.12.01		from: 30.09.2024
//#
//#	Bug Fix:
//#		-	improve handling of brightness fade
//#
//#-------------------------------------------------------------------------
//#
//#	Version:	x.12.00		from: 14.08.2024
//#
//#	Implementation:
//#		-	improvement of approach kennlicht special handling
//#
//#-------------------------------------------------------------------------
//#
//#	Version:	x.11.00		from: 14.08.2024
//#
//#	Implementation:
//#		-	improvement of sequence handling
//#
//#-------------------------------------------------------------------------
//#
//#	Version:	x.10.02		from: 12.08.2024
//#
//#	Bug Fix:
//#		-	when approach signal will be switched off then it should
//#			go to signal aspect SIGNAL_ASPECT_HP0
//#
//#-------------------------------------------------------------------------
//#
//#	Version:	x.10.01		from: 09.08.2024
//#
//#	Bug Fix:
//#		-	handling of extra signals during a sequence was wrong
//#			every time all signals were handled where only a part should be
//#
//#-------------------------------------------------------------------------
//#
//#	Version:	x.10.00		from: 05.08.2024
//#
//#	Implementation:
//#		-	improve selection of main signal aspects. Now, only one
//#			message is needed to start a signal aspec sequence
//#
//#-------------------------------------------------------------------------
//#
//#	Version:	x.09.00		from: 05.08.2024
//#
//#	Implementation:
//#		-	add a new signal type SIGNAL_TYPE_APPROACH
//#
//#-------------------------------------------------------------------------
//#
//#	Version:	x.08.01		from: 31.07.2024
//#
//#	Bug Fix:
//#		-	extra signals were not switched during approach signal handling
//#
//#-------------------------------------------------------------------------
//#
//#	Version:	x.08.00		from: 18.02.2024
//#
//#	Implementation:
//#		-	improve handling of extra signals
//#		-	improvement of notify messages
//#			don't react on notifySwitchReport and notifySwitchState
//#			to avoid missinterpretation of requests.
//#
//#-------------------------------------------------------------------------
//#
//#	Version:	x.07.04		from: 07.11.2023
//#
//#	Bug Fix:
//#		-	switch off green LED was too late in class 'my_loconet'
//#		-	change handling of brightness and fade in class 'io_control'
//#			first check step, then period
//#
//#-------------------------------------------------------------------------
//#
//#	Version:	x.07.03		from: 26.07.2023
//#
//#	Bug Fix:
//#		-	by mistake no acknowledge was send when receiving a programming
//#			start with broadcast as address.
//#
//#-------------------------------------------------------------------------
//#
//#	Version:	x.07.02		from: 18.07.2023
//#
//#	Bug Fix:
//#		-	do not go into prog mode when a programming start msg
//#			with broadcast address was detected
//#
//#-------------------------------------------------------------------------
//#
//#	Version:	x.07.01		from: 11.07.2023
//#
//#	Implementation:
//#		-	improve handling when two transitions should be processed at
//#			the same time
//#
//#-------------------------------------------------------------------------
//#
//#	Version:	x.07.00		from: 08.06.2023
//#
//#	Implementation:
//#		-	add handling if no main signal is present
//#
//#-------------------------------------------------------------------------
//#
//#	Version:	x.06.04		from: 04.06.2023
//#
//#	Bug Fix:
//#		-	do not go into prog mode when a discover msg was detected
//#
//#-------------------------------------------------------------------------
//#
//#	Version:	x.06.03		from: 18.05.2023
//#
//#	Bug Fix:
//#		-	change input pin for track contact from PB2 to PB3
//#
//#-------------------------------------------------------------------------
//#
//#	Version:	x.06.02		from: 30.03.2023
//#
//#	Implementation:
//#		-	add handling for a 5th rangier signal
//#
//#-------------------------------------------------------------------------
//#
//#	Version:	x.06.01		from: 24.03.2023
//#
//#	Implementation:
//#		-	add handling for rangier signal aspects
//#
//#-------------------------------------------------------------------------
//#
//#	Version:	x.06.00		from: 23.03.2023
//#
//#	Implementation:
//#		-	introducing transitions to easy implement the control of
//#			different amount of signals
//#
//#-------------------------------------------------------------------------
//#
//#	Version:	x.05.02		from: 16.03.2023
//#
//#	Bug Fix:
//#		-	switching between night and day
//#
//#-------------------------------------------------------------------------
//#
//#	Version:	x.05.01		from: 06.03.2023
//#
//#	Bug Fix:
//#		-	problem after change of module address fixed
//#
//#-------------------------------------------------------------------------
//#
//#	Version:	x.05.00		from: 02.02.2023
//#
//#	Implementation:
//#		-	new possibilities in the sequences
//#
//#-------------------------------------------------------------------------
//#
//#	Version:	x.04.10		from: 01.02.2023
//#
//#	Bug Fix:
//#		-	could not switch off signal aspect
//#
//#-------------------------------------------------------------------------
//#
//#	Version:	x.04.09		from: 27.01.2023
//#
//#	Implementation:
//#		-	add flashing of Ersatzsignal
//#
//#-------------------------------------------------------------------------
//#
//#	Version:	x.04.08		from: 17.01.2023
//#
//#	Implementation:
//#		-	add new possibilities for the sequence control
//#				pause		the sequence will pause for 200ms
//#				flash LED	in addition to fade the LEDs of a signal
//#							it is now possible to flash the LEDs
//#
//#-------------------------------------------------------------------------
//#
//#	Version:	x.04.07		from: 14.12.2022
//#
//#	Implementation:
//#		-	add track contact delay time
//#			when the track contact gets free then a timer is started
//#			to delay the sending of the new state.
//#			If the contact gets occupied again during the delay time
//#			than the timer will be reset.
//#
//#-------------------------------------------------------------------------
//#
//#	Version:	x.04.06		from: 12.12.2022
//#
//#	Implementation:
//#		-	add handling of the track contact
//#
//#-------------------------------------------------------------------------
//#
//#	Version:	x.04.05		from: 29.11.2022
//#
//#	Bug Fix:
//#		-	LED handling blocked if a LED was switched to the actual
//#			state again.
//#
//#-------------------------------------------------------------------------
//#
//#	Version:	x.04.04		from: 28.11.2022
//#
//#	Bug Fix:
//#		-	in function 'ProcessStateMachine()' the call to change the
//#			approach aspect to Vr0 had a parameter mismatch.
//#
//#-------------------------------------------------------------------------
//#
//#	Version:	x.04.03		from: 25.11.2022
//#
//#	Implementation:
//#		-	ignore handling of approach signal if there is no
//#			approach signal
//#			new variable	m_bApproachSignalPresent
//#		-	set the correct LEDs to Hp0 during Init()
//#			change function
//#				SetSignalType()	the function will return 'true' if the
//#								signal is a type of 'A-Sig'
//#		-	include function GetFirstStepApproach() into function
//#			StartAspectTransitionApproach()
//#
//#-------------------------------------------------------------------------
//#
//#	Version:	x.04.02		from: 25.11.2022
//#
//#	Implementation:
//#		-	new use of LED_GREEN and LED_BLUE for debugging:
//#				LED_GREEN:	wait time for signal aspect is active
//#							signal aspect transition is active
//#				LED_BLUE:	fading is active
//#		-	removed LED_N for debugging purpose
//#
//#-------------------------------------------------------------------------
//#
//#	Version:	x.04.01		from: 22.10.2022
//#
//#	Bug Fix:
//#		-	switch the correct LEDs for approch signal
//#		-	correction of access to the sequences
//#
//#-------------------------------------------------------------------------
//#
//#	Version:	x.04.00		from: 22.10.2022
//#
//#	Implementation:
//#		-	rework of aspect sequences
//#			split for main aspects and approach aspects
//#
//#-------------------------------------------------------------------------
//#
//#	Version:	x.03.00		from: 12.10.2022
//#
//#	Implementation:
//#		-	implement signal aspects for the approach signal
//#
//#-------------------------------------------------------------------------
//#
//#	Version:	x.02.00		from: 11.10.2022
//#
//#	Implementation:
//#		-	implementation of signal aspects for signal type 59 finished
//#
//#-------------------------------------------------------------------------
//#
//#	Version:	x.01.02		from: 23.09.2022
//#
//#	Implementation:
//#		-	add checks to avoid a crash by wrong commands or parameters
//#		-	simplify sequencer tables
//#		-	only write to EEPROM if the new value is different
//#			than the old one
//#
//#-------------------------------------------------------------------------
//#
//#	Version:	x.01.01		from: 22.09.2022
//#
//#	Implementation:
//#		-	add version number into EEPROM
//#		-	initialize the signal with Hp0
//#		-	implement signal aspects for signal type '59'
//#
//#	Bug Fix:
//#		-	green LED will blink now in prog mode
//#
//#-------------------------------------------------------------------------
//#
//#	Version:	x.01.00		from: 17.09.2022
//#
//#	Implementation:
//#		-	first test version
//#
//##########################################################################
