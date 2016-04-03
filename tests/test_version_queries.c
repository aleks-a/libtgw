/* test_version_queries.c
 *
 * Copyright (C) 2016 aleks-a
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */

#include "tgw_test_fw.h"
#include "tgw_utils.h"

#include <stdio.h>




int main(int argc, char **argv)
{
	(void)argc;
	(void)argv;
	
	start_test();
	
	printf("Major version:     %d\n", tgw_major_version());
	printf("Minor version:     %d\n", tgw_minor_version());
	printf("Patch version:     %d\n", tgw_patch_version());
	printf("Version int:       %d\n", tgw_version_int());
	printf("Version str:       %s\n", tgw_version_str());
	
	end_test();
	
	return EXIT_SUCCESS;
}

