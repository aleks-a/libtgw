/* stupid_gen_cmake.c
 *
 * Copyright (C) 2016 aleks-a
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */

#include "stupid_tree.h"

int main(int argc, char **argv)
{
	(void)argv;
	
	if(argc > 2)
	{
		fprintf(stderr, "USAGE %s [dir_path]\n", argv[0]);
	}
	
	stupid_tree_t tree;
	const char *dir = (argc == 2) ? argv[1] : ".";
	
	try
	{
		load_stupid_tree(tree, dir);
		dump_stupid_tree(tree);
	}
	catch(stupid_exception &e)
	{
		s_err("%s\n", e.what());
		return EXIT_FAILURE;
	}
	catch(std::exception &e)
	{
		s_err("stdexception: %s\n", e.what());
		return EXIT_FAILURE;
	}
	
	return 0;
}
