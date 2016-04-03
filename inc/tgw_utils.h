/* tgw_utils.h
 *
 * Copyright (C) 2016 aleks-a
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */
 
#ifndef TGW__TGW_UTILS__H
#define TGW__TGW_UTILS__H

#include "tgw_hdr_config.h"

#ifdef __cplusplus
extern "C" {
#endif

int tgw_major_version(void);
int tgw_minor_version(void);
int tgw_patch_version(void);

int tgw_version_int(void);
const char * tgw_version_str(void);

void tgw_dump_info(void);

#ifdef __cplusplus
}
#endif

#endif

