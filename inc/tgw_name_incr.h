/* tgw_name_incr.h
 *
 * Copyright (C) 2016 aleks-a
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */

#ifndef TGW__TGW_NAME_INCR__H
#define TGW__TGW_NAME_INCR__H

#include "tgw_hdr_config.h"

#ifdef __cplusplus
extern "C" {
#endif

struct tgw_name_incr
{
	char *str;
	size_t len;
	size_t adj_chars;
	size_t step;
};

int tgw_name_incr_init(struct tgw_name_incr *incr, char *str);
char * tgw_name_incr_next(struct tgw_name_incr *incr);

#ifdef __cplusplus
}
#endif

#endif
