/* stupid_common.h
 *
 * Copyright (C) 2016 aleks-a
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */
 
#ifndef STUPID__STUPID_COMMON__H
#define STUPID__STUPID_COMMON__H

#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include <cstdio>
#include <cstdlib>

#define s_info(...)		fprintf(stdout, __VA_ARGS__)
#define s_dump(...)		fprintf(stdout, __VA_ARGS__)
#define s_warn(...)		fprintf(stderr, __VA_ARGS__)
#define s_err(...)		fprintf(stderr, __VA_ARGS__)

#define s_tres(...)		fprintf(stdout, __VA_ARGS__)

#define FSS_C '/'
#define FSS "/"


typedef std::vector<std::string> strvec_t;
typedef std::pair<std::string, std::string> kv_pair_t;
typedef std::vector<kv_pair_t> kv_vec_t;

typedef std::vector<std::string> lines_t;
typedef std::map<std::string, std::string> keyval_map_t;


std::string basename(const std::string &str);
std::string dirname(const std::string &str);
std::string path_rem_dot(const std::string &str);
bool file_exists(const char *path);

void read_file(const char *fname, lines_t &lines);
void parse_keyval(const lines_t &lines, keyval_map_t &map);
void parse_kv_line(const std::string &line, std::string &key, std::string &val);

bool parse_bool(const std::string &str);
inline const char * bool_str(bool b)
{
	return b ? "true" : "false";
}

class stupid_exception: virtual public std::exception
{
public:
	stupid_exception();
	stupid_exception(const char *msg, ...);
	virtual const char * what() const throw();
	
private:
	char str[4096];
};


#endif
