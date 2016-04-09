/* run_command.cpp
 *
 * Copyright (C) 2016 aleks-a
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */
 
 
#include "run_command.h"

#include <boost/filesystem.hpp>

#include <cstdlib>
 
void run_command(const std::string &cmd, int &rc)
{
	if(cmd.empty())
	{
		throw stupid_exception("cmd argument empty");
	}
	//printf("(CMD) %s\n", cmd.c_str());
	rc = system(cmd.c_str());
	if(rc == -1)
	{
		throw stupid_exception("system() call failed with cmd: %s",
			cmd.c_str());
	}
	
	rc = WEXITSTATUS(rc);
}
 
void mkdirp(const std::string &dir)
{
	boost::system::error_code ec;
	boost::filesystem::create_directories(dir, ec);
	if(ec != boost::system::errc::success)
	{
		throw stupid_exception(
			"Failed to create directory \"%s\": %s",
			dir,  ec.message().c_str());
	}
}

bool diff_files(const std::string &p1, const std::string &p2, const std::string &ofile)
{
	(void)ofile;
	
	std::ifstream ifs1(p1.c_str()), ifs2(p2.c_str());
	std::string l1, l2;
	
	if( ! ifs1.good() )
	{
		throw stupid_exception("Failed to open file %s", p1.c_str());
	}
	
	if( ! ifs2.good() )
	{
		throw stupid_exception("Failed to open file %s", p2.c_str());
	}
	
	while(std::getline(ifs1, l1) && std::getline(ifs2, l2))
	{
		if(l1 != l2)
		{
			
			int rc;
			std::string cmd = "diff \"" + p1 + "\" \"" + p2 + "\" "
				" > \"" + ofile + "\"";
			run_command(cmd, rc);
			
			return false;
		}
	}
	
	return true;
}

