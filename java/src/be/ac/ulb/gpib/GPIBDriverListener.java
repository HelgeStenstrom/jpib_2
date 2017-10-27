/*
 * OsciViewer
 *  Copyright (C) 2006-2012 Ralf Tralow (www.ringwelt.de)
 *
 * This software is free software; you can redistribute it and/or
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

/**
 * The listener interface for receiving GPIBDriver events.
 * The class that is interested in processing a GPIBDriver
 * event implements this interface, and the object created
 * with that class is registered with a component using the
 * component's <code>addGPIBDriverListener<code> method. When
 * the GPIBDriver event occurs, that object's appropriate
 * method is invoked.
 *
 * @author Ralf Tralow
 */
public interface GPIBDriverListener
{
	
	/**
	 * Bytes readed.
	 *
	 * @param bytesReaded the bytes readed
	 */
	public void bytesReaded(int bytesReaded );
}
