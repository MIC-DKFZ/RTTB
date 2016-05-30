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
// @version $Revision: 1233 $ (last changed revision)
// @date    $Date: 2016-01-20 15:47:47 +0100 (Mi, 20 Jan 2016) $ (last change date)
// @author  $Author: hentsch $ (last changed by)
*/

#include <iostream>
#include <fstream>
#include <cstdio>

#include "litCheckMacros.h"

#include "boost/filesystem.hpp"

namespace rttb
{
	namespace testing
	{

		//path to the current running directory. DoseTool is in the same directory (Debug/Release)
		extern const char* _callingAppPath;

		static std::string readFile(const std::string& filename);

		int DoseToolDVHTest(int argc, char* argv[])
		{
			PREPARE_DEFAULT_TEST_REPORTING;

			std::string doseToolExecutable;
			std::string doseFilename;
			std::string structFilename;
			std::string structName;
			std::string referenceXMLFilename;

			boost::filesystem::path callingPath(_callingAppPath);

			if (argc > 5)
			{
				doseToolExecutable = argv[1];
				doseFilename = argv[2];
				structFilename = argv[3];
				structName = argv[4];
				referenceXMLFilename = argv[5];
			}

			std::string voxelizerToolExeWithPath = callingPath.parent_path().string() + "/" +
			                                       doseToolExecutable;

			std::string defaultOutputFilename = "dicomDVHOutput.xml";

			std::string baseCommand = voxelizerToolExeWithPath;
			baseCommand += " -d " + doseFilename;
			baseCommand += " -s " + structFilename;

			if (structName != "")
			{
				baseCommand += " -n " + structName;
			}
			else
			{
				baseCommand += " -u itk ";
			}

			std::string defaultDVHCommand = baseCommand + " -z " + defaultOutputFilename;
			std::cout << "Command line call: " + defaultDVHCommand << std::endl;
			CHECK_EQUAL(system(defaultDVHCommand.c_str()), 0);

			//check if file exists
			CHECK_EQUAL(boost::filesystem::exists(defaultOutputFilename), true);

			//check if file is the same than reference file
			std::string defaultAsIs = readFile(defaultOutputFilename);
			std::string defaultExpected = readFile(referenceXMLFilename);

			CHECK_EQUAL(defaultAsIs, defaultExpected);

			//delete file again
			CHECK_EQUAL(std::remove(defaultOutputFilename.c_str()), 0);

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
