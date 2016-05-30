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

#ifndef __DOSETOOL_HELPER_H
#define __DOSETOOL_HELPER_H

#include "DoseToolApplicationData.h"
#include "rttbDoseAccessorInterface.h"
#include "rttbDoseIteratorInterface.h"
#include "rttbMaskAccessorInterface.h"
#include "rttbDoseStatistics.h"
#include "rttbDVH.h"

namespace rttb
{
	namespace apps
	{
		namespace doseTool
		{
			/*! @brief loads a dose from a file based on the loadingStyle.
				@exception Throws an rttb::Exception if loading fails
			*/
			core::DoseAccessorInterface::DoseAccessorPointer loadDose(const std::string& fileName,
			        const rttb::apps::doseTool::ApplicationData::LoadingStyleArgType& args);
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
          const rttb::apps::doseTool::ApplicationData::LoadingStyleArgType& args, const std::string& structNameRegex = "");

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

			/*! @brief Contains the business logic of processing all information to calculate the dose statistics and writing them to an xml file.
			@details Uses appData for the input data and the correct configuration.
			*/
			void processData(ApplicationData& appData);

			/*! @brief Generates a mask from the struct file by using the boostAccessor. In case of itk image, it directly loads the voxelized image.
			*/
			std::vector<core::MaskAccessorInterface::MaskAccessorPointer> generateMasks(
			    ApplicationData& appData);

			algorithms::DoseStatistics::DoseStatisticsPointer calculateDoseStatistics(
			    core::DoseIteratorInterface::DoseIteratorPointer
			    doseIterator, bool calculateComplexDoseStatistics, DoseTypeGy prescribedDose);

			core::DVH::DVHPointer calculateDVH(core::DoseIteratorInterface::DoseIteratorPointer doseIterator,
			                                   IDType structUID,
			                                   IDType doseUID);

			/*! @brief Writes the dose statistics as XML to a file
				@details adds a <config>....</config> part to the RTTB generated xml where the used files and struct names are stored.
			*/
			void writeDoseStatisticsFile(algorithms::DoseStatistics::DoseStatisticsPointer statistics,
			                             const std::string& filename,
			                             const std::string& structName,
			                             rttb::apps::doseTool::ApplicationData& appData);

			void writeDVHFile(core::DVH::DVHPointer dvh, const std::string& filename);

			core::DoseIteratorInterface::DoseIteratorPointer generateMaskedDoseIterator(
			    core::MaskAccessorInterface::MaskAccessorPointer
			    maskAccessorPtr, core::DoseAccessorInterface::DoseAccessorPointer doseAccessorPtr);

			std::string assembleFilenameWithStruct(const std::string& originalFilename,
			                                       const std::string& structName);
		}
	}
}


#endif
