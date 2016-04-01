#!/bin/sh

#======================================================================
# helpers.sh
#
# Copyright (C) 2016 aleks-a
#
# This software may be modified and distributed under the terms
# of the MIT license.  See the LICENSE file for details.
#======================================================================

is_in_list()
{
	VAL=$1
	LIST=$2
	
	if [ "$VAL" = "" ] || [ "$LIST" = "" ] ; then
		return 1
	fi
	
	for i in $LIST ; do
		if [ "$VAL" = "$i" ] ; then
			return 0
		fi
	done
	
	return 1
}	
