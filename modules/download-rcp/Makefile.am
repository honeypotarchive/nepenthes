# nepenthes module Makefile
# Paul Baecher, Maximillian Dornseif, Markus Koetter
# $Id: Makefile.am 2260 2006-01-13 20:35:08Z common $

AUTOMAKE_OPTIONS = foreign

AM_CPPFLAGS = -I$(top_srcdir)/nepenthes-core/include -I$(top_srcdir)/nepenthes-core/src -pipe -D _GNU_SOURCE
AM_CXXFLAGS = -Wall -Werror
AM_LDFLAGS = -module -no-undefined -avoid-version

pkglib_LTLIBRARIES = downloadrcp.la

downloadrcp_la_SOURCES = download-rcp.cpp download-rcp.hpp
downloadrcp_la_SOURCES += RCPDialogue.cpp RCPDialogue.hpp

downloadrcp_la_LDFLAGS = -module -no-undefined -avoid-version
