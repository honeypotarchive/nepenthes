/* $Id: strsep.h,v 1.2 2004/05/27 20:09:29 vbrimhall Exp $ */

#ifndef COMPABILITY_HPP
#define COMPABILITY_HPP

#ifndef HAVE_STRSEP
extern char *strsep(char **stringp, const char *delim);
#endif 

#ifdef WIN32
extern int stat(const char *file_name, struct _stat *buf);
#endif

#endif 

