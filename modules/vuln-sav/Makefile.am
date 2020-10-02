# nepenthes module Makefile
# Paul Baecher, Maximillian Dornseif, Markus Koetter
# $Id: Makefile.am 722 2006-12-29 07:23:08Z common $

AUTOMAKE_OPTIONS = foreign

AM_CPPFLAGS = -I$(top_srcdir)/nepenthes-core/include -I$(top_srcdir)/nepenthes-core/src -pipe -D _GNU_SOURCE
AM_CXXFLAGS = -Wall -Werror

pkglib_LTLIBRARIES = vulnsav.la

vulnsav_la_SOURCES = vuln-sav.cpp vuln-sav.hpp

vulnsav_la_LDFLAGS = -module -no-undefined -avoid-version
