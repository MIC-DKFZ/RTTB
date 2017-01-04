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

#ifndef __DOSE_ACC_HELPER_H
#define __DOSE_ACC_HELPER_H

#include <vector>
#include <string>

#include "rttbDoseAccessorInterface.h"
#include "DoseAccApplicationData.h"

namespace rttb
{
	namespace apps
	{
		namespace doseAcc
		{
            typedef std::vector<std::string> LoadingStyleArgType;
			/**loads the dose from a file. Throws exception if loading fails*/
			core::DoseAccessorInterface::DoseAccessorPointer loadDose(const std::string& fileName,
			        const LoadingStyleArgType& args);

			/**loads the dose from a file using the dicom dose generator. Throws exception if loading fails*/
			core::DoseAccessorInterface::DoseAccessorPointer loadDicomDose(const std::string& fileName);
			/**loads the dose from a path using the helax io dose generator. Throws exception if loading fails*/
			core::DoseAccessorInterface::DoseAccessorPointer loadHelaxDose(const std::string& path);
			/**loads the dose from a file stored in an ITK supported data format. Throws exception if loading fails*/
			core::DoseAccessorInterface::DoseAccessorPointer loadITKDose(const std::string& fileName);

			ApplicationData::RegistrationType::Pointer loadRegistration(const std::string& fileName);

			/**Contains the business logic for the accumulation of the doses and the storing of the result.
			 Uses appData for the input data and the correct configuration.*/
			void processData(ApplicationData& appData);

		}
	}
}


#endif
