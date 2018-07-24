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

		int DoseAccInvalidParametersTest(int argc, char* argv[])
		{
			PREPARE_DEFAULT_TEST_REPORTING;

			std::string doseAccExecutable;

			if (argc > 1)
			{
                doseAccExecutable = argv[1];
			}

			boost::filesystem::path callingPath(_callingAppPath);
            std::string doseAccExeWithPath = callingPath.parent_path().string() + "/" + doseAccExecutable;

			//call with too few parameters
            std::string toofewParametersCommand = doseAccExeWithPath;
			toofewParametersCommand += " test";
			std::cout << "Command line call: " + toofewParametersCommand << std::endl;
			CHECK_EQUAL(system(toofewParametersCommand.c_str()) != 0, true);

            toofewParametersCommand = doseAccExeWithPath;
			toofewParametersCommand += " test test";
			std::cout << "Command line call: " + toofewParametersCommand << std::endl;
			CHECK_EQUAL(system(toofewParametersCommand.c_str()) != 0, true);

            toofewParametersCommand = doseAccExeWithPath;
			toofewParametersCommand += " -d test";
			toofewParametersCommand += " -e test";
			std::cout << "Command line call: " + toofewParametersCommand << std::endl;
			CHECK_EQUAL(system(toofewParametersCommand.c_str()) != 0, true);

            std::string minimalCLI = doseAccExeWithPath + " test test test ";

			//call with invalid interpolator
            
			std::string invalidInterpolatorOption = minimalCLI;
			invalidInterpolatorOption += "-i wrongInterpolator";
			std::cout << "Command line call: " + invalidInterpolatorOption << std::endl;
			CHECK_EQUAL(system(invalidInterpolatorOption.c_str()) != 0, true);

            //call with invalid operator
            std::string invalidOperatorOption = minimalCLI;
            invalidOperatorOption += "-p -";
            std::cout << "Command line call: " + invalidOperatorOption << std::endl;
            CHECK_EQUAL(system(invalidOperatorOption.c_str()) != 0, true);

            //call with operator* and weight
            std::string invalidOperatorMultWithWeightOption = minimalCLI;
            invalidOperatorMultWithWeightOption += "-p * --weight1 2.0";
            std::cout << "Command line call: " + invalidOperatorMultWithWeightOption << std::endl;
            CHECK_EQUAL(system(invalidOperatorMultWithWeightOption.c_str()) != 0, true);

			//call with invalid dose1 load option
			std::string invalidDose1LoadOption = minimalCLI;
			invalidDose1LoadOption += "-t invalidLoadStyleOption";
			std::cout << "Command line call: " + invalidDose1LoadOption << std::endl;
			CHECK_EQUAL(system(invalidDose1LoadOption.c_str()) != 0, true);

            //call with invalid dose2 load option
            std::string invalidDose2LoadOption = minimalCLI;
            invalidDose2LoadOption += "-u invalidLoadStyleOption";
            std::cout << "Command line call: " + invalidDose2LoadOption << std::endl;
            CHECK_EQUAL(system(invalidDose2LoadOption.c_str()) != 0, true);

			RETURN_AND_REPORT_TEST_SUCCESS;
		}
	} //namespace testing
} //namespace rttb
