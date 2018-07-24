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

#include <iostream>

#include "litCheckMacros.h"

#include "boost/filesystem.hpp"

namespace rttb
{
	namespace testing
	{

		//path to the current running directory. DoseMap is in the same directory (Debug/Release)
		extern const char* _callingAppPath;

		int DoseMapInvalidParametersTest(int argc, char* argv[])
		{
			PREPARE_DEFAULT_TEST_REPORTING;

			std::string doseMapExecutable;

			if (argc > 1)
			{
				doseMapExecutable = argv[1];
			}

			boost::filesystem::path callingPath(_callingAppPath);
			std::string doseMapExeWithPath = callingPath.parent_path().string() + "/" + doseMapExecutable;

			//call with too few parameters
			std::string toofewParametersCommand = doseMapExeWithPath + " -d test";
			std::cout << "Command line call: " + toofewParametersCommand << std::endl;
			CHECK_EQUAL(system(toofewParametersCommand.c_str()) != 0, true);

			toofewParametersCommand = doseMapExeWithPath + " test";
			std::cout << "Command line call: " + toofewParametersCommand << std::endl;
			CHECK_EQUAL(system(toofewParametersCommand.c_str()) != 0, true);


			//call with invalid dose load option
			std::string minimalCLI = doseMapExeWithPath + " test test ";
			std::string invalidDoseLoadOption = minimalCLI + "-l wrongOption";
			std::cout << "Command line call: " + invalidDoseLoadOption << std::endl;
			CHECK_EQUAL(system(invalidDoseLoadOption.c_str()) != 0, true);

			//call with invalid interpolator
			std::string invalidInterpolatorOption = minimalCLI + "-i wrongOption";
			std::cout << "Command line call: " + invalidInterpolatorOption << std::endl;
			CHECK_EQUAL(system(invalidInterpolatorOption.c_str()) != 0, true);

			RETURN_AND_REPORT_TEST_SUCCESS;
		}
	} //namespace testing
} //namespace rttb
