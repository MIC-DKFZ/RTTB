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

#include <iostream>
#include "litCheckMacros.h"
#include <boost/filesystem.hpp>
#include <vector>

/*! @brief VoxelizerToolTest.
Test all structs.
*/

int main(int argc, char* argv[])
{
	PREPARE_DEFAULT_TEST_REPORTING;

	if (argc > 3)
	{
		const std::string PathToBinaryDirectory = argv[1];
		const std::string tempDirectory = argv[2];
		const std::string RTToolBoxTestingDirectory = argv[3];

		std::vector<std::string> structs;
		structs.push_back("Aussenkontur");
		structs.push_back("Rueckenmark");
		structs.push_back("Niere re.");
		structs.push_back("Niere li.");
		structs.push_back("Magen/DD");
		structs.push_back("Leber");
		structs.push_back("Darm");
		structs.push_back("Ref.Pkt.");
		structs.push_back("PTV");
		structs.push_back("Boost");

		std::vector<std::string> filenames;
		filenames.push_back("Aussenkontur");
		filenames.push_back("Rueckenmark");
		filenames.push_back("Niere re");
		filenames.push_back("Niere li");
		filenames.push_back("Magen_DD");
		filenames.push_back("Leber");
		filenames.push_back("Darm");
		filenames.push_back("Ref.Pkt");
		filenames.push_back("PTV");
		filenames.push_back("Boost");

		const std::string baseCommand = PathToBinaryDirectory +
		                                "Release/VoxelizerTool -s " + RTToolBoxTestingDirectory +
		                                "data/DICOM/StructureSet/RS1.3.6.1.4.1.2452.6.841242143.1311652612.1170940299.4217870819.dcm -r " +
		                                RTToolBoxTestingDirectory + "data/DICOM/TestDose/ConstantTwo.dcm -e \"";



		for (int i = 0; i < structs.size(); i++)
		{
			std::string command = baseCommand + structs.at(i) + "\"";
			int returnValue = system(command.c_str());

			CHECK_EQUAL(returnValue, 0);

			std::string HDRfileName = tempDirectory + "\\out_" +   filenames.at(i) + ".hdr";
			std::string IMGfileName = tempDirectory + "\\out_" +   filenames.at(i) + ".img";

			CHECK_EQUAL(
			    boost::filesystem::exists(HDRfileName),
			    true);
			CHECK_EQUAL(
			    boost::filesystem::exists(IMGfileName),
			    true);

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