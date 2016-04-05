#======================================================================
# CMakeLists.txt
#
# Copyright (C) 2016 aleks-a
#
# This software may be modified and distributed under the terms
# of the MIT license.  See the LICENSE file for details.
#======================================================================

cmake_minimum_required(VERSION 3.0.0)
project(libtgw)


set(TGW_MAJOR_VERSION 0)
set(TGW_MINOR_VERSION 2)
set(TGW_PATCH_VERSION 0)
set(TGW_VERSION_STRING
	"${TGW_MAJOR_VERSION}.${TGW_MINOR_VERSION}.${TGW_PATCH_VERSION}")
 
 
 
set(INSTALL_LIB_DIR lib CACHE PATH
	"Installation directory for libraries")
	
set(INSTALL_INCLUDE_DIR include CACHE PATH
	"Installation directory for header files")



option(TGW_NO_MALLOC
	"Don't use dynamic memory" true)
	
option(TGW_REG_ORDERED_NAMES
	"Faster look-up by name but consumed more memory" false)

set(TGW_REG_LOCK TGW_RLOCK_PTHREAD CACHE STRING
	"Locking mode used for registers")


set(TGW_COMPONENT_COUNT 256 CACHE STRING
	"Maxmimum component count or default initial size")

set(TGW_ERR_CODE_COUNT 1024 CACHE STRING
	"Maxmimum error code count or default initial size")

set(TGW_LOG_LEVEL_COUNT 128 CACHE STRING
	"Maxmimum log level count or default initial size")



if("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU")

	set(C_OPTS "-Wall -Wextra -Werror ")
	set(C_OPTS "${C_OPTS} -ansi -pedantic -pedantic-errors -fno-strict-aliasing")
	
	add_definitions(-D_XOPEN_SOURCE=600)
	
	set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} ${C_OPTS} -g -O0")
	set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} ${C_OPTS} -O3")
	
elseif ("${CMAKE_C_COMPILER_ID}" MATCHES "Clang")

	set(C_OPTS "-Wall -Wextra -Weverything -Werror ")
	set(C_OPTS "${C_OPTS} -std=c89 -pedantic -pedantic-errors")
	
	add_definitions(-D_XOPEN_SOURCE=600)
	 
	set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} ${C_OPTS} -g -O0")
	set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} ${C_OPTS} -O3")
	
endif()

if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
	add_definitions(-DTGW_DEBUG)
endif()


add_custom_target(
	build_stupid
	WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/stupid"
	COMMAND cmake . && make
)



include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}/inc
	${CMAKE_CURRENT_BINARY_DIR}/inc)

add_subdirectory(src)
add_subdirectory(inc)
add_subdirectory(tests)
