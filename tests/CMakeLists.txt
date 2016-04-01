#======================================================================
# CMakeLists.txt
#
# Copyright (C) 2016 aleks-a
#
# This software may be modified and distributed under the terms
# of the MIT license.  See the LICENSE file for details.
#======================================================================


include_directories(fw)
add_definitions(-DTGW_TEST_SRC_DIR="${CMAKE_SOURCE_DIR}/tests")

add_library(test_fw STATIC fw/tgw_test_fw.c)
set(LIBS test_fw tgw)

add_executable(test_name_incr test_name_incr.c)
target_link_libraries(test_name_incr ${LIBS})

add_executable(test_version_queries test_version_queries.c)
target_link_libraries(test_version_queries ${LIBS})
