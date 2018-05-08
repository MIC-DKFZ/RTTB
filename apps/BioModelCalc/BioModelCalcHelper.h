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

#include <deque>

#include "rttbAccessorInterface.h"
#include "rttbDoseAccessorInterface.h"

namespace rttb
{
	namespace apps
	{
		namespace bioModelCalc
		{
      class ApplicationData;
            
			void processData(ApplicationData& appData);

			core::AccessorInterface::Pointer generateBioModel(
			    core::DoseAccessorInterface::Pointer dose, const std::string& model,
                const std::vector<double>& modelParameters, unsigned int nFractions=1, double doseScaling = 1.0);

            rttb::core::AccessorInterface::Pointer generateBioModelWithMaps(
                rttb::core::DoseAccessorInterface::Pointer dose, const std::string& model,
                const std::deque<rttb::core::AccessorInterface::Pointer>& modelParameterMaps, unsigned int nFractions = 1, double doseScaling = 1.0);

		}
	}
}


#endif
