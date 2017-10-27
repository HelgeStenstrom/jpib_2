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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "ib.h"
#include "be_ac_ulb_gpib_LinuxGPIBDriver.h"
#include "stdgpib.h"
#include "ni4882.h"



void throwException (JNIEnv *env, char *msg)
{
    char*    buf;
    jclass clazz = env->FindClass ("java/io/IOException");

    if (!clazz)
    {
        env->ExceptionDescribe ();
        env->ExceptionClear ();
        return;
    }
    buf=strcat(msg, ErrorMnemonic[iberr]);
    env->ThrowNew (clazz, buf);
}

JNIEXPORT void JNICALL Java_be_ac_ulb_gpib_LinuxGPIBDriver_scanDevices
  (JNIEnv *env, jobject obj)
	{

	jclass cls = env->GetObjectClass(obj);
    	jmethodID mid = env->GetMethodID(cls, "addScannedDevice", "(ILjava/lang/String;)V");
    	jstring jni_name;

	Addr4882_t *padlist=new Addr4882_t[32];
	Addr4882_t *resultlist=new Addr4882_t[32];
	int limit=32;
	int i;
	char buffer[100];
	
	// Initialize looked up devices list
	for (i=0;i<MAXDEVICES_ON_BUS;i++) padlist[i]=i+1;
	padlist[MAXDEVICES_ON_BUS]=NOADDR;

	// Reset whole bus
	SendIFC (GPIB_BUS_ZERO);

	//Look for devices on all adresses
        FindLstn (GPIB_BUS_ZERO, padlist,resultlist, limit);

	// Identify each device detected on the bus
	for (i=ibcntl-1;i>=0;i--) 
		{
		Send(GPIB_BUS_ZERO,resultlist[i],"*IDN?",5L,NLend);
		Receive(GPIB_BUS_ZERO,resultlist[i],buffer,100L,NLend);
		buffer[ibcntl]='\0';
		jni_name=env->NewStringUTF(buffer);
		env->CallVoidMethod(obj, mid,resultlist[i],jni_name);		
		}

	delete padlist;
	delete resultlist;

	// Take the bus online
	ibonl(0, 1);
	};

JNIEXPORT void JNICALL Java_be_ac_ulb_gpib_LinuxGPIBDriver_clearDeviceImpl
  (JNIEnv *env, jobject obj, jint descr)
	{
	ibclr (descr);
    	if (ibsta & ERR) throwException(env," unable to clear device ");
	};

JNIEXPORT jstring JNICALL Java_be_ac_ulb_gpib_LinuxGPIBDriver_sendCommandImpl
  (JNIEnv *env, jobject obj, jstring command, jint descr)
	{
	jstring jni_buffer;
	char buffer[100];
	
	const char *str = env->GetStringUTFChars(command, 0);
	
	// Tell the device to send a "command completion" event when ready to send result
	ibwrt (descr, "*SRE 1",6L);
	if (ibsta&ERR) { throwException(env," while requesting event generation "); }

	// Send the command
	ibwrt (descr, (void*)str,strlen(str));
	if (ibsta&ERR) { throwException(env," while sending command "); }

	// Wait for completion of command
	ibwait(descr,CMPL);
	if ( ibsta&ERR ) { throwException(env," while waiting for command completion event");} 

	// Read result
	ibrd (descr,buffer,100);
	if (ibsta&ERR) { throwException(env," while reading command result "); }

	// Prepare result for return
	buffer[ibcntl-1]='\0';
	jni_buffer=env->NewStringUTF(buffer);
	env->ReleaseStringUTFChars(command, str);

	return jni_buffer;
	};

JNIEXPORT jint JNICALL Java_be_ac_ulb_gpib_LinuxGPIBDriver_openDeviceImpl
  (JNIEnv *env, jobject obj, jint address)
	{
	int Dev;

	// Get device descritptor (ala "file descriptor")
	Dev = ibdev (0, address, NO_SECONDARY_ADDR,TIMEOUT, EOTMODE, EOSMODE);
	if (ibsta & ERR)
    		{
		throwException(env," unable to open device ");
       		Dev=-1;
    		}

	// Clear the device for proper initialization
	ibclr (Dev);
    	if (ibsta & ERR)
    		{
       		throwException(env," Unable to clear device ");
		Dev= -1;
    		}

	return Dev;
	};
