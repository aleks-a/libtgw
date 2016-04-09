/* run_command.h
 *
 * Copyright (C) 2016 aleks-a
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */
 
#ifndef STUPID__RUN_COMMAND__H
#define STUPID__RUN_COMMAND__H

#include "stupid_common.h"

void run_command(const std::string &cmd, int &rc);

void mkdirp(const std::string &dir);
bool diff_files(const std::string &p1, const std::string &p2, const std::string &ofile);

#endif
