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

 /* $Id: sch_generic_bind.cpp 1777 2005-08-04 00:29:45Z common $ */

#include <netinet/in.h>

#include "LogManager.hpp"
#include "Message.hpp"
#include "sch_generic_bind.hpp"
#include "Socket.hpp"
#include "Nepenthes.hpp"
#include "Utilities.hpp"
#include "DialogueFactoryManager.hpp"
#include "SocketManager.hpp"

#include "Config.hpp"
#include "shellcode-generic.hpp"

#ifdef STDTAGS 
#undef STDTAGS 
#endif
#define STDTAGS l_sc | l_hlr

using namespace nepenthes;

GenericBind::GenericBind(ShellcodeManager *shellcodemanager)
{
	m_ShellcodeManager = shellcodemanager;
	m_ShellcodeHandlerName = "GenericBind";
	m_ShellcodeHandlerDescription = "various bindshells";
	
}

GenericBind::~GenericBind()
{

}

bool GenericBind::Init()
{
	logPF();

	StringList sList;
	try
	{
		sList = *g_GenericShellcodeHandler->getConfig()->getValStringList("shellcode-generic.generic_bind");
	} catch ( ... )
	{
		logCrit("%s","Error setting needed vars, check your config\n");
		return false;
	}

	unsigned int i = 0;
	while (i < sList.size())
	{
		const char *name = sList[i];
		i++;

		



		const char *pattern = sList[i];
		logInfo("pcre is %s \n",pattern);
		const char * pcreEerror;
		int pcreErrorPos;
		pcre *mypcre=NULL;
		if((mypcre = pcre_compile(pattern, PCRE_DOTALL, &pcreEerror, &pcreErrorPos, 0)) == NULL)
		{
			logCrit("GenericBind could not compile pattern \n\t\"%s\"\n\t Error:\"%s\" at Position %u", 
					pattern, pcreEerror, pcreErrorPos);
			return false;
		}else
		{
			logInfo("Adding %s \n",name);
			PcreContext *ctx = new PcreContext;
			ctx->m_Name = name;
			ctx->m_Pcre = mypcre;
			m_Pcres.push_back(ctx);
		}


		i++;
	}


	return true;
}

bool GenericBind::Exit()
{
	return true;
}

sch_result GenericBind::handleShellcode(Message **msg)
{
	logPF();
	logSpam("Shellcode is %i bytes long \n",(*msg)->getMsgLen());
	char *shellcode = (*msg)->getMsg();
	unsigned int len = (*msg)->getMsgLen();

	int output[10 * 3];


	list <PcreContext *>::iterator it;
	unsigned int i;
	for ( it=m_Pcres.begin(), i=0; it != m_Pcres.end();it++,i++ )
	{
		int result=0;
		if ( (result = pcre_exec((*it)->m_Pcre, 0, (char *) shellcode, len, 0, 0, output, sizeof(output)/sizeof(int))) > 0 )
		{
			const char * match;
			unsigned short port;

			pcre_get_substring((char *) shellcode, output, result, 1, &match);

			port = ntohs(*(unsigned long *) match);
			logInfo("Detected Generic listenshell shellcode #%s, :%u \n",(*it)->m_Name.c_str(), port);
			pcre_free_substring(match);

			Socket *socket;
			if ( (socket = g_Nepenthes->getSocketMgr()->bindTCPSocket(0,port,60,30)) == NULL )
			{
				logCrit("%s","Could not bind socket %u \n",port);
				return SCH_DONE;
			}

			DialogueFactory *diaf;
			if ( (diaf = g_Nepenthes->getFactoryMgr()->getFactory("WinNTShell DialogueFactory")) == NULL )
			{
				logCrit("%s","No WinNTShell DialogueFactory availible \n");
				return SCH_DONE;
			}

			socket->addDialogueFactory(diaf);
			return SCH_DONE;
		}
	}
	return SCH_NOTHING;
}


