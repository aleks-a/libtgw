#include "stupid_common.h"

#include "gen_config.h"
#include "gen_token_table.h"
#include "write_cmake.h"

int main(int argc, char **argv)
{
	s_info("---------- stupid-generate\n");
	
	if(argc != 3)
	{
		s_err("Invalid number of arguments\n");
		s_err("Usage: %s <config_file> <root_dir>\n", argv[0]);
		return EXIT_FAILURE;
	}
	
	s_info("Configuration file:          %s\n", argv[1]);
	s_info("Root directory:              %s\n", argv[2]);
	
	gen_config cfg;
	cfg.root_dir = argv[2];
	
	lines_t stupid_fl, tmpl;
	lines_t::const_iterator lit;
	std::string str, dname;
	token_table_t tt, tt_tmp;
	token_table_t::iterator tt_it;
	try
	{
		load_gen_config(argv[1], cfg);
		dump_gen_config(cfg);
		
		read_file(cfg.stupid_list.c_str(), stupid_fl);
		for(lit = stupid_fl.begin(); lit != stupid_fl.end(); ++lit)
		{
			tmpl.clear();
			tt_tmp.clear();
			
			str = cfg.root_dir + FSS + cfg.src_path + FSS + *lit;
			read_file(str.c_str(), tmpl);
			
			std::sort(tmpl.begin(), tmpl.end());
			
			parse_tokens(tmpl, tt_tmp);
			
			dname = path_rem_dot(dirname(*lit) + FSS);
			str = cfg.root_dir + FSS + cfg.src_path + FSS + dname;
			write_cmake(str, cfg, tt_tmp);
			
			for(tt_it = tt_tmp.begin(); tt_it != tt_tmp.end(); ++tt_it)
			{
				tt.push_back(*tt_it);
				tt.back().name = dname + tt.back().name;
			}
		}
		
		dump_tokens(tt);
	}
	catch(std::exception &e)
	{
		const char *err_str;
		err_str = e.what() ? e.what() : "Unknown error happened";
		s_err("%s\n", err_str);
		return 1;
	}
	
	return 0;
}


