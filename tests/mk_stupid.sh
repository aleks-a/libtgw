#!/bin/sh

if [ ! -x "$(pwd)/mk_stupid.sh" ] ; then
	echo "Most likely you are in the wrong directory"
	exit 1
fi

find . -type f -name 'stupid.txt' > stupid-list.txt

echo "";

make -C ../stupid || exit 1

echo "";
echo "################################################################################"
echo "################################################################################"
echo "";

../stupid/stupid-generate stupid-cfg.txt "$(pwd)/.."

echo "";
