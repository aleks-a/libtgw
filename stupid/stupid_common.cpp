#include "stupid_common.h"

#include <algorithm>
#include <cstdarg>

void trim(std::string &str)
{
	//if(str.length() == 0) { return; }
	std::string::iterator s;
	s = std::find_if(
		str.begin(),
		str.end(),
		std::not1(std::ptr_fun<int, int>(std::isspace)));
	str.erase(str.begin(), s);
	
	//if(str.length() == 0) { return; }
	std::string::reverse_iterator e;
	e = std::find_if(
		str.rbegin(),
		str.rend(),
		std::not1(std::ptr_fun<int, int>(std::isspace)));
	str.erase(e.base(), str.end());
}

std::string basename(const std::string &str)
{
	size_t i = str.rfind(FSS_C);
	return (i != std::string::npos) ? str.substr(i + 1) : str;
}

std::string dirname(const std::string &str)
{
	return str.substr(0, str.rfind(FSS_C));
}


std::string path_rem_dot(const std::string &str)
{
	if(str.length() >= 2 && str[0] == '.' && str[1] == FSS_C)
	{
		return str.substr(2);
	}
	
	return str;
}

bool file_exists(const char *path)
{
	std::ifstream ifs(path);
	return ifs.good();
}

void read_file(const char *fname, lines_t &lines)
{
	std::ifstream ifs(fname);
	if(ifs.fail())
	{
		throw stupid_exception("Failed to open file \"%s\"", fname);
	}
	
	std::string line;
	std::string buf;
	
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
						fname);
				}
				else
				{
					throw stupid_exception(
						"Problem with reading file \"%s\"",
						fname);
				}
			}
			line.erase(line.length() - 1);
			line.append(buf);
		}
		
		trim(line);
		if(line.length() == 0 || line[0] == '#') { continue; }
		lines.push_back(line);
	}
	
	
	if( ! ifs.eof() )
	{
		throw stupid_exception("Problem with reading file \"%s\"", fname);
	}
}

void parse_keyval(const lines_t &lines, keyval_map_t &map)
{
	lines_t::const_iterator it;
	for(it = lines.begin(); it != lines.end(); ++it)
	{
		std::string key;
		std::string val;
		
		parse_kv_line(*it, key, val);
	
		if(map.find(key) != map.end())
		{
			throw stupid_exception("Double key: %s", key.c_str());
		}
		
		map[key] = val;
	}
}

void parse_kv_line(const std::string &line, std::string &key, std::string &val)
{
	std::string::const_iterator str_it;
	std::string::const_iterator eq;
	
	eq = find(++line.begin(), --line.end(), '=');
	if(eq == --line.end())
	{
		throw stupid_exception("Malformed line: %s", line.c_str());
	}
	
	key = std::string(line.begin(), eq);
	val = std::string(++eq, line.end());
	trim(key);
	trim(val);
	
	for(str_it = key.begin(); str_it != key.end(); ++str_it)
	{
		if( ! (std::isalnum(*str_it) || *str_it == '-' || *str_it == '_') )
		{
			throw stupid_exception("Invalid key: %s", key.c_str());
		}
	}
}

bool parse_bool(const std::string &str)
{
	if(str == "true" || str == "1" || str == "yes")
	{
		return true;
	}
	else if(str == "false" || str == "0" || str == "no")
	{
		return false;
	}
	else
	{
		throw stupid_exception("Invalid boolean value: %s", str);
	}
}

stupid_exception::stupid_exception():
	str("")
{
}

stupid_exception::stupid_exception(const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	vsnprintf(str, sizeof(str), fmt, args);
	va_end(args);
}

const char * stupid_exception::what() const throw()
{
	return str;
}


