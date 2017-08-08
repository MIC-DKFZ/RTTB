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
// @version $Revision: 1374 $ (last changed revision)
// @date    $Date: 2016-05-30 14:15:42 +0200 (Mo, 30 Mai 2016) $ (last change date)
// @author  $Author: hentsch $ (last changed by)
*/

#ifndef __DOSETOOL_HELPER_H
#define __DOSETOOL_HELPER_H


#include "rttbDoseAccessorInterface.h"
#include "rttbStructureSetGeneratorInterface.h"
#include "rttbDoseIteratorInterface.h"
#include "rttbMaskAccessorInterface.h"

namespace rttb
{
	namespace apps
	{
		namespace doseTool
		{
            class ApplicationData;
            typedef std::vector<std::string> LoadingStyleArgType;
		
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

			/*! @brief Contains the business logic of processing all information to calculate the dose statistics and writing them to an xml file.
			@details Uses appData for the input data and the correct configuration.
			*/
			void processData(ApplicationData& appData);

			/*! @brief Generates a mask from the struct file by using the boostAccessor. In case of itk image, it directly loads the voxelized image.
			*/
			std::vector<core::MaskAccessorInterface::MaskAccessorPointer> generateMasks(
			    ApplicationData& appData);

			/*algorithms::DoseStatistics::DoseStatisticsPointer calculateDoseStatistics(
			    core::DoseIteratorInterface::DoseIteratorPointer
			    doseIterator, bool calculateComplexDoseStatistics, DoseTypeGy prescribedDose);

			core::DVH::DVHPointer calculateDVH(core::DoseIteratorInterface::DoseIteratorPointer doseIterator,
			                                   IDType structUID,
			                                   IDType doseUID);*/

			core::DoseIteratorInterface::DoseIteratorPointer generateMaskedDoseIterator(
			    core::MaskAccessorInterface::MaskAccessorPointer
			    maskAccessorPtr, core::DoseAccessorInterface::DoseAccessorPointer doseAccessorPtr);

			std::string assembleFilenameWithStruct(const std::string& originalFilename,
			                                       const std::string& structName);
		}
	}
}


#endif
