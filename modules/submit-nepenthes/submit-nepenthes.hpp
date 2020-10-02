
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

 /* $Id: submit-nepenthes.hpp 1926 2005-08-27 20:52:47Z dp $ */

#include "Module.hpp"
#include "ModuleManager.hpp"
#include "SocketManager.hpp"
#include "Nepenthes.hpp"
#include "SubmitHandler.hpp"


using namespace std;

namespace nepenthes
{

	class SubmitNepenthes : public Module , public SubmitHandler
	{
	public:
		SubmitNepenthes(Nepenthes *);
		~SubmitNepenthes();
		bool Init();
		bool Exit();

		void Submit(Download *down);
		void Hit(Download *down);
	protected:
		uint32_t	m_Host;
		uint32_t 	m_Port;
		uint32_t 	m_ConnectTimeout;


	};

}

extern nepenthes::Nepenthes *g_Nepenthes;
