// -----------------------------------------------------------------------
// RTToolbox - DKFZ radiotherapy quantitative evaluation library
//
// Copyright (c) German Cancer Research Center (DKFZ),
// Software development for Integrated Diagnostics and Therapy (SIDT).
// ALL RIGHTS RESERVED.
// See rttbCopyright.txt or
// http://www.dkfz.de/en/sidt/projects/rttb/copyright.html [^]
//
// This software is distributed WITHOUT ANY WARRANTY; without even
// the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
// PURPOSE. See the above copyright notices for more information.
//
//------------------------------------------------------------------------
/*!
// @file
// @version $Revision$ (last changed revision)
// @date    $Date$ (last change date)
// @author  $Author$ (last changed by)
*/

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "registrationHelper.h"

#include "litTestImageIO.h"
#include "litCheckMacros.h"
#include "litImageTester.h"
#include "litPointSetTester.h"

#include <fstream>

int setImageFileNames(std::string targetImage, std::string movingImage, bool isDirectory,
                      AppGlobals& globals)
{
	globals.targetImageFileName = targetImage;
	globals.movingImageFileName = movingImage;
	globals.isDirectory = isDirectory;

	return EXIT_SUCCESS;
}

int loadData(AppGlobals& globals)
{
	if (!globals.isDirectory)
	{
		globals.spTargetImage =
		    lit::TestImageIO<short, map::core::discrete::Elements<3>::InternalImageType>::readImage(
		        globals.targetImageFileName);
	}
	else
	{
		globals.spTargetImage = map::io::readImage<ImageType::PixelType, ImageType::PixelType, 3>
		                        (globals.targetImageFileName, map::io::ImageSeriesReadStyle::Dicom);
	}

	if (globals.spTargetImage.IsNull())
	{
		std::cerr << "Error. Cannot load target image: " << globals.targetImageFileName << std::endl;
		return EXIT_FAILURE;
	}

	if (!globals.isDirectory)
	{
		globals.spMovingImage =
		    lit::TestImageIO<short, map::core::discrete::Elements<3>::InternalImageType>::readImage(
		        globals.movingImageFileName);
	}
	else
	{
		globals.spMovingImage = map::io::readImage<ImageType::PixelType, ImageType::PixelType, 3>
		                        (globals.movingImageFileName, map::io::ImageSeriesReadStyle::Dicom);
	}

	if (globals.spMovingImage.IsNull())
	{
		std::cerr << "Error. Cannot load moving image: " << globals.movingImageFileName << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

AppGlobals::AppGlobals()
{
};