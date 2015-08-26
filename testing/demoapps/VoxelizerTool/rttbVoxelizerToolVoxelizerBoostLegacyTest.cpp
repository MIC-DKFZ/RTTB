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
// @version $Revision: 5503 $ (last changed revision)
// @date    $Date: 2015-07-24 12:33:26 +0200 (Fr, 24 Jul 2015) $ (last change date)
// @author  $Author: strubel $ (last changed by)
*/

#include "litCheckMacros.h"
#include <iostream>
#include <boost/filesystem.hpp>
#include <vector>

/*! @brief VoxelizerToolTest5.
Test the paramter boost and legacy Voxelization.
*/

int main(int argc, char* argv[])
{
	PREPARE_DEFAULT_TEST_REPORTING;

	if (argc > 3)
	{
		const std::string PathToBinaryDirectory = argv[1];
		const std::string PathToTestingDirectory = argv[2];
		const std::string RTToolBoxTestingDirectory = argv[3];

		std::vector<std::string> OutputVoxelization;
		OutputVoxelization.push_back("Rueckenmark -o Test.hdr");
		OutputVoxelization.push_back("Leber -l");
		OutputVoxelization.push_back("Darm -b");
		OutputVoxelization.push_back("Leber -o Legacy.hdr -l");
		OutputVoxelization.push_back("Darm -o Boost.hdr -b");

		std::vector<std::string> OutputFiles;
		OutputFiles.push_back("/Boost_Darm.hdr");
		OutputFiles.push_back("/Boost_Darm.img");
		OutputFiles.push_back("/Legacy_Leber.hdr");
		OutputFiles.push_back("/Legacy_Leber.img");
		OutputFiles.push_back("/out_Darm.hdr");
		OutputFiles.push_back("/out_Darm.img");
		OutputFiles.push_back("/out_Leber.hdr");
		OutputFiles.push_back("/out_Leber.img");
		OutputFiles.push_back("/Test_Rueckenmark.hdr");
		OutputFiles.push_back("/Test_Rueckenmark.img");

		const std::string baseCommand = PathToBinaryDirectory +
		                                "Release/VoxelizerTool -s " + RTToolBoxTestingDirectory +
		                                "data/DICOM/StructureSet/RS1.3.6.1.4.1.2452.6.841242143.1311652612.1170940299.4217870819.dcm -r " +
		                                RTToolBoxTestingDirectory + "data/DICOM/TestDose/ConstantTwo.dcm -e ";

		for (int i = 0 ; i < OutputVoxelization.size(); i++)
		{
			std::string Command = baseCommand + OutputVoxelization.at(i);
			int returnValue = system(Command.c_str());
			CHECK_EQUAL(returnValue, 0);
		}

		for (int i = 0; i < OutputFiles.size(); i++)
		{
			std::string pathToFile = PathToTestingDirectory + OutputFiles.at(i);

			CHECK_EQUAL(boost::filesystem::exists(pathToFile), true);

			if (boost::filesystem::exists(pathToFile))
			{
				boost::filesystem::remove(pathToFile);
			}
		}
	}

	RETURN_AND_REPORT_TEST_SUCCESS;
}