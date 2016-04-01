/* test_fw.h
 *
 * Copyright (C) 2016 aleks-a
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */
 
#ifndef TGW__TEST_FW__H
#define TGW__TEST_FW__H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void start_test(void);
void end_test(void);

void print_fail_str(const char *file, int line);

#define TEST_FAIL() print_fail_str(__FILE__, __LINE__)


#endif
