// -----------------------------------------------------------------------
// RTToolbox - DKFZ radiotherapy quantitative evaluation library
//
// Copyright (c) German Cancer Research Center (DKFZ),
// Software development for Integrated Diagnostics and Therapy (SIDT).
// ALL RIGHTS RESERVED.
// See rttbCopyright.txt or
// http://www.dkfz.de/en/sidt/projects/rttb/copyright.html
//
// This software is distributed WITHOUT ANY WARRANTY; without even
// the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
// PURPOSE.  See the above copyright notices for more information.
//
//------------------------------------------------------------------------
/*!
// @file
// @version $Revision: 771 $ (last changed revision)
// @date    $Date: 2014-09-25 14:41:34 +0200 (Do, 25 Sep 2014) $ (last change date)
// @author  $Author: zhangl $ (last changed by)
*/

#include <iostream>

#include "litCheckMacros.h"

#include "boost/filesystem.hpp"

namespace rttb
{
	namespace testing
	{

		//path to the current running directory. DoseTool is in the same directory (Debug/Release)
		extern const char* _callingAppPath;

		int DoseToolBasicUsageTest(int argc, char* argv[])
		{
			PREPARE_DEFAULT_TEST_REPORTING;

			const std::string doseToolExe = "DoseTool.exe";

			boost::filesystem::path callingPath(_callingAppPath);
			std::string voxelizerToolExeWithPath = callingPath.parent_path().string() + "/" + doseToolExe;

			//call without parameters
			std::cout << "Command line call: " + voxelizerToolExeWithPath << std::endl;
			CHECK_EQUAL(system(voxelizerToolExeWithPath.c_str()), -1);

			//call with help parameters
			std::string helpCommandShort = voxelizerToolExeWithPath + " -h";
			std::string helpCommandLong = voxelizerToolExeWithPath + " --help";
			std::cout << "Command line call: " + helpCommandShort << std::endl;
			CHECK_EQUAL(system(helpCommandShort.c_str()), 0);
			std::cout << "Command line call: " + helpCommandLong << std::endl;
			CHECK_EQUAL(system(helpCommandLong.c_str()), 0);

			//call with version parameter
			std::string versionCommandShort = voxelizerToolExeWithPath + " -v";
			std::string versionCommandLong = voxelizerToolExeWithPath + " --version";
			std::cout << "Command line call: " + versionCommandShort << std::endl;
			CHECK_EQUAL(system(versionCommandShort.c_str()), 0);
			std::cout << "Command line call: " + versionCommandLong << std::endl;
			CHECK_EQUAL(system(versionCommandLong.c_str()), 0);

			//call with

			RETURN_AND_REPORT_TEST_SUCCESS;
		}
	} //namespace testing
} //namespace rttb
