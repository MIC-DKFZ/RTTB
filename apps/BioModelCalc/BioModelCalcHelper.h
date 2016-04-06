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
// @version $Revision: 1210 $ (last changed revision)
// @date    $Date: 2015-11-24 15:52:45 +0100 (Di, 24 Nov 2015) $ (last change date)
// @author  $Author: hentsch $ (last changed by)
*/

#ifndef __BIO_MODEL_CALC_HELPER_H
#define __BIO_MODEL_CALC_HELPER_H

#include "BioModelCalcApplicationData.h"
#include "rttbAccessorInterface.h"

namespace rttb
{
	namespace apps
	{
		namespace bioModelCalc
		{
			/*! @brief loads a dose from a file based on the loadingStyle.
				@details Throws an rttb::Exception if loading fails
			*/
			core::DoseAccessorInterface::DoseAccessorPointer loadDose(const std::string& fileName,
			        const rttb::apps::bioModelCalc::ApplicationData::LoadingStyleArgType& args);
			/*! @brief loads a dicom dose from a file.
				@details Throws an rttb::Exception if loading fails
				@sa DicomFileDoseAccessorGenerator
			*/
			core::DoseAccessorInterface::DoseAccessorPointer loadDicomDose(const std::string& fileName);
			/*! @brief loads a helax dose from a file.
				@details Throws an rttb::Exception if loading fails
				@sa DicomHelaxFileDoseAccessorGenerator
			*/
			core::DoseAccessorInterface::DoseAccessorPointer loadHelaxDose(const std::string& path);
			/*! @brief loads an itk dose from a file.
				@details Throws an rttb::Exception if loading fails. Might be of all formats that ITK know (*.mhd, *.nrrd, ...). The absolute image values are taken as dose.
				@sa ITKImageFileAccessorGenerator
			*/
			core::DoseAccessorInterface::DoseAccessorPointer loadITKDose(const std::string& fileName);
			/*! @brief loads a virtuos dose from a file.
			@details Throws an rttb::Exception if loading fails
			@sa VirtuosPlanFileDoseAccessorGenerator
			*/
			core::DoseAccessorInterface::DoseAccessorPointer loadVirtuosDose(const std::string& fileName,
			        const std::string& planFileName);

			/*! @brief Contains the business logic of processing all information to calculate a bioModel from the dose and writing it back to an image.
			@details Uses appData for the input data and the correct configuration.
			*/
			void processData(ApplicationData& appData);

			core::AccessorInterface::AccessorPointer generateBioModel(
			    core::DoseAccessorInterface::DoseAccessorPointer dose, const std::string& model,
			    const std::vector<double>& modelParameters, double doseScaling = 1.0);

		}
	}
}


#endif
