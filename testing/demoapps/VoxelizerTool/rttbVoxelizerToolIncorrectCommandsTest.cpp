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
// @version $Revision$ (last changed revision)
// @date    $Date$ (last change date)
// @author  $Author$ (last changed by)
*/

#include <iostream>
#include "litCheckMacros.h"
#include <boost/filesystem.hpp>

/*! @brief VoxelizerToolTest4.
Test incorrect commands with a wrong structfile, referencefile and a wrong struct.
if the command return one, the program could not run to the end.
return zero the command is correct
*/
namespace rttb
{
	namespace testing
	{
		//path to the current running directory. VoxelizerTool is in the same directory (Debug/Release)
		extern const char* _callingAppPath;
		int VoxelizerToolIncorrectCommandsTest(int argc, char* argv[])
		{
			PREPARE_DEFAULT_TEST_REPORTING;

			std::string voxelizerToolExe;
			std::string structFile;
			std::string invalidStructFile;
			std::string invalidReferenceFile;
			std::string referenceFile;
			std::string structureName;
			std::string invalidStructureName;

			if (argc > 7)
			{
				voxelizerToolExe = argv[1];
				structFile = argv[2];
				invalidStructFile = argv[3];
				referenceFile = argv[4];
				invalidReferenceFile = argv[5];
				structureName = argv[6];
				invalidStructureName = argv[7];
			}

			boost::filesystem::path callingPath(_callingAppPath);
			std::string voxelizerToolExeWithPath = callingPath.parent_path().string() + "/" + voxelizerToolExe;

			std::string tooFewArgumentsCommand = voxelizerToolExeWithPath;
			tooFewArgumentsCommand += " -s " + invalidStructFile;
			tooFewArgumentsCommand += " -r " + referenceFile;
			std::cout << "Command line call: " + tooFewArgumentsCommand << std::endl;
			CHECK_EQUAL(system(tooFewArgumentsCommand.c_str()), 1);

			tooFewArgumentsCommand = voxelizerToolExeWithPath;
			tooFewArgumentsCommand += " -s " + invalidStructFile;
			tooFewArgumentsCommand += " -e " + structureName;
			std::cout << "Command line call: " + tooFewArgumentsCommand << std::endl;
			CHECK_EQUAL(system(tooFewArgumentsCommand.c_str()), 1);

			tooFewArgumentsCommand = voxelizerToolExeWithPath;
			std::cout << "Command line call: " + tooFewArgumentsCommand << std::endl;
			CHECK_EQUAL(system(tooFewArgumentsCommand.c_str()), 1);

			std::string noOutputEndingCommand = voxelizerToolExeWithPath;
			noOutputEndingCommand += " -s " + invalidStructFile;
			noOutputEndingCommand += " -r " + referenceFile;
			noOutputEndingCommand += " -e " + structureName;
			noOutputEndingCommand += " -o bla";
			std::cout << "Command line call: " + noOutputEndingCommand << std::endl;
			CHECK_EQUAL(system(noOutputEndingCommand.c_str()), 1);

			std::string structCommand = voxelizerToolExeWithPath;
			structCommand += " -s " + invalidStructFile;
			structCommand += " -r " + referenceFile;
			structCommand += " -e " + structureName;
			std::cout << "Command line call: " + structCommand << std::endl;
			CHECK_EQUAL(system(structCommand.c_str()), 2);

			std::string referenceCommand = voxelizerToolExeWithPath;
			referenceCommand += " -s " + structFile;
			referenceCommand += " -r " + invalidReferenceFile;
			referenceCommand += " -e " + structureName;
			std::cout << "Command line call: " + referenceCommand << std::endl;
			CHECK_EQUAL(system(referenceCommand.c_str()), 2);

			std::string structureNameCommand = voxelizerToolExeWithPath;
			structureNameCommand += " -s " + structFile;
			structureNameCommand += " -r " + referenceFile;
			structureNameCommand += +" -e " + invalidStructureName;
			std::cout << "Command line call: " + structureNameCommand << std::endl;
			CHECK_EQUAL(system(structureNameCommand.c_str()), 0);

			std::string referenceLoadingStyleCommand = voxelizerToolExeWithPath;
			referenceLoadingStyleCommand += " -s " + structFile;
			referenceLoadingStyleCommand += " -r " + referenceFile;
			referenceLoadingStyleCommand += +" -e " + invalidStructureName;
			referenceLoadingStyleCommand += +" -y nonsense";
			std::cout << "Command line call: " + referenceLoadingStyleCommand << std::endl;
			CHECK_EQUAL(system(referenceLoadingStyleCommand.c_str()), 1);

			RETURN_AND_REPORT_TEST_SUCCESS;
		}

	}
}
