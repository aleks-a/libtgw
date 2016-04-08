/* stupid_utils.h
 *
 * Copyright (C) 2016 aleks-a
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */
 
#ifndef STUPID__STRING_UTILS__H
#define STUPID__STRING_UTILS__H

#include "stupid_common.h"


bool read_logical_line(std::ifstream &ifs, std::string &line,
	const char *fname = "");

std::string trim_str(const std::string &str);
void itrim_str(std::string &str);


void tokenize(const std::string &str, strvec_t &vec,
	char delim = ' ', bool ignore_empty = true, bool trim = true);


void parse_line(const std::string &str, kv_vec_t &vec);
void dump_kv_vec(const  kv_vec_t &vec);

#endif
