# nepenthes module Makefile
# Paul Baecher, Maximillian Dornseif, Markus Koetter
# $Id: Makefile.am 332 2006-02-20 09:28:45Z common $

AUTOMAKE_OPTIONS = foreign

AM_CPPFLAGS = -I$(top_srcdir)/nepenthes-core/include -I$(top_srcdir)/nepenthes-core/src -pipe -D _GNU_SOURCE
AM_CXXFLAGS = -Wall -Werror
AM_LDFLAGS = -lpcre

pkglib_LTLIBRARIES = uploadhttp.la

uploadhttp_la_SOURCES = upload-http.cpp upload-http.hpp HTTPUPDialogue.cpp HTTPUPDialogue.hpp

uploadhttp_la_LDFLAGS = -module -no-undefined -avoid-version
