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

/* $Id: sch_generic_url.cpp 1777 2005-08-04 00:29:45Z common $ */


/* Additional notes:
 *
 * The pcre's and processing logic in this module is derived from mwcollect written by Georg Wicherski
 *
 * if you got any idea what has to be done to relicense bsd code on a gpl license mail us
 * wikipedia states bsd code can be relicensed on to gpl, but we got no information what has to be done
 * 
 *
 */


 
#include "sch_generic_url.hpp"
#include "LogManager.hpp"
#include "Message.hpp"
#include "DownloadManager.hpp"
#include "Nepenthes.hpp"


#ifdef STDTAGS 
#undef STDTAGS 
#endif
#define STDTAGS l_sc | l_hlr


using namespace nepenthes;


GenericUrl::GenericUrl(ShellcodeManager *shellcodemanager)
{
	m_ShellcodeManager = shellcodemanager;
	m_ShellcodeHandlerName = "GenericUrl";
	m_ShellcodeHandlerDescription = "generic Url decoder";
	m_pcre = NULL;

	fprintf(stderr,"\n");
	fprintf(stderr,"The generic url shellcodehandler is based on \n");
	fprintf(stderr,"mwcollects generic url shellcodehandler \n");
	fprintf(stderr,"mwcollect is\n"); 
	fprintf(stderr,"Copyright (c) 2005, Honeynet Project\n");
	fprintf(stderr,"All rights reserved.\n");
	fprintf(stderr,"published on a bsd license\n");
	fprintf(stderr,"and written by Georg Wicherski\n");
	fprintf(stderr,"http://www.mwcollect.org for more information about mwcollect\n");
	fprintf(stderr,"\n");


}

GenericUrl::~GenericUrl()
{

}

bool GenericUrl::Init()
{
	const char *urlpcre = ".*((http|https|ftp):\\/\\/[a-zA-Z0-9\\/\\\\\\.\\+:]+).*";
//	".*((http|https|ftp):\\/\\/[a-zA-Z0-9\\/\\\\\\.\\+:]+).*\\xDF+.*$";
	//"^.*\\xEB.((http|https|ftp):\\/\\/.*?)\\xDF+.*$";
	const char * pcreEerror;
	int pcreErrorPos;
	if((m_pcre = pcre_compile(urlpcre, PCRE_DOTALL, &pcreEerror, &pcreErrorPos, 0)) == NULL)
	{
		logCrit("GenericUrl could not compile pattern \n\t\"%s\"\n\t Error:\"%s\" at Position %u", 
				urlpcre, pcreEerror, pcreErrorPos);
		return false;
	}
	return true;
}

bool GenericUrl::Exit()
{
	if(m_pcre != NULL)
    	free(m_pcre);
	return true;
}

sch_result GenericUrl::handleShellcode(Message **msg)
{
	logPF();
	logSpam("Shellcode is %i bytes long \n",(*msg)->getMsgLen());

	bool bMatch=false;
	unsigned char *shellcode = (unsigned char *)(*msg)->getMsg();
	unsigned int len = (*msg)->getMsgLen();
	int piOutput[10 * 3];
	int iResult=0;

	if((iResult = pcre_exec(m_pcre, 0, (char *) shellcode, len, 0, 0, piOutput, sizeof(piOutput)/sizeof(int))) > 0)
	{
		const char * pUrl;

		pcre_get_substring((char *) shellcode, piOutput, iResult, 1, &pUrl);

		logDebug("Detected generic prepended unencoded URL Shellcode: \"%s\"\n", pUrl);
		// FIXME hier das file ziehen
		m_ShellcodeManager->getNepenthes()->getDownloadMgr()->downloadUrl((char *)pUrl,(*msg)->getRemoteHost(),"generic url decoder");
		pcre_free_substring(pUrl);
		bMatch = true;
	}

	if(bMatch == true)
    	return SCH_DONE;

    return SCH_NOTHING;
}
