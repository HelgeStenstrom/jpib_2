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
#ifndef _Included_stdgpib
#define _Included_stdgpib

#define GPIB_BUS_ZERO         0	    	// First board ID
#define GPIB_BUS_MAX          31    	// Last board ID
#define NO_SECONDARY_ADDR     0     	// Secondary address of device
#define TIMEOUT               T3s  		// Timeout in seconds
#define EOTMODE               1     	// Enable the END message     
#define EOSMODE               0     	// Disable the EOS mode
#define MAXDEVICES_ON_BUS     30		// max. amount of devices on a single interface card

char ErrorMnemonic[21][5] = {"EDVR", "ECIC", "ENOL", "EADR", "EARG",
                             "ESAC", "EABO", "ENEB", "EDMA", "",
                             "EOIP", "ECAP", "EFSO", "", "EBUS",
                             "ESTB", "ESRQ", "", "", "", "ETAB"};

#endif