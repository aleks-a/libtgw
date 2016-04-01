#======================================================================
# CMakeLists.txt
#
# Copyright (C) 2016 aleks-a
#
# This software may be modified and distributed under the terms
# of the MIT license.  See the LICENSE file for details.
#======================================================================

set(SRCS tgw_utils.c tgw_name_incr.c)

add_library(tgw SHARED ${SRCS})

if("${TGW_REG_LOCK}" STREQUAL "TGW_RLOCK_PTHREAD")
	target_link_libraries(tgw pthread)
endif()

install(
	TARGETS tgw
	LIBRARY DESTINATION "${INSTALL_LIB_DIR}" COMPONENT shlib
	PUBLIC_HEADER DESTINATION "${INSTALL_INCLUDE_DIR}" COMPONENT dev
	)



