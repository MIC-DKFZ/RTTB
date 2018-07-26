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

		int BioModelCalcSimpleTest(int argc, char* argv[])
		{
			PREPARE_DEFAULT_TEST_REPORTING;

			std::string bioModelCalcExecutable;
			std::string doseFilename;
            std::string doseFilenameLoadStyle;
            std::string modelName;
			std::string modelArguments;
			std::string referenceFilename;
            std::string referenceWithNFractionsFilename;

			boost::filesystem::path callingPath(_callingAppPath);


			if (argc > 7)
			{
                bioModelCalcExecutable = argv[1];
				doseFilename = argv[2];
                doseFilenameLoadStyle = argv[3];
                modelName = argv[4];
                modelArguments = argv[5];
                referenceFilename = argv[6];
                referenceWithNFractionsFilename = argv[7];
			}

            std::string bioModelCalcExeWithPath = callingPath.parent_path().string() + "/" + bioModelCalcExecutable;

			std::string defaultOutputFilename = "bioModelOutput.nrrd";

			std::string baseCommand = bioModelCalcExeWithPath;
			baseCommand += " -d \"" + doseFilename + "\"";
            baseCommand += " -t " + doseFilenameLoadStyle;
            baseCommand += " -o " + defaultOutputFilename;
            baseCommand += " -m " + modelName;
            baseCommand += " -p " + modelArguments;

			std::string defaultDoseStatisticsCommand = baseCommand;
			std::cout << "Command line call: " + defaultDoseStatisticsCommand << std::endl;
			CHECK_EQUAL(system(defaultDoseStatisticsCommand.c_str()), 0);

            CHECK_EQUAL(boost::filesystem::exists(defaultOutputFilename), true);

            std::string defaultOutputWithNFractionsFilename = "bioModelWithNFractionsOutput.nrrd";
            std::string baseCommandWithNFractions = bioModelCalcExeWithPath;
            baseCommandWithNFractions += " -d \"" + doseFilename + "\"";
            baseCommandWithNFractions += " -t " + doseFilenameLoadStyle;
            baseCommandWithNFractions += " -o " + defaultOutputWithNFractionsFilename;
            baseCommandWithNFractions += " -m " + modelName;
            baseCommandWithNFractions += " -p " + modelArguments;
            baseCommandWithNFractions += " -f 2";

            std::cout << "Command line call: " + baseCommandWithNFractions << std::endl;
            CHECK_EQUAL(system(baseCommandWithNFractions.c_str()), 0);

            CHECK_EQUAL(boost::filesystem::exists(defaultOutputFilename), true);

            // Check result against reference
            typedef ::itk::Image<double, 3> TestImageType;

            TestImageType::Pointer referenceImage = io::itk::readITKDoubleImage(referenceFilename);
            TestImageType::Pointer outputImage = io::itk::readITKDoubleImage(defaultOutputFilename);

            lit::ImageTester<TestImageType, TestImageType> tester;
            tester.setExpectedImage(referenceImage);
            tester.setActualImage(outputImage);
            tester.setCheckThreshold(0.0);

            CHECK_TESTER(tester);

            CHECK_EQUAL(std::remove(defaultOutputFilename.c_str()), 0);

            CHECK_EQUAL(boost::filesystem::exists(defaultOutputWithNFractionsFilename), true);

            // Check result against reference

            TestImageType::Pointer referenceImageNFractions = io::itk::readITKDoubleImage(referenceWithNFractionsFilename);
            TestImageType::Pointer outputImageNFractions = io::itk::readITKDoubleImage(defaultOutputWithNFractionsFilename);

            lit::ImageTester<TestImageType, TestImageType> testerWithNFractions;
            testerWithNFractions.setExpectedImage(referenceImageNFractions);
            testerWithNFractions.setActualImage(outputImageNFractions);
            testerWithNFractions.setCheckThreshold(0.0);

            CHECK_TESTER(testerWithNFractions);

            CHECK_EQUAL(std::remove(defaultOutputWithNFractionsFilename.c_str()), 0);

			RETURN_AND_REPORT_TEST_SUCCESS;
		}
	} //namespace testing
} //namespace rttb
