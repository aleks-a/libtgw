/* stupid_run.c
 *
 * Copyright (C) 2016 aleks-a
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */

#include "stupid_tree.h"

#include <cstring>

#include "run_command.h"

static void run_tests(const stupid_tree_t &tree, const char *type);


int main(int argc, char **argv)
{
	(void)argv;
	
	if(argc < 3 || argc > 4)
	{
		fprintf(stderr, "USAGE %s type bin_dir [src_dir]\n", argv[0]);
	}
	
	stupid_tree_t tree;
	const char *const type = argv[1];
	const char *const bin = argv[2];
	const char *const dir = (argc == 4) ? argv[3] : ".";
	
	
	try
	{
		load_stupid_tree(tree, dir);
		dump_stupid_tree(tree);
		tree.bin_dir = bin;
		tree.out_dir = tree.bin_dir + FSS + "test_out";
		tree.tmp_dir = tree.out_dir + FSS + "tmp";
		run_tests(tree, type);
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

static void run_node_tests(const stupid_tree_t &tree,
	const st_node_t &node, const char *type);

static bool check_test(const char *type, const st_test_t &test);

static void run_tests(const stupid_tree_t &tree, const char *type)
{
	run_node_tests(tree, tree.root_node, type);
}


static void run_corr_test(const stupid_tree_t &tree,
	const st_test_t &test,
	const std::string &bin, const std::string res_dir);

static void run_mem_test(const stupid_tree_t &tree,
	const st_test_t &test,
	const std::string &bin, const std::string res_dir);

#define DISPATCH_TEST(TYPE) \
	if(strcmp(type, #TYPE ) == 0) \
		{ run_ ## TYPE ## _test(tree, *tit, bin, res_pre); }

static void run_node_tests(const stupid_tree_t &tree,
	const st_node_t &node, const char *type)
{
	std::vector<st_node_t>::const_iterator cit;
	std::vector<st_test_t>::const_iterator tit;
	std::string res_pre, bin, tpath;
	
	mkdirp(tree.out_dir + FSS + node.path);
	
	
	for(tit = node.tests.begin(); tit != node.tests.end(); ++tit)
	{
		if( ! check_test(type, *tit) ) { continue; }
		
		tpath = (tree.flatten_bin_paths) ? tit->name : tit->path;
		bin = "\"" + tree.bin_dir + FSS + tpath + "\"";
		
		res_pre = tree.out_dir + FSS + tit->path;
		
		DISPATCH_TEST(corr)
		DISPATCH_TEST(mem)
	}
	
	
	for(cit = node.children.begin(); cit != node.children.end(); ++cit)
	{
		run_node_tests(tree, *cit, type);
	}
}

#undef DISPATCH_TEST




static void run_corr_test(const stupid_tree_t &tree,
	const st_test_t &test,
	const std::string &bin, const std::string res_dir)
{
	std::string name;
	char msg[128];
	std::string diff;
		
	const char *status;
	int rc;
	std::string cmd;
	cmd = bin + " > \"" + res_dir + ".corr.txt\" 2>&1";
	run_command(cmd, rc);
	
	if(rc == 0)
	{
		if(diff_files(
			res_dir + ".corr.txt",
			tree.root_path + FSS + test.path + ".corr.txt",
			res_dir + ".diff.txt"))
		{
			status = "OK";
			msg[0] = '\0';
		}
		else
		{
			status = "Fail";
			snprintf(msg, 128, "Files differ");
		}
	}
	else if(rc == 127)
	{
		status = "Fail";
		snprintf(msg, 128, "Binary %s not found", bin.c_str());
	}
	else
	{
		status = "Fail";
		snprintf(msg, 128, "Return code: %d", rc);
	}
	
	name = test.path.substr(2) + ":";
	s_tres("%-39s [%s] %s\n", name.c_str(), status, msg);
}


static void run_mem_test(const stupid_tree_t &tree,
	const st_test_t &test,
	const std::string &bin, const std::string res_dir)
{
	(void)tree;
	
	std::string name;
	char msg[128];
	std::string diff;
		
	const char *status;
	int rc;
	std::string cmd;
	cmd = "valgrind --trace-children=yes --leak-check=full " + bin +
		" 2> \"" + res_dir + ".mem.txt\" > /dev/null";
	run_command(cmd, rc);
	
	if(rc == 0)
	{
		msg[0] = '\0';
		status = "Ran";
	}
	else if(rc == 127)
	{
		status = "Err";
		snprintf(msg, 128, "Binary %s not found", bin.c_str());
	}
	else
	{
		status = "Err";
		snprintf(msg, 128, "Return code: %d", rc);
	}
	
	name = test.path.substr(2) + ":";
	s_tres("%-39s [%s] %s\n", name.c_str(), status, msg);
}





#define CHECK_TEST(TYPE) \
	if(strcmp(type, #TYPE ) == 0) { return test.tests.TYPE ; }

static bool check_test(const char *type, const st_test_t &test)
{
	CHECK_TEST(cmiss)
	CHECK_TEST(corr)
	CHECK_TEST(cov)
	CHECK_TEST(mem)
	CHECK_TEST(perf)
	CHECK_TEST(prof)
	
	throw stupid_exception("Unknown test type: %s", type);
}
#undef CHECK_TEST

