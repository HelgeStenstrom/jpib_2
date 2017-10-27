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

import java.util.Enumeration;

/**
 * 
 * Used to identify every GPIB present on the bus. One can get an instance of a given device via its identifer using the getDevice() method
 * 
 * @author Jean-Michel DRICOT
 * 
 *         changed by Ralf Tralow
 */

public final class GPIBDeviceIdentifier extends GPIBDevice
{
	/** The name. */
	private String name = "Unknown_device";

	/** The driver loaded. */
	private static boolean driverLoaded = false;

	/** The properties found. */
	private static boolean propertiesFound = false;

	/** The master id list. */
	static GPIBDeviceIdentifier masterIdList;

	/** The next. */
	GPIBDeviceIdentifier next = null;

	
	/**
	 * Returns the state of the driver; true if loaded false if not.
	 * 
	 * @return true, if successful
	 */
	public static boolean driverLoaded()
	{
		return driverLoaded;
	}

	/**
	 * Properties found.
	 * 
	 * @return true, if successful
	 */
	public static boolean propertiesFound()
	{
		return propertiesFound;
	}


	/**
	 * Instantiates a new gPIB device identifier.
	 *
	 * @param _name the _name
	 * @param _address the _address
	 * @param _driver the _driver
	 * @param _controllerNumber the _controller number
	 * @param _controllerSpeed the _controller speed
	 */
	private GPIBDeviceIdentifier(String _name, int _address, AbstractGPIBDriver _driver, int _controllerNumber, int _controllerSpeed)
	{
		super(_address, _driver, _controllerNumber, _controllerSpeed);
		this.name = _name;
		next = null;
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see java.lang.Object#toString()
	 */
	public String toString()
	{
		return "Device: " + name + " " + address + " " + driver + "\r\n"; //$NON-NLS-1$ //$NON-NLS-2$ //$NON-NLS-3$ //$NON-NLS-4$
	}

	/**
	 * Gets the devices.
	 * 
	 * @return an enumeration representing all devices present on the bus
	 */
	public static Enumeration<?> getDevices()
	{
		return new GPIBEnumerator();
	}

	
	/**
	 * Adds the device.
	 *
	 * @param _name the _name
	 * @param _address the _address
	 * @param _driver the _driver
	 */
	public static void addDevice(String _name, int _address, AbstractGPIBDriver _driver)
	{
		addDevice(_name, _address, _driver, 0, 0);
	}
	
	
	/**
	 * Adds a valid GPIB device present on local system.
	 *
	 * @param _name the name of the device. It is the string returned by the IEEE488.2 command: "IDN?"
	 * @param _address the GPIB address of the device
	 * @param _driver the driver who will be used to access this device
	 * @param _controllerNumber the _controller number
	 * @param _controllerSpeed the _controller speed
	 */
	public static void addDevice(String _name, int _address, AbstractGPIBDriver _driver, int _controllerNumber, int _controllerSpeed)
	{
		final String UNKNOWN_IDENTIFIER = "[Unknown]"; //$NON-NLS-1$
		GPIBDeviceIdentifier id = new GPIBDeviceIdentifier(_name, _address, _driver, _controllerNumber, _controllerSpeed);
		GPIBDeviceIdentifier id1 = masterIdList;
		GPIBDeviceIdentifier id2 = null;

		java.util.StringTokenizer tk = new java.util.StringTokenizer(_name, ","); //$NON-NLS-1$
		// System.out.println ("Tokens: " + tk.countTokens());
		if (tk.countTokens() >= 1)
			id.vendor = tk.nextToken();
		else
			id.vendor = UNKNOWN_IDENTIFIER;

		if (tk.countTokens() >= 2)
			id.model = tk.nextToken();
		else
			id.model = UNKNOWN_IDENTIFIER;

		if (tk.countTokens() >= 3)
			id.version = tk.nextToken();
		else
			id.version = UNKNOWN_IDENTIFIER;

		if (tk.countTokens() >= 4)
			id.stepping = tk.nextToken();
		else
			id.stepping = UNKNOWN_IDENTIFIER;

		for (; id1 != null; id1 = id1.next)
			id2 = id1;
		if (id2 != null)
			id2.next = id;
		else
			masterIdList = id;

	}


	/**
	 * Gets the name.
	 * 
	 * @return the name
	 */
	public String getName()
	{
		return this.name;
	}


	/**
	 * Takes care of reading GPIB properties file for local system and find out which driver should be used to access GPIB devices. <br>
	 * It also calls finally the initialize() method of the Driver.
	 * 
	 * @param driverName	the driver name
	 * @param getIDN			TRUE: also executes the command *idn?; FALSE: without *idn?
	 */
	public static void initialize(String driverName, boolean getIDN)
	{
		initialize(driverName, getIDN, true);
	}
	
	
	/**
	 * Takes care of reading GPIB properties file for local system and find out which driver should be used to access GPIB devices. <br>
	 * It also calls finally the initialize() method of the Driver.
	 * 
	 * @param driverName        the driver name
	 * @param getIDN			TRUE: also executes the command *idn?; FALSE: without *idn?
	 * @param _clearDevice			TRUE: clears the device, FALSE without clearing the device
	 * on some old devices, the clear command also resets the settings on the frontpanel
	 */
	public static void initialize(String driverName, boolean getIDN, boolean clearDevice)
	{
			System.out.println("Trying to load driver " + driverName); //$NON-NLS-1$
			try
			{
				AbstractGPIBDriver gpibdriver = (AbstractGPIBDriver) Class.forName(driverName).newInstance();
				System.out.println("Loaded driver " + gpibdriver); //$NON-NLS-1$
				if (gpibdriver.initialize(getIDN, clearDevice))
				{

				}
				else
				{

				}
				driverLoaded = true;
			}
			catch (IllegalAccessException e1)
			{
				System.err.println("Caught IllegalAccessException " + e1 + " while loading driver " + driverName); //$NON-NLS-1$ //$NON-NLS-2$
				e1.printStackTrace();
			}
			catch ( InstantiationException e2)
			{
				System.err.println("Caught InstantiationException " + e2 + " while loading driver " + driverName); //$NON-NLS-1$ //$NON-NLS-2$
				e2.printStackTrace();
			}
			catch (Exception e3)
			{
				System.err.println("Caught Exception " + e3 + " while loading driver " + driverName); //$NON-NLS-1$ //$NON-NLS-2$
				e3.printStackTrace();
			}
			propertiesFound = true;
	}
	

	/**
	 * Clear list.
	 */
	public static void clearList()
	{
		masterIdList = null;
	}


}
