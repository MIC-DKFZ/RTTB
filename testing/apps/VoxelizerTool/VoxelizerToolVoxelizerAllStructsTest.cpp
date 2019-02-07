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
#include <boost/filesystem.hpp>
#include <vector>

/*! @brief VoxelizerToolTest.
Tests a selection of structs.
*/

namespace rttb
{
	namespace testing
	{
		//path to the current running directory. VoxelizerTool is in the same directory (Debug/Release)
		extern const char* _callingAppPath;
		int VoxelizerToolVoxelizerAllStructsTest(int argc, char* argv[])
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

			boost::filesystem::path callingPath(_callingAppPath);
			std::string voxelizerToolExeWithPath = callingPath.parent_path().string() + "/" + voxelizerToolExe;

			std::vector<std::string> structNames;
			structNames.push_back("Niere.*");
			structNames.push_back("Magen/DD");
			structNames.push_back("PTV");

			std::string baseCommand = voxelizerToolExeWithPath;
			baseCommand += " -s \"" + structFile + "\"";
			baseCommand += " -r \"" + referenceFile + "\"";
            baseCommand += " -o outputAllStructs.nrrd";
			baseCommand += " -e \"";

			for (size_t i = 0; i < structNames.size(); i++)
			{
				std::string command = baseCommand + structNames.at(i) + "\"";
				std::cout << "Command line call: " + command << std::endl;
				int returnValue = system(command.c_str());

				CHECK_EQUAL(returnValue, 0);

				boost::filesystem::path NRRDFile(tempDirectory);
				NRRDFile /= "outputAllStructs.nrrd";

				CHECK_EQUAL(
				    boost::filesystem::exists(NRRDFile),
				    true);

				if (boost::filesystem::exists(NRRDFile))
				{
					boost::filesystem::remove(NRRDFile);
				}
			}

			RETURN_AND_REPORT_TEST_SUCCESS;
		}
	}
}
