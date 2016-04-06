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

		int DoseToolVirtuosDoseTest(int argc, char* argv[])
		{
			PREPARE_DEFAULT_TEST_REPORTING;

			std::string doseToolExecutable;
			std::string doseFilename;
			std::string planFilename;
			std::string structFilename;
			std::string ctxFilename;
			std::string structName;
			std::string referenceXMLFilename;
			std::string referenceXMLComplexFilename;

			boost::filesystem::path callingPath(_callingAppPath);


			if (argc > 8)
			{
				doseToolExecutable = argv[1];
				doseFilename = argv[2];
				planFilename = argv[3];
				structFilename = argv[4];
				ctxFilename = argv[5];
				structName = argv[6];
				referenceXMLFilename = argv[7];
				referenceXMLComplexFilename = argv[8];
			}

			std::string doseToolExeWithPath = callingPath.parent_path().string() + "/" + doseToolExecutable;

			std::string defaultOutputFilename = "virtuosOutput.xml";
			std::string complexOutputFilename = "virtuosOutputComplex.xml";

			std::string baseCommand = doseToolExeWithPath;
			baseCommand += " -d " + doseFilename;
			baseCommand += " -t virtuos " + planFilename;
			baseCommand += " -s " + structFilename;
			baseCommand += " -u virtuos " + ctxFilename;
			baseCommand += " -n " + structName;
			std::string defaultDoseStatisticsCommand = baseCommand + " -y " + defaultOutputFilename;
			std::cout << "Command line call: " + defaultDoseStatisticsCommand << std::endl;
			CHECK_EQUAL(system(defaultDoseStatisticsCommand.c_str()), 0);

			std::string complexDoseStatisticsCommand = baseCommand + " -y " + complexOutputFilename;
			//prescribed dose is 50 Gy
			complexDoseStatisticsCommand += " -x -p 50";

			std::cout << "Command line call: " + complexDoseStatisticsCommand << std::endl;
			CHECK_EQUAL(system(complexDoseStatisticsCommand.c_str()), 0);

			//check if file exists
			CHECK_EQUAL(boost::filesystem::exists(defaultOutputFilename), true);
			CHECK_EQUAL(boost::filesystem::exists(complexOutputFilename), true);

			//check if file is the same than reference file
			std::string defaultAsIs = readFile(defaultOutputFilename);
			std::string defaultExpected = readFile(referenceXMLFilename);
			//add doseFile and structFile
			std::string emptyDoseFileTag = "<doseFile></doseFile>";
			std::string validDoseFileTag = "<doseFile>" + doseFilename + "</doseFile>";
			boost::replace_all(defaultExpected, emptyDoseFileTag, validDoseFileTag);

			std::string emptyStructFileTag = "<structFile></structFile>";
			std::string validStructFileTag = "<structFile>" + structFilename + "</structFile>";
			boost::replace_all(defaultExpected, emptyStructFileTag, validStructFileTag);

			CHECK_EQUAL(defaultAsIs, defaultExpected);

			//add doseFile and structFile
			std::string complexAsIs = readFile(complexOutputFilename);
			std::string complexExpected = readFile(referenceXMLComplexFilename);

			boost::replace_all(complexExpected, emptyDoseFileTag, validDoseFileTag);
			boost::replace_all(complexExpected, emptyStructFileTag, validStructFileTag);

			CHECK_EQUAL(complexAsIs, complexExpected);

			//delete file again
			CHECK_EQUAL(std::remove(defaultOutputFilename.c_str()), 0);
			CHECK_EQUAL(std::remove(complexOutputFilename.c_str()), 0);

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
