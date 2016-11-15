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
// @version $Revision: 1221 $ (last changed revision)
// @date    $Date: 2015-12-01 13:43:31 +0100 (Di, 01 Dez 2015) $ (last change date)
// @author  $Author: hentsch $ (last changed by)
*/

#include "litCheckMacros.h"
#include "litImageTester.h"

#include "boost/filesystem.hpp"

#include "itkImage.h"

#include "rttbITKIOHelper.h"

namespace rttb
{
	namespace testing
	{

		//path to the current running directory. DoseTool is in the same directory (Debug/Release)
		extern const char* _callingAppPath;

		int DoseAccNeutralWeightTest(int argc, char* argv[])
		{
			PREPARE_DEFAULT_TEST_REPORTING;

			std::string doseAccExecutable;
			std::string doseFilename;
            std::string referenceFilename;
            std::string reference2Filename;

			boost::filesystem::path callingPath(_callingAppPath);


			if (argc > 4)
			{
                doseAccExecutable = argv[1];
                doseFilename = argv[2];
                referenceFilename = argv[3];
                reference2Filename = argv[4];
			}

            std::string doseAccExeWithPath = callingPath.parent_path().string() + "/" + doseAccExecutable;

			const std::string defaultOutputAddFilename = "doseAccAddOutput.nrrd";
            const std::string defaultOutputMultFilename = "doseAccMultOutput.nrrd";

            std::string baseCommand = doseAccExeWithPath;
            baseCommand += " -d " + doseFilename;
            baseCommand += " -e " + doseFilename;

            std::string defaultDoseAccAddCommand = baseCommand + " -o " + defaultOutputAddFilename + " --weight1 0.0";
            std::cout << "Command line call: " + defaultDoseAccAddCommand << std::endl;
            CHECK_EQUAL(system(defaultDoseAccAddCommand.c_str()), 0);

            CHECK_EQUAL(boost::filesystem::exists(defaultOutputAddFilename), true);

            std::string defaultDoseAccMultCommand = baseCommand + " -o " + defaultOutputMultFilename + " --operator \"*\"";
            std::cout << "Command line call: " + defaultDoseAccMultCommand << std::endl;
            CHECK_EQUAL(system(defaultDoseAccMultCommand.c_str()), 0);

            CHECK_EQUAL(boost::filesystem::exists(defaultOutputMultFilename), true);

            // Check result against reference
            typedef ::itk::Image<double, 3> TestImageType;

            TestImageType::Pointer referenceAddImage = io::itk::readITKDoubleImage(referenceFilename);
            TestImageType::Pointer outputAddImage = io::itk::readITKDoubleImage(defaultOutputAddFilename);

            lit::ImageTester<TestImageType, TestImageType> tester;
            tester.setExpectedImage(referenceAddImage);
            tester.setActualImage(outputAddImage);
            tester.setCheckThreshold(1e-5);

            CHECK_TESTER(tester);

            TestImageType::Pointer referenceMultImage = io::itk::readITKDoubleImage(reference2Filename);
            TestImageType::Pointer outputMultImage = io::itk::readITKDoubleImage(defaultOutputMultFilename);

            tester.setExpectedImage(referenceMultImage);
            tester.setActualImage(outputMultImage);
            tester.setCheckThreshold(1e-5);

            CHECK_TESTER(tester);

            CHECK_EQUAL(std::remove(defaultOutputAddFilename.c_str()), 0);
            CHECK_EQUAL(std::remove(defaultOutputMultFilename.c_str()), 0);

			RETURN_AND_REPORT_TEST_SUCCESS;
		}
	} //namespace testing
} //namespace rttb
