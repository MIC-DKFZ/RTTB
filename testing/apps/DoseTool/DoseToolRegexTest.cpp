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

#include "rttbDoseStatisticsXMLReader.h"
#include "../../io/other/CompareDoseStatistic.h"

#include "boost/filesystem.hpp"

namespace rttb
{
	namespace testing
	{

		//path to the current running directory. DoseTool is in the same directory (Debug/Release)
		extern const char* _callingAppPath;

		int DoseToolRegexTest(int argc, char* argv[])
		{
			PREPARE_DEFAULT_TEST_REPORTING;

			std::string doseToolExecutable;
			std::string doseFilename;
			std::string doseLoadStyle;
			std::string structFilename;
			std::string structLoadStyle;
			std::string structName;
			std::string referenceXMLFilename;
			std::string referenceXMLFilename2;

			boost::filesystem::path callingPath(_callingAppPath);

			if (argc > 8)
			{
				doseToolExecutable = argv[1];
				doseFilename = argv[2];
				doseLoadStyle = argv[3];
				structFilename = argv[4];
				structLoadStyle = argv[5];
				structName = argv[6];
				referenceXMLFilename = argv[7];
				referenceXMLFilename2 = argv[8];
			}

			std::string doseToolExeWithPath = callingPath.parent_path().string() + "/" + doseToolExecutable;

			std::string defaultOutputFilename = "regexOutput.xml";
			std::string defaultExpectedOutputFilename = "regexOutput_Nodes.xml";
			std::string defaultExpectedOutputFilename2 = "regexOutput_Heart.xml";

			std::string baseCommand = doseToolExeWithPath;
            baseCommand += " -d \"" + doseFilename + "\"";
			baseCommand += " -t " + doseLoadStyle;
            baseCommand += " -s \"" + structFilename + "\"";
			baseCommand += " -u " + structLoadStyle;
			baseCommand += " -n \"" + structName + "\"";
			baseCommand += " -y " + defaultOutputFilename;

			std::cout << "Command line call: " + baseCommand << std::endl;
			CHECK_EQUAL(system(baseCommand.c_str()), 0);

			CHECK_EQUAL(boost::filesystem::exists(defaultOutputFilename), true);
			CHECK_EQUAL(boost::filesystem::exists(defaultExpectedOutputFilename), false);
			CHECK_EQUAL(boost::filesystem::exists(defaultExpectedOutputFilename2), false);
			CHECK_EQUAL(std::remove(defaultOutputFilename.c_str()), 0);

			std::string defaultDoseStatisticsCommand = baseCommand + " -m";
			std::cout << "Command line call: " + defaultDoseStatisticsCommand << std::endl;
			CHECK_EQUAL(system(defaultDoseStatisticsCommand.c_str()), 0);

			//check if two file were written
			CHECK_EQUAL(boost::filesystem::exists(defaultExpectedOutputFilename), true);
			CHECK_EQUAL(boost::filesystem::exists(defaultExpectedOutputFilename2), true);

            //check if file has dose statistics that are same than these in than reference file
            io::other::DoseStatisticsXMLReader readerDefaultExpected(referenceXMLFilename);

            auto doseStatisticsDefaultExpected = readerDefaultExpected.generateDoseStatistic();

            io::other::DoseStatisticsXMLReader readerDefaultActual(defaultExpectedOutputFilename);

            auto doseStatisticsDefaultActual = readerDefaultActual.generateDoseStatistic();

            CHECK(checkEqualDoseStatistic(doseStatisticsDefaultExpected, doseStatisticsDefaultActual));

            io::other::DoseStatisticsXMLReader readerDefaultExpected2(referenceXMLFilename2);
            auto doseStatisticsDefaultExpected2 = readerDefaultExpected2.generateDoseStatistic();
            io::other::DoseStatisticsXMLReader readerDefaultActual2(defaultExpectedOutputFilename2);
            auto doseStatisticsDefaultActual2 = readerDefaultActual2.generateDoseStatistic();

            CHECK(checkEqualDoseStatistic(doseStatisticsDefaultExpected2, doseStatisticsDefaultActual2));

			//delete file again
			CHECK_EQUAL(std::remove(defaultExpectedOutputFilename.c_str()), 0);
			CHECK_EQUAL(std::remove(defaultExpectedOutputFilename2.c_str()), 0);

			RETURN_AND_REPORT_TEST_SUCCESS;
		}
	} //namespace testing
} //namespace rttb
