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
 * A GPIB (IEEE488.1 and IEEE488.2) device
 * 
 * @author Jean-Michel DRICOT
 * 
 *         changed by Ralf Tralow
 */

public class GPIBDevice
{

	/** The address. */
	protected int address = -1;

	/** The controller number. */
	protected int controllerNumber = 0;

	/** The controller speed. */
	protected int controllerSpeed = 0;

	/** The timeout. */
	protected float timeout = 0.3f; // !< timeout in seconds

	/** The driver. */
	protected AbstractGPIBDriver driver = null;

	/** The vendor. */
	protected String vendor = ""; //$NON-NLS-1$

	/** The stepping. */
	protected String stepping = ""; //$NON-NLS-1$

	/** The version. */
	protected String version = ""; //$NON-NLS-1$

	/** The model. */
	protected String model = ""; //$NON-NLS-1$

	/** The descriptor. */
	protected int descriptor = -1;

	/**
	 * Instantiates a new gPIB device.
	 */
	public GPIBDevice()
	{

	}

	/**
	 * Instantiates a new gPIB device.
	 * 
	 * @param _address
	 *          the _address
	 * @param _driver
	 *          the _driver
	 */
	public GPIBDevice(int _address, AbstractGPIBDriver _driver)
	{
		//		System.out.println("GPIBDevice | Call to constructor GPIBDevice("+ _address +")"); //$NON-NLS-1$ //$NON-NLS-2$
		address = _address;
		driver = _driver;
		controllerNumber = 0;
		controllerSpeed = 0;

		// try
		// {
		//			System.out.println("DLL Version: " + driver.getDLLVersion()); //$NON-NLS-1$
		//			System.out.println("Max receive buffer length: " + driver.getMaxBufferLength()); //$NON-NLS-1$
		// }
		// catch (IOException e)
		// {
		// }
	}

