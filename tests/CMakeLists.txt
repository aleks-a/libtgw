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
set(TEST_LIBS test_fw tgw)




# Custom-built tests




# Script things
set(TEST_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(TEST_BIN_DIR "${CMAKE_CURRENT_BINARY_DIR}")
set(TEST_OUT_DIR "${CMAKE_BINARY_DIR}/test_out" CACHE PATH
	"Output dir for tests")


configure_file(
	"${CMAKE_CURRENT_SOURCE_DIR}/scripts/test_config.txt"
	"${CMAKE_BINARY_DIR}/test_config.txt"
	@ONLY
)
	
execute_process(
	WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
	COMMAND "${CMAKE_CURRENT_SOURCE_DIR}/scripts/generate_files.sh"
)

include(test_build.cmake)

