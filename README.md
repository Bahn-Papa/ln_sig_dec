# FREMO signal decoder

This program will control one or more light signal(s).

The signal consists of a main signal, a distant signal and some additional indicators.
The lights are implemented using LEDs that slowly fade in and out.
The different aspects of the signal are controlled by Loconet messages.

## Building the program with Arduino IDE
First select _**FREMO Boards**_ from the menu **_Tools_ -> _board_ -> _Arduino AVR boards_**<br>
Then select the application **Signal Decoder (1515, ATmega328P  16 MHz)** from the menu **_Tools_ -> _Application (Article No)_**<br>
Now you can compile the application: select **_Sketch_ -> _Export compiled Binary_**<br>
This will compile the application and create two **.hex** files. One with and one without bootloader. The file without bootloader can be used to update the software over loconet.<br>
But you can still upload the application using a programmer.

**NOTE:** If you don't find the menu entries see below how to create them.

### Setup FREMO specific menu entries in Arduino IDE
To simplify the build process add FREMO specific menu entries to the _**Tools**_ menu.<br>
There are two scenarios:

- No **FREMO Boards** menu entry in **_Tools_ -> _board_ -> _Arduino AVR boards_**<br>
Just copy the file **boards.local.txt** from this repositories **ide** folder to the correct folder.<br>
Which folder is the correct one depends on the IDE installation.<br>
If you have a portable setup of the IDE you need to copy the file here:<br>
**...\portable\packages\arduino\hardware\avr\\\<avr version><br>**
Otherwise you need to copy it here:<br>
**...\hardware\arduino\avr**<br>
To verify if you found the correct folder: there should be the file **boards.txt** in there.

- No **Signal Decoder (1515, ATmega328P  16 MHz)** menu entry in **_Tools_ -> _Application (Article No)_**<br>
In this case you have to add the content of the file **boards-local-add-on.txt** from this repositories **ide** folder to the file **boards.local.txt**.<br>
Where to find the file **boards.local.txt** see above.

### Upload the application over Loconet
If you want to upload the application over Loconet you need the upload tool and the bootloader found in this repositories **ide** folder. Copy the bootloader into the **fremo** folder inside the **bootloaders** folder. Then use the Arduino IDE to flash the bootloader.
