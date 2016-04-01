#======================================================================
# CMakeLists.txt
#
# Copyright (C) 2016 aleks-a
#
# This software may be modified and distributed under the terms
# of the MIT license.  See the LICENSE file for details.
#======================================================================


set(TGW_TEST_BIN_DIR ${CMAKE_CURRENT_BINARY_DIR})
set(TGW_TEST_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(TGW_TEST_OUT_DIR ${CMAKE_BINARY_DIR}/test_results)
set(TGW_TEST_SCRIPT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/util)

configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/util/run_tests.sh.in
	${CMAKE_CURRENT_BINARY_DIR}/util/run_tests.sh
	ESCAPE_QUOTES @ONLY
)

file(COPY
	${CMAKE_CURRENT_BINARY_DIR}/util/run_tests.sh
	DESTINATION ${CMAKE_BINARY_DIR}
	FILE_PERMISSIONS
		OWNER_READ OWNER_WRITE OWNER_EXECUTE
		GROUP_READ GROUP_EXECUTE 
		WORLD_READ
)



add_definitions(-DTGW_TEST_SRC_DIR="${CMAKE_SOURCE_DIR}/tests")
include_directories(fw)
add_library(test_fw STATIC fw/tgw_test_fw.c)
set(LIBS test_fw tgw)


function(process_test_lists STUPID_FILE OUTLIST)
	file(STRINGS "${STUPID_FILE}" LINES)
	foreach(L IN LISTS LINES)
		if("${L}" MATCHES "^([ \t]*)#")
			continue()
		elseif("${L}" MATCHES "^([ \t]*)$")
			continue()
		endif()
		separate_arguments(TOKENS UNIX_COMMAND "${L}")
		foreach(T IN LISTS TOKENS)
			if("${T}" STREQUAL "autobuild=true")
				list(GET TOKENS 0 CURRENT_NAME)
				list(APPEND BLIST "${CURRENT_NAME}")
			endif()
		endforeach(T)
	endforeach(L)
	set(${OUTLIST} ${BLIST} PARENT_SCOPE)
endfunction(process_test_lists)

# TODO: This would need fixing so that changing stupid.txt
# causes rebuilding of the list, however seems to be easier
# said than done

file(GLOB_RECURSE STUPID_FILES
	RELATIVE "${TGW_TEST_SRC_DIR}"
	"stupid.txt"
)

foreach(F IN LISTS STUPID_FILES)
	process_test_lists("${TGW_TEST_SRC_DIR}/${F}" AUTOBUILD_LIST OUTLIST)
	list(APPEND AUTOBUILD_LIST ${OUTLIST})
endforeach(F)

list(REMOVE_DUPLICATES AUTOBUILD_LIST)


message("Generating autobuild list for tests")
foreach(S IN LISTS AUTOBUILD_LIST)
	message("--- ${S}")
	add_executable(${S} ${S}.c)
	target_link_libraries(${S} ${LIBS})
endforeach(S)





