# $Id: mkmodmakefile.sh 1386 2005-04-16 23:55:17Z dp $
#!/bin/sh

if [ $# -lt 2 ]
then
	echo "Usage `basename $0` TEMPLATE MODNAME_0 .. MODNAME_n"
fi

TEMPLATE=$1

shift

for mod in $@
do
	sed -e "s/{{{MODNAME}}}/$mod/" < $TEMPLATE > modules/$mod/Makefile
done
