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

		//path to the current running directory. DoseTool is in the same directory (Debug/Release)
		extern const char* _callingAppPath;

		int BioModelCalcInvalidParametersTest(int argc, char* argv[])
		{
			PREPARE_DEFAULT_TEST_REPORTING;

			std::string bioModelCalcExecutable;

			if (argc > 1)
			{
                bioModelCalcExecutable = argv[1];
			}

			boost::filesystem::path callingPath(_callingAppPath);
            std::string bioModelCalcExeWithPath = callingPath.parent_path().string() + "/" + bioModelCalcExecutable;

			//call with too few parameters
            std::string toofewParametersCommand = bioModelCalcExeWithPath;
			toofewParametersCommand += " test";
			std::cout << "Command line call: " + toofewParametersCommand << std::endl;
			CHECK_EQUAL(system(toofewParametersCommand.c_str()) != 0, true);

            toofewParametersCommand = bioModelCalcExeWithPath;
			toofewParametersCommand += " test test";
			std::cout << "Command line call: " + toofewParametersCommand << std::endl;
			CHECK_EQUAL(system(toofewParametersCommand.c_str()) != 0, true);

            toofewParametersCommand = bioModelCalcExeWithPath;
			toofewParametersCommand += " -d test";
			toofewParametersCommand += " -o test";
			std::cout << "Command line call: " + toofewParametersCommand << std::endl;
			CHECK_EQUAL(system(toofewParametersCommand.c_str()) != 0, true);

			//call with invalid model
            std::string minimalCLI = bioModelCalcExeWithPath + " test test -p 1.0 1.0 ";
			std::string invalidModelOption = minimalCLI;
			invalidModelOption += "-m wrongModel";
			std::cout << "Command line call: " + invalidModelOption << std::endl;
			CHECK_EQUAL(system(invalidModelOption.c_str()) != 0, true);

			//call with only one parameter
            std::string toofewModelParameterOption = bioModelCalcExeWithPath + " test test -p 1.0";
            std::cout << "Command line call: " + toofewModelParameterOption << std::endl;
            CHECK_EQUAL(system(toofewModelParameterOption.c_str()) != 0, true);

            toofewModelParameterOption = bioModelCalcExeWithPath + " test test -a parameterMapsTest";
            std::cout << "Command line call: " + toofewModelParameterOption << std::endl;
            CHECK_EQUAL(system(toofewModelParameterOption.c_str()) != 0, true);

			//call with invalid dose load option
			std::string invalidDoseLoadOption = minimalCLI;
			invalidDoseLoadOption += "-t invalidLoadStyleOption";
			std::cout << "Command line call: " + invalidDoseLoadOption << std::endl;
			CHECK_EQUAL(system(invalidDoseLoadOption.c_str()) != 0, true);

			//call with negative dose scaling
			std::string negativeDoseScalingCommand = minimalCLI;
            negativeDoseScalingCommand += "-e -1.0";
            std::cout << "Command line call: " + negativeDoseScalingCommand << std::endl;
            CHECK_EQUAL(system(negativeDoseScalingCommand.c_str()) != 0, true);

			RETURN_AND_REPORT_TEST_SUCCESS;
		}
	} //namespace testing
} //namespace rttb
