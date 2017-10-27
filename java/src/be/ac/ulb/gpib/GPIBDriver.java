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
package be.ac.ulb.gpib;

import java.io.IOException;
import java.util.ArrayList;
import java.util.Enumeration;

import be.ac.ulb.tools.OSUtils;

/**
 * JAVA GPIB Driver for Windows platforms.
 *
 * @author Jean-Michel DRICOT
 * 
 * extended by Ralf Tralow
 */

public final class GPIBDriver implements AbstractGPIBDriver
{
	
	/** The gpib error. */
	private String gpibError = ""; //$NON-NLS-1$

	/** The listener list. */
	private ArrayList<GPIBDriverListener> listenerList = new ArrayList<GPIBDriverListener>();
	
	/*
	 * initialize() will be called by the GPIBDeviceIdentifier's static initializer.
	 * The responsibility of this method is:
	 * 		1) Ensure that that the hardware is present.
	 * 		2) Load any required native libraries.
	 * 		3) Register the port names with the CommPortIdentifier.
	 * @see be.ac.ulb.gpib.GPIBDriver#initialize(boolean)
	 */
	public boolean initialize(boolean _getIDN)
	{
		scanDevices(_getIDN);
		return true;
	}

	/* 
	 * @see be.ac.ulb.gpib.AbstractGPIBDriver#initialize(boolean, boolean)
	 */
	@Override
	public boolean initialize(boolean _getIDN, boolean _clearDevices)
	{
		scanDevicesClear(_getIDN, _clearDevices);
		return true;
	}
	
	/**
	 * Scan devices.
	 *
	 * @param _getIDN the get idn
	 */
	private native void scanDevices(boolean _getIDN);

	/**
	 * Scan devices.
	 *
	 * @param _getIDN					TRUE: also executes the command *idn
	 * @param _clearDevices		TRUE: clear the devices, FALSE: without clearing the devices
	 * on some old devices, the clear command also resets the settings on the frontpanel
	 */
	private native void scanDevicesClear(boolean _getIDN, boolean _clearDevices);

	/**
	 * Adds the scanned device.
	 *
	 * @param address the address
	 * @param name the name
	 * @param controllernumber the number of the interface board
	 */
	private void addScannedDevice(int address, String name, int controllerNumber)
	{
		if (name != null)
		{
			GPIBDeviceIdentifier.addDevice(name.trim(), address, this, controllerNumber, 0);
		}
		else
		{
			GPIBDeviceIdentifier.addDevice("[Unknown],[Unknown],[Unknown],[Unknown]", address, this, controllerNumber, 0); //$NON-NLS-1$
		}
	}

	/* (non-Javadoc)
	 * @see be.ac.ulb.gpib.GPIBDriver#getDevice(int)
	 */
	public GPIBDevice getDevice(int address)
	{
		GPIBDevice dev = new GPIBDevice(address, this);
		return (GPIBDevice) dev;
	}

	/* (non-Javadoc)
	 * @see be.ac.ulb.gpib.GPIBDriver#clearDevice(int)
	 */
	synchronized public void clearDevice(int _descr) throws IOException
	{
		clearDeviceImpl(_descr);
	}

	/* (non-Javadoc)
	 * @see be.ac.ulb.gpib.GPIBDriver#localDevice(int)
	 */
	synchronized public void localDevice(int _descr) throws IOException
	{
		localDeviceImpl(_descr);
	}

	/* (non-Javadoc)
	 * @see be.ac.ulb.gpib.GPIBDriver#remoteDevice(int)
	 */
	synchronized public void remoteDevice(int _descr) throws IOException
	{
		remoteDeviceImpl(_descr);
	}

	
	/**
	 * Opens a device and returns a descriptor integer that can be used internally later.
	 *
	 * @param controllerNumber the controller number
	 * @param _address the _address
	 * @param _timeout the _timeout
	 * @return the int
	 * @throws IOException Signals that an I/O exception has occurred.
	 */
	public int openDevice(int _address, float _timeout) throws IOException
	{
		return  openDevice(0, _address, _timeout, true);
	}
	
	
	/* (non-Javadoc)
	 * @see be.ac.ulb.gpib.AbstractGPIBDriver#openDevice(int, float, boolean)
	 */
	@Override
	public int openDevice(int _address, float _timeout, boolean _clearDevice) throws IOException
	{
		return  openDevice(0, _address, _timeout, _clearDevice);
	}

