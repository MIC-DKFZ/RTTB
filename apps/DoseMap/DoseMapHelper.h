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

#ifndef __DOSE_MAP_HELPER_H
#define __DOSE_MAP_HELPER_H

#include "DoseMapApplicationData.h"

namespace rttb
{
	namespace apps
	{
		namespace doseMap
		{
			ApplicationData::RegistrationType::Pointer loadRegistration(const std::string& fileName);

			/**Contains the business logic for the accumulation of the doses and the storing of the result.
			 Uses appData for the input data and the correct configuration.*/
			void processData(ApplicationData& appData);

		}
	}
}


#endif
