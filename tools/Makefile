# Nepenthes tools collection
# Makefile
#
# $Id: Makefile 1780 2005-08-04 00:57:03Z dp $


CC = gcc
CCFLAGS = -O2 -Wall -Werror

ifdef DEBUG
CCFLAGS += -g
endif

#LDFLAGS =

all:
	$(CC) $(CCFLAGS) bdiffm/bdiffm.c -o bdiffm/bdiffm
	$(CC) $(CCFLAGS) mkcarray/mkcarray.c -o mkcarray/mkcarray

install: all
	cp bdiffm/bdiffm /usr/bin
	cp mkcarray/mkcarray /usr/bin
	cp mwcollect-dump/mwcollect-dump.pl /usr/bin

clean:
	rm -f bdiffm/bdiffm
	rm -f mkcarray/mkcarray
