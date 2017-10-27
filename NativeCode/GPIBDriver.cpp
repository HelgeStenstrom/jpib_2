/*
* A GPIB driver based on NI (www.ni.com) API.
*  Copyright (C) 2001-2003 Jean-Michel DRICOT (jdricot@ulb.ac.be)
*
* This library is free software; you can redistribute it and/or
* modify it under the terms of the GNU Lesser General Public
* License as published by the Free Software Foundation; either
* version 2.1 of the License, or (at your option) any later version.
*
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with this library; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*
*/

/*
 Changed by Ralf Tralow since 2012.01.21
 See "JPIB_Changelog"
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#include <dos.h>
#endif
#include "include/ni4882.h"
#include "be_ac_ulb_gpib_GPIBDriver.h"
#include "stdgpib.h"


#ifdef ____
#elif _WIN32
#define	snprintf  sprintf_s
#else
#endif


#define VERSIONSINFO	"1.3.2.0"

#define GPIB_BUFFER_SIZE (30000l)


/*
 *
 */
void throwException (JNIEnv *env, const char *msg)
{
    char  buf[256];
    jclass clazz = env->FindClass ("java/io/IOException");

    if (!clazz)
    {
		////printf("clazz not found\r\n");
        env->ExceptionDescribe ();
        env->ExceptionClear ();
        return;
    }

	snprintf (buf, sizeof(buf), "JPIB C++: %s [%s]", msg, ErrorMnemonic[iberr]);
	env->ThrowNew (clazz, buf);   

}


/*
 * Scan for all devices on max 31 controllers with clearing the devices
 */
JNIEXPORT void JNICALL Java_be_ac_ulb_gpib_GPIBDriver_scanDevices
	(JNIEnv *env, jobject obj, jboolean getIDN)
{
	Java_be_ac_ulb_gpib_GPIBDriver_scanDevicesClear(env, obj, getIDN, true);
}


/*
 * Scan for all devices on max 31 controllers with/without clearing the devices
 * on some old devices, the clear command also resets the settings on the frontpanel
 */
JNIEXPORT void JNICALL Java_be_ac_ulb_gpib_GPIBDriver_scanDevicesClear
	(JNIEnv *env, jobject obj, jboolean getIDN, jboolean clearDevice)
{

	jclass cls = env->GetObjectClass(obj);
    	jmethodID mid = env->GetMethodID(cls, "addScannedDevice", "(ILjava/lang/String;I)V");	// methode: addScannedDevice(int, string, int)
    	jstring jni_name;

	Addr4882_t *padlist=new Addr4882_t[32];
	Addr4882_t *resultlist=new Addr4882_t[32];
	int limit=32;
	int i;
	char buffer[GPIB_BUFFER_SIZE];
	
	// Initialize looked up devices list
	for (i=0;i<MAXDEVICES_ON_BUS;i++)
	{
		padlist[i]=i+1;
		resultlist[i] = NOADDR; // Just to initialize...
	}
	padlist[MAXDEVICES_ON_BUS]=NOADDR;

/*
	// JLP:
	ibonl (0, 0); 
	// Reset whole bus
	SendIFC (GPIB_BUS_ZERO);

	// JLP: This configures us as system controller (See NI.com docs)
	ibrsc (0, 1);
	ibsic (0);	// Assert IFC for 100us to clear bus. This unaddresses all devices, and makes board CIC
	ibsre (0, 1); // Assert the Remote Enable line
*/
	for(int controller = 0; controller <= GPIB_BUS_MAX; controller++)
	{
		// Reset whole bus
		SendIFC (controller);
		if (ibsta & ERR)
		{
//				printf ("Got error 0x%x returned from SendIFC...\n", iberr);
		}

		//Look for devices on all adresses. <ibcntl> will contain number of listeners found.
		FindLstn (controller, padlist,resultlist, limit);
		if (ibsta & ERR)
		{
//				printf ("Got error 0x%x returned from FindLsnt...\n", iberr);
		}

		// Identify each device detected on the bus
		for (i=ibcntl-1;i>=0;i--) 
		{
			// JLP
			int Dev = ibdev (controller, resultlist[i], NO_SECONDARY_ADDR,TIMEOUT, EOTMODE, EOSMODE);
			if (ibsta & ERR)
			{
				throwException(env," unable to open device ");
					return;
			}

			if( clearDevice)
			{
				// Clear the device for proper initialization
				ibclr (Dev);
				if (ibsta & ERR)
				{
   					throwException(env," Unable to clear device ");
					return;
				}	
			}
			// End JLP
		
			if( getIDN)
			{
				Send(controller,resultlist[i],"*IDN?",5L,NLend);
				Receive(controller,resultlist[i],buffer,GPIB_BUFFER_SIZE,NLend);
				buffer[ibcntl]='\0';
				jni_name=env->NewStringUTF(buffer);
			}
			else
			{
				jni_name=env->NewStringUTF("");
			}
			env->CallVoidMethod(obj, mid,resultlist[i],jni_name, controller);

			// JLP: take the device back offline; it hasn't been opened yet.
			ibonl (Dev, 0);
		}
	}

	delete padlist;
	delete resultlist;

	// Take the bus online
//	ibonl(0, 1);
};


