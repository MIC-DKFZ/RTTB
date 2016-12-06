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

#include <iostream>

#include "litCheckMacros.h"
#include "litImageTester.h"
#include "litTestImageIO.h"

#include "boost/filesystem.hpp"

#include "rttbITKImageAccessorConverter.h"


namespace rttb
{
	namespace testing
	{

		//path to the current running directory. DoseMap is in the same directory (Debug/Release)
		extern const char* _callingAppPath;

		static std::string readFile(const std::string& filename);

		int DoseMapSimpleTest(int argc, char* argv[])
		{
			PREPARE_DEFAULT_TEST_REPORTING;

			std::string doseMapExecutable;
			std::string regFilename;
			std::string inputFilename;
			std::string refDoseLoadStyle;
			std::string inputDoseLoadStyle;
            std::string referenceFilename;

			boost::filesystem::path callingPath(_callingAppPath);


			if (argc > 5 )
			{
				doseMapExecutable = argv[1];
				inputFilename = argv[2];
				inputDoseLoadStyle = argv[3];
				regFilename = argv[4];
                referenceFilename = argv[5];
			}

            std::string defaultOutputFilename = "output.nrrd";

			std::string doseMapExeWithPath = callingPath.parent_path().string() + "/" + doseMapExecutable;
            std::string baseCommand = doseMapExeWithPath + " \"" + inputFilename + "\" " + defaultOutputFilename;

			std::string defaultDoseMapCommand = baseCommand+" -l "+ inputDoseLoadStyle;
			std::cout << "Command line call: " + defaultDoseMapCommand << std::endl;
			CHECK_EQUAL(system(defaultDoseMapCommand.c_str()), 0);

            CHECK_EQUAL(boost::filesystem::exists(defaultOutputFilename), true);

            auto expectedImage =
                lit::TestImageIO<double, rttb::io::itk::ITKImageAccessorConverter::ITKImageType>::readImage(
                referenceFilename);
            auto actualImage =
                lit::TestImageIO<double, rttb::io::itk::ITKImageAccessorConverter::ITKImageType>::readImage(
                defaultOutputFilename);

            ::lit::ImageTester<io::itk::ITKImageAccessorConverter::ITKImageType, io::itk::ITKImageAccessorConverter::ITKImageType >
                tester;
            tester.setExpectedImage(expectedImage);
            tester.setActualImage(actualImage);

            CHECK_TESTER(tester);

            CHECK_EQUAL(std::remove(defaultOutputFilename.c_str()), 0);

			std::string defaultDoseMapCommandWithRegistration = defaultDoseMapCommand+ " -r \"" + regFilename + "\"";
			std::cout << "Command line call: " + defaultDoseMapCommandWithRegistration << std::endl;
			CHECK_EQUAL(system(defaultDoseMapCommandWithRegistration.c_str()), 0);

            CHECK_EQUAL(boost::filesystem::exists(defaultOutputFilename), true);

            actualImage = lit::TestImageIO<double, rttb::io::itk::ITKImageAccessorConverter::ITKImageType>::readImage(
                defaultOutputFilename);
            tester.setActualImage(actualImage);
            CHECK_TESTER(tester);

            CHECK_EQUAL(std::remove(defaultOutputFilename.c_str()), 0);

			RETURN_AND_REPORT_TEST_SUCCESS;
		}
	} //namespace testing
} //namespace rttb
