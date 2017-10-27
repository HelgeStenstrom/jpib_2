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
 Changed by Ralf Tralow in 2012.01.21
 See "JPIB_Changelog"
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <windows.h>
#include <dos.h>
#include "include\ni4882.h"
#include "be_ac_ulb_gpib_WindowsGPIBDriver.h"
#include "stdgpib.h"


#define VERSIONSINFO	"1.2.1.0"

#define GPIB_BUFFER_SIZE (30000l)


void throwException (JNIEnv *env, char *msg)
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

//	buf[0] = '\0';
//    strcat_s(buf, sizeof(buf), msg);
//    strcat_s(buf, sizeof(buf), ErrorMnemonic[iberr]);
	sprintf_s (buf, sizeof(buf), "JPIB C++: %s [%s]", msg, ErrorMnemonic[iberr]);
	env->ThrowNew (clazz, buf);   

}


JNIEXPORT void JNICALL Java_be_ac_ulb_gpib_WindowsGPIBDriver_scanDevices
	(JNIEnv *env, jobject obj, jboolean getIDN)
	{

		//printf("\r\nDebug: In scanDevices\r\n");

	jclass cls = env->GetObjectClass(obj);
    	jmethodID mid = env->GetMethodID(cls, "addScannedDevice", "(ILjava/lang/String;)V");
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

	// JLP:
	ibonl (0, 0); 
	// Reset whole bus
	SendIFC (GPIB_BUS_ZERO);

	// JLP: This configures us as system controller (See NI.com docs)
	ibrsc (GPIB_BUS_ZERO, 1);
	ibsic (GPIB_BUS_ZERO);	// Assert IFC for 100us to clear bus. This unaddresses all devices, and makes board CIC
	ibsre (GPIB_BUS_ZERO, 1); // Assert the Remote Enable line

	//Look for devices on all adresses. <ibcntl> will contain number of listeners found.
    FindLstn (GPIB_BUS_ZERO, padlist,resultlist, limit);

	if (ibsta & ERR)
    {
		printf ("Got error %x returned from FindLsnt...");
	}

	for (i=0;(unsigned long)i<ibcntl;i++) 
	{
		////printf("Addr %d: %d\r\n", i, resultlist[i]);
	}


	// Identify each device detected on the bus
	for (i=ibcntl-1;i>=0;i--) 
	{
		// JLP: how did this ever work?
		int Dev = ibdev (0, resultlist[i], NO_SECONDARY_ADDR,TIMEOUT, EOTMODE, EOSMODE);
		if (ibsta & ERR)
	    {
			throwException(env," unable to open device ");
				return;
	    }
	
		// Clear the device for proper initialization
		ibclr (Dev);
	   	if (ibsta & ERR)
		{
	   		throwException(env," Unable to clear device ");
			return;
		}		
		// End JLP
		
		if( getIDN)
		{
			Send(GPIB_BUS_ZERO,resultlist[i],"*IDN?",5L,NLend);
			Receive(GPIB_BUS_ZERO,resultlist[i],buffer,GPIB_BUFFER_SIZE,NLend);
			buffer[ibcntl]='\0';
			jni_name=env->NewStringUTF(buffer);
		}
		else
		{
			jni_name=env->NewStringUTF("");
		}
		env->CallVoidMethod(obj, mid,resultlist[i],jni_name);

		// JLP: take the device back offline; it hasn't been opened yet.
		ibonl (Dev, 0);
	}

	delete padlist;
	delete resultlist;

	// Take the bus online
	ibonl(0, 1);
	};

JNIEXPORT jint JNICALL Java_be_ac_ulb_gpib_WindowsGPIBDriver_clearDeviceImpl
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

// TODO dont work
JNIEXPORT jint JNICALL Java_be_ac_ulb_gpib_WindowsGPIBDriver_localDeviceImpl
  (JNIEnv *env, jobject obj, jint descr)
	{
		ibsre (GPIB_BUS_ZERO, 0); // Clears the Remote Enable line
		if (ibsta & ERR)
		{
			throwException(env," unable to set device to local");
			return 0;
		}
		return 0;
	};

// TODO dont work
JNIEXPORT jint JNICALL Java_be_ac_ulb_gpib_WindowsGPIBDriver_remoteDeviceImpl
  (JNIEnv *env, jobject obj, jint descr)
	{
		ibsre (GPIB_BUS_ZERO, 1); // Assert the Remote Enable line
		if (ibsta & ERR)
		{
			throwException(env," unable to set device to remote");
			return 0;
		}
		return 0;
	};

JNIEXPORT jstring JNICALL Java_be_ac_ulb_gpib_WindowsGPIBDriver_sendCommandImpl
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
	ibwait(descr,CMPL);
	if ( ibsta&ERR ) 
	{ 
		throwException(env," while waiting for GPIB command completion event");
		return 0;
	} 

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


JNIEXPORT jbyteArray JNICALL Java_be_ac_ulb_gpib_WindowsGPIBDriver_sendCommandBinImpl
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
	ibwait(descr,CMPL);
	if ( ibsta&ERR ) 
	{ 
		throwException(env," while waiting for GPIB command completion event");
		return 0;
	} 

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


JNIEXPORT jint JNICALL Java_be_ac_ulb_gpib_WindowsGPIBDriver_getMaxBufferLengthImpl
  (JNIEnv *env, jobject obj)
	{
		return GPIB_BUFFER_SIZE;
	};


JNIEXPORT jstring JNICALL Java_be_ac_ulb_gpib_WindowsGPIBDriver_getDLLVersionImpl
  (JNIEnv *env, jobject obj)
	{
		return env->NewStringUTF(VERSIONSINFO);
	};


int scanForDevice (jint address)
{
	Addr4882_t padlist[3];
	Addr4882_t resultlist[3];
	padlist[1]=NOADDR;
	int limit=1;

	////printf("Debug: openDeviceImpl, addr %X\r\n", address);

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
 * Umrechnung von Sekunden in vordefinierte Werte für PCI-Adapter
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



/**
 *
 */
JNIEXPORT jint JNICALL Java_be_ac_ulb_gpib_WindowsGPIBDriver_openDeviceImpl
  (JNIEnv *env, jobject obj, jint address, jfloat timeout)
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

	//Java_be_ac_ulb_gpib_WindowsGPIBDriver_scanDevices (env, obj);
#endif // if SCAN_BUS_FOR_DEVICE

	// Get device descriptor (ala "file descriptor")
	Dev = ibdev (0, address, NO_SECONDARY_ADDR, calcTimoutValue(timeout), EOTMODE, EOSMODE);
	if (ibsta & ERR)
    		{
		throwException(env," unable to open device ");
			return -1;
    		}

	
	// Clear the device for proper initialization
	ibclr (Dev);
   	if (ibsta & ERR)
	{
   		throwException(env," Unable to clear device ");
		return -1;
	}
	////printf("Debug: openDeviceImpl done, Dev %X\r\n", Dev);

		return Dev;
	};

JNIEXPORT jint JNICALL Java_be_ac_ulb_gpib_WindowsGPIBDriver_writeCommandImpl
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

JNIEXPORT jint JNICALL Java_be_ac_ulb_gpib_WindowsGPIBDriver_writeCommandDataImpl
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
//	memcpy(buffer+length, (void*)carr, 1402 );
	length += env->GetArrayLength(data);
//	length += 1402;

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