	/* (non-Javadoc)
	 * @see be.ac.ulb.gpib.AbstractGPIBDriver#openDevice(int, int, float, boolean)
	 */
	@Override
	public int openDevice(int _controllerNumber, int _address, float _timeout) throws IOException
	{
		return  openDevice(_controllerNumber, _address, _timeout, true);
	}
	
	
	/**
	 * Opens a device and returns a descriptor integer that can be used internally later.
	 *
 	 * @param _controllerNumber the controller number
	 * @param _address					of the device
	 * @param _timeout					in seconds
	 * @param _clearDevice			TRUE: clears the device, FALSE without clearing the device
	 * on some old devices, the clear command also resets the settings on the frontpanel
	 * 
	 * @return the int
	 * @throws IOException Signals that an I/O exception has occurred.
	 */
	public int openDevice(int _controllerNumber, int _address, float _timeout, boolean _clearDevice) throws IOException
	{
		int res;
		boolean found = false;

		// Make sure that we were able to load the driver
		if (!GPIBDeviceIdentifier.driverLoaded())
		{
			throw new IOException("No GPIB driver loaded."); //$NON-NLS-1$
		}

		// JLP: which devices have we enumerated?
		for (Enumeration<?> e = GPIBDeviceIdentifier.getDevices(); e.hasMoreElements();)
		{
			GPIBDeviceIdentifier gdi = (GPIBDeviceIdentifier) e.nextElement();
			// System.out.println("GDI:" + gdi.toString() + gdi.getAddress());
			if (gdi.getAddress() == _address)
			{
				found = true;
			}
		}

		if (!found)
		{
			throw new IOException("Device at " + _address + " not found on bus; cannot open."); //$NON-NLS-1$ //$NON-NLS-2$
		}
		else if ((res = openDeviceClearImpl(_controllerNumber, _address, _timeout, _clearDevice)) == -1)
		{
			throw new IOException("Unable to open device"); //$NON-NLS-1$
		}

		return res;
	}


	/* (non-Javadoc)
	 * @see be.ac.ulb.gpib.GPIBDriver#sendCommand(java.lang.String, int)
	 */
	synchronized public String sendCommand(String command, int reference) throws IOException
	{
		String result = ""; //$NON-NLS-1$
		result = sendCommandImpl(command, reference);
		fireBytesReaded(result.length());
		return result;
	}

	/* (non-Javadoc)
	 * @see be.ac.ulb.gpib.GPIBDriver#sendCommandBin(java.lang.String, int)
	 */
	synchronized public byte[] sendCommandBin(String command, int reference) throws IOException
	{
		byte[] result;
		result = sendCommandBinImpl(command, reference);
		fireBytesReaded(result.length);
		return result;
	}

	/* (non-Javadoc)
	 * @see be.ac.ulb.gpib.GPIBDriver#writeCommand(java.lang.String, int)
	 */
	synchronized public void writeCommand(String command, int reference) throws IOException
	{
		writeCommandImpl(command, reference);
	}

	/* (non-Javadoc)
	 * @see be.ac.ulb.gpib.GPIBDriver#writeCommandData(java.lang.String, byte[], int)
	 */
	synchronized public void writeCommandData(String command, byte[] data, int reference) throws IOException
	{
		writeCommandDataImpl(command, data, reference);
	}

	/* (non-Javadoc)
	 * @see be.ac.ulb.gpib.GPIBDriver#getMaxBufferLength()
	 */
	synchronized public int getMaxBufferLength() throws IOException
	{
		return getMaxBufferLengthImpl();
	}

	/* (non-Javadoc)
	 * @see be.ac.ulb.gpib.GPIBDriver#getDLLVersion()
	 */
	synchronized public String getDLLVersion() throws IOException
	{
		return getDLLVersionImpl();
	}

	// TODO: very strange, but the only function from which we can successfully throw an exception
	// (and catch it in Java level) is the openDeviceImpl, which does not actually declare anything to be thrown!
	// private native int openDeviceImpl(int address);
	// private native void clearDeviceImpl(int descriptor) throws IOException;
	// private native String sendCommandImpl(String command,int address) throws IOException ;
	// private native void writeCommandImpl(String command,int address) throws IOException ;
	/**
	 * Open device impl.
	 * @param controllerNumber	number of the interface board
	 * @param address 					the address
	 * @param timeout 					the timeout
	 * @return the int
	 */
	private native int openDeviceImpl(int controllerNumber, int address, float timeout);

	/**
	 * Open device impl.
	 * @param controllerNumber	number of the interface board
	 * @param address 					address of the device
	 * @param timeout 					timeout in seconds
	 * @param _clearDevice			TRUE: clears the device, FALSE without clearing the device
	 * on some old devices, the clear command also resets the settings on the frontpanel
	 * 
	 * @return the int
	 */
	private native int openDeviceClearImpl(int controllerNumber, int address, float timeout, boolean _clearDevice);

	/**
	 * Asserts IFC (Interface Clear) signal. This re-initializes the GPIB system.
	 *
	 * @param controllernumber
	 * @return the int
	 */
	private native int resetControllerImpl(int controllerNumber);
	
	/**
	 * Clear device impl.
	 *
	 * @param descriptor the descriptor
	 * @return the int
	 */
	private native int clearDeviceImpl(int descriptor);

	/**
	 * Local device impl.
	 *
	 * @param descriptor the descriptor
	 * @return the int
	 */
	private native int localDeviceImpl(int descriptor);

