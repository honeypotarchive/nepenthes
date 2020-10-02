# nepenthes module Makefile
# Paul Baecher, Maximillian Dornseif, Markus Koetter
# $Id: Makefile.am 2243 2006-01-04 18:02:15Z common $

AUTOMAKE_OPTIONS = foreign

AM_CPPFLAGS = -I$(top_srcdir)/nepenthes-core/include -I$(top_srcdir)/nepenthes-core/src -pipe -D _GNU_SOURCE
AM_CXXFLAGS = -Wall -Werror
AM_LDFLAGS = -lpcre

pkglib_LTLIBRARIES = x2.la

x2_la_SOURCES = x-2.cpp x-2.hpp x-2.conf.dist

x2_la_LDFLAGS = -module -no-undefined -avoid-version
