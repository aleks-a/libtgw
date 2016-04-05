#include "gen_config.h"

#include <algorithm>


static void check_cfg(gen_config &cfg);
static void parse_cfg(gen_config &cfg, const std::string &key, const std::string &val);


void load_gen_config(const char *file, gen_config &cfg)
{
	lines_t lines;
	keyval_map_t map;
	
	read_file(file, lines);
	parse_keyval(lines, map);
	
	keyval_map_t::iterator it;
	for(it = map.begin(); it != map.end(); ++it)
	{
		parse_cfg(cfg, it->first, it->second);
	}
	check_cfg(cfg);
}



#define PARSE_VAL(CFG_NAME, NAME) \
	if(key == NAME) { cfg.CFG_NAME = (val); return; }
	
#define PARSE_VAL_C(CFG_NAME, NAME, CONV) \
	if(key == NAME) { cfg.CFG_NAME = CONV(val); return; }
	
static void parse_cfg(gen_config &cfg, const std::string &key, const std::string &val)
{
	PARSE_VAL(src_path,					"source-path")
	PARSE_VAL(bin_path,					"binary-path")
	PARSE_VAL(res_path,					"result-path")
	PARSE_VAL(out_path,					"output-path")
	PARSE_VAL(script_out_path,			"script-output-path")
	PARSE_VAL(stupid_list,				"stupid-list")
	
	PARSE_VAL_C(flatten_bin_paths,		"flatten-bin-paths", 	parse_bool)
	PARSE_VAL_C(generate_cmake_files,	"generate-cmake-files",	parse_bool)
	
	s_warn("Unknown key: %s", key.c_str());
}

static void check_cfg(gen_config &cfg)
{
	if(cfg.src_path.empty()) { throw stupid_exception("source-path not supplied"); }
	if(cfg.bin_path.empty()) { cfg.bin_path = cfg.src_path; }
	if(cfg.res_path.empty()) { cfg.res_path = cfg.src_path; }
	if(cfg.out_path.empty()) { throw stupid_exception("output-path not supplied"); }
	if(cfg.script_out_path.empty()) { cfg.script_out_path = cfg.bin_path; }
	if(cfg.stupid_list.empty()) { throw stupid_exception("stupid-list not supplied"); }
}



void dump_gen_config(const gen_config &cfg)
{
	std::cout << "================================================================================" << std::endl;
	std::cout << "Root directory               " << cfg.root_dir << std::endl;
	std::cout << std::endl;
	std::cout << "Source path:                 " << cfg.src_path << std::endl;
	std::cout << "Binary path:                 " << cfg.bin_path << std::endl;
	std::cout << "Result path:                 " << cfg.src_path << std::endl;
	std::cout << "Output path:                 " << cfg.res_path << std::endl;
	std::cout << "Script output path:          " << cfg.out_path << std::endl;
	std::cout << "Stupid list file:            " << cfg.stupid_list << std::endl;
	std::cout << std::endl;
	std::cout << "Flatten binary paths:        " << bool_str(cfg.flatten_bin_paths) << std::endl;
	std::cout << "Generate CMake files:        " << bool_str(cfg.generate_cmake_files) << std::endl;
	std::cout << "================================================================================" << std::endl;
}
