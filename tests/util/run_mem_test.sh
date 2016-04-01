#!/bin/sh

#======================================================================
# run_mem_test.sh
#
# Copyright (C) 2016 aleks-a
#
# This software may be modified and distributed under the terms
# of the MIT license.  See the LICENSE file for details.
#======================================================================

#
# Exit codes:
#    0: OK
#    1: Not enough arguments
#    2: File not found or not writeable or executable
#    3: Binary returned non-zero
#    4: Output didn't match
#

MEM_FILE=$1
PGM_FILE=$2
BIN_FILE=$3
ARGS=$4


# Enough arguments?
if [ "$BIN_FILE" = "" ] ; then
	echo "Not enough arguments supplied" >&2
	echo "Usage: $0 <VALGRIND_OUT> <OUTFILE> <BINARY> <ARGS>" >&2
	exit 1
fi

#Check program output file
mkdir -p $(dirname "$MEM_FILE" 2> /dev/null) > /dev/null
touch "$MEM_FILE" 2> /dev/null
if [ $? != 0 ] || [ ! -w "$MEM_FILE" ] ; then
	echo "Valgrind output file \"$MEM_FILE\" isn't writeable" >&2
	exit 2
fi
#Check program output file
mkdir -p $(dirname "$PGM_FILE" 2> /dev/null) > /dev/null
touch "$PGM_FILE" 2> /dev/null
if [ $? != 0 ] || [ ! -w "$PGM_FILE" ] ; then
	echo "Program output file \"$PGM_FILE\" isn't writeable" >&2
	exit 2
fi

#Check binary file
if [ ! -f "$BIN_FILE" ] ; then
	echo "Binary file \"$BIN_FILE\" doesn't exist" >&2
	exit 2
elif [ ! -x "$BIN_FILE" ] ; then
	echo "Binary file \"$BIN_FILE\" isn't executable" >&2
	exit 2
fi


if [ "$TGW_TEST_DEBUG" = "1" ] ; then
	echo "Valgrind output:  $MEM_FILE"
	echo "Program output:   $PGM_FILE"
	echo "Binary file:      $BIN_FILE"
	echo "Arguments:        $ARGS"
fi

MEMCHECK=valgrind
MEMCHECK_ARGS="--leak-check=full --trace-children=yes"


"$MEMCHECK" $MEMCHECK_ARGS --log-file="$MEM_FILE" "$BIN_FILE" \
	> "$PGM_FILE" 2>&1

#"$BIN_FILE" $ARGS > "$OUT_FILE" 2>&1
#RC=$?
#if [ $RC != 0 ] ; then
#	echo "\"$BIN_FILE\" failed with return code $RC" >&2
#	exit 3
#elif [ "$TGW_TEST_DEBUG" = "1" ] ; then
#	echo "\"$BIN_FILE\" returned with code $RC"
#fi
#
#diff -q "$RES_FILE" "$OUT_FILE" > /dev/null 2>&1
#RC=$?
#if [ $RC != 0 ] ; then
#	echo "\"$BIN_FILE\" failed: output didn't match" >&2
#	diff "$RES_FILE" "$OUT_FILE" >&2
#	exit 4
#fi

#echo "\"$BIN_FILE\" succeeded"

