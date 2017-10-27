NI-488.2 for Linux
Copyright National Instruments Corporation.
All Rights Reserved.

NOTE: BEFORE RUNNING MAKE, SEE THE FINAL PARAGRAPH OF THIS FILE.

The C sample programs illustrate how to use the NI-488.2 function
calls to communicate with your instrument.

The findinstr example uses the FindLstn function to detect
GPIB Listeners (devices) on the GPIB.  The GPIB board number
is hardcoded in the .c file and may need to be changed if you
have more than one GPIB interface in your system.

The devquery example uses basic device level calls to send a query
to an instrument on the GPIB.  The instrument address, GPIB board
number, data string, and other options are hardcoded in the .c
file and will need to be changed to suit your particular instrument.

To compile these examples, copy the .c and makefile to a directory
of your choice. This will ensure a clean uninstallation of the driver.
Use the "make" command in that directory in order to compile the example
programs against the default architecture of the operating system.
If you are on a 64 bit operating system and would like to compile 32 bit
example applications set the ARCHITECTURE parameter of the makefile to x86.
ex. "make ARCHITECTURE=x86"