/*
 * Resets the controller board
 */
JNIEXPORT jint JNICALL Java_be_ac_ulb_gpib_GPIBDriver_resetControllerImpl
  (JNIEnv *env, jobject obj, jint board)
{
	// Asserts IFC (Interface Clear) signal. This re-initializes the GPIB system.
	ibsic (board);
	if (ibsta & ERR)
	{
		throwException(env," unable to clear board ");
		return 0;
	}
	return 0;
};


/*
 * Clears the device
 */
JNIEXPORT jint JNICALL Java_be_ac_ulb_gpib_GPIBDriver_clearDeviceImpl
  (JNIEnv *env, jobject obj, jint descr)
{
	ibclr (descr);
	if (ibsta & ERR)
	{
		throwException(env," unable to clear device ");
		return 0;
	}
	return 0;
};


/*
 * Set the device to local state
 */
JNIEXPORT jint JNICALL Java_be_ac_ulb_gpib_GPIBDriver_localDeviceImpl
  (JNIEnv *env, jobject obj, jint board)
{
	ibconfig (board, IbcSRE, 0); // Clears the Remote Enable line of the specified board
	if (ibsta & ERR)
	{
		throwException(env," unable to set board to local");
		return 0;
	}
	return 0;
};


/*
 * Sets the device to remote state
 */
JNIEXPORT jint JNICALL Java_be_ac_ulb_gpib_GPIBDriver_remoteDeviceImpl
  (JNIEnv *env, jobject obj, jint board)
{
	ibconfig (board, IbcSRE, 1); // Assert the Remote Enable line of the specified board
	if (ibsta & ERR)
	{
		throwException(env," unable to set board to remote");
		return 0;
	}
	return 0;
};


/*
 * Send a command to the device and returns the answer
 */
JNIEXPORT jstring JNICALL Java_be_ac_ulb_gpib_GPIBDriver_sendCommandImpl
  (JNIEnv *env, jobject obj, jstring command, jint descr)
{
	jstring jni_buffer;
	char buffer[GPIB_BUFFER_SIZE];
	
	const char *str = env->GetStringUTFChars(command, 0);
	
	//printf("Debug: descr is %0X, str %s, command %s\r\n", descr, str, command);
	// Tell the device to send a "command completion" event when ready to send result
	// ibwrt (descr, "*SRE 1",6L);
	// if (ibsta&ERR) { throwException(env," while requesting event generation "); }

	// Send the command
	ibwrt (descr, (void*)str,strlen(str));
	if (ibsta&ERR) 
	{ 
		throwException(env," while sending GPIB command "); 
		return 0;
	}

	// Wait for completion of command
//	ibwait(descr, CMPL);
//	ibwait(descr, CMPL | END | ERR | RQS | TIMO);
/*	if ( ibsta&ERR ) 
	{ 
		throwException(env," while waiting for GPIB command completion event");
		return 0;
	} 
*/
	// Read result
	ibrd (descr,buffer,GPIB_BUFFER_SIZE);
	if (ibsta&ERR) 
	{ 
		throwException(env," while reading GPIB command result "); 
		return 0;
	}

	// Prepare result for return
	buffer[ibcntl-1]='\0';
	jni_buffer=env->NewStringUTF(buffer);
	env->ReleaseStringUTFChars(command, str);

	return jni_buffer;
};


/*
 * Senda a binary command to the device and returns the answer
 */
