#include "gen_token_table.h"

static void parse_token(const std::string &line, token_t &token);
static void tokenize_string(const std::string &line, lines_t &vec);
static void parse_build_types(const std::string &str, token_t &token);
static void parse_one_build_type(const std::string &str, token_t &token);
static void process_kv_pair(const std::string &key, const std::string &val, token_t &token);

void parse_tokens(const lines_t &lines, token_table_t &tt)
{
	lines_t::const_iterator it;
	token_t tok;
	
	for(it = lines.begin(); it != lines.end(); ++it)
	{
		parse_token(*it, tok);
		tt.push_back(tok);
	}
}


static void parse_token(const std::string &line, token_t &token)
{
	(void)token;
	lines_t vec;
	
	tokenize_string(line, vec);
	//std::cout << "LINE: " << line << std::endl;
	
	if(vec.size() < 2)
	{
		throw stupid_exception("Invalid line: %s", line.c_str());
	}
	
	token.name = vec[0];
	parse_build_types(vec[1], token);
	//std::cout << "N: " << vec[0] << std::endl;
	//std::cout << "B: " << vec[1] << std::endl;
	std::string key, val;
	for(size_t i = 2; i < vec.size(); ++i)
	{
		parse_kv_line(vec[i], key, val);
		process_kv_pair(key, val, token);
		//std::cout << " --- K: " << key << ", V: " << val << std::endl;
	}
}
//parse_kv_line
static void tokenize_string(const std::string &line, lines_t &vec)
{
	bool is_quoting = false;
	bool is_escaping = false;
	std::string tmp;
	std::string::const_iterator it;
	
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
}


static void parse_build_types(const std::string &str, token_t &token)
{
	token.builds.cmiss = false;
	token.builds.cov = false;
	token.builds.corr = false;
	token.builds.mem = false;
	token.builds.perf = false;
	token.builds.prof = false;
	
	std::string tmp;
	std::string::const_iterator it;
	for(it = str.begin(); it != str.end(); ++it)
	{
		if(*it == ',')
		{
			if( ! tmp.empty() )
			{
				trim(tmp);
				parse_one_build_type(tmp, token);
				tmp.clear();
			}
			continue;
		}
		tmp.push_back(*it);
	}
	
	if( ! tmp.empty() )
	{
		trim(tmp);
		parse_one_build_type(tmp, token);
	}
}

#define SET_BUILD(NAME) \
	if(str == #NAME) { token.builds.NAME = true; return; }
	
static void parse_one_build_type(const std::string &str, token_t &token)
{
	SET_BUILD(cmiss)
	SET_BUILD(cov)
	SET_BUILD(corr)
	SET_BUILD(mem)
	SET_BUILD(perf)
	SET_BUILD(prof)
	
	s_warn("Unknown build type: %s", str.c_str());
}

#define PARSE_VAL(CFG_NAME, NAME) \
	if(key == NAME) { token.CFG_NAME = (val); return; }
	
#define PARSE_VAL_C(CFG_NAME, NAME, CONV) \
	if(key == NAME) { token.CFG_NAME = CONV(val); return; }
	

static void process_kv_pair(const std::string &key, const std::string &val, token_t &token)
{
	PARSE_VAL(autobuild_srcs,			"autobuild-srcs")
	PARSE_VAL(exec_args,				"exec-args")
	PARSE_VAL_C(autobuild,				"autobuild",	parse_bool)
	
	s_warn("Unknown option: %s=%s", key.c_str(), val.c_str());
}

std::string build_type_str(const token_t &t)
{
	std::string str;
	if(t.builds.cmiss)	{ str += "cmiss ";	}
	if(t.builds.cov)	{ str += "cov ";	}
	if(t.builds.corr)	{ str += "corr ";	}
	if(t.builds.mem)	{ str += "mem ";	}
	if(t.builds.perf)	{ str += "perf ";	}
	if(t.builds.prof)	{ str += "prof ";	}
	
	if(str.length() == 0) { str = "(None)"; }
	else if(*str.rbegin() == ' ') { str.erase(str.length() - 1); }
	
	return str;
}

void dump_tokens(const token_table_t &tt)
{
	token_table_t::const_iterator it;
	for(it = tt.begin(); it != tt.end(); ++it)
	{
		token_t tok = *it;
		std::cout << "Name:                        " << tok.name << std::endl;
		std::cout << "--- Build types:             " << build_type_str(tok) << std::endl;
		std::cout << "--- Autobuild:               " << bool_str(tok.autobuild) << std::endl;
		std::cout << "--- Autobuild sources:       " << tok.autobuild_srcs << std::endl;
		std::cout << "--- Exec arguments:          " << tok.exec_args << std::endl;
		std::cout << std::endl;
	}
	
	
}




