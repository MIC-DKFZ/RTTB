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
// @version $Revision: 5603 $ (last changed revision)
// @date    $Date: 2015-08-24 15:17:30 +0200 (Mo, 24 Aug 2015) $ (last change date)
// @author  $Author: strubel $ (last changed by)
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

			std::string StructCommand = voxelizerToolExeWithPath;
			StructCommand += " -s " + invalidStructFile;
			StructCommand += " -r " + referenceFile;
			StructCommand += " -e " + structureName;
			std::cout << "Command line call: " + StructCommand << std::endl;
			CHECK_EQUAL(system(StructCommand.c_str()), 1);

			std::string ReferenceCommand = voxelizerToolExeWithPath;
			ReferenceCommand += " -s " + structFile;
			ReferenceCommand += " -r " + invalidReferenceFile;
			ReferenceCommand += " -e " + structureName;
			std::cout << "Command line call: " + ReferenceCommand << std::endl;
			CHECK_EQUAL(system(ReferenceCommand.c_str()), 1);

			std::string Structure = voxelizerToolExeWithPath;
			Structure += " -s " + structFile;
			Structure += " -r " + referenceFile;
			Structure += +" -e " + invalidStructureName;
			std::cout << "Command line call: " + Structure << std::endl;
			CHECK_EQUAL(system(Structure.c_str()), 0);

			RETURN_AND_REPORT_TEST_SUCCESS;
		}

	}
}
