#include "stupid_common.h"

#include "gen_config.h"
#include "gen_token_table.h"
#include "write_cmake.h"

static void mk_corr_script(const gen_config &cfg, const token_table_t &tt);

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
		
		
		mk_corr_script(cfg, tt);
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

static std::string mk_bin_path(const gen_config &cfg, const std::string &name)
{
	std::string tmp;
	tmp = (cfg.flatten_bin_paths) ? basename(name) : name;
	return cfg.root_dir + FSS + cfg.bin_path + FSS + tmp;
}

static std::string mk_res_path(const gen_config &cfg, const std::string &name)
{
	return cfg.root_dir + FSS + cfg.res_path + FSS + name;
}

static std::string mk_out_path(const gen_config &cfg, const std::string &name)
{
	return cfg.root_dir + FSS + cfg.out_path + FSS + name;
}

static void mk_corr_script(const gen_config &cfg, const token_table_t &tt)
{
	std::string path = cfg.root_dir + FSS + cfg.script_out_path + FSS + "run_corr_tests.sh";
	std::ofstream ofs(path.c_str(), std::ios_base::out | std::ios_base::trunc);
	if( ! ofs.good() )
	{
		throw stupid_exception("Failed to open file: %s", path.c_str());
	}
	
	ofs << "#/bin/sh" << std::endl << std::endl;
	ofs << "mkdir -p \"" << mk_out_path(cfg, "") << "\"" << std::endl << std::endl;
	
	token_table_t::const_iterator it;
	std::string tmp;
	for(it = tt.begin(); it != tt.end(); ++it)
	{
		if( ! (*it).builds.corr ) { continue; }	
		ofs << "\"" << mk_bin_path(cfg, (*it).name) << "\" "
			<< (*it).exec_args << " > \""
			<< mk_out_path(cfg, (*it).name + ".run.txt")
			<< "\" 2>&1" << std::endl;
			
			
		ofs << "diff \"" << mk_out_path(cfg, (*it).name + ".run.txt")
			<< "\" \"" << mk_res_path(cfg, (*it).name+".txt")
			<< "\" > \"" << mk_out_path(cfg, (*it).name + ".diff.txt")
			<< "\"" << std::endl;
		
		ofs << "RC=$?" << std::endl
			<< "if [ $RC != 0 ] ; then" << std::endl
			<< "\techo \"" << (*it).name << "    [Fail]\"" << std::endl
			<< "\techo \"********************************************************************************\"" << std::endl
			<< "\tcat \"" << mk_out_path(cfg, (*it).name + ".out.txt") << "\"" << std::endl
			<< "\techo \"--------------------------------------------------------------------------------\"" << std::endl
			<< "\tcat \"" << mk_out_path(cfg, (*it).name + ".diff.txt") << "\"" << std::endl
			<< "\techo \"********************************************************************************\"" << std::endl
			<< "else" << std::endl
			<< "\techo \"" << (*it).name << "    [OK]\"" << std::endl
			<< "fi" << std::endl << std::endl;
	}
}

