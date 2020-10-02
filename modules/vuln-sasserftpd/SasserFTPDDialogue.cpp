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

 /* $Id: SasserFTPDDialogue.cpp 1644 2005-07-14 16:19:15Z dp $ */

#include <ctype.h>

#include "SasserFTPDDialogue.hpp"
#include "vuln-sasserftpd.hpp"

#include "SocketManager.hpp"

#include "DownloadManager.hpp"
#include "LogManager.hpp"
#include "DialogueFactoryManager.hpp"


#include "Buffer.hpp"


#include "Message.hpp"


#include "ShellcodeManager.hpp"

#include "Utilities.hpp"


#ifdef STDTAGS 
#undef STDTAGS 
#endif
#define STDTAGS l_mod

using namespace nepenthes;



/**
 * Dialogue::Dialogue(Socket *)
 * construktor for the SasserFTPDDialogue, creates a new SasserFTPDDialogue
 * 
 * replies some crap to the socket
 * 
 * @param socket the Socket the Dialogue has to use
 */
SasserFTPDDialogue::SasserFTPDDialogue(Socket *socket)
{
	m_Socket = socket;
    m_DialogueName = "SasserFTPDDialogue";
	m_DialogueDescription = "eXample Dialogue";

	m_ConsumeLevel = CL_ASSIGN;

	m_Buffer = new Buffer(512);

	m_State = SasserFTPD_NULL;
}

SasserFTPDDialogue::~SasserFTPDDialogue()
{
	switch (m_State)
	{

	case SasserFTPD_NULL:
	case SasserFTPD_SHELLCODE:
	case SasserFTPD_PASS:
		logWarn("Unknown SasserFTPD exploit %i bytes State %i\n",m_Buffer->getSize(), m_State);
		g_Nepenthes->getUtilities()->hexdump(STDTAGS,(byte *) m_Buffer->getData(), m_Buffer->getSize());
		break;

	case SasserFTPD_DONE:
		break;
	}

	delete m_Buffer;
}

/**
 * Dialogue::incomingData(Message *)
 * 
 * a small and ugly shell where we can use
 * "download protocol://localction:port/path/to/file
 * to trigger a download
 * 
 * @param msg the Message the Socker received.
 * 
 * 
 * @return CL_ASSIGN
 */
ConsumeLevel SasserFTPDDialogue::incomingData(Message *msg)
{

	m_Buffer->add(msg->getMsg(),msg->getMsgLen());

	switch (m_State)
	{
	case SasserFTPD_NULL:
		if ( m_Buffer->getSize() >= strlen("USER") )
		{
			if ( memcmp(m_Buffer->getData(),"USER",strlen("USER")) == 0 )
			{
				char reply[64];
				memset(reply,0,64);
				msg->getResponder()->doRespond(reply,64);
				m_State = SasserFTPD_PASS;
				m_Buffer->clear();
			}
		}
		break;

	case SasserFTPD_PASS:
		if ( m_Buffer->getSize() >= strlen("PASS") )
		{
			if ( memcmp(m_Buffer->getData(),"PASS",strlen("PASS")) == 0 )
			{
				char reply[64];
				memset(reply,0,64);
				msg->getResponder()->doRespond(reply,64);
				m_State = SasserFTPD_SHELLCODE;
				m_Buffer->clear();
			}
		}
		break;


	case SasserFTPD_SHELLCODE:
		{
        	Message *Msg = new Message((char *)m_Buffer->getData(), m_Buffer->getSize(),m_Socket->getLocalPort(), m_Socket->getRemotePort(),
									   m_Socket->getLocalHost(), m_Socket->getRemoteHost(), m_Socket, m_Socket);
			if ( g_Nepenthes->getShellcodeMgr()->handleShellcode(&Msg) == SCH_DONE )
			{
				m_Buffer->clear();
				m_State = SasserFTPD_DONE;
			}
			delete Msg;

		}
		break;

	case SasserFTPD_DONE:
		break;
	}
	

	

	return CL_ASSIGN;
}

/**
 * Dialogue::outgoingData(Message *)
 * as we are not interested in these socket actions 
 * we simply return CL_DROP to show the socket
 * 
 * @param msg
 * 
 * @return CL_DROP
 */
ConsumeLevel SasserFTPDDialogue::outgoingData(Message *msg)
{
	return CL_ASSIGN;
}

/**
 * Dialogue::handleTimeout(Message *)
 * as we are not interested in these socket actions 
 * we simply return CL_DROP to show the socket
 * 
 * @param msg
 * 
 * @return CL_DROP
 */
ConsumeLevel SasserFTPDDialogue::handleTimeout(Message *msg)
{
	return CL_DROP;
}

/**
 * Dialogue::connectionLost(Message *)
 * as we are not interested in these socket actions 
 * we simply return CL_DROP to show the socket
 * 
 * @param msg
 * 
 * @return CL_DROP
 */
ConsumeLevel SasserFTPDDialogue::connectionLost(Message *msg)
{
	return CL_DROP;
}

/**
 * Dialogue::connectionShutdown(Message *)
 * as we are not interested in these socket actions 
 * we simply return CL_DROP to show the socket
 * 
 * @param msg
 * 
 * @return CL_DROP
 */
ConsumeLevel SasserFTPDDialogue::connectionShutdown(Message *msg)
{
	return CL_DROP;
}

