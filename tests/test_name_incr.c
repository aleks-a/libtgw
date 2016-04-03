/* test_name_incr.c
 *
 * Copyright (C) 2016 aleks-a
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */

#include "tgw_test_fw.h"
#include "tgw_name_incr.h"

#include <stdio.h>


static void test_1(int dynamic_mem);
static void test_2(void);

static void test_static_mem(char *str);
static void test_dynamic_mem(char *str);


int main(int argc, char **argv)
{
	(void)argc;
	(void)argv;
	
	start_test();
	
	test_1(0);
	test_1(1);
	
	test_2();
	
	end_test();
	
	return EXIT_SUCCESS;
}

static void test_1(int dynamic_mem)
{
	char a[] = "foo";
	char b[] = "foo-";
	char c[] = "foo*";
	char d[] = "foo-*";
	char e[] = "foo-**";
	char f[] = "foo-***";
	
	void(*func)(char *);
	func = dynamic_mem ? test_dynamic_mem : test_static_mem;
	
	func(a);
	func(b);
	func(c);
	func(d);
	func(e);
	func(f);
}

#define CHECK(NAME, COND) \
	do { \
		if( (COND) ) { \
			printf("%s: OK\n", NAME); \
		} else { \
			printf("%s: Failed\n", NAME); \
			TEST_FAIL(); \
		} \
	} while(0)
	
static void test_2(void)
{
	char str[] = "bar-**";
	struct tgw_name_incr incr;
	int r;
	char *s;
	
	r = tgw_name_incr_init(NULL, str);
	CHECK("Init with NULL ptr", r == -1);
	
	r = tgw_name_incr_init(&incr, NULL);
	CHECK("Init with NULL str", r == -1);
	
	r = tgw_name_incr_init(&incr, str);
	CHECK("Proper init", r == 0);
	
	s = tgw_name_incr_next(NULL);
	CHECK("Next with NULL ptr", s == NULL);
}

#undef CHECK

static void test_static_mem(char *str)
{
	struct tgw_name_incr incr;
	if(tgw_name_incr_init(&incr, str) != 0)
	{
		printf("String %s failed\n", str);
		TEST_FAIL();
	}
	
	while((str = tgw_name_incr_next(&incr)) != NULL)
	{
		printf("%s\n", str);
	}
	
	printf("\n");
}

static void test_dynamic_mem(char *str)
{
	struct tgw_name_incr *incr;
	incr = malloc(sizeof(struct tgw_name_incr));
	
	if(incr == NULL)
	{
		TEST_FAIL();
	}
	
	if(tgw_name_incr_init(incr, str) != 0)
	{
		TEST_FAIL();
	}
	
	while((str = tgw_name_incr_next(incr)) != NULL)
	{
		printf("%s\n", str);
	}
	
	printf("\n");
	free(incr);
}

