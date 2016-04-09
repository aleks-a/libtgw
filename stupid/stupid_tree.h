/* stupid_tree.h
 *
 * Copyright (C) 2016 aleks-a
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */
 
#ifndef STUPID__STUPID_TREE__H
#define STUPID__STUPID_TREE__H

#include "stupid_common.h"


struct st_test_t
{
	// Constructor to initialize variables
	st_test_t();
	
	// Basename of the file
	std::string name;
	
	// Path relative to root node. Starts with dot.
	// Convenience variable, since can be constructed
	// from name-members.
	std::string path;
	
	struct
	{
		bool cmiss, corr, cov, mem, perf, prof;
	} tests;
	
	bool build;
	std::string build_srcs;
	
	// Used IN ADDITION to similarly named variables in st_node_t
	std::string build_flags;
	std::string build_defs;
	std::string build_libs;
	std::string exec_args;
	std::string exec_envs;
};


struct st_node_t
{	
	// Constructor to initialize variables
	st_node_t();
	
	// Inherited from parents if not supplied
	std::string build_flags;
	std::string build_defs;
	std::string build_libs;
	std::string exec_args;
	std::string exec_envs;
	
	// Default values if not supplied
	bool generate_cmakelists;
	bool generate_subdir_incs;
	
	// Directory name, relative to parent. Does not contain
	// FS separators or dots
	std::string name;
	
	// Path relative to root node. Starts with dot and
	// ends with FS separator. Convenience variable, since can be
	// constructed from name-members.
	std::string path;
	
	// Actual data arrays
	std::vector<st_node_t> children;
	std::vector<st_test_t> tests;
};


struct stupid_tree_t
{
	// Constructor to initialize variables
	stupid_tree_t();
	
	// Config
	std::string out_path;
	bool flatten_bin_paths;
	
	// Internal
	std::string root_path;
	std::string bin_dir;
	std::string out_dir;
	std::string tmp_dir;
	bool root_node_valid;
	
	
	st_node_t root_node;
};


void load_stupid_tree(stupid_tree_t &t, const char *dir);
void dump_stupid_tree(const stupid_tree_t &t);

#endif

