/* test_fw.c
 *
 * Copyright (C) 2016 aleks-a
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */
 
#include "tgw_test_fw.h"


void start_test(void)
{
	/*
	printf("******************** START OF THE TEST ********************\n");
	*/
}

void end_test(void)
{
	/*
	printf("******************** END OF THE TEST **********************\n");
	*/
}


void print_fail_str(const char *file, int line)
{
	if(strncmp(TGW_TEST_SRC_DIR, file, strlen(TGW_TEST_SRC_DIR)) == 0)
	{
		file += strlen(TGW_TEST_SRC_DIR);
		if(*file == '/' || *file == '\\')
		{
			file++;
		}
	}
	
	printf("Test %s failed on line %d\n", file, line);
	exit(EXIT_FAILURE);
}
