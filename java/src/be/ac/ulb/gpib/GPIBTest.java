/*
* JAVA Test software for GPIB driver
*  Copyright (C) 2012 - 2016 Ralf Tralow (www.ringwelt.de)
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
import java.util.Enumeration;

/**
 * The Class GPIBTest
 * 
 * @author Ralf Tralow
 */
public class GPIBTest
{
	
	/**
	 * The main method.
	 *
	 * @param args the arguments
	 */
	public static void main(String[] args)
	{
		GPIBDeviceIdentifier deviceIdentifier;
		
		// Load the Class-Driver
		GPIBDeviceIdentifier.initialize("be.ac.ulb.gpib.GPIBDriver", false); //$NON-NLS-1$
		System.out.println("");

		
		// Get list of all found devices
		Enumeration<?> gpibDevicesList;
		gpibDevicesList = GPIBDeviceIdentifier.getDevices();
		while (gpibDevicesList.hasMoreElements())
		{
			deviceIdentifier = (GPIBDeviceIdentifier) gpibDevicesList.nextElement();
			int addr = deviceIdentifier.getAddress();
			if (addr > 0)
			{
				AbstractGPIBDriver gpibDriver = deviceIdentifier.getDriver();
				System.out.println("Found GPIB device with address '"+ addr + "' on controller: " + deviceIdentifier.getControllerNumber());

				// GET IDN
				GPIBDevice myDevice = new GPIBDevice(addr, gpibDriver, deviceIdentifier.getControllerNumber(), 100000);
				try
				{
					myDevice.open(0.1f);
					System.out.println("Descriptor: " + myDevice.descriptor);
					
					myDevice.remote(deviceIdentifier.getControllerNumber());
					try
					{
						System.out.println(myDevice.sendCommand("*IDN?"));
					}
					catch(IOException e1)
					{
						System.out.println("No answer for the command '*IDN?'");

						try
						{
							System.out.println(myDevice.sendCommand("ID"));
						}
						catch(IOException e2)
						{
							System.out.println("No answer for the command 'ID'");
						}
					}

					System.out.println("");
					myDevice.local(deviceIdentifier.getControllerNumber());
					
					myDevice.close();
				}
				catch (IOException e)
				{
					System.out.println("Error on setting GPIB");
//					e.printStackTrace();
				}
			}
		}
		System.out.println("\nSearch finished.");
	}
}
