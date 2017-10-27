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
/* Header for class be_ac_ulb_gpib_GPIBDriver */

#ifndef _Included_be_ac_ulb_gpib_GPIBDriver
#define _Included_be_ac_ulb_gpib_GPIBDriver
#ifdef __cplusplus
extern "C" {
#endif

/*
 * Class:     be_ac_ulb_gpib_GPIBDriver
 * Method:    scanDevices
 * Signature: (B)V
 */
JNIEXPORT void JNICALL Java_be_ac_ulb_gpib_GPIBDriver_scanDevices
  (JNIEnv *, jobject, jboolean);


/*
 * Class:     be_ac_ulb_gpib_GPIBDriver
 * Method:    scanDevicesClear
 * Signature: (B,B)V
 */
JNIEXPORT void JNICALL Java_be_ac_ulb_gpib_GPIBDriver_scanDevicesClear
  (JNIEnv *, jobject, jboolean, jboolean);

/*
 * Class:     be_ac_ulb_gpib_GPIBDriver
 * Method:    resetControllerImpl
 * Signature: (I)V
 */
JNIEXPORT jint JNICALL Java_be_ac_ulb_gpib_GPIBDriver_resetControllerImpl
  (JNIEnv *, jobject, jint);

/*
 * Class:     be_ac_ulb_gpib_GPIBDriver
 * Method:    clearDeviceImpl
 * Signature: (I)V
 */
JNIEXPORT jint JNICALL Java_be_ac_ulb_gpib_GPIBDriver_clearDeviceImpl
  (JNIEnv *, jobject, jint);

/*
 * Class:     be_ac_ulb_gpib_GPIBDriver
 * Method:    localDeviceImpl
 * Signature: (I)V
 */
JNIEXPORT jint JNICALL Java_be_ac_ulb_gpib_GPIBDriver_localDeviceImpl
  (JNIEnv *, jobject, jint);

/*
 * Class:     be_ac_ulb_gpib_GPIBDriver
 * Method:    remoteDeviceImpl
 * Signature: (I)V
 */
JNIEXPORT jint JNICALL Java_be_ac_ulb_gpib_GPIBDriver_remoteDeviceImpl
  (JNIEnv *, jobject, jint);

/*
 * Class:     be_ac_ulb_gpib_GPIBDriver
 * Method:    openDeviceImpl
 * Signature: (I,I,F)I
 */
JNIEXPORT jint JNICALL Java_be_ac_ulb_gpib_GPIBDriver_openDeviceImpl
  (JNIEnv *, jobject, jint, jint, jfloat);


/*
 * Class:     be_ac_ulb_gpib_GPIBDriver
 * Method:    openDeviceClearImpl
 * Signature: (I,I,F,B)I
 */
JNIEXPORT jint JNICALL Java_be_ac_ulb_gpib_GPIBDriver_openDeviceClearImpl
  (JNIEnv *, jobject, jint, jint, jfloat, jboolean);


/*
 * Class:     be_ac_ulb_gpib_GPIBDriver
 * Method:    sendCommandImpl
 * Signature: (Ljava/lang/String;I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_be_ac_ulb_gpib_GPIBDriver_sendCommandImpl
  (JNIEnv *, jobject, jstring, jint);

/*
 * Class:     be_ac_ulb_gpib_GPIBDriver
 * Method:    sendCommandBinImpl
 * Signature: (Ljava/lang/String;I)Ljava/lang/String;
 */
JNIEXPORT jbyteArray JNICALL Java_be_ac_ulb_gpib_GPIBDriver_sendCommandBinImpl
  (JNIEnv *, jobject, jstring, jint);

/*
 * Class:     be_ac_ulb_gpib_GPIBDriver
 * Method:    writeCommandImpl
 * Signature: (Ljava/lang/String;I)V
 */
JNIEXPORT jint JNICALL Java_be_ac_ulb_gpib_GPIBDriver_writeCommandImpl
  (JNIEnv *, jobject, jstring, jint);

/*
 * Class:     be_ac_ulb_gpib_GPIBDriver
 * Method:    writeCommandImpl
 * Signature: (Ljava/lang/String;I)V
 */
JNIEXPORT jint JNICALL Java_be_ac_ulb_gpib_GPIBDriver_writeCommandDataImpl
	(JNIEnv *, jobject, jstring, jbyteArray, jint);


/*
 * Class:     be_ac_ulb_gpib_GPIBDriver
 * Method:    getMaxBufferLengthImpl
 * Signature: (Ljava/lang/String;I)Ljava/lang/String;
 */
JNIEXPORT jint JNICALL Java_be_ac_ulb_gpib_GPIBDriver_getMaxBufferLengthImpl
  (JNIEnv *, jobject);


/*
 * Class:     be_ac_ulb_gpib_GPIBDriver
 * Method:    getDLLVersionImpl
 * Signature: (Ljava/lang/String;I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_be_ac_ulb_gpib_GPIBDriver_getDLLVersionImpl
  (JNIEnv *, jobject);




#ifdef __cplusplus
}
#endif
#endif
