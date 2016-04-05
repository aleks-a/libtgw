#ifndef STUPID__GEN_TOKEN_TABLE__H
#define STUPID__GEN_TOKEN_TABLE__H

#include "stupid_common.h"

struct token_t
{
	std::string name;
	struct
	{
		bool cmiss, cov, corr, mem, perf, prof;
	} builds;
	
	bool autobuild;
	std::string autobuild_srcs;
	std::string exec_args;
};

typedef std::vector<token_t> token_table_t;

void parse_tokens(const lines_t &lines, token_table_t &tt);
void dump_tokens(const token_table_t &tt);

#endif
