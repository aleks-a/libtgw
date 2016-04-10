#!/bin/sh

#======================================================================
# generate_files.sh
#
# Copyright (C) 2016 aleks-a
#
# This software may be modified and distributed under the terms
# of the MIT license.  See the LICENSE file for details.
#======================================================================

parse_line()
{
	line=$1
	i=0;
	
	if [ "$line" = "" ] ; then continue ; fi
	if [ "$(echo "$line" | head -c 1)" = "#" ] ; then continue ; fi
	
	BUILD=false
	SCRIPT=false
	INTERNAL=false
	
	for tok in $line
	do
		if [ $i = 0 ] ; then
			SOURCE=$tok
			NAME=$(basename $tok)
			NAME=${NAME%.*}
			NBASE=${SOURCE%.*}
		elif [ $i = 1 ] ; then
			i=$((i+1))
			continue;
		elif [ "$tok" = "build" ] ; then
			BUILD=true
		elif [ "$tok" = "script" ] ; then
			SCRIPT=true
		elif [ "$tok" = "internal" ] ; then
			INTERNAL=true
		else
			echo "Unknown token: $tok" >&2
			exit 1
		fi
		
		i=$((i+1))
	done
	set +f
	
	if [ $i = 0 ] ; then echo "Internal problems here..." >&2 ; exit 1 ; fi
	if [ $i = 1 ] ; then echo "Please supply tests to run" >&2 ; exit 1 ; fi
	
	if [ "$BUILD" = "true" ] && [ "$SCRIPT" = "true" ] ; then
		echo "Can't use both 'script' and 'build' options simultaneously" >&2
		exit 1
	fi
}

dump_test_info()
{
	echo "-----------------------------------------------------------"
	echo "NAME:           $NAME"
	echo "NAME BASE:      $NBASE"
	echo "SOURCE:         $SOURCE"
	echo "BUILD:          $BUILD"
	echo "SCRIPT:         $SCRIPT"
}

SRC_DIR="."
if [ "$1" != "" ] ; then SRC_DIR="$1" ; fi

OFILE="${SRC_DIR}/test_build.cmake"


echo "# This is an automatically generated file" > $OFILE
echo "" >> $OFILE

#echo ""

while read line
do
	parse_line "$line"
	
	#dump_test_info
	
	if [ "$BUILD" = "true" ] ; then
		echo "add_executable(${NAME} ${SOURCE})" >> $OFILE
		echo "target_link_libraries(${NAME} \${TEST_LIBS})" >> $OFILE
		echo "" >> $OFILE
	fi
	
done < "${SRC_DIR}/test_list.txt"

#echo "-----------------------------------------------------------"
#echo ""
