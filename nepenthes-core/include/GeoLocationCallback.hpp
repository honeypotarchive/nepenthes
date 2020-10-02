/********************************************************************************
 *                              Nepenthes
 *                        - finest collection -
 *
 *
 *
 * Copyright (C) 2005  Paul Baecher & Markus Koetter
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 * 
 * 
 *             contact nepenthesdev@users.sourceforge.net  
 *
 *******************************************************************************/

/* $Id: GeoLocationCallback.hpp 332 2006-02-20 09:28:45Z common $ */

#include "config.h"

#ifdef HAVE_GEOLOCATION

#ifndef HAVE_GEOLOCATIONCALLBACK_HPP
#define HAVE_GEOLOCATIONCALLBACK_HPP

#include <string>

using namespace std;

namespace nepenthes
{
	class GeoLocationResult;

	/**
	 * as geolocationg something can take some time, and we got no time, 
	 * the GeoLocationHandler will call the one who asked for geolocation services back, if the task is done.
	 */
	class GeoLocationCallback
	{
	public:
		virtual ~GeoLocationCallback(){};
		virtual void locationSuccess(GeoLocationResult *result)=0;
		virtual void locationFailure(GeoLocationResult *result)=0;

	};
};


#endif

#endif //HAVE_GEOLOCATION
