#include "gen_config.h"

#include <algorithm>
#include <map>

typedef std::map<std::string, std::string> keyval_map;

static void process_line(std::string &line, keyval_map &map);

void load_gen_config(const char *file, gen_config &cfg)
{
	(void)cfg;
	
	std::ifstream ifs(file);
	if(ifs.fail())
	{
		throw stupid_exception("Failed to open file \"%s\"", file);
	}
	
	std::string line;
	std::string buf;
	
	keyval_map map;
	
	// Read logical lines
	while(std::getline(ifs, line))
	{
		while(line.length() > 0 && (*line.rbegin()) == '\\')
		{
			if ( ! std::getline(ifs, buf) )
			{
				if(ifs.eof())
				{
					throw stupid_exception(
						"Reached an early EOF while reading \"%s\"",
						file);
				}
				else
				{
					throw stupid_exception(
						"Problem with reading file \"%s\"",
						file);
				}
			}
			line.erase(line.length() - 1);
			line.append(buf);
		}
		
		process_line(line, map);
	}
	
	
	if( ! ifs.eof() )
	{
		throw stupid_exception("Problem with reading file \"%s\"", file);
	}
}


static void process_line(std::string &line, keyval_map &map)
{
	(void)map;
	trim(line);
	if(line.length() == 0) { return; }
	
	if(line[0] == '#') { return; }
	
	std::string::iterator eq;
	eq = find(++line.begin(), --line.end(), '=');
	if(eq == --line.end())
	{
		throw stupid_exception("Malformed line: %s", line.c_str());
	}

	std::string key(line.begin(), eq);
	std::string val(++eq, line.end());
	
	trim(key);
	trim(val);
	
	std::cout << "key: " << key << std::endl;
	std::cout << "val: " << val << std::endl;
	std::cout << std::endl;
}




void dump_gen_config(const gen_config &cfg)
{
	std::cout << "================================================================================" << std::endl;
	std::cout << "Source path:             " << cfg.src_path << std::endl;
	std::cout << "Binary path:             " << cfg.bin_path << std::endl;
	std::cout << "Result path:             " << cfg.src_path << std::endl;
	std::cout << "Output path:             " << cfg.res_path << std::endl;
	std::cout << "Script output path:      " << cfg.out_path << std::endl;
	std::cout << std::endl;
	std::cout << "Flatten binary paths:    " << bool_str(cfg.flatten_bin_paths) << std::endl;
	std::cout << "Generate CMake files:    " << bool_str(cfg.generate_cmake_files) << std::endl;
	std::cout << "================================================================================" << std::endl;
}
