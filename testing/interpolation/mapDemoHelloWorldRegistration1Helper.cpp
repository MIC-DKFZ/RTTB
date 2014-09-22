// -----------------------------------------------------------------------
// MatchPoint - DKFZ translational registration framework
//
// Copyright (c) German Cancer Research Center (DKFZ),
// Software development for Integrated Diagnostics and Therapy (SIDT).
// ALL RIGHTS RESERVED.
// See mapCopyright.txt or
// http://www.dkfz.de/en/sidt/projects/MatchPoint/copyright.html
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
// Subversion HeadURL: $HeadURL: https://svn/sbr/Sources/SBR-Projects/MatchPoint/trunk/Examples/Algorithms/mapDemoHelloWorldRegistration1Helper.cpp $
*/

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "mapDemoHelloWorldRegistration1Helper.h"

#include "litTestImageIO.h"
#include "litCheckMacros.h"
#include "litImageTester.h"
#include "litPointSetTester.h"

#include <fstream>

int setImageFileNames(std::string targetImage, std::string movingImage, std::string resultImage, bool isDirectory, AppGlobals& globals)
{
	globals.targetImageFileName = targetImage;
	globals.movingImageFileName = movingImage;
	globals.resultImageFileName = resultImage;
	globals.isDirectory = isDirectory;

	return EXIT_SUCCESS;
}

int loadData(AppGlobals& globals)
{
	if (!globals.isDirectory){
		globals.spTargetImage =
			lit::TestImageIO<short, map::core::discrete::Elements<3>::InternalImageType>::readImage(
			globals.targetImageFileName);
	}
	else {
		globals.spTargetImage = map::io::readImage<ImageType::PixelType,ImageType::PixelType,3>(globals.targetImageFileName, map::io::ImageSeriesReadStyle::Dicom);
	}

	if (globals.spTargetImage.IsNull())
	{
		std::cerr << "Error. Cannot load target image: " << globals.targetImageFileName << std::endl;
		return EXIT_FAILURE;
	}

	if (!globals.isDirectory){
		globals.spMovingImage =
			lit::TestImageIO<short, map::core::discrete::Elements<3>::InternalImageType>::readImage(
			globals.movingImageFileName);
	}
	else {
		globals.spMovingImage = map::io::readImage<ImageType::PixelType,ImageType::PixelType,3>(globals.movingImageFileName, map::io::ImageSeriesReadStyle::Dicom);
	}

	if (globals.spMovingImage.IsNull())
	{
		std::cerr << "Error. Cannot load moving image: " << globals.movingImageFileName << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

int saveResults(AppGlobals& globals)
{
	std::cout << std::endl << "Save result data..." << std::endl;

	lit::TestImageIO<short, map::core::discrete::Elements<3>::InternalImageType>::writeImage(
		globals.spResultImage, globals.resultImageFileName);

	return EXIT_SUCCESS;
}

AppGlobals::AppGlobals()
{
};