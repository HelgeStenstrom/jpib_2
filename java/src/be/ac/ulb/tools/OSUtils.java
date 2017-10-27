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
package be.ac.ulb.tools;


/**
 * JAVA Utils platforms testing.
 * 
 * @author Ralf Tralow
 */

public class OSUtils
{

	/**
	 * The Enum OS.
	 */
	public enum OS
	{
		NON,
		LINUX32,
		LINUX64,
		MAC32,
		MAC64,
		WINDOWS32,
		WINDOWS64
	}

	/**
	 * Gets the oS.
	 * 
	 * @return the oS
	 */
	public static OS getOS()
	{
		if (System.getProperty("os.name").toUpperCase().contains("LINUX")) //$NON-NLS-1$ //$NON-NLS-2$
		{
			if( is64BitOS() )
				return OS.LINUX64;
			else
				return OS.LINUX32;
		}
		else if (System.getProperty("os.name").toUpperCase().contains("MAC")) //$NON-NLS-1$ //$NON-NLS-2$
		{
			if( is64BitOS() )
				return OS.MAC64;
			else
				return OS.MAC32;
		}
		else if (System.getProperty("os.name").toUpperCase().contains("WINDOWS")) //$NON-NLS-1$ //$NON-NLS-2$
		{
			if( is64BitOS() )
				return OS.WINDOWS64;
			else
				return OS.WINDOWS32;
		}
		else
		{
			return OS.NON;
		}
	}

	/**
	 * Checks if the plattform is 64bit.
	 * 
	 * @return true, if the plattform is 64bit
	 */
	private static boolean is64BitOS()
	{
		boolean is64bit = false;

		is64bit = (System.getProperty("os.arch").contains("64")); //$NON-NLS-1$ //$NON-NLS-2$

		return is64bit;
	}

}