	/**
	 * Remote device impl.
	 *
	 * @param descriptor the descriptor
	 * @return the int
	 */
	private native int remoteDeviceImpl(int descriptor);

	/**
	 * Send command impl.
	 *
	 * @param command the command
	 * @param address the address
	 * @return the string
	 */
	private native String sendCommandImpl(String command, int address);

	/**
	 * Send command bin impl.
	 *
	 * @param command the command
	 * @param address the address
	 * @return the byte[]
	 */
	private native byte[] sendCommandBinImpl(String command, int address);

	/**
	 * Write command impl.
	 *
	 * @param command the command
	 * @param address the address
	 * @return the int
	 */
	private native int writeCommandImpl(String command, int address);

	/**
	 * Write command data impl.
	 *
	 * @param command the command
	 * @param data the data
	 * @param address the address
	 * @return the int
	 */
	private native int writeCommandDataImpl(String command, byte[] data, int address);

	/**
	 * Gets the max buffer length impl.
	 *
	 * @return the max buffer length impl
	 */
	private native int getMaxBufferLengthImpl();

	/**
	 * Gets the dLL version impl.
	 *
	 * @return the dLL version impl
	 */
	private native String getDLLVersionImpl();

	static
	{
	  switch (OSUtils.getOS())
		{
			case LINUX64:
				// get application path 
			  DirectoryManager.init(GPIBDriver.class);
//			  System.out.println(DirectoryManager.appdir.toString());
			  // set path to libraries
			  System.setProperty("java.library.path", DirectoryManager.appdir.toString());
				System.out.println("GPIB Linux Library 64bit"); //$NON-NLS-1$
				System.loadLibrary("jpib_64"); //$NON-NLS-1$	
				break;
				
			case LINUX32:
				// get application path 
			  DirectoryManager.init(GPIBDriver.class);
//			  System.out.println(DirectoryManager.appdir.toString());
			  // set path to libraries
			  System.setProperty("java.library.path", DirectoryManager.appdir.toString());
				System.out.println("GPIB Linux Library 32bit"); //$NON-NLS-1$
				System.loadLibrary("jpib_32"); //$NON-NLS-1$	
				break;
	
			case WINDOWS64:
				System.out.println("GPIB Windows Library 64bit"); //$NON-NLS-1$
				System.loadLibrary("jpib_64"); //$NON-NLS-1$
				break;

			case WINDOWS32:
				System.out.println("GPIB Windows Library 32bit"); //$NON-NLS-1$
				System.loadLibrary("jpib_32"); //$NON-NLS-1$
				break;

			default:
				System.out.println("OS not supported!"); //$NON-NLS-1$
				break;
		}
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see be.ac.ulb.gpib.GPIBDriver#getControllerName()
	 */
	@Override
	public String getControllerName()
	{
		return "GPIB"; //$NON-NLS-1$
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see be.ac.ulb.gpib.GPIBDriver#addListener(be.ac.ulb.gpib.GPIBDriverListener)
	 */
	@Override
	public void addListener(GPIBDriverListener gpibDriverListener)
	{
		this.listenerList.add(gpibDriverListener);
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see be.ac.ulb.gpib.GPIBDriver#removeListener(be.ac.ulb.gpib.GPIBDriverListener)
	 */
	@Override
	public void removeListener(GPIBDriverListener gpibDriverListener)
	{
		this.listenerList.remove(gpibDriverListener);
	}

	/**
	 * Fire bytes readed.
	 *
	 * @param bytesReaded the bytes readed
	 */
	protected void fireBytesReaded(int bytesReaded)
	{
		GPIBDriverListener[] listenerAr = (GPIBDriverListener[]) listenerList.toArray(new GPIBDriverListener[listenerList.size()]);

		for (int i = 0; i < listenerAr.length; i++)
		{
			listenerAr[i].bytesReaded(bytesReaded);
		}
	}
	
	
	/*
	 * (non-Javadoc)
	 * 
	 * @see be.ac.ulb.gpib.GPIBDriver#isAsciiPossible()
	 */
	@Override
	public boolean isAsciiPossible()
	{
		return true;
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see be.ac.ulb.gpib.GPIBDriver#isBinaryPossible()
	 */
	@Override
	public boolean isBinaryPossible()
	{
		return true;
	}

	/* (non-Javadoc)
	 * @see be.ac.ulb.gpib.GPIBDriver#closeDevice(int)
	 */
	@Override
	public void closeDevice(int controllerNumber)
	{
	}

	/* (non-Javadoc)
	 * @see be.ac.ulb.gpib.GPIBDriver#getDriverError()
	 */
	@Override
	public String getDriverError()
	{
		return gpibError;
	}

	/* (non-Javadoc)
	 * @see be.ac.ulb.gpib.AbstractGPIBDriver#resetController(int)
	 */
	@Override
	public void resetController(int _controllerNumber) throws IOException
	{
		resetControllerImpl(_controllerNumber);
	}

}
