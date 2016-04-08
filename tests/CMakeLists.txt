#======================================================================
# CMakeLists.txt
#
# Copyright (C) 2016 aleks-a
#
# This software may be modified and distributed under the terms
# of the MIT license.  See the LICENSE file for details.
#======================================================================



# TGW test framework
add_definitions(-DTGW_TEST_SRC_DIR="${CMAKE_SOURCE_DIR}/tests")
include_directories(fw)
add_library(test_fw STATIC fw/tgw_test_fw.c)
set(STUPID_AUTOBUILD_LIBS test_fw tgw)

execute_process(
	COMMAND "${CMAKE_SOURCE_DIR}/stupid/generate_scripts.sh"
		"${CMAKE_CURRENT_SOURCE_DIR}"
		"${CMAKE_CURRENT_BINARY_DIR}"
		"${CMAKE_BINARY_DIR}"
		"${CMAKE_BINARY_DIR}/test_results"
		"${CMAKE_SOURCE_DIR}/stupid"
		"${LIBS}"
)

include("${CMAKE_CURRENT_SOURCE_DIR}/stupid_build.cmake")

# Non-autobuilt tests






