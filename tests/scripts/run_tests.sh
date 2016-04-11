#!/bin/sh

#======================================================================
# run_tests.sh
#
# Copyright (C) 2016 aleks-a
#
# This software may be modified and distributed under the terms
# of the MIT license.  See the LICENSE file for details.
#======================================================================


list_intersect()
{
	for i in $1 ; do
		for j in $2 ; do
			if [ "$i" = "$j" ] ; then
				return 0
			fi
		done
	done
	
	return 1
}


validate_run_type()
{
	ALLOWED="all mem corr cov callg"
	if ! list_intersect "$ALLOWED" "$1" ; then
		echo "Illegal run type: $1" >&2
		exit 1
	fi
}

validate_test_type()
{
	ALLOWED="cmiss corr corr_nocov mem callg"
	if ! list_intersect "$ALLOWED" "$1" ; then
		echo "Illegal test type: $1" >&2
		exit 1
	fi
}


parse_line()
{
	line=$1
	i=0;
	
	BUILD=false
	SCRIPT=false
	INTERNAL=false
	
	if [ "$line" = "" ] ; then continue ; fi
	if [ "$(echo "$line" | head -c 1)" = "#" ] ; then continue ; fi
	
	for tok in $line
	do
		if [ $i = 0 ] ; then
			SOURCE=$tok
			NAME=$(basename $tok)
			NAME=${NAME%.*}
			NBASE=${SOURCE%.*}
		elif [ $i = 1 ] ; then
			TYPE_LIST="$(echo $tok | sed 's/,/ /g')"
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
	
	
	TMP=""
	for t in $TYPE_LIST ; do
		validate_test_type $t
		if [ "$t" = "corr" ] ; then
			TMP="$TMP corr_cov cov"
		elif [ "$t" = "corr_nocov" ] ; then
			TMP="$TMP corr_nocov"
		else
			TMP="$TMP $t"
		fi
	done
	
	TYPE_LIST=""
	for t in $TMP ; do
		if ! list_intersect "$TYPE_LIST" "$t" ; then
			TYPE_LIST="$TYPE_LIST $t"
		fi
	done
}


run_this_test()
{
	# Check that type is ok
	if ! list_intersect "$1" "$2" ; then return 1 ; fi
	
	# Check that name is ok
	if [ "$4" != "" ] && ! list_intersect "$3" "$4" ; then
		return 1
	fi
	
	return 0;
}


load_config()
{
	if [ "$1" = "" ] ; then
		echo "Please supply test types to run" >&2
		exit 1
	fi
	
	if [ "$1" = "internal" ] ; then
		RUN_INTERNAL=true
	else
		RUN_INTERNAL=false
		
		TMP=""
		for t in $(echo $1 | sed 's/,/ /g') ; do
			validate_run_type $t
			if [ "$t" = "corr" ] ; then
				TMP="$TMP corr_cov corr_nocov"
			elif [ "$t" = "cov" ] ; then
				TMP="$TMP cov corr_nocov"
			else
				TMP="$TMP $t"
			fi
		done
		
		IGN_CORR_COV=false
		if list_intersect "$TMP" "corr_cov" && list_intersect "$TMP" "cov"
		then
			IGN_CORR_COV=true
		fi
		
		RUN_TYPES=""
		for t in $TMP ; do
			if ! list_intersect "$RUN_TYPES" "$t" ; then
				if [ "$t" = "corr_cov" ] && [ "$IGN_CORR_COV" = "true" ]
				then
					continue
				fi
				
				RUN_TYPES="$RUN_TYPES $t"
			fi
		done
	fi
	
	CFG_DIR="."
	
	i=0
	while read line
	do
		if [ $i = 0 ] ; then SRC_DIR="$line"
		elif [ $i = 1 ] ; then BIN_DIR="$line"
		elif [ $i = 2 ] ; then OUT_DIR="$line"
		fi
		
		i=$((i+1))
	done < "${CFG_DIR}/test_config.txt"

	if [ $i != 3 ] ; then
		echo "Invalid ${CFG_DIR}/test_config.txt file" >&2
		exit 1
	fi
	
	shift
	
	RUN_NAMES=""
	for name in $@ ; do
		RUN_NAMES="$RUN_NAMES $name"
	done
	
	
	TMP_DIR="$OUT_DIR/tmp"
	mkdir -p "$TMP_DIR"
}

print_status()
{
	if [ "$3" != "" ] ; then PS_TYPE=$3
	else PS_TYPE=$STYPE ; fi
	
	if [ "$4" = "NONE" ] ; then PS_NAME=""
	elif [ "$4" != "" ] ; then PS_NAME="$4"
	else PS_NAME="$NAME" ; fi
	
	if [ "$PS_TYPE" = "corr_cov" ] || [ "$PS_TYPE" = "corr_nocov" ]
	then
		TS="CORR"
	else
		TS=$(echo $PS_TYPE | tr '[:lower:]' '[:upper:]')
	fi
	
	printf "%s %-7s %-37s %s[%s]${CLEAR}\n" \
		"---" "(${TS})" "$PS_NAME" "$1" "$2"
}

RED=$(tput setaf 1)
YELLOW=$(tput setaf 3)
GREEN=$(tput setaf 2)
MAG=MAGENTA=$(tput setaf 5)
CLEAR=$(tput sgr0)

