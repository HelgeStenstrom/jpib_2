/*
 * JAVA GPIB driver based on NI (www.ni.com) API.
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

package be.ac.ulb.gpib;

import java.io.IOException;

/**
 * 
 * An abstract GPIB driver interface that any driver implementation should implement.
 * 
 * @author Jean-Michel DRICOT
 * 
 *         changed by Ralf Tralow
 */

public interface AbstractGPIBDriver
{

	/**
	 * initialize() will be called by the GPIBDeviceIdentifier's static initializer.
	 * The responsibility of this method is:
	 * 		1) Ensure that the hardware is present.
	 * 		2) Load any required native libraries.
	 * 		3) Register the port names with the CommPortIdentifier.
	 *
	 * @param getIDN false=no *IDN? send to found devices, true=try to get IDN from devices
	 * @return true, if successful
	 */
	public abstract boolean initialize(boolean _getIDN);

	
	/**
	 * initialize() will be called by the GPIBDeviceIdentifier's static initializer.
	 * The responsibility of this method is:
	 * 		1) Ensure that the hardware is present.
	 * 		2) Load any required native libraries.
	 * 		3) Register the port names with the CommPortIdentifier.
	 *
	 * @param getIDN false=no *IDN? send to found devices, true=try to get IDN from devices
	 * @return true, if successful
	 */
	public abstract boolean initialize(boolean _getIDN, boolean _clearDevice);

	
	/**
	 * Gets the driver error.
	 * 
	 * @return the driver error
	 */
	public abstract String getDriverError();

	
	/**
	 * Gets a short controller name for identify in a combobox etc.
	 *
	 * @return the controller name
	 */
	public abstract String getControllerName();
	
	
	/**
	 * returns a GPIB device using its GPIB address. See IEEE488 and IEEE488.2 specifications
	 * 
	 * @param address
	 *          the GPIB address (from 0 to 31)
	 * @return the device
	 */
	public abstract GPIBDevice getDevice(int _address);

	/**
	 * sends a valid GPIB command to the device.
	 * 
	 * @param _message
	 *          the _message
	 * @param _descriptor
	 *          the _descriptor
	 * @return a String representing device's answer to the command
	 * @throws IOException
	 *           Signals that an I/O exception has occurred.
	 */
	public abstract String sendCommand(String _message, int _descriptor) throws IOException;

	/**
	 * sends a valid GPIB command to the device.
	 * 
	 * @param _message
	 *          the _message
	 * @param _descriptor
	 *          the _descriptor
	 * @return a byteArray representing device's answer to the command
	 * @throws IOException
	 *           Signals that an I/O exception has occurred.
	 */
	public abstract byte[] sendCommandBin(String _message, int _descriptor) throws IOException;

	/**
	 * sends a valid GPIB command to the device.
	 * 
	 * @param _message
	 *          the _message
	 * @param _descriptor
	 *          the _descriptor
	 * @throws IOException
	 *           Signals that an I/O exception has occurred.
	 */
	public abstract void writeCommand(String _message, int _descriptor) throws IOException;

	/**
	 * sends a valid GPIB command with binary data to the device.
	 *
	 * @param _message the _message
	 * @param _data the _data
	 * @param _descriptor the _descriptor
	 * @throws IOException Signals that an I/O exception has occurred.
	 */
	public abstract void writeCommandData(String _message, byte[] _data, int _descriptor) throws IOException;

	/**
	 * 
	 * 
	 * @param _controllerNumber
	 *          an unique integer referecing the controller <i>internally</i>. It is <i>not</i> the GPIB address.
	 * @throws IOException
	 *           Signals that an I/O exception has occurred.
	 */
	public abstract void resetController(int _controllerNumber) throws IOException;

	/**
	 * Clears a specific device.
	 * 
	 * @param _descriptor
	 *          an unique integer referecing the device <i>internally</i>. It is <i>not</i> necessarely the GPIB address.
	 * @throws IOException
	 *           Signals that an I/O exception has occurred.
	 */
	public abstract void clearDevice(int _descriptor) throws IOException;