JNIEXPORT jbyteArray JNICALL Java_be_ac_ulb_gpib_GPIBDriver_sendCommandBinImpl
  (JNIEnv *env, jobject obj, jstring command, jint descr)
{
	char buffer[GPIB_BUFFER_SIZE];
	
	const char *str = env->GetStringUTFChars(command, 0);
	
	//printf("Debug: descr is %0X, str %s, command %s\r\n", descr, str, command);
	// Tell the device to send a "command completion" event when ready to send result
	// ibwrt (descr, "*SRE 1",6L);
	// if (ibsta&ERR) { throwException(env," while requesting event generation "); }

	// Send the command
	ibwrt (descr, (void*)str,strlen(str));
	if (ibsta&ERR) 
	{ 
		throwException(env," while sending GPIB command "); 
		return 0;
	}

	// Wait for completion of command
//	ibwait(descr,CMPL);
//	ibwait(descr, CMPL | END | ERR | RQS | TIMO);
/*	if ( ibsta&ERR ) 
	{ 
		throwException(env," while waiting for GPIB command completion event");
		return 0;
	} 
*/
	// Read result
	ibrd (descr,buffer,GPIB_BUFFER_SIZE);
	if (ibsta&ERR) 
	{ 
		throwException(env," while reading GPIB command result "); 
		return 0;
	}

	// Prepare result for return
	int size = ibcntl-1;
	jbyteArray iarr = env->NewByteArray(size);
	env->SetByteArrayRegion(iarr, 0, size, (jbyte *) buffer);

	env->ReleaseStringUTFChars(command, str);

	return iarr;
};


/*
 * Returns the maximal buffer length
 */
JNIEXPORT jint JNICALL Java_be_ac_ulb_gpib_GPIBDriver_getMaxBufferLengthImpl
  (JNIEnv *env, jobject obj)
{
	return GPIB_BUFFER_SIZE;
};


/*
 * Returns the version of the DLL
 */
JNIEXPORT jstring JNICALL Java_be_ac_ulb_gpib_GPIBDriver_getDLLVersionImpl
  (JNIEnv *env, jobject obj)
{
	return env->NewStringUTF(VERSIONSINFO);
};


/*
 * 
 */
int scanForDevice (jint address)
{
	Addr4882_t padlist[3];
	Addr4882_t resultlist[3];
	padlist[1]=NOADDR;
	int limit=1;

	//printf("Debug: openDeviceImpl, addr %X\r\n", address);

	padlist[0] = (Addr4882_t)(unsigned short)address;
	padlist[1]=NOADDR;
	limit = 2;


	/*
	 *  Your board needs to be the Controller-In-Charge in order to find
	 *  all instrument on the GPIB.  To accomplish this, the function
	 *  SendIFC is called.  If the error bit ERR is set in ibsta, call
	 *  GPIBCleanup with an error message.
	 */
    SendIFC(GPIB_BUS_ZERO);
    if (ibsta & ERR)
    {
       //printf ("Unable to open board");
       //return 1;
    }

    FindLstn (GPIB_BUS_ZERO, padlist,resultlist, limit);
    if (ibsta & ERR)
    {
		//printf("Unable to call FindLsnt, err %x\r\n", ibsta);
       //throwException(env, "Unable to issue FindLstn call");
       //return 1;
    }	

	//ibcntl contains the actual number of addresses stored in the Result array. 
    if (ibcntl == 0)
    {
       return -1; 
    }

	// Take the bus online
	ibonl(0, 1);
	return 0;
}

/*
 * Conversion of seconds into predefined values for PCI adapters
 */
int calcTimoutValue(float timeout)
{
	int value = T10s;

	if( timeout <= 10e-6 )
	{
		value = T10us;
	}
	else
	if( timeout <= 30e-6 )
	{
		value = T30us;
	}
	else
	if( timeout <= 100e-6 )
	{
		value = T100us;
	}
	else
	if( timeout <= 300e-6 )
	{
		value = T300us;
	}
	else
	if( timeout <= 1e-3 )
	{
		value = T1ms;
	}
	else
	if( timeout <= 3e-3 )
	{
		value = T3ms;
	}
	else
	if( timeout <= 10e-3 )
	{
		value = T10ms;
	}
	else
	if( timeout <= 30e-3 )
	{
		value = T30ms;
	}
	else
	if( timeout <= 100e-3 )
	{
		value = T100ms;
	}
	else
	if( timeout <= 300e-3)
	{
		value = T300ms;
	}
	else
	if( timeout <= 1 )
	{
		value = T1s;
	}
	else
	if( timeout <= 3 )
	{
		value = T3s;
	}
	else
	if( timeout <= 10 )
	{
		value = T10s;
	}
	else
	if( timeout <= 30 )
	{
		value = T30s;
	}
	else
	if( timeout <= 100 )
	{
		value = T100s;
	}
	else
	if( timeout <= 300 )
	{
		value = T300s;
	}
	else
	if( timeout <= 1000 )
	{
		value = T1000s;
	}
	else
	if( timeout > 1000 )
	{
		value = TNONE;
	}

	return value;
}



/*
 * openDeviceImpl always with Clear ot the device
 */
JNIEXPORT jint JNICALL Java_be_ac_ulb_gpib_GPIBDriver_openDeviceImpl
  (JNIEnv *env, jobject obj, jint controllerNumber, jint address, jfloat timeout)
{
	return Java_be_ac_ulb_gpib_GPIBDriver_openDeviceClearImpl(env, obj, controllerNumber, address, timeout, true);
}


