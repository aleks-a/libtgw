/* stupid_tree.cpp
 *
 * Copyright (C) 2016 aleks-a
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */
 
#include "stupid_tree.h"
#include "string_utils.h"

st_test_t::st_test_t():
	build(false)
{
	tests.cmiss	= false;
	tests.corr	= false;
	tests.cov	= false;
	tests.mem	= false;
	tests.perf	= false;
	tests.prof	= false;
}

st_node_t::st_node_t():
	generate_cmakelists(false)
{	
}

stupid_tree_t::stupid_tree_t():
	flatten_bin_paths(false), root_node_valid(false)
{
}

static void load_config(stupid_tree_t &t);
static void handle_config_var(const kv_pair_t &pair, stupid_tree_t &t);

static void load_stupid_file(const stupid_tree_t &t, st_node_t &node);
static void handle_special_line(const kv_vec_t &vec,
	const stupid_tree_t &t, st_node_t &node);
static void handle_test_var(const kv_pair_t &pair,
	const stupid_tree_t &t, st_test_t &test);

static void parse_tests_str(const std::string &str, st_test_t &test);
static std::string find_source_file(const std::string &base);

void load_stupid_tree(stupid_tree_t &t, const char *dir)
{
	t.root_path = dir;
	load_config(t);
	
	t.root_node.name = "";
	t.root_node.path = "./";
	load_stupid_file(t, t.root_node);
	t.root_node_valid = true;
}


static void load_config(stupid_tree_t &t)
{
	std::string tmp;
	kv_vec_t vec;
	kv_vec_t::iterator kv_it;
	
	tmp = t.root_path + FSS + "stupid-cfg.txt";
	const char *const path = tmp.c_str();
	std::ifstream ifs(path);
	if( ! ifs.good() )
	{
		throw stupid_exception("Failed to open file \"%s\"", path);
	}
	
	while(read_logical_line(ifs, tmp, path))
	{
		parse_line(tmp, vec);
		for(kv_it = vec.begin(); kv_it != vec.end(); ++kv_it)
		{
			handle_config_var(*kv_it, t);
		}
		vec.clear();
	}
}

static void handle_config_var(const kv_pair_t &pair, stupid_tree_t &t)
{
	const std::string key(pair.first);
	const std::string val(pair.second);
	
	if(key == "output-path")
	{
		t.out_path = val;
	}
	else if(key == "flatten-bin-paths")
	{
		t.flatten_bin_paths = parse_bool(val);
	}
	else
	{
		throw stupid_exception("Invalid key: %s\n", key.c_str());
	}
}

static void load_stupid_file(const stupid_tree_t &t, st_node_t &node)
{
	std::string tmp;
	kv_vec_t vec;
	kv_vec_t::iterator kv_it;
	
	tmp = t.root_path + FSS + node.path + FSS + "stupid.txt";
	const char *const path = tmp.c_str();
	std::ifstream ifs(path);
	
	if( ! ifs.good() )
	{
		throw stupid_exception("Failed to open file \"%s\"", path);
	}
	
	while(read_logical_line(ifs, tmp, path))
	{
		if(tmp[0] == '%')
		{
			tmp = tmp.substr(1);
			parse_line(tmp, vec);
			handle_special_line(vec, t, node);
			vec.clear();
			continue;
		}
		
		parse_line(tmp, vec);
		
		if(vec.empty())
		{
			// Internal error, never should be here
			throw stupid_exception("Internal error: empty line: %s\n", tmp.c_str());
		}
		
		if(vec.size() > 0 && tmp[0] == '%')
		{
			handle_special_line(vec, t, node);
			continue;
		}
		
		if(vec.size() < 2)
		{
			throw stupid_exception("Invalid line: %s\n", tmp.c_str());
		}
		if( ! vec[0].first.empty() || ! vec[1].first.empty() )
		{
			throw stupid_exception("Invalid line: %s\n", tmp.c_str());
		}
		
		st_test_t tmp_test;
		
		tmp_test.name = vec[0].second;
		tmp_test.path = node.path + tmp_test.name;
		parse_tests_str(vec[1].second, tmp_test);
		
		for(kv_it = vec.begin()+2; kv_it != vec.end(); ++kv_it)
		{
			if(kv_it->first.empty())
			{
				throw stupid_exception("Invalid line: %s\n", tmp.c_str());
			}
			
			handle_test_var(*kv_it, t, tmp_test);
		}
		
		if(tmp_test.build && tmp_test.build_srcs == "")
		{
			tmp_test.build_srcs = tmp_test.name + "." +
				find_source_file(t.root_path + FSS + tmp_test.path);
		}
		
		node.tests.push_back(tmp_test);
		vec.clear();
	}
}

