#ifndef STUPID__WRITE_CMAKE__H
#define STUPID__WRITE_CMAKE__H

#include "stupid_common.h"
#include "gen_token_table.h"
#include "gen_config.h"

void write_cmake(const std::string &dir, const gen_config &cfg, token_table_t &tokens);

#endif
