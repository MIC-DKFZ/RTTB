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

#include "litCheckMacros.h"

#include "rttbDoseStatisticsXMLReader.h"
#include "../../io/other/CompareDoseStatistic.h"

#include "boost/filesystem.hpp"

namespace rttb
{
	namespace testing
	{

		//path to the current running directory. DoseTool is in the same directory (Debug/Release)
		extern const char* _callingAppPath;

		int DoseToolITKDoseTest(int argc, char* argv[])
		{
			PREPARE_DEFAULT_TEST_REPORTING;

			std::string doseToolExecutable;
			std::string doseFilename;
			std::string structFilename;
			std::string structName;
			std::string referenceXMLFilename;
			std::string referenceXMLComplexFilename;
			std::string defaultOutputFilename;
			std::string complexOutputFilename;

			boost::filesystem::path callingPath(_callingAppPath);

			if (argc > 8)
			{
				doseToolExecutable = argv[1];
				doseFilename = argv[2];
				structFilename = argv[3];
				structName = argv[4];
				referenceXMLFilename = argv[5];
				referenceXMLComplexFilename = argv[6];
				defaultOutputFilename = argv[7];
				complexOutputFilename = argv[8];
			}

			std::string doseToolExeWithPath = callingPath.parent_path().string() + "/" + doseToolExecutable;

			std::string baseCommand = doseToolExeWithPath;
            baseCommand += " -d \"" + doseFilename + "\"";
            baseCommand += " -s \"" + structFilename + "\"";
			baseCommand += " -t itk ";

			if (structName != "")
			{
				baseCommand += " -n " + structName;
			}
			if (boost::filesystem::extension(structFilename).compare(".nrrd") == 0) {
				baseCommand += " -u itk ";
			}
			std::string defaultDoseStatisticsCommand = baseCommand + " -y " + defaultOutputFilename;
			std::cout << "Command line call: " + defaultDoseStatisticsCommand << std::endl;
			CHECK_EQUAL(system(defaultDoseStatisticsCommand.c_str()), 0);

			std::string complexDoseStatisticsCommand = baseCommand + " -y " + complexOutputFilename;
			//prescribed dose is 14 Gy
			complexDoseStatisticsCommand += " -f -p 14";

			std::cout << "Command line call: " + complexDoseStatisticsCommand << std::endl;
			CHECK_EQUAL(system(complexDoseStatisticsCommand.c_str()), 0);

			//check if file exists
			CHECK_EQUAL(boost::filesystem::exists(defaultOutputFilename), true);
			CHECK_EQUAL(boost::filesystem::exists(complexOutputFilename), true);

            //check if file has dose statistics that are same than these in than reference file
            io::other::DoseStatisticsXMLReader readerDefaultExpected(referenceXMLFilename);
            auto doseStatisticsDefaultExpected = readerDefaultExpected.generateDoseStatistic();
            io::other::DoseStatisticsXMLReader readerDefaultActual(defaultOutputFilename);
            auto doseStatisticsDefaultActual = readerDefaultActual.generateDoseStatistic();

            CHECK(checkEqualDoseStatistic(doseStatisticsDefaultExpected, doseStatisticsDefaultActual));

            io::other::DoseStatisticsXMLReader readerComplexExpected(referenceXMLComplexFilename);
            auto doseStatisticsComplexExpected = readerComplexExpected.generateDoseStatistic();
            io::other::DoseStatisticsXMLReader readerComplexActual(complexOutputFilename);
            auto doseStatisticsComplexActual = readerComplexActual.generateDoseStatistic();

            CHECK(checkEqualDoseStatistic(doseStatisticsComplexExpected, doseStatisticsComplexActual));

			//delete file again
			CHECK_EQUAL(std::remove(defaultOutputFilename.c_str()), 0);
			CHECK_EQUAL(std::remove(complexOutputFilename.c_str()), 0);

			RETURN_AND_REPORT_TEST_SUCCESS;
		}
	} //namespace testing
} //namespace rttb