	/**
	 * Instantiates a new gPIB device.
	 * 
	 * @param _address
	 *          the _address
	 * @param _driver
	 *          the _driver
	 * @param _controllerNumber
	 *          the _controller number
	 * @param _controllerSpeed
	 *          the _controller speed
	 */
	public GPIBDevice(int _address, AbstractGPIBDriver _driver, int _controllerNumber, int _controllerSpeed)
	{
		address = _address;
		driver = _driver;
		controllerNumber = _controllerNumber;
		controllerSpeed = _controllerSpeed;
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see java.lang.Object#toString()
	 */
	public String toString()
	{
		return new String("GPIB Device on bus " + this.controllerNumber + " @ address " + this.address); //$NON-NLS-1$
	}

	
	/**
	 * Clears the device by sending it the CLEAR signal.
	 * 
	 * @throws IOException
	 *           Signals that an I/O exception has occurred.
	 */
	public void clear() throws IOException
	{
		try
		{
			driver.clearDevice(this.descriptor);
//			driver.localDevice(this.descriptor);
		}
		catch (IOException e)
		{
			throw e;
		}
	}

	/**
	 * Asserts IFC (Interface Clear) signal. This re-initializes the GPIB system.
	 * 
	 * @param controllerNumber
	 * @throws IOException
	 *           Signals that an I/O exception has occurred.
	 */
	public void reset(int controllerNumber) throws IOException
	{
		try
		{
			driver.resetController(controllerNumber);
		}
		catch (IOException e)
		{
			throw e;
		}
	}

	
	/**
	 * Set the device to Local.
	 * 
	 * @param controllerNumber
	 * @throws IOException
	 *           Signals that an I/O exception has occurred.
	 */
	public void local(int controllerNumber) throws IOException
	{
		try
		{
			driver.localDevice(controllerNumber);
		}
		catch (IOException e)
		{
			throw e;
		}
	}

	/**
	 * Set the device to Remote.
	 * 
	 * @param controllerNumber
	 * @throws IOException
	 *           Signals that an I/O exception has occurred.
	 */
	public void remote(int controllerNumber) throws IOException
	{
		try
		{
			driver.remoteDevice(controllerNumber);
		}
		catch (IOException e)
		{
			throw e;
		}
	}

	/**
	 * opens the device and sets the descriptor of the device.
	 * 
	 * @throws IOException
	 *           Signals that an I/O exception has occurred.
	 */
	public void open() throws IOException
	{
		descriptor = driver.openDevice(getControllerNumber(), this.address, timeout, true);
		setTimeout(timeout);
	}

	/**
	 * opens the device and sets the descriptor of the device.
	 * 
	 * @param _timeout
	 *          Timeout for the GPIB driver in seconds
	 * @throws IOException
	 *           Signals that an I/O exception has occurred.
	 */
	public void open(float _timeout) throws IOException
	{
		descriptor = driver.openDevice(getControllerNumber(), this.address, _timeout, true);
		setTimeout(_timeout);
	}
	
	
	/**
	 * opens the device and sets the descriptor of the device.
	 * 
	 * @param _timeout			Timeout for the GPIB driver in seconds
	 * @param _clearDevice			TRUE: clears the device, FALSE without clearing the device
	 * 
	 * @throws IOException  Signals that an I/O exception has occurred.
	 */
	public void open(float _timeout, boolean _clearDevice) throws IOException
	{
		descriptor = driver.openDevice(getControllerNumber(), this.address, _timeout, _clearDevice);
		setTimeout(_timeout);
	}
	
	
	public void close() throws IOException
	{
		driver.closeDevice(getControllerNumber());
	}
	
	/**
	 * Sends a GPIB command that can be understand by the device. Refer your manual for valid commands that can be used with your device.
	 * 
	 * @param command
	 *          the command
	 * @return the answer string sent by the device. It returns an empty string if no return was available.
	 * @throws IOException
	 *           Signals that an I/O exception has occurred.
	 */
	public String sendCommand(String command) throws IOException
	{
		String res = ""; //$NON-NLS-1$
		res = driver.sendCommand(command, this.descriptor);
		return res;
	}

	/**
	 * Send command bin.
	 * 
	 * @param command
	 *          the command
	 * @return the answer byteArray sent by the device. It returns an empty byteArray if no return was available.
	 * @throws IOException
	 *           Signals that an I/O exception has occurred.
	 */
	public byte[] sendCommandBin(String command) throws IOException
	{
		byte[] res;
		res = driver.sendCommandBin(command, this.descriptor);
		return res;
	}

	/**
	 * Write command.
	 * 
	 * @param command
	 *          the command
	 * @throws IOException
	 *           Signals that an I/O exception has occurred.
	 */
	public void writeCommand(String command) throws IOException
	{
		driver.writeCommand(command, this.descriptor);
	}

	/**
	 * Write command.
	 * 
	 * @param command
	 *          the command
	 * @param data
	 *          the data
	 * @throws IOException
	 *           Signals that an I/O exception has occurred.
	 */
	public void writeCommand(String command, byte[] data) throws IOException
	{
		driver.writeCommandData(command, data, this.descriptor);
	}

	/**
	 * Gets the max buffer length.
	 * 
	 * @return the max buffer length
	 * @throws IOException
	 *           Signals that an I/O exception has occurred.
	 */
	public int getMaxBufferLength() throws IOException
	{
		return driver.getMaxBufferLength();
	}

	/**
	 * Gets the dLL version.
	 * 
	 * @return the dLL version
	 * @throws IOException
	 *           Signals that an I/O exception has occurred.
	 */
	public String getDLLVersion() throws IOException
	{
		return driver.getDLLVersion();
	}

	/**
	 * Sets the full name.
	 * 
	 * @param _vendor
	 *          the _vendor
	 * @param _model
	 *          the _model
	 * @param _stepping
	 *          the _stepping
	 * @param _version
	 *          the _version
	 * @param _name
	 *          the _name
	 */
	public void setFullName(String _vendor, String _model, String _stepping, String _version, String _name)
	{
		this.stepping = _stepping;
		this.model = _model;
		this.version = _version;
		this.vendor = _vendor;
	}

	/**
	 * Gets the stepping.
	 * 
	 * @return the stepping
	 */
	public String getStepping()
	{
		return this.stepping;
	}

	/**
	 * Gets the vendor.
	 * 
	 * @return the vendor
	 */
	public String getVendor()
	{
		return this.vendor;
	}

	/**
	 * Gets the model.
	 * 
	 * @return the model
	 */
	public String getModel()
	{
		return this.model;
	}

	/**
	 * Gets the version.
	 * 
	 * @return the version
	 */
	public String getVersion()
	{
		return this.version;
	}

	/**
	 * Gets the address.
	 * 
	 * @return the address
	 */
	public int getAddress()
	{
		return address;
	}

	/**
	 * Gets the controller number.
	 * 
	 * @return the controller number
	 */
	public int getControllerNumber()
	{
		return controllerNumber;
	}

	/**
	 * Gets the controller speed.
	 * 
	 * @return the controller speed
	 */
	public int getControllerSpeed()
	{
		return controllerSpeed;
	}

	/**
	 * Sets the controller speed.
	 * 
	 * @param controllerSpeed
	 *          the new controller speed
	 */
	public void setControllerSpeed(int controllerSpeed)
	{
		this.controllerSpeed = controllerSpeed;
	}

	/**
	 * Gets the timeout.
	 * 
	 * @return the timeout in seconds
	 */
	public float getTimeout()
	{
		return this.timeout;
	}

	/**
	 * Sets the timeout
	 * 
	 * @param timeout
	 *          the new timeout in seconds
	 */
	public void setTimeout(float timeout)
	{
		this.timeout = timeout;
	}

	/**
	 * Gets the driver.
	 * 
	 * @return the driver
	 */
	public AbstractGPIBDriver getDriver()
	{
		return this.driver;
	}

}
