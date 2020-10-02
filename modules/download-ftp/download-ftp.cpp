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

 /* $Id: download-ftp.cpp 1644 2005-07-14 16:19:15Z dp $ */

#include <ctype.h>

#include "download-ftp.hpp"
#include "CTRLDialogue.hpp"
#include "FILEDialogue.hpp"
#include "FTPContext.hpp"

#include "SocketManager.hpp"
#include "Message.hpp"

#include "LogManager.hpp"
#include "DialogueFactoryManager.hpp"

#include "DownloadManager.hpp"

#include "Download.hpp"
#include "DownloadUrl.hpp"
#include "DownloadBuffer.hpp"

#include "DNSManager.hpp"
#include "DNSResult.hpp"

#ifdef STDTAGS 
#undef STDTAGS 
#endif
#define STDTAGS l_mod

using namespace nepenthes;

/**
 * as we may need a global pointer to our Nepenthes in our modules,
 * and cant access the cores global pointer to nepenthes
 * we have to use a own global pointer to nepenthes per module
 * we need this pointer for logInfo() etc
 */
Nepenthes *g_Nepenthes;
FTPDownloadHandler *g_FTPDownloadHandler;

/**
 * The Constructor
 * creates a new FTPDownloadHandler Module, 
 * FTPDownloadHandler is an example for binding a socket & setting up the Dialogue & DialogueFactory
 * 
 * 
 * it can be used as a shell emu to allow trigger commands 
 * 
 * 
 * sets the following values:
 * - m_DialogueFactoryName
 * - m_DialogueFactoryDescription
 * 
 * @param nepenthes the pointer to our Nepenthes
 */
FTPDownloadHandler::FTPDownloadHandler(Nepenthes *nepenthes)
{
	m_ModuleName        = "download-ftp";
	m_ModuleDescription = "painless simple activex-2l ftp client";
	m_ModuleRevision    = "$Rev: 1644 $";
	m_Nepenthes = nepenthes;

	m_DialogueFactoryName = "download-ftp";
	m_DialogueFactoryDescription = "download-ftp dialogue factory";

	m_DownloadHandlerDescription = "simple ftp downloadhandler";
	m_DownloadHandlerName  = "ftp download handler";

	g_Nepenthes = nepenthes;
	g_FTPDownloadHandler = this;
}

FTPDownloadHandler::~FTPDownloadHandler()
{

}


/**
 * Module::Init()
 * 
 * binds the port, adds the DialogueFactory to the Socket
 * 
 * @return returns true if everything was fine, else false
 *         false indicates a fatal error
 */
bool FTPDownloadHandler::Init()
{
	m_ModuleManager = m_Nepenthes->getModuleMgr();
	REG_DOWNLOAD_HANDLER(this,"ftp");
	return true;
}

bool FTPDownloadHandler::Exit()
{
	return true;
}

/**
 * DialogueFactory::createDialogue(Socket *x-2)
 * 
 * creates a new FTPDownloadHandlerDialogue
 * 
 * @param socket the socket the DIalogue has to use, can be NULL if the Dialogue can handle it
 * 
 * @return returns the new created dialogue
 */
Dialogue *FTPDownloadHandler::createDialogue(Socket *socket)
{
	logPF();
	logInfo("Incoming connection on port %i\n",socket->getLocalPort());

	list<FTPContext *>::iterator it;
	for (it= m_Contexts.begin();it != m_Contexts.end();it++)
	{
		logSpam("Ports  %i <-> %i \n",(*it)->getActiveFTPBindPort(), socket->getLocalPort());
		if ((*it)->getActiveFTPBindPort() == socket->getLocalPort())
		{

			Dialogue *dia = new FILEDialogue(socket,(*it)->getDownload(), (*it)->getCTRLDialogue());
			FTPContext *delme = *it;
			m_Contexts.erase(it);
			delete delme;
			return dia;

		}
	}
//	return new FTPDownloadHandlerDialogue(socket);
	return NULL;
}




bool FTPDownloadHandler::download(Download *down)
{
	logPF();

	unsigned long host = inet_addr(down->getDownloadUrl()->getHost().c_str());

	if ((int)host == -1)
	{
		logInfo("url %s has a dns as hostname, we have to resolve it \n", down->getUrl().c_str());
        g_Nepenthes->getDNSMgr()->addDNS(this,(char *)down->getDownloadUrl()->getHost().c_str(), down);
		return true;
	}else
	{
		logInfo("url has %s ip, we will download it now\n",down->getUrl().c_str());
		Socket *socket = g_Nepenthes->getSocketMgr()->connectTCPHost(0,host,down->getDownloadUrl()->getPort(),30);
		CTRLDialogue *dia = new CTRLDialogue(socket,down);
		socket->addDialogue(dia);
		FTPContext *context = new FTPContext(down,dia);
		dia->setContext(context);
		m_Contexts.push_back(context);
	}

	return true;
}



bool FTPDownloadHandler::dnsResolved(DNSResult *result)
{
	logInfo("url %s resolved \n",result->getDNS().c_str());
	unsigned long host = result->getIP4List().front();
	Download *down = (Download *) result->getObject();
	Socket *socket = g_Nepenthes->getSocketMgr()->connectTCPHost(0,host,down->getDownloadUrl()->getPort(),30);
	CTRLDialogue *dia = new CTRLDialogue(socket,down);
	socket->addDialogue(dia);
	FTPContext *context = new FTPContext(down,dia);
	dia->setContext(context);
	m_Contexts.push_back(context);

	return true;
}

bool FTPDownloadHandler::dnsFailure(DNSResult *result)
{
	logInfo("url %s unresolved \n",result->getDNS().c_str());
	return true;
}

bool FTPDownloadHandler::removeContext(FTPContext *context)
{
	logPF();
	list<FTPContext *>::iterator it;
	for (it= m_Contexts.begin();it != m_Contexts.end();it++)
	{
		if ((*it) == context)
		{
			FTPContext *delme = *it;
			m_Contexts.erase(it);
			delete delme;
			return true;
		}
	}

	return false;
}

extern "C" int module_init(int version, Module **module, Nepenthes *nepenthes)
{
	if (version == MODULE_IFACE_VERSION) {
        *module = new FTPDownloadHandler(nepenthes);
        return 1;
    } else {
        return 0;
    }
}
