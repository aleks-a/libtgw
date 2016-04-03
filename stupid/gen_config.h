#ifndef STUPID__GEN_CONFIG__H
#define STUPID__GEN_CONFIG__H

#include "stupid_common.h"

struct gen_config
{
	gen_config():
		flatten_bin_paths(false), generate_cmake_files(false)
	{
	}
	
	std::string src_path;
	std::string bin_path;
	std::string res_path;
	std::string out_path;
	std::string script_out_path;
	
	bool flatten_bin_paths;
	bool generate_cmake_files;
};

void load_gen_config(const char *file, gen_config &cfg);
void dump_gen_config(const gen_config &cfg);

#endif
