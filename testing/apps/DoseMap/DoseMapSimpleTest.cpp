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
// @version $Revision: 1221 $ (last changed revision)
// @date    $Date: 2015-12-01 13:43:31 +0100 (Di, 01 Dez 2015) $ (last change date)
// @author  $Author: hentsch $ (last changed by)
*/

#include <iostream>



#include "litCheckMacros.h"

#include "boost/filesystem.hpp"


namespace rttb
{
	namespace testing
	{

		//path to the current running directory. DoseMap is in the same directory (Debug/Release)
		extern const char* _callingAppPath;

		static std::string readFile(const std::string& filename);

		int DoseMapSimpleTest(int argc, char* argv[])
		{
			PREPARE_DEFAULT_TEST_REPORTING;

			std::string doseMapExecutable;
			std::string regFilename;
			std::string inputFilename;
			std::string outputFilename;
			std::string refDoseLoadStyle;
			std::string inputDoseLoadStyle;


			boost::filesystem::path callingPath(_callingAppPath);


			if (argc > 5 )
			{
				doseMapExecutable = argv[1];
				inputFilename = argv[2];
				outputFilename = argv[3];
				inputDoseLoadStyle = argv[4];
				regFilename = argv[5];
				
			}


			//check if file exists
			


			std::string doseMapExeWithPath = callingPath.parent_path().string() + "/" + doseMapExecutable;
			std::string baseCommand = doseMapExeWithPath +" "+ inputFilename +" "+ outputFilename;



			std::string defaultDoseStatisticsCommand = baseCommand+" -l "+ inputDoseLoadStyle;
			std::cout << "Command line call: " + defaultDoseStatisticsCommand << std::endl;
			CHECK_EQUAL(system(defaultDoseStatisticsCommand.c_str()), 0);

			CHECK_EQUAL(boost::filesystem::exists(outputFilename), true);
			CHECK_EQUAL(std::remove(outputFilename.c_str()),0);

			std::string defaultDoseStatisticsCommandwithregistration = defaultDoseStatisticsCommand+ " -r " + regFilename;
			std::cout << "Command line call: " + defaultDoseStatisticsCommandwithregistration << std::endl;
			CHECK_EQUAL(system(defaultDoseStatisticsCommandwithregistration.c_str()), 0);

			CHECK_EQUAL(boost::filesystem::exists(outputFilename), true);
			CHECK_EQUAL(std::remove(outputFilename.c_str()), 0);


			RETURN_AND_REPORT_TEST_SUCCESS;
		}
	} //namespace testing
} //namespace rttb