static void handle_special_line(const kv_vec_t &vec,
	const stupid_tree_t &t, st_node_t &node)
{
	kv_vec_t::const_iterator it;
	
	//printf("!!! SPEC\n");
	//dump_kv_vec(vec);
	
	if(vec[0].first != "")
	{
		if(vec.size() != 1)
		{
			throw stupid_exception("Invalid line");
		}
		
		const std::string key(vec[0].first);
		const std::string val(vec[0].second);
		
		if(key == "generate-cmakelists")
		{
			node.generate_cmakelists = parse_bool(val);
		}
		else
		{
			throw stupid_exception("Invalid key %s", key.c_str());
		}
	}
	else
	{
		if(vec[0].second == "subdir")
		{
			for(it = ++vec.begin(); it != vec.end(); ++it)
			{
				if( ! it->first.empty() )
				{
					throw stupid_exception("Invalid line");
				}
				
				st_node_t tmp_node;
				tmp_node.name = it->second;
				tmp_node.path = node.path + it->second + FSS;
				node.children.push_back(tmp_node);
				load_stupid_file(t, node.children.back());
				
			}
		}
	}
	
	(void)t;
}

static void handle_test_var(const kv_pair_t &pair,
	const stupid_tree_t &t, st_test_t &test)
{
	(void)t;
	
	if(pair.first == "build")
	{
		test.build = parse_bool(pair.second);
	}
	else if(pair.first == "build-srcs")
	{
		test.build_srcs = pair.second;
	}
	else
	{
		throw stupid_exception("Invalid key: %s\n", pair.first.c_str());
	}
}

static void parse_tests_str(const std::string &str, st_test_t &test)
{
	strvec_t vec;
	strvec_t::iterator it;
	tokenize(str, vec, ',');
	
	for(it = vec.begin(); it != vec.end(); ++it)
	{
		if(*it == "cmiss")	{ test.tests.cmiss	= true; }
		if(*it == "corr")	{ test.tests.corr	= true; }
		if(*it == "cov")	{ test.tests.cov	= true; }
		if(*it == "mem")	{ test.tests.mem	= true; }
		if(*it == "perf")	{ test.tests.perf	= true; }
		if(*it == "prof")	{ test.tests.prof	= true; }
	}
}


#define CHECK_EXISTS(EXT) \
	tmp = base + "." + EXT ; if(file_exists(tmp.c_str())) { return EXT; }

static std::string find_source_file(const std::string &base)
{
	std::string tmp;
	CHECK_EXISTS("c")
	CHECK_EXISTS("cc")
	CHECK_EXISTS("cpp")
	CHECK_EXISTS("cxx")
	throw stupid_exception("Can't find source file for %s", base.c_str());
}

#undef CHECK_EXISTS

static void dump_config(const stupid_tree_t &t);
static void dump_node(const st_node_t &n, size_t indent);
static void dump_test(const st_test_t &t, size_t indent);

static void add_ind(size_t indent = 0, char ind_char = '-');
static void dump_kv(const std::string &key, const std::string &val, size_t indent = 0, char ind_char = '-', bool align = true);

