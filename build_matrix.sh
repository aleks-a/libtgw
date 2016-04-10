#!/bin/sh

#======================================================================
# build_matrix.sh
#
# Copyright (C) 2016 aleks-a
#
# This software may be modified and distributed under the terms
# of the MIT license.  See the LICENSE file for details.
#======================================================================

BUILD_DIR="./build_autom"
IDX=0

parse_line()
{
	if [ "$line" = "" ] ; then continue ; fi
	if [ "$(echo "$line" | head -c 1)" = "#" ] ; then continue ; fi
	
	BUILD_TYPE=$1
	shift
	BUILD_ARGS="$@"
}

while read line
do
	mkdir -p "${BUILD_DIR}"
	rm -Rf "${BUILD_DIR}/*"
	parse_line $line
	
	echo "Type:    $BUILD_TYPE"
	echo "Args:    $BUILD_ARGS"
	
done < "./build_matrix_cfg.txt"