	/**
	 * Opens a GPIB device present at specific address on the controller 0 and clears the device
	 *
	 * @param _address the GPIB address of the device
	 * @param _timeout the _timeout
	 * @return the int
	 * @throws IOException Signals that an I/O exception has occurred
	 */
	public abstract int openDevice(int _address, float _timeout) throws IOException;

	
	/**
	 * Opens a GPIB device present at specific address on a specific controller and clears the device
	 *
	 * @param controllerNumber the controller number
	 * @param _address the _address
	 * @param _timeout the _timeout
	 * @return the int
	 * @throws IOException Signals that an I/O exception has occurred.
	 */
	public abstract int openDevice(int _controllerNumber, int _address, float _timeout) throws IOException;

	
	/**
	 * Opens a GPIB device present at specific address on the controller 0.
	 *
	 * @param _address the GPIB address of the device
	 * @param _timeout
	 * @param _clearDevice open the device with/without clearing the device	
	 * @return the int
	 * @throws IOException Signals that an I/O exception has occurred
	 */
	public abstract int openDevice(int _address, float _timeout, boolean _clearDevice) throws IOException;

	
	/**
	 * Opens a GPIB device present at specific address on a specific controller.
	 *
	 * @param controllerNumber the controller number
	 * @param _address the _address
	 * @param _timeout the _timeout
	 * @param _clearDevice open the device with/without clearing the device	
	 * @return the int
	 * @throws IOException Signals that an I/O exception has occurred.
	 */
	public abstract int openDevice(int _controllerNumber, int _address, float _timeout, boolean _clearDevice) throws IOException;

	
	/**
	 * Close device
	 * 
	 * at the moment mainly used for COM port drivers
	 *
	 * @param controllerNumber the controller number
	 */
	public abstract void closeDevice(int controllerNumber);

	
	/**
	 * Set a specific device to Local mode.
	 * 
	 * @param _descriptor
	 *          an unique integer referecing the device <i>internally</i>. It is <i>not</i> necessarely the GPIB address.
	 * @throws IOException
	 *           Signals that an I/O exception has occurred.
	 */
	public abstract void localDevice(int _descriptor) throws IOException;

	/**
	 * Set a specific device to Remote mode.
	 * 
	 * @param _descriptor
	 *          an unique integer referecing the device <i>internally</i>. It is <i>not</i> necessarely the GPIB address.
	 * @throws IOException
	 *           Signals that an I/O exception has occurred.
	 */
	public abstract void remoteDevice(int _descriptor) throws IOException;

	/**
	 * Gets the max buffer length.
	 * 
	 * @return the max buffer length
	 * @throws IOException
	 *           Signals that an I/O exception has occurred.
	 */
	public abstract int getMaxBufferLength() throws IOException;

	/**
	 * Gets the dLL version.
	 * 
	 * @return the dLL version
	 * @throws IOException
	 *           Signals that an I/O exception has occurred.
	 */
	public abstract String getDLLVersion() throws IOException;
 

	/**
	 * Adds the listener.
	 *
	 * @param gpibDriverListener the gpib driver listener
	 */
	public abstract void addListener(GPIBDriverListener _gpibDriverListener);
	
	
	/**
	 * Removes the listener.
	 *
	 * @param gpibDriverListener the gpib driver listener
	 */
	public abstract void removeListener(GPIBDriverListener _gpibDriverListener);
	
	
	/**
	 * Checks if an ascii transmission is possible.
	 *
	 * @return true, if is ascii possible
	 */
	public abstract boolean isAsciiPossible();
	
	/**
	 * Checks if a binary transmission is possible.
	 *
	 * @return true, if is binary possible
	 */
	public abstract boolean isBinaryPossible();

	
	
}
