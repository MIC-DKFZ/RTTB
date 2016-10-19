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
#include "boost/algorithm/string.hpp"

#include "itkImage.h"

#include "rttbITKIOHelper.h"

namespace rttb
{
    namespace testing
    {

        //path to the current running directory. DoseTool is in the same directory (Debug/Release)
        extern const char* _callingAppPath;

        int BioModelCalcParameterMapsTest(int argc, char* argv[])
        {
            PREPARE_DEFAULT_TEST_REPORTING;

            std::string bioModelCalcExecutable;
            std::string doseFilename;
            std::string doseFilenameLoadStyle;
            std::string modelName;
            std::string modelParameterMaps;
            std::string modelParameterMapsLoadStyle;
            std::string referenceFilename;

            boost::filesystem::path callingPath(_callingAppPath);


            if (argc > 7)
            {
                bioModelCalcExecutable = argv[1];
                doseFilename = argv[2];
                doseFilenameLoadStyle = argv[3];
                modelName = argv[4];
                modelParameterMaps = argv[5];
                modelParameterMapsLoadStyle = argv[6];
                referenceFilename = argv[7];
            }

            std::string bioModelCalcExeWithPath = callingPath.parent_path().string() + "/" + bioModelCalcExecutable;

            std::string defaultOutputFilename = "bioModelParameterMapOutput.nrrd";

            std::string baseCommand = bioModelCalcExeWithPath;
            baseCommand += " -d " + doseFilename;
            baseCommand += " -t " + doseFilenameLoadStyle;
            baseCommand += " -o " + defaultOutputFilename;
            baseCommand += " -m " + modelName;
            baseCommand += " -a " + modelParameterMaps;
            baseCommand += " -u " + modelParameterMapsLoadStyle;

            std::string defaultDoseStatisticsCommand = baseCommand;
            std::cout << "Command line call: " + defaultDoseStatisticsCommand << std::endl;
            CHECK_EQUAL(system(defaultDoseStatisticsCommand.c_str()), 0);

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

            RETURN_AND_REPORT_TEST_SUCCESS;
        }
    } //namespace testing
} //namespace rttb
