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
// @version $Revision: 1315 $ (last changed revision)
// @date    $Date: 2016-04-12 14:18:24 +0200 (Di, 12 Apr 2016) $ (last change date)
// @author  $Author: floca $ (last changed by)
*/

#ifndef __DOSETOOL4V_HELPER_H
#define __DOSETOOL4V_HELPER_H

#include "rttbDoseAccessorInterface.h"
#include "rttbStructureSetGeneratorInterface.h"

namespace rttb
{
	namespace apps
	{
		namespace doseTool4V
		{
			typedef std::vector<std::string> LoadingStyleArgType;
			/*! @brief loads a dose from a file based on the loadingStyle.
				@exception Throws an rttb::Exception if loading fails
			*/
			core::DoseAccessorInterface::DoseAccessorPointer loadDose(const std::string& fileName,
				const LoadingStyleArgType& args);

			/*! @brief loads a dicom dose from a file.
			@exception Throws an rttb::Exception if loading fails
			@sa DicomFileDoseAccessorGenerator
			*/
			core::DoseAccessorInterface::DoseAccessorPointer loadDicomDose(const std::string& fileName);
			/*! @brief loads a helax dose from a file.
			@exception Throws an rttb::Exception if loading fails
			@sa DicomHelaxFileDoseAccessorGenerator
			*/
			core::DoseAccessorInterface::DoseAccessorPointer loadHelaxDose(const std::string& path);
			/*! @brief loads an itk dose from a file.
			@exception Throws an rttb::Exception if loading fails.
			@details Might be of all formats that ITK know (*.mhd, *.nrrd, ...). The absolute image values are taken as dose.
			@sa ITKImageFileAccessorGenerator
			*/
			core::DoseAccessorInterface::DoseAccessorPointer loadITKDose(const std::string& fileName);

			/*! @brief loads a virtuos dose from a file.
			@exception Throws an rttb::Exception if loading fails
			@sa VirtuosPlanFileDoseAccessorGenerator
			*/
			core::DoseAccessorInterface::DoseAccessorPointer loadVirtuosDose(const std::string& fileName,
			        const std::string& planFileName);

			/*! @brief loads a struct from a file based on the loadingStyle.
			You may pass a structure name regex. If is not empty, it will be used to filter structure in the
			loading process. Only structures with a name matching the reg ex will be loaded. This speeds up the
			loading process significantly if you need only one structure out of a structure set.
			@exception Throws an rttb::Exception if loading fails
			@details voxelized itk images are read in generateMask() directly
			*/
			core::StructureSetGeneratorInterface::StructureSetPointer loadStruct(const std::string& fileName,
			        const LoadingStyleArgType& args, const std::string& structNameRegex = "");

			/*! @brief loads a dicom struct from a file.
			You may pass a structure name regex. If is not empty, it will be used to filter structure in the
			loading process. Only structures with a name matching the reg ex will be loaded. This speeds up the
			loading process significantly if you need only one structure out of a structure set.
			@exception Throws an rttb::Exception if loading fails
			@sa DicomFileStructureSetGenerator
			*/
			core::StructureSetGeneratorInterface::StructureSetPointer loadDicomStruct(
				const std::string& fileName, const std::string& structNameRegex = "");

			/*! @brief loads a virtuos struct from a file.
			@exception Throws an rttb::Exception if loading fails
			@sa VirtuosPlanFileDoseAccessorGenerator
			*/
			core::StructureSetGeneratorInterface::StructureSetPointer loadVirtuosStruct(
			    const std::string& fileName,
			    const std::string& ctxFileName);
		}
	}
}


#endif
