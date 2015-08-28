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
#include <vector>

/*! @brief VoxelizerToolTest4.
Test incorrect commands with a wrong structfile, referencefile and a wrong struct.
if the command return one, the program could not run to the end.
return zero the command is correct
*/
namespace rttb
{
	namespace testing
	{

		int VoxelizerToolIncorrectCommandsTest(int argc, char* argv[])
		{
			PREPARE_DEFAULT_TEST_REPORTING;

			if (argc > 3)
			{
				const std::string PathToBinaryDirectory = argv[1];
				const std::string RTToolBoxTestingDirectory = argv[3];

				const std::string StructCommand = PathToBinaryDirectory +
				                                  "/Release/RTTBVoxelizerTool -s " + RTToolBoxTestingDirectory +
				                                  "/DICOM/StructureSet/Wrong_Data_Struct_file.dicom -r " +
				                                  RTToolBoxTestingDirectory + "/DICOM/TestDose/ConstantTwo.dcm -e Rueckenmark";

				CHECK_EQUAL(system(StructCommand.c_str()), 1);

				const std::string ReferenceCommand = PathToBinaryDirectory +
				                                     "/Release/RTTBVoxelizerTool -s " + RTToolBoxTestingDirectory +
				                                     "/DICOM/StructureSet/RS1.3.6.1.4.1.2452.6.841242143.1311652612.1170940299.4217870819.dcm -r " +
				                                     RTToolBoxTestingDirectory + "/DICOM/TestDose/Wrong_Reference_file.dicom -e Rueckenmark";
				CHECK_EQUAL(system(ReferenceCommand.c_str()), 1);

				const std::string Structure = PathToBinaryDirectory +
				                              "/Release/RTTBVoxelizerTool -s " + RTToolBoxTestingDirectory +
				                              "/DICOM/StructureSet/RS1.3.6.1.4.1.2452.6.841242143.1311652612.1170940299.4217870819.dcm -r " +
				                              RTToolBoxTestingDirectory + "/DICOM/TestDose/ConstantTwo.dcm -e blablabla";
				CHECK_EQUAL(system(Structure.c_str()), 0);
			}

			RETURN_AND_REPORT_TEST_SUCCESS;
		}

	}
}
