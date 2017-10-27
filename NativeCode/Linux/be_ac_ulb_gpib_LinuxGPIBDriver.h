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

#include "jni.h"
/* Header for class be_ac_ulb_gpib_LinuxGPIBDriver */

#ifndef _Included_be_ac_ulb_gpib_LinuxGPIBDriver
#define _Included_be_ac_ulb_gpib_LinuxGPIBDriver
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     be_ac_ulb_gpib_LinuxGPIBDriver
 * Method:    scanDevices
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_be_ac_ulb_gpib_LinuxGPIBDriver_scanDevices
  (JNIEnv *, jobject);

/*
 * Class:     be_ac_ulb_gpib_LinuxGPIBDriver
 * Method:    clearDeviceImpl
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_be_ac_ulb_gpib_LinuxGPIBDriver_clearDeviceImpl
  (JNIEnv *, jobject, jint);

/*
 * Class:     be_ac_ulb_gpib_LinuxGPIBDriver
 * Method:    openDeviceImpl
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_be_ac_ulb_gpib_LinuxGPIBDriver_openDeviceImpl
  (JNIEnv *, jobject, jint);

/*
 * Class:     be_ac_ulb_gpib_LinuxGPIBDriver
 * Method:    sendCommandImpl
 * Signature: (Ljava/lang/String;I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_be_ac_ulb_gpib_LinuxGPIBDriver_sendCommandImpl
  (JNIEnv *, jobject, jstring, jint);

#ifdef __cplusplus
}
#endif
#endif