void dump_stupid_tree(const stupid_tree_t &t)
{
	s_dump("################################################################################\n");
	dump_config(t);
	s_dump("================================================================================\n");
	if(t.root_node_valid)
	{
		dump_node(t.root_node, 4);
	}
	else
	{
		s_dump("(Empty)\n");
	}
	s_dump("################################################################################\n");
}

static void dump_config(const stupid_tree_t &t)
{
	s_dump("---------- Config\n");
	dump_kv("Output path:", t.out_path, 2, ' ');
	dump_kv("Flatten binary paths:", bool_str(t.flatten_bin_paths),
		2, ' ');
	s_dump("\n");
	s_dump("---------- Internal\n");
	dump_kv("Root path:", t.root_path, 2, ' ');
	dump_kv("Root node valid:",
		bool_str(t.root_node_valid), 2, ' ');
	s_dump("\n");
}

static void dump_node(const st_node_t &n, size_t indent)
{
	add_ind(indent+2, '#');
	s_dump("%s (%s)\n", n.path.c_str(), n.name.c_str());
	
	dump_kv("Build flags:",			n.build_flags,	indent, '=');
	dump_kv("Build definitions:",	n.build_defs,	indent, '=');
	dump_kv("Build libraries:",		n.build_libs,	indent, '=');
	dump_kv("Exec arguments:",		n.exec_args,	indent, '=');
	dump_kv("Exec env vars:",		n.exec_envs,	indent, '=');
	
	dump_kv("Generate CMakeLists:",
		bool_str(n.generate_cmakelists),	indent, '=');
	
	
	
	std::vector<st_test_t>::const_iterator t_it;
	for(t_it = n.tests.begin(); t_it != n.tests.end(); ++t_it)
	{
		dump_test(*t_it, indent+4);
	}
	
	std::vector<st_node_t>::const_iterator c_it;
	for(c_it = n.children.begin(); c_it != n.children.end(); ++c_it)
	{
		dump_node(*c_it, indent+4);
	}
}

static void dump_test(const st_test_t &t, size_t indent)
{
	std::string tmp;
	
	// Name and path
	add_ind(indent+2, '#');
	s_dump("%s (%s)\n", t.path.c_str(), t.name.c_str());
	
	// Tests
	if(t.tests.cmiss)	{ tmp += "cmiss, ";	}
	if(t.tests.corr)	{ tmp += "corr, ";	}
	if(t.tests.cov)		{ tmp += "cov, ";	}
	if(t.tests.mem)		{ tmp += "mem, ";	}
	if(t.tests.perf)	{ tmp += "perf, ";	}
	if(t.tests.prof)	{ tmp += "prof, ";	}
	if(tmp == "") { tmp = "(None)"; }
	else { tmp = tmp.substr(0, tmp.length() - 2); }
	
	dump_kv("Tests:",				 tmp,			 indent);
	
	// Build and sources
	tmp = std::string(bool_str(t.build))+" ("+t.build_srcs+")";
	dump_kv("Build:",				 tmp, 			indent);
	
	// String variables
	dump_kv("Build flags:",			t.build_flags,	indent);
	dump_kv("Build definitions:",	t.build_defs,	indent);
	dump_kv("Build libraries:",		t.build_libs,	indent);
	dump_kv("Exec arguments:",		t.exec_args,	indent);
	dump_kv("Exec env vars:",		t.exec_envs,	indent);
	
	s_dump("\n");
}

static void add_ind(size_t indent, char ind_char)
{
	while(indent-- > 1) { s_dump("%c", ind_char); }
	if(indent == 0) { s_dump(" "); }
}

static void dump_kv(const std::string &key, const std::string &val, size_t indent, char ind_char, bool align)
{
	add_ind(indent, ind_char);
	
	const char *const val_p =  (val.empty()) ? "(Empty)" : val.c_str();
	size_t cnt = 39;
	if(align) { cnt -= indent; }
	s_dump("%-*s %s\n", (int)cnt, key.c_str(), val_p);
}


