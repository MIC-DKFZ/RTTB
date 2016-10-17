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
// @version $Revision: 1333 $ (last changed revision)
// @date    $Date: 2016-04-22 11:12:14 +0200 (Fr, 22 Apr 2016) $ (last change date)
// @author  $Author: hentsch $ (last changed by)
*/

#include "litCheckMacros.h"
#include <iostream>
#include <boost/filesystem.hpp>
#include <vector>

/*! @brief VoxelizerToolTest3.
Test the output, multipleStructs and the booleanVoxelization parameter.
*/
namespace rttb
{
	namespace testing
	{
		//path to the current running directory. VoxelizerTool is in the same directory (Debug/Release)
		extern const char* _callingAppPath;
		int VoxelizerToolDifferentCommandsTest(int argc, char* argv[])
		{
			PREPARE_DEFAULT_TEST_REPORTING;

			std::string voxelizerToolExe;
			std::string tempDirectory;
			std::string structFile;
			std::string referenceFile;

			if (argc > 4)
			{
				voxelizerToolExe = argv[1];
				tempDirectory = argv[2];
				structFile = argv[3];
				referenceFile = argv[4];
			}

			std::vector<std::string> commands;
			commands.push_back("\"Niere.*\" -m -o Test.hdr");
			commands.push_back("\"Leber\" -o Boolean.hdr -z");

			std::vector<std::string> filenames;
			filenames.push_back("Test_Niere li");
			filenames.push_back("Test_Niere re");
			filenames.push_back("Boolean_Leber");

			boost::filesystem::path callingPath(_callingAppPath);
			std::string voxelizerToolExeWithPath = callingPath.parent_path().string() + "/" + voxelizerToolExe;

			std::string baseCommand = voxelizerToolExeWithPath;
			baseCommand += " -s " + structFile;
			baseCommand += " -r " + referenceFile;
			baseCommand += " -e ";

			for (size_t i = 0; i < commands.size(); i++)
			{
				std::string command = baseCommand + commands.at(i);
				int returnValue = system(command.c_str());
				std::cout << "Command line call: " + command << std::endl;
				CHECK_EQUAL(returnValue, 0);
			}

			for (size_t i = 0; i < filenames.size(); i++)
			{
				const std::string HDRfileName = tempDirectory + "/" + filenames.at(i) + ".hdr";
				boost::filesystem::path HDRFile(HDRfileName);

				const std::string IMGfileName = tempDirectory + "/" + filenames.at(i) + ".img";
				boost::filesystem::path IMGFile(IMGfileName);

				CHECK_EQUAL(boost::filesystem::exists(HDRFile), true);
				CHECK_EQUAL(boost::filesystem::exists(IMGFile), true);

				if (boost::filesystem::exists(IMGFile))
				{
					boost::filesystem::remove(IMGFile);
				}

				if (boost::filesystem::exists(HDRFile))
				{
					boost::filesystem::remove(HDRFile);
				}
			}

			RETURN_AND_REPORT_TEST_SUCCESS;
		}

	}
}
