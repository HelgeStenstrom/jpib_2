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
 * An enumerator used to Enumerate every GPIB device present on the bus.
 * 
 * @author Jean-Michel DRICOT
 *
*/
class GPIBEnumerator implements Enumeration<Object>
{
    private GPIBDeviceIdentifier curEntry;

    GPIBEnumerator()
    {
        curEntry=null;
    }

	/**
	* Enables to get next GPIB device present on the bus
	* @return the next GPIBDeviceIdentifier relative to the next GPIB device on the bus
	*/
    public Object nextElement()
    {
        if(curEntry != null) curEntry = curEntry.next;
        else	curEntry = GPIBDeviceIdentifier.masterIdList;
        return curEntry;
    }

    public boolean hasMoreElements()
    {
        if(curEntry != null)
        {
            boolean flag = curEntry.next != null;
            return flag;
        }
        else return (GPIBDeviceIdentifier.masterIdList != null);
    }

}
