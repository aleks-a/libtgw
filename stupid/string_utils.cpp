/* stupid_utils.cpp
 *
 * Copyright (C) 2016 aleks-a
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */
 
 
 #include "string_utils.h"
 
 #include <algorithm>


bool read_logical_line(std::ifstream &ifs, std::string &line,
	const char *fname)
{
	std::string buf;
	
	// Read logical lines
	line.clear();
	while(std::getline(ifs, line))
	{
		// If previous line ended with '\' read some more
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
						"Error reading line from file \"%s\"",
						fname);
				}
			}
			line.erase(line.length() - 1);
			line.append(buf);
		}
		
		itrim_str(line);
		if(line.length() == 0 || line[0] == '#') { continue; }
		
		// !!! Return here!
		return true;
	}
	
	
	if( ! ifs.eof() )
	{
		throw stupid_exception("Problem with reading file \"%s\"", fname);
	}
	
	// EOF
	return false;
}

std::string trim_str(const std::string &str)
{
	std::string tmp(str);
	itrim_str(tmp);
	return tmp;
}

void itrim_str(std::string &str)
{
	std::string::iterator s;
	s = std::find_if(
		str.begin(),
		str.end(),
		std::not1(std::ptr_fun<int, int>(std::isspace)));
	str.erase(str.begin(), s);
	
	std::string::reverse_iterator e;
	e = std::find_if(
		str.rbegin(),
		str.rend(),
		std::not1(std::ptr_fun<int, int>(std::isspace)));
	str.erase(e.base(), str.end());
}

void tokenize(const std::string &str, strvec_t &vec,
	char delim, bool ignore_empty, bool trim)
{
	std::string::const_iterator it;
	std::string tmp;
	
	for(it = str.begin(); it != str.end(); ++it)
	{
		if(*it == delim)
		{
			if(trim) { itrim_str(tmp); }
			if(ignore_empty && tmp.empty()) { continue; }
			vec.push_back(tmp);
			tmp.clear();
		}
		else
		{
			tmp.push_back(*it);
		}
	}
	
	if(trim) { itrim_str(tmp); }
	if(ignore_empty && tmp.empty()) { return; }
	vec.push_back(tmp);	
}


static void split_quoted_line(const std::string &str, strvec_t &vec);
static bool assemble_kv_vec(const strvec_t &strvec, kv_vec_t &vec);
static void validate_key(const std::string &key);

void parse_line(const std::string &str, kv_vec_t &vec)
{
	strvec_t strvec;
	
	split_quoted_line(str, strvec);
	if( ! assemble_kv_vec(strvec, vec) )
	{
		throw stupid_exception(
			"Invalid line: %s", str.c_str());
	}
}

static void split_quoted_line(const std::string &str, strvec_t &vec)
{
	bool is_quoting = false;
	bool is_escaping = false;
	std::string tmp, key;
	std::string::const_iterator it;
	
	const char *const cstr = str.c_str();
	char c;
	
	for(it = str.begin(); it != str.end(); ++it)
	{
		c = *it;
		
		
		if(is_escaping)
		{
			tmp.push_back(c);
			is_escaping = false;
		}
		else if(c == '\\')
		{
			is_escaping = true;
		}
		else if(c == '"')
		{
			// Closing quote
			if(is_quoting &&
				(it+1) != str.end() && ! std::isspace(*(it+1)))
			{
				throw stupid_exception(
					"Closing quote not followed by a whitespace or EOL: %s\n",
					cstr);
			}
			// Opening quote
			else if( ! is_quoting &&
				it != str.begin() &&
				! std::isspace(*(it-1)) && *(it-1) != '=')
			{
				throw stupid_exception(
					"Opening quote not preceeded by a whitespace, start of line or an equal sign: %s\n",
					cstr);
			}
			
			is_quoting = ! is_quoting;
		}
		else if(is_quoting)
		{
			tmp.push_back(c);
		}
		else if(c == '=')
		{
			if( ! tmp.empty())
			{
				vec.push_back(tmp);
				tmp.clear();
			}
			
			vec.push_back("=");
		}
		else if(std::isspace(c))
		{
			if( ! tmp.empty())
			{
				vec.push_back(tmp);
				tmp.clear();
			}
		}
		else
		{
			tmp.push_back(c);
		}
	}
	
	if(is_quoting)
	{
		throw stupid_exception("Unclosed quoted string: %s\n", cstr);
	}
	
	if(is_escaping)
	{
		throw stupid_exception("INTERNAL ERROR: Last char was '\\': %s\n", cstr);
	}
	
	if( ! tmp.empty())
	{
		vec.push_back(tmp);
	}
}

static bool assemble_kv_vec(const strvec_t &strvec, kv_vec_t &vec)
{
	strvec_t::const_iterator it;
	validate_key("hello-world");
	
	// First make sure that the list is valid
	for(it = strvec.begin(); it != strvec.end(); ++it)
	{
		if(*it == "=")
		{
			if(it == strvec.begin() || it+1 == strvec.end()
				|| *(it+1) == "=")
			{
				return false;
			}
			
			if(it+2 != strvec.end() && *(it+2) == "=")
			{
				return false;
			}
		}
	}
	
	std::string key;
	
	// Then assemble it into kv_vec
	for(it = strvec.begin(); it != strvec.end(); ++it)
	{
		// If token is key
		if((it+1) != strvec.end() && *(it+1) == "=")
		{
			key = *it;
			validate_key(key);
		}
		// If token is value
		else if(it != strvec.begin() && *(it-1) == "=")
		{
			vec.push_back(std::make_pair(key, *it));
		}
		// Token is single value, "="-tokens are ignored
		else if(*it != "=")
		{
			vec.push_back(std::make_pair(std::string(""), *it));
		}
	}
	
	return true;
}

static void validate_key(const std::string &key)
{
	const std::string specials = "-_.";
	std::string::const_iterator it;
	for(it = key.begin(); it != key.end(); ++it)
	{
		if( ! std::isalnum(*it)
			&& specials.find(*it) == std::string::npos)
		{
			throw stupid_exception(
				"Invalid key: %s", key.c_str());
		}
	}
}

#if 0


	for(it = line.begin(); it != line.end(); ++it)
	{
		if(is_escaping)
		{
			tmp.push_back(*it);
			is_escaping = false;
			continue;
		}
		
		if(*it == '\\')
		{
			is_escaping = true;
			continue;
		}
		
		if(*it == '"')
		{
			//tmp.push_back(*it);
			is_quoting = ! is_quoting;
			continue;
		}
		
		if(is_quoting)
		{
			tmp.push_back(*it);
			continue;
		}
		
		if(std::isspace(*it))
		{
			if( ! tmp.empty())
			{
				vec.push_back(tmp);
				tmp.clear();
			}
			continue;
		}
		
		tmp.push_back(*it);
	}
	
	if( ! tmp.empty())
	{
		vec.push_back(tmp);
	}

#endif

void dump_kv_vec(const  kv_vec_t &vec)
{
	kv_vec_t::const_iterator it;
	s_dump("========================================\n");
	for(it = vec.begin(); it != vec.end(); ++it)
	{
		if(it->first == "")
		{
			s_dump("--- T: %s\n", it->second.c_str());
		}
		else
		{
			s_dump("--- K: %s, V: %s\n",
				it->first.c_str(), it->second.c_str());
		}
	}
}
