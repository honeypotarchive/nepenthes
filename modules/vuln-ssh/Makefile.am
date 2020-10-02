# nepenthes module Makefile
# Paul Baecher, Maximillian Dornseif, Markus Koetter
# $Id: Makefile.am 332 2006-02-20 09:28:45Z common $

AUTOMAKE_OPTIONS = foreign

AM_CPPFLAGS = -I$(top_srcdir)/nepenthes-core/include -I$(top_srcdir)/nepenthes-core/src -pipe -D _GNU_SOURCE
AM_CXXFLAGS = -Wall -Werror
#AM_LDFLAGS = -lssh -L/usr/lib/

pkglib_LTLIBRARIES = vulnssh.la

vulnssh_la_SOURCES =  vuln-ssh.cpp vuln-ssh.hpp SSHSocket.cpp SSHSocket.hpp SSHDialogue.cpp SSHDialogue.hpp vuln-ssh.conf.dist

vulnssh_la_LDFLAGS = -module -no-undefined -avoid-version
