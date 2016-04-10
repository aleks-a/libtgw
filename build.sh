#!/bin/sh

#======================================================================
# build.sh
#
# Copyright (C) 2016 aleks-a
#
# This software may be modified and distributed under the terms
# of the MIT license.  See the LICENSE file for details.
#======================================================================

BUILD_DIR="./build_autom"
OUT_DIR="./build_autom/results"
IDX=0

RED=$(tput setaf 1)
YELLOW=$(tput setaf 3)
GREEN=$(tput setaf 2)
MAG=MAGENTA=$(tput setaf 5)
CLEAR=$(tput sgr0)

parse_cfg_line()
{
	if [ "$line" = "" ] ; then continue ; fi
	if [ "$(echo "$line" | head -c 1)" = "#" ] ; then continue ; fi
	
	BUILD_TYPE=$1
	shift
	BUILD_ARGS="$@"
}

mk_build()
{
	DIR="${BUILD_DIR}/build_${1}"
	OUT="${OUT_DIR}/build_${1}"
	NAME="mtx-${1}"
	TYPE="$2"
	OPTS="$3"
	
	echo "================================================================================"
	
	mkdir -p "$DIR"
	mkdir -p "$OUT"
	
	#if [ "$TYPE" = "cov" ] ; then
	#	echo "COV: dir: $DIR"
		
	#	cp src inc tests CMakeLists.txt "$DIR/" -R
	#fi
	
	OUT="$(realpath "$OUT")"
	
	(
		cd $DIR
		
		find "$OUT" -not -name 'build-log.txt' \
			-and -not -name 'config-log.txt' \
			-mindepth 1 -exec rm -Rf {} +
		
	#	if [ "$TYPE" = "cov" ] ; then
	#		CMAKE_PATH="."
	#	else
			CMAKE_PATH="../.."
	#	fi
		
		echo "$ cmake $CMAKE_PATH $OPTS" > "${OUT}/config-log.txt"
		cmake $CMAKE_PATH $OPTS >> "${OUT}/config-log.txt" 2>&1
		if [ "$?" != 0 ] ; then
			printf "(CONF)  %-41s %s\n" "$NAME" "${RED}[Fail]${CLEAR}"
			return
		else
			printf "(CONF)  %-41s %s\n" "$NAME" "${GREEN}[OK]${CLEAR}"
		fi
		
		make -j 4 > "${OUT}/build-log.txt" 2>&1
		if [ "$?" != 0 ] ; then
			printf "(BUILD) %-41s %s\n" "$NAME" "${RED}[Fail]${CLEAR}"
			return
		else
			printf "(BUILD) %-41s %s\n" "$NAME" "${GREEN}[OK]${CLEAR}"
		fi
		
		RC=0
		TMP_OUT=$(../../tests/scripts/run_tests.sh $TYPE)
		RC=$?
		
		echo "$TMP_OUT"
		echo "$TMP_OUT" > "${OUT}/test-log.txt"
		
		mv test_out/* "${OUT}/"
		rm -Rf "${OUT}/tmp"
		
		if [ "$RC" != "0" ] ; then
			printf "(TEST)  %-41s %s\n" "$NAME" "${RED}[FAIL]${CLEAR}"
			return
		else
			printf "(TEST)  %-41s %s\n" "$NAME" "${GREEN}[OK]${CLEAR}"
		fi
	)
	
	
	#echo "DIR: $DIR, OPTS: $OPTS"
}


REBUILD=false
MATRIX=false

for o in $@ ; do
	if [ "$o" = "rebuild" ] ; then REBUILD=true ; fi
	if [ "$o" = "matrix" ] ; then MATRIX=true ; fi
done

if [ "$REBUILD" = "true" ] ; then
	rm -Rf "${BUILD_DIR}"
	rm -Rf "${OUT_DIR}"
fi
	
echo ""

while read line
do
	parse_cfg_line $line
	
	while read opts
	do
		if [ "$opts" = "" ] ; then continue ; fi
		if [ "$(echo "$opts" | head -c 1)" = "#" ] ; then continue ; fi
		if [ "$opts" = "%empty" ] ; then opts="" ; fi
		
		mk_build "$IDX" "$BUILD_TYPE" "$BUILD_ARGS $opts"
		
		IDX=$((IDX+1))
		
		if [ "$MATRIX" != "true" ] ; then break ; fi
		if [ "$BUILD_TYPE" = "internal" ] ; then break ; fi
	done < "./build_opts.txt"
	
	if [ "$MATRIX" != "true" ] ; then break ; fi
done < "./build_cfg.txt"

echo "================================================================================"
echo ""