clear_cov()
{
	COV_LOG="${OUT_DIR}/cov_log.txt"
	echo "" > "$COV_LOG"
	
	#lcov --base-directory . --directory . --zerocounters -q
	#lcov --zerocounters \
	#lcov --zerocounters \
	#	>> "$COV_LOG" 2>&1
	
	lcov --zerocounters --directory . \
		>> "$COV_LOG" 2>&1
	
	if [ "$?" != "0" ] ; then
		print_status $RED FAIL LCOV0 NONE
		exit 1
	fi
	echo "" >> "$COV_LOG"
	echo "" >> "$COV_LOG"
	
	print_status $GREEN OK LCOV NONE
}

collect_cov()
{
	COV_FILE="${OUT_DIR}/cov.info"
	COV_OUT="${OUT_DIR}/cov"
	COV_LOG="${OUT_DIR}/cov_log.txt"
	
	
	lcov  --directory . -c -o "$COV_FILE" \
		>> "$COV_LOG" 2>&1
	if [ "$?" != "0" ] ; then
		print_status $RED FAIL LCOV1 NONE
		exit 1
	fi
	
	lcov --remove "$COV_FILE" "/usr*" -o "$COV_FILE" \
		>> "$COV_LOG" 2>&1
	if [ "$?" != "0" ] ; then
		print_status $RED FAIL LCOV2 NONE
		exit 1
	fi
	
	lcov --remove "$COV_FILE" "*/tests/*" -o "$COV_FILE" \
		>> "$COV_LOG" 2>&1
	if [ "$?" != "0" ] ; then
		print_status $RED FAIL LCOV3 NONE
		exit 1
	fi
	
	
	rm -rf "$COV_OUT"
	
	genhtml -o "$COV_OUT" -t "LibTGW Coverage" --num-spaces 4 \
		"$COV_FILE" \
		>> "$COV_LOG" 2>&1
	if [ "$?" != "0" ] ; then
		print_status $RED FAIL LCOV4 NONE
		exit 1
	fi
	
	print_status $GREEN OK LCOV NONE
}


run_corr_test()
{
	RES_FILE="${SRC_DIR}/${NBASE}.corr.txt"
	RUN_OUT="${OUT_DIR}/${NAME}.corr.txt"
	DIFF_OUT="${OUT_DIR}/${NAME}.diff.txt"
	
	STATUS="${MAG}[WTF]${CLEAR}"
	
	"$BINARY" > "$RUN_OUT" 2>&1
	RC=$?
	if [ $RC = 0 ] ; then
		
		diff "$RUN_OUT" "$RES_FILE" > "$DIFF_OUT"
		RC=$?
		if [ $RC = 0 ] ; then
			print_status $GREEN OK
		else
			print_status $RED DIFF
		fi
	else
		print_status $RED "RC $RC"
	fi
}

run_mem_test()
{
	MEM_OUT="${OUT_DIR}/${NAME}.mem.txt"
	
	STATUS="${MAG}[WTF]${CLEAR}"
	
	valgrind --leak-check=full --trace-children=yes \
		"$BINARY" 2> "$MEM_OUT" 1> /dev/null
	
	RC=$?
	if [ $RC = 0 ] ; then
		if grep -Fq "ERROR SUMMARY: 0 errors from 0 contexts" "$MEM_OUT"
		then
			print_status $GREEN OK
		else
			print_status $YELLOW ERR
		fi
	else
		print_status $RED "RC $RC"
	fi
}

run_callg_test()
{
	
	MEM_OUT="${OUT_DIR}/${NAME}.mem.txt"
	
	STATUS="${MAG}[WTF]${CLEAR}"
	
	valgrind --tool=callgrind \
		--callgrind-out-file="${OUT_DIR}/${NAME}.callg.out" \
		"$BINARY" \
		> "${OUT_DIR}/${NAME}.callg.txt" 2>&1
	
	RC=$?
	if [ $RC = 0 ] ; then
		print_status $GREEN OK
	else
		print_status $RED "RC $RC"
	fi
}


run_tests()
{
	STYPE=$1
	
	if [ "$STYPE" = "cov" ] ; then clear_cov ; fi
	
	while read line
	do
		parse_line "$line"
		
		if ! run_this_test $STYPE "$TYPE_LIST" $NAME "$NAME_LIST"
		then
			continue
		fi
		
		if [ "$INTERNAL" != "$RUN_INTERNAL" ] ; then continue ; fi
		
		BINARY="${BIN_DIR}/$NAME"
		
		if list_intersect "corr_cov corr_nocov cov" $STYPE
				then run_corr_test
		elif [ "$STYPE" = "mem" ]
			then run_mem_test
		elif [ "$STYPE" = "callg" ]
			then run_callg_test
		else
			echo "Unsupported test type $STYPE" >&2
			exit 1
		fi
		
	done < "${SRC_DIR}/test_list.txt"
	
	if [ "$STYPE" = "cov" ] ; then collect_cov ; fi
	
}


load_config $@


if [ "$RUN_INTERNAL" = "true" ] ; then
	RUN_TYPES="corr_cov corr_nocov mem"
fi

for t in $RUN_TYPES ; do
	run_tests $t
done

