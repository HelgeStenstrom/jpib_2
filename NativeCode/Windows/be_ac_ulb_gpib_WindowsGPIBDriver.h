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
#include <jni.h>
/* Header for class be_ac_ulb_gpib_WindowsGPIBDriver */

#ifndef _Included_be_ac_ulb_gpib_WindowsGPIBDriver
#define _Included_be_ac_ulb_gpib_WindowsGPIBDriver
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     be_ac_ulb_gpib_WindowsGPIBDriver
 * Method:    scanDevices
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_be_ac_ulb_gpib_WindowsGPIBDriver_scanDevices
	(JNIEnv *, jobject, jboolean);

/*
 * Class:     be_ac_ulb_gpib_WindowsGPIBDriver
 * Method:    clearDeviceImpl
 * Signature: (I)V
 */
JNIEXPORT jint JNICALL Java_be_ac_ulb_gpib_WindowsGPIBDriver_clearDeviceImpl
  (JNIEnv *, jobject, jint);

/*
 * Class:     be_ac_ulb_gpib_WindowsGPIBDriver
 * Method:    localDeviceImpl
 * Signature: (I)V
 */
JNIEXPORT jint JNICALL Java_be_ac_ulb_gpib_WindowsGPIBDriver_localDeviceImpl
  (JNIEnv *, jobject, jint);

/*
 * Class:     be_ac_ulb_gpib_WindowsGPIBDriver
 * Method:    remoteDeviceImpl
 * Signature: (I)V
 */
JNIEXPORT jint JNICALL Java_be_ac_ulb_gpib_WindowsGPIBDriver_remoteDeviceImpl
  (JNIEnv *, jobject, jint);

/*
 * Class:     be_ac_ulb_gpib_WindowsGPIBDriver
 * Method:    openDeviceImpl
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_be_ac_ulb_gpib_WindowsGPIBDriver_openDeviceImpl
  (JNIEnv *, jobject, jint, jfloat);

/*
 * Class:     be_ac_ulb_gpib_WindowsGPIBDriver
 * Method:    sendCommandImpl
 * Signature: (Ljava/lang/String;I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_be_ac_ulb_gpib_WindowsGPIBDriver_sendCommandImpl
  (JNIEnv *, jobject, jstring, jint);

/*
 * Class:     be_ac_ulb_gpib_WindowsGPIBDriver
 * Method:    sendCommandImpl
 * Signature: (Ljava/lang/String;I)Ljava/lang/String;
 */
JNIEXPORT jbyteArray JNICALL Java_be_ac_ulb_gpib_WindowsGPIBDriver_sendCommandBinImpl
  (JNIEnv *, jobject, jstring, jint);

/*
 * Class:     be_ac_ulb_gpib_WindowsGPIBDriver
 * Method:    writeCommandImpl
 * Signature: (Ljava/lang/String;I)V
 */
JNIEXPORT jint JNICALL Java_be_ac_ulb_gpib_WindowsGPIBDriver_writeCommandImpl
  (JNIEnv *, jobject, jstring, jint);

/*
 * Class:     be_ac_ulb_gpib_WindowsGPIBDriver
 * Method:    writeCommandImpl
 * Signature: (Ljava/lang/String;I)V
 */
JNIEXPORT jint JNICALL Java_be_ac_ulb_gpib_WindowsGPIBDriver_writeCommandDataImpl
	(JNIEnv *, jobject, jstring, jbyteArray, jint);


/*
 * Class:     be_ac_ulb_gpib_WindowsGPIBDriver
 * Method:    getMaxBufferLengthImpl
 * Signature: (Ljava/lang/String;I)Ljava/lang/String;
 */
JNIEXPORT jint JNICALL Java_be_ac_ulb_gpib_WindowsGPIBDriver_getMaxBufferLengthImpl
  (JNIEnv *, jobject);


/*
 * Class:     be_ac_ulb_gpib_WindowsGPIBDriver
 * Method:    getDLLVersionImpl
 * Signature: (Ljava/lang/String;I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_be_ac_ulb_gpib_WindowsGPIBDriver_getDLLVersionImpl
  (JNIEnv *, jobject);




#ifdef __cplusplus
}
#endif
#endif
