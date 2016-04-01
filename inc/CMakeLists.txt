#======================================================================
# CMakeLists.txt
#
# Copyright (C) 2016 aleks-a
#
# This software may be modified and distributed under the terms
# of the MIT license.  See the LICENSE file for details.
#======================================================================

configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/tgw_hdr_config.h.in
	${CMAKE_CURRENT_BINARY_DIR}/tgw_hdr_config.h
	@ONLY)

configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/tgw_src_config.h.in
	${CMAKE_CURRENT_BINARY_DIR}/tgw_src_config.h
	@ONLY)

set(HDRS "tgw_hdr_config.h tgw_utils.h")

set_target_properties(tgw PROPERTIES PUBLIC_HEADER "${HDRS}")
