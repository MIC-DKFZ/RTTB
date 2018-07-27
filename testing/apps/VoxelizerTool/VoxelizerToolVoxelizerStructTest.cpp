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

#include "litCheckMacros.h"

#include <boost/filesystem.hpp>
#include <iostream>

/*! @brief VoxelizerToolTest.
	Tests a struct with multiple files and a single file
*/

namespace rttb
{
	namespace testing
	{
		//path to the current running directory. VoxelizerTool is in the same directory (Debug/Release)
		extern const char* _callingAppPath;
		int VoxelizerToolVoxelizerStructTest(int argc, char* argv[]) {
			PREPARE_DEFAULT_TEST_REPORTING;

			std::string voxelizerToolExe;
			std::string tempDirectory;
			std::string structFile;
			std::string referenceFile;

			boost::filesystem::path HDRFile(tempDirectory);
			HDRFile /= "outputSingleStruct.hdr";

			boost::filesystem::path IMGFile(tempDirectory);
			IMGFile /= "outputSingleStruct.img";

			if (argc > 4) {
				voxelizerToolExe = argv[1];
				tempDirectory = argv[2];
				structFile = argv[3];
				referenceFile = argv[4];
			}

			boost::filesystem::path callingPath(_callingAppPath);
			std::string voxelizerToolExeWithPath = callingPath.parent_path().string() + "/" + voxelizerToolExe;

			std::string structName = "Heart";

			std::string baseCommand = voxelizerToolExeWithPath;
			baseCommand += " -s \"" + structFile + "\"";
			baseCommand += " -r \"" + referenceFile + "\"";
			baseCommand += " -o outputSingleStruct.hdr";
			baseCommand += " -y itkDicom";
			baseCommand += " -e \"";

			std::string command = baseCommand + structName + "\"";
			std::cout << "Command line call: " + command << std::endl;
			int returnValue = system(command.c_str());

			CHECK_EQUAL(returnValue, 0);

			CHECK_EQUAL(boost::filesystem::exists(HDRFile), true);
			CHECK_EQUAL(boost::filesystem::exists(IMGFile), true);



			baseCommand = voxelizerToolExeWithPath;
			baseCommand += " -s \"" + structFile + "\"";
			baseCommand += " -r \"" + HDRFile.string() + "\"";
			baseCommand += " -o outputSingleStruct.hdr";
			baseCommand += " -y itk";
			baseCommand += " -e \"";

			command = baseCommand + structName + "\"";
			std::cout << "Command line call: " + command << std::endl;
			returnValue = system(command.c_str());

			CHECK_EQUAL(returnValue, 0);

			if (boost::filesystem::exists(IMGFile)) {
				boost::filesystem::remove(IMGFile);
			}

			if (boost::filesystem::exists(HDRFile)) {
				boost::filesystem::remove(HDRFile);
			}

			RETURN_AND_REPORT_TEST_SUCCESS;
		}
	}
}
