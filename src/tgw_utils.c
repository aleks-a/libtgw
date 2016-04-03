/* tgw_utils.c
 *
 * Copyright (C) 2016 aleks-a
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */
 

#include "tgw_src_config.h"
#include "tgw_utils.h"



int tgw_major_version(void)
{
	return TGW_MAJOR_VERSION;
}

int tgw_minor_version(void)
{
	return TGW_MINOR_VERSION;
}

int tgw_patch_version(void)
{
	return TGW_PATCH_VERSION;
}

int tgw_version_int(void)
{
	return TGW_VERSION_INT;
}

const char *tgw_version_str(void)
{
	return TGW_VERSION_STRING;
}


void tgw_dump_info(void)
{
#ifdef TGW_NO_MALLOC
	const char *no_malloc = "true";
#else
	const char *no_malloc = "false";
#endif

#ifdef TGW_REG_ORDERED_NAMES
	const char *ordered_names = "true";
#else
	const char *ordered_names = "false";
#endif

#if TGW_REG_LOCK == TGW_RLOCK_NONE
	const char *rlock = "none";
#elif TGW_REG_LOCK == TGW_RLOCK_PTHREAD
	const char *rlock = "pthread";
#endif

	debug_out("\n");
	debug_out("[tgw_dump_info] Header version:           %s\n", TGW_VERSION_STRING);
	debug_out("[tgw_dump_info] Header version (int):     %d\n", TGW_VERSION_INT);
	debug_out("[tgw_dump_info] Binary version:           %s\n", tgw_version_str());
	debug_out("[tgw_dump_info] Binary version (int):     %d\n", tgw_version_int());
	debug_out("\n");
	debug_out("[tgw_dump_info] Component count:          %d\n", TGW_COMPONENT_COUNT);
	debug_out("[tgw_dump_info] Error code count:         %d\n", TGW_ERR_CODE_COUNT);
	debug_out("[tgw_dump_info] Log level count:          %d\n", TGW_LOG_LEVEL_COUNT);
	debug_out("\n");
	debug_out("[tgw_dump_info] No malloc:                %s\n", no_malloc);
	debug_out("[tgw_dump_info] Ordered names:            %s\n", ordered_names);
	debug_out("[tgw_dump_info] Register locking:         %s\n", rlock);
	debug_out("\n");
}

#ifdef TGW_DEBUG

#include <stdarg.h>
#include <stdio.h>

#ifdef __clang__
__attribute__((__format__ (__printf__, 1, 0)))
#endif
void debug_out(const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	vprintf(fmt, args);
	va_end(args);
}

#else /* TGW_DEBUG */

void debug_out(const char *fmt, ...)
{
	(void)fmt;
}

#endif /* TGW_DEBUG */
