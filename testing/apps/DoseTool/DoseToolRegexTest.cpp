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
#include <fstream>
#include <cstdio>

#include "litCheckMacros.h"

#include "boost/filesystem.hpp"
#include "boost/algorithm/string.hpp"

namespace rttb
{
	namespace testing
	{

		//path to the current running directory. DoseTool is in the same directory (Debug/Release)
		extern const char* _callingAppPath;

		static std::string readFile(const std::string& filename);

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
			baseCommand += " -d " + doseFilename;
			baseCommand += " -t " + doseLoadStyle;
			baseCommand += " -s " + structFilename;
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

			//check if file is the same than reference file
			std::string defaultAsIs = readFile(defaultExpectedOutputFilename);
			std::string defaultExpected = readFile(referenceXMLFilename);
			//add doseFile and structFile
			std::string emptyDoseFileTag = "<doseFile></doseFile>";
			std::string validDoseFileTag = "<doseFile>" + doseFilename + "</doseFile>";
			boost::replace_all(defaultExpected, emptyDoseFileTag, validDoseFileTag);

			std::string emptyStructFileTag = "<structFile></structFile>";
			std::string validStructFileTag = "<structFile>" + structFilename + "</structFile>";
			boost::replace_all(defaultExpected, emptyStructFileTag, validStructFileTag);

			std::string requestedStructRegexTag = "<requestedStructRegex>Nodes</requestedStructRegex>";
			std::string validStructRegexTag = "<requestedStructRegex>" + structName + "</requestedStructRegex>";
			boost::replace_all(defaultExpected, requestedStructRegexTag, validStructRegexTag);

			CHECK_EQUAL(defaultAsIs, defaultExpected);

			//add doseFile and structFile
			std::string default2AsIs = readFile(defaultExpectedOutputFilename2);
			std::string default2Expected = readFile(referenceXMLFilename2);

			boost::replace_all(default2Expected, emptyDoseFileTag, validDoseFileTag);
			boost::replace_all(default2Expected, emptyStructFileTag, validStructFileTag);

			requestedStructRegexTag = "<requestedStructRegex>Heart</requestedStructRegex>";
			validStructRegexTag = "<requestedStructRegex>" + structName + "</requestedStructRegex>";
			boost::replace_all(default2Expected, requestedStructRegexTag, validStructRegexTag);

			CHECK_EQUAL(default2AsIs, default2Expected);

			//delete file again
			CHECK_EQUAL(std::remove(defaultExpectedOutputFilename.c_str()), 0);
			CHECK_EQUAL(std::remove(defaultExpectedOutputFilename2.c_str()), 0);

			RETURN_AND_REPORT_TEST_SUCCESS;
		}

		std::string readFile(const std::string& filename)
		{
			std::ifstream fileStream(filename.c_str());
			std::string content((std::istreambuf_iterator<char>(fileStream)),
			                    (std::istreambuf_iterator<char>()));
			return content;
		}
	} //namespace testing
} //namespace rttb
