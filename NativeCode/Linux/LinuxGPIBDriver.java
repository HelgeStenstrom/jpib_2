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

public class LinuxGPIBDriver implements GPIBDriver
{

    /*
     * initialize() will be called by the CommPortIdentifier's static
     * initializer. The responsibility of this method is:
     * 1) Ensure that that the hardware is present.
     * 2) Load any required native libraries.
     * 3) Register the port names with the CommPortIdentifier.
     *
     * @author Jean-Michel DRICOT
     */

    public void initialize()
    {
        scanDevices();

    }

    private native void scanDevices();

    private void addScannedDevice(int address,String name)
    {
        GPIBDeviceIdentifier.addDevice(name.trim(),address,this);
    }

    public GPIBDevice getDevice(int address)
    {
        // System.out.println("WindowsGPIBDriver  | driver.getDevice()");
        GPIBDevice dev=new GPIBDevice(address,this);
        return (GPIBDevice)dev;
    }

    public void clearDevice(int _descr) throws IOException
    {
        try { clearDeviceImpl(_descr); }
        catch (IOException e) { throw e; }
    }

	public int openDevice(int _address) throws IOException
	{
	int res;
        if ((res=openDeviceImpl(_address))==-1) { throw new IOException("Unable to open device"); }
	return res;
    }

    public String sendCommand(String command,int address) throws IOException
    {
        String result="";
        try { result=sendCommandImpl(command,address); } 
        catch (IOException e) { throw e; }
        return result;
    }

    private native void clearDeviceImpl(int descriptor) throws IOException;
    private native int openDeviceImpl(int address);
    private native String sendCommandImpl(String command,int address) throws IOException ;

    static
    {
        System.loadLibrary("jpib");
    }

}
