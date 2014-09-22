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
// Subversion HeadURL: $HeadURL: https://svn/sbr/Sources/SBR-Projects/MatchPoint/trunk/Examples/Algorithms/mapDemoHelloWorldRegistration1Helper.h $
*/

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#ifndef __MAP_DEMO_HELLO_WORLD_REGISTRATION1_HELPER_H
#define __MAP_DEMO_HELLO_WORLD_REGISTRATION1_HELPER_H

#include "mapContinuousElements.h"
#include "mapDiscreteElements.h"
#include "mapImageReader.h"

typedef map::core::discrete::Elements<3>::InternalImageType ImageType;
typedef map::core::continuous::Elements<3>::InternalPointSetType   LandmarksType;

struct AppGlobals
{
	std::string targetImageFileName;
	std::string movingImageFileName;
	std::string resultImageFileName;

	bool isDirectory;

	ImageType::Pointer spTargetImage;
	ImageType::Pointer spMovingImage;

	ImageType::Pointer spResultImage;

	AppGlobals();
};

int setImageFileNames(std::string targetImage, std::string movingImage, std::string resultImage, bool isDirectory, AppGlobals& globals);

int loadData(AppGlobals& globals);

int saveResults(AppGlobals& globals);

#endif