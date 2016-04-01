#!/bin/sh

#======================================================================
# run_corr_test.sh
#
# Copyright (C) 2016 aleks-a
#
# This software may be modified and distributed under the terms
# of the MIT license.  See the LICENSE file for details.
#======================================================================

#
# USAGE: run_corr_test.sh <RESULTS> <OUTFILE> <BINARY> <ARGS>
#
# Exit codes:
#    0: OK
#    1: Not enough arguments
#    2: File not found or not writeable or executable
#    3: Binary returned non-zero
#    4: Output didn't match
#

RES_FILE=$1
OUT_FILE=$2
BIN_FILE=$3
ARGS=$4


# Enough arguments?
if [ "$BIN_FILE" = "" ] ; then
	echo "Not enough arguments supplied" >&2
	echo "Usage: $0 <RESULTS> <OUTFILE> <BINARY> <ARGS>" >&2
	exit 1
fi

# Check result file
if [ ! -f "$RES_FILE" ] ; then
	echo "Result file \"$RES_FILE\" doesn't exist" >&2
	exit 2
elif [ ! -r "$RES_FILE" ] ; then
	echo "Result file \"$RES_FILE\" not readable" >&2
	exit 2
fi

#Check output file
touch "$OUT_FILE" 2> /dev/null
if [ $? != 0 ] || [ ! -w "$OUT_FILE" ] ; then
	echo "Output file \"$OUT_FILE\" isn't writeable" >&2
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
	echo "Result file:      $RES_FILE"
	echo "Output file:      $OUT_FILE"
	echo "Binary file:      $BIN_FILE"
	echo "Arguments:        $ARGS"
fi


"$BIN_FILE" $ARGS > "$OUT_FILE" 2>&1
RC=$?
if [ $RC != 0 ] ; then
	echo "\"$BIN_FILE\" failed with return code $RC"
	exit 3
elif [ "$TGW_TEST_DEBUG" = "1" ] ; then
	echo "\"$BIN_FILE\" returned with code $RC"
fi

diff -q "$RES_FILE" "$OUT_FILE" > /dev/null 2>&1
RC=$?
if [ $RC != 0 ] ; then
	echo "\"$BIN_FILE\" failed: output didn't match"
	diff "$RES_FILE" "$OUT_FILE"
	exit 4
fi

echo "\"$BIN_FILE\" succeeded"

