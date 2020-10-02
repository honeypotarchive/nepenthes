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

 /* $Id: download-link.hpp 1686 2005-07-24 01:10:54Z common $ */

#ifndef HAVE_CSENDDOWNLOADHANDLER_HPP
#define HAVE_CSENDDOWNLOADHANDLER_HPP

#include "Module.hpp"
#include "ModuleManager.hpp"
#include "SocketManager.hpp"
#include "Nepenthes.hpp"
#include "DownloadHandler.hpp"
#include "DownloadManager.hpp"

#include "Dialogue.hpp"
#include "DialogueFactory.hpp"

using namespace std;

namespace nepenthes
{
	struct LinkBindContext
	{
		unsigned long 		m_RemoteHost;
		unsigned short int 	m_LocalPort;
		string				m_Challenge;
		Download 			*m_Download;
	};

	class LinkDialogue;

	class LinkDownloadHandler : public Module , public DownloadHandler, public DialogueFactory
	{
	public:
		LinkDownloadHandler(Nepenthes *);
		~LinkDownloadHandler();

		bool Init();
		bool Exit();

		bool download(Download *down);

		Dialogue *createDialogue(Socket *socket);
		void socketClosed(Socket *socket);

	protected:
		unsigned int m_MaxFileSize;
		unsigned int m_ConnectTimeout;

		list <LinkBindContext *> m_BindContexts;


	};


	

}
extern nepenthes::Nepenthes *g_Nepenthes;
#endif
