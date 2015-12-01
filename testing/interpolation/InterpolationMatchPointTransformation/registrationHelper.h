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

#ifndef __REGISTRATION_HELPER_H
#define __REGISTRATION_HELPER_H

#include "mapContinuousElements.h"
#include "mapDiscreteElements.h"
#include "mapImageReader.h"

typedef map::core::discrete::Elements<3>::InternalImageType ImageType;
typedef map::core::continuous::Elements<3>::InternalPointSetType   LandmarksType;

struct AppGlobals
{
	std::string targetImageFileName;
	std::string movingImageFileName;

	bool isDirectory;

	ImageType::Pointer spTargetImage;
	ImageType::Pointer spMovingImage;

	ImageType::Pointer spResultImage;

	AppGlobals();
};

int setImageFileNames(std::string targetImage, std::string movingImage, bool isDirectory,
                      AppGlobals& globals);

int loadData(AppGlobals& globals);

#endif