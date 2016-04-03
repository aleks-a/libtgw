#include "stupid_common.h"

#include "gen_config.h"

int main(int argc, char **argv)
{
	s_info("---------- stupid-generate\n");
	
	if(argc != 2)
	{
		s_err("Invalid number of arguments\n");
		s_err("Usage: %s <config_file>\n", argv[0]);
		return EXIT_FAILURE;
	}
	
	const char *cfg_file = argv[1];
	s_info("Configuration file: %s\n", cfg_file);
	
	gen_config cfg;
	try
	{
		load_gen_config(cfg_file, cfg);
		dump_gen_config(cfg);
		
		
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


