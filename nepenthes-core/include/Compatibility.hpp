/* $Id: Compatibility.hpp 446 2006-03-29 22:24:33Z common $ */

#ifndef COMPABILITY_HPP
#define COMPABILITY_HPP

#ifndef HAVE_STRSEP
extern char *strsep(char **stringp, const char *delim);
#endif 

#ifdef WIN32
extern int32_t stat(const char *file_name, struct _stat *buf);
#endif

#ifndef HAVE_STRNDUP
extern char *strndup(char *str,int size);
#endif 

#endif 

