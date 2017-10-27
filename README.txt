JPIB is a GPIB API for JAVA2 platforms.
It is maintained by the Informatics and Networks Engineering Dpt. at the Universite Libre de Bruxelles, Belgium.

Source code is maintained by Jean-Michel DRICOT <jdricot@ulb.ac.be> and is
freely available under the LGPL public license. 

Latest packages stable releases can be found at <https://sourceforge.net/projects/jpib/>

========================================================================================

Version 1.3.2.0
Date	2016.12.22
Author	Ralf Tralow
Bugs:
	- two extended commands: (because on some old devices, the clear command also resets the settings on the frontpanel)
		1. Java_be_ac_ulb_gpib_GPIBDriver_scanDevicesClear
		2. Java_be_ac_ulb_gpib_GPIBDriver_openDeviceClearImpl

Version 1.3.1.0
Date	2016.12.14
Author	Ralf Tralow
Bugs:
	- "java.library.path" is now only set internaly on linux systems

Version 1.3.0.0
Date	2016.11.24
Author	Ralf Tralow
Bugs:
	- Program stalls, when using the adapter Agilent 82357B
	- Remote/Local-Line of the controller is set correctly
Changes:
	- makefile for linux corrected (libraries can be compiled, but not yet tested; I can't execute updateNIDrivers)
	- the detection for the used os is now handled by the driver himselve; no longer needed in the application that loads the driver
New:
	- Search on the first 32 controllers (not only controller 0)
	- Reset command implemented
	
Version 1.2.1.0
Date	2012.03.25
Author	Ralf Tralow
Bugs:			
Changes:
	- GPIBDeviceIdentifier now overloaded from GPIBDevice
New:
	- Additional command writeCommandData (for sending binary data)
	- Java driver interface extended:
	- Listener for readed bytes (for example to update a progressBar)
	- isAsciiPossible and isBinaryPossible: For drivers that are based on JPIB, but eventually can transfer only ASCII									
	- getControllerName: to get the alias name of the controller
	
Version 1.2.0.1
Date	2012.01.21
Author	Ralf Tralow
Changes;
	- writeCommand changed (es wird kein gesondertes Commando gesendet, um auf den Abschluss zu warten; funktioniert sowieso nicht bei sehr alten Ger?ten)
	- scanDevices um Parameter erweitert; (alte Devices melden sich sowieso nicht bei *IDN? Also optional jetzt anzugeben)

Version 1.2.0.0
Date	2012.01.21
Author	Ralf Tralow
Changes:
	- Driver available for Windows 32/64 bit (Linux driver not compiled and not testet)
	- Additional command "sendCommandBin" (special for receive binary data)
	- Additional command "localDevice" (set device to local)
	- Additional command "remoteDevice" (set device to remote)
	- Additional parameter "Timeout" for "openDevice"
	- Buffer expanded to 30000
	- Additional command "getMaxBufferLength" (how much data can received at once from the device)
	- Additional command "getDLLVersion"
	