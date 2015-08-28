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
		int VoxelizerToolDifferentCommandsTest(int argc, char* argv[])
		{
			PREPARE_DEFAULT_TEST_REPORTING;

			if (argc > 3)
			{
				const std::string PathToBinaryDirectory = argv[1];
				const std::string path = argv[2];
				const std::string RTToolBoxTestingDirectory = argv[3];

				std::vector<std::string> commands;
				commands.push_back(".*");
				commands.push_back(".* -m -o M.hdr");
				commands.push_back("Niere.* -m -o Test.hdr");
				commands.push_back("Leber -o Boolean.hdr -v");

				std::vector<std::string> structures;
				structures.push_back("M_Aussenkontur");
				structures.push_back("M_Rueckenmark");
				structures.push_back("M_Niere re");
				structures.push_back("M_Niere li");
				structures.push_back("M_Magen_DD");
				structures.push_back("M_Leber");
				structures.push_back("M_Darm");
				structures.push_back("M_Ref.Pkt");
				structures.push_back("M_PTV");
				structures.push_back("M_Boost");
				structures.push_back("out_Aussenkontur");
				structures.push_back("Test_Niere li");
				structures.push_back("Test_Niere re");
				structures.push_back("Boolean_Leber");

				const std::string baseCommand = PathToBinaryDirectory +
				                                "/Release/RTTBVoxelizerTool -s " + RTToolBoxTestingDirectory +
				                                "/DICOM/StructureSet/RS1.3.6.1.4.1.2452.6.841242143.1311652612.1170940299.4217870819.dcm -r " +
				                                RTToolBoxTestingDirectory + "/DICOM/TestDose/ConstantTwo.dcm -e ";

				for (int i = 0; i < commands.size(); i++)
				{
					std::string command = baseCommand + commands.at(i);
					int returnValue = system(command.c_str());
					CHECK_EQUAL(returnValue, 0);
				}


				for (int i = 0; i < structures.size(); i++)
				{
					std::string IMGfileName = path + "/" + structures.at(i) + ".img";
					std::string HDRfileName = path + "/" + structures.at(i) + ".hdr";

					CHECK_EQUAL(boost::filesystem::exists(HDRfileName), true);
					CHECK_EQUAL(boost::filesystem::exists(IMGfileName), true);

					boost::filesystem::path imgFile(IMGfileName);

					if (boost::filesystem::exists(imgFile))
					{
						boost::filesystem::remove(imgFile);
					}

					boost::filesystem::path hdrFile(HDRfileName);

					if (boost::filesystem::exists(hdrFile))
					{
						boost::filesystem::remove(hdrFile);
					}

				}
			}

			RETURN_AND_REPORT_TEST_SUCCESS;
		}

	}
}