/*
 * openDeviceImpl with/without Clear of the device
 */
JNIEXPORT jint JNICALL Java_be_ac_ulb_gpib_GPIBDriver_openDeviceClearImpl
  (JNIEnv *env, jobject obj, jint controllerNumber, jint address, jfloat timeout, jboolean clearDevice)
{
	int Dev;

	// JLP: this seems to cause some problems... push up to java level.
	//#define SCAN_BUS_FOR_DEVICE
	#ifdef SCAN_BUS_FOR_DEVICE
	if (scanForDevice (address) != 0)
    {
		//printf("Could not find device.\r\n");
		char szTemp[64];
		sprintf_s (szTemp, sizeof(szTemp),"Could not find device at GPIB address %d.", address);
       throwException(env, szTemp);
       return 0; 
    }	

	//Java_be_ac_ulb_gpib_GPIBDriver_scanDevices (env, obj);
	#endif // if SCAN_BUS_FOR_DEVICE

	// Get device descriptor (ala "file descriptor")
	Dev = ibdev (controllerNumber, address, NO_SECONDARY_ADDR, calcTimoutValue(timeout), EOTMODE, EOSMODE);
	if (ibsta & ERR)
    {
		throwException(env," unable to open device ");
		return -1;
    }

	
	if( clearDevice )
	{
		// Clear the device for proper initialization
		ibclr (Dev);
   		if (ibsta & ERR)
		{
   			throwException(env," Unable to clear device ");
			return -1;
		}
	}

	return Dev;
};


/*
 * Write a command to the device without waiting for an answer
 */
JNIEXPORT jint JNICALL Java_be_ac_ulb_gpib_GPIBDriver_writeCommandImpl
  (JNIEnv *env, jobject obj, jstring command, jint descr)
{
	const char *str = env->GetStringUTFChars(command, 0);
	
/*
	// Tell the device to send a "command completion" event when ready to send result
	ibwrt (descr, "*SRE 1",6L);
	if (ibsta&ERR) 
	{ 
		throwException(env," while requesting event generation "); 
		return 0;	// Do not make other JNI calls after throwing exception; undefined behavior may result
	}
*/

	// Send the command
	ibwrt (descr, (void*)str,strlen(str));
	if (ibsta&ERR) 
	{ 
		throwException(env," while sending command "); 
		return 0;	// Do not make other JNI calls after throwing exception; undefined behavior may result
	}
/*
	// Wait for completion of command
	ibwait(descr,CMPL);
	if ( ibsta&ERR ) 
	{ 
		throwException(env," while waiting for command completion event");
		return 0;	// Do not make other JNI calls after throwing exception; undefined behavior may result
	} 
*/
	env->ReleaseStringUTFChars(command, str);
	return 1;
};


/*
 * Write a command and data to the device without waiting for an answer
 */
JNIEXPORT jint JNICALL Java_be_ac_ulb_gpib_GPIBDriver_writeCommandDataImpl
	(JNIEnv *env, jobject obj, jstring command, jbyteArray data, jint descr)
{
	int length = 0;
	char buffer[GPIB_BUFFER_SIZE];
	const char *str = env->GetStringUTFChars(command, 0);
	jbyte *carr = env->GetByteArrayElements(data, 0);
	
    if (carr == NULL)
	{
		env->ReleaseStringUTFChars(command, str);
        return 0; /* exception occurred */
    }

	length = strlen(str);
	memcpy(buffer, (void*)str, length);
	memcpy(buffer+length, (void*)carr, env->GetArrayLength(data) );
	length += env->GetArrayLength(data);

/*
	// Tell the device to send a "command completion" event when ready to send result
	ibwrt (descr, "*SRE 1",6L);
	if (ibsta&ERR) 
	{ 
		throwException(env," while requesting event generation "); 
		return 0;	// Do not make other JNI calls after throwing exception; undefined behavior may result
	}
*/

	// Send the commandv
	ibwrt (descr, (void*)buffer, length);
	if (ibsta&ERR) 
	{ 
		throwException(env," while sending command "); 
		return 0;	// Do not make other JNI calls after throwing exception; undefined behavior may result
	}
/*
	// Wait for completion of command
	ibwait(descr,CMPL);
	if ( ibsta&ERR ) 
	{ 
		throwException(env," while waiting for command completion event");
		return 0;	// Do not make other JNI calls after throwing exception; undefined behavior may result
	} 
*/
	env->ReleaseStringUTFChars(command, str);
	env->ReleaseByteArrayElements(data, carr, 0);
	return 1;
};

