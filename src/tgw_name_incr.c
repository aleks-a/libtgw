/* tgw_name_incr.c
 *
 * Copyright (C) 2016 aleks-a
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */
 

#include "tgw_src_config.h"
#include "tgw_name_incr.h"
#include <stdio.h>

static size_t get_digit_count(size_t number);

/* Zero-based indexing starting from right */
static char get_nth_digit(size_t n, size_t number);

int tgw_name_incr_init(struct tgw_name_incr *incr, char *str)
{
	size_t len, count = 0;
	
	if(incr == NULL) { return -1; }
	
	if(str == NULL || (len = strlen(str)) <= 0)
	{
		incr->str = NULL;
		return -1;
	}
	
	while(len > 0 && str[--len] == '*') { count++; }
	if(len <= 0 || str[len] != '-' || count == 0)
	{
		count = 0;
		len = strlen(str);
	}
	
	incr->len = len;
	incr->str = str;
	incr->str[len] = '\0';
	incr->adj_chars = count;
	incr->step = 0;
	
	return 0;
}

char * tgw_name_incr_next(struct tgw_name_incr *incr)
{
	size_t dcnt, i = 0;
	
	/* Not exhaustive list, but probably enough since one is
	 * not supposed to call this if init failed anyway... */
	if(incr == NULL || incr->str == NULL || incr->len < 1)
	{
		return NULL;
	}
	
	dcnt = get_digit_count(incr->step);
	if(incr->step == 0)
	{
		incr->step++;
		return incr->str;
	}
	else if(incr->adj_chars < dcnt)
	{
		return NULL;
	}
	
	incr->str[incr->len] = '-';
	while(i < dcnt)
	{
		incr->str[incr->len + dcnt - i] = get_nth_digit(i, incr->step);
		i++;
	}
	incr->str[incr->len+dcnt+1] = '\0';
	
	incr->step++;
	
	return incr->str;
}

static size_t get_digit_count(size_t number)
{
	size_t cnt;
	
	if(number == 0)
	{
		return 0;
	}
	
	cnt = 1;
	while((number = number / 10) > 0) cnt++;
	
	return cnt;
}

static char get_nth_digit(size_t n, size_t number)
{
	size_t a = 1;
	while(n-- > 0) a *= 10;
	a = (number / a) % 10;
	return (char)a + '0';
}

