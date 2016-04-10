/* test_mem_ok.c
 *
 * Copyright (C) 2016 aleks-a
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */

#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	char *tmp = malloc(128);
	snprintf(tmp, 128, "Hello mem\n");
	puts(tmp);
	free(tmp);
	
	return 0;
}
