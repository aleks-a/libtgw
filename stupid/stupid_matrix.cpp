/* stupid_matrix.cpp
 *
 * Copyright (C) 2016 aleks-a
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */
 
 
#include "stupid_common.h"

#include "run_command.h"
#include "string_utils.h"

struct st_mtx_cfg_t
{
	std::string out_dir, build_dir, tests_dir;
};

typedef std::vector<strvec_t> st_mtx_t;
static void read_matrix(st_mtx_t &mtx, st_mtx_cfg_t &cfg);
static void craft_commands(strvec_t &cmds,
	const st_mtx_t &mtx, const st_mtx_cfg_t &cfg,
	size_t pos = 0, const std::string &base = std::string(""));

int main(int argc, char **argv)
{
	(void)argv;
	
	if(argc != 1)
	{
		fprintf(stderr,
			"This program doesn't accept any command-line arguments\n");
		return EXIT_FAILURE;
	}
	
	
	st_mtx_t mtx;
	st_mtx_cfg_t cfg;
	strvec_t vec;
	strvec_t::const_iterator it;
	
	try
	{
		read_matrix(mtx, cfg);
		craft_commands(vec, mtx, cfg);
		
		for(it = vec.begin(); it != vec.end(); ++it)
		{
			printf("CMD: %s\n", it->c_str());
		}
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



static void handle_special_line(const std::string &line,
	st_mtx_t &mtx, st_mtx_cfg_t &cfg);

static void check_config(const st_mtx_cfg_t &cfg);

static void read_matrix(st_mtx_t &mtx, st_mtx_cfg_t &cfg)
{
	std::ifstream ifs("./stupid-matrix.txt");
	if( ! ifs.good() )
	{
		throw stupid_exception("Failed to open file stupid-matrix.txt");
	}
	
	std::string line;
	
	{
		strvec_t tmp;
		mtx.push_back(tmp);
	}
	bool prev_was_separator = true;
	
	while(read_logical_line(ifs, line))
	{
		if(line == "---")
		{
			if(prev_was_separator)
			{
				throw stupid_exception("Empty list");
			}
			
			strvec_t tmp;
			mtx.push_back(tmp);
			
			prev_was_separator = true;
			continue;
		}
		
		if(line[0] == '%')
		{
			line = line.substr(1);
			handle_special_line(line, mtx, cfg);
			continue;
		}
		
		mtx.back().push_back(line);
		
		prev_was_separator = false;
	}
	if(prev_was_separator)
	{
		throw stupid_exception("Empty list");
	}
	
	check_config(cfg);
}

static void handle_special_line(const std::string &line,
	st_mtx_t &mtx, st_mtx_cfg_t &cfg)
{
	(void)mtx;
	
	kv_vec_t toks;
	kv_vec_t::iterator it;
	
	parse_line(line, toks);
	
	for(it = toks.begin(); it != toks.end(); ++it)
	{
		if(it->first == "output-dir") { cfg.out_dir = it->second; }
		else if(it->first == "build-dir") { cfg.build_dir = it->second; }
		else if(it->first == "tests-dir") { cfg.tests_dir = it->second; }
		else if(it->first == "" && it->second == "none")
		{
			mtx.back().push_back(std::string(""));
		}
		else
		{
			throw stupid_exception("Invalid line: %s", line.c_str());
		}
	}
}

#define CHECK_REQ_STR(CFG, NAME) \
	if(cfg.CFG.empty()) { \
		throw stupid_exception( \
			"Please supply " NAME " config option"); \
	}

static void check_config(const st_mtx_cfg_t &cfg)
{
	CHECK_REQ_STR(out_dir, "output-dir")
	CHECK_REQ_STR(build_dir, "build-dir")
	CHECK_REQ_STR(tests_dir, "tests-dir")
}

#undef CHECK_REQ_STR


static void craft_commands(strvec_t &cmds,
	const st_mtx_t &mtx, const st_mtx_cfg_t &cfg,
	size_t pos, const std::string &base)
{
	strvec_t::const_iterator it;
	std::string tmp;
	
	if(pos >= mtx.size())
	{
		tmp = trim_str(base);
		if(tmp.empty())
		{
			throw stupid_exception("Empty command string");
		}
		cmds.push_back(tmp);
		return;
	}
	
	
	
	
	for(it = mtx[pos].begin(); it != mtx[pos].end(); ++it)
	{
		tmp = base + " " + *it;
		craft_commands(cmds, mtx, cfg, pos + 1, tmp);
	}
}
