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
// @version $Revision $ (last changed revision)
// @date $Date $ (last change date)
// @author $Author $ (last changed by)
*/
#include "rttbDoseBasedModels.h"
#include "rttbInvalidParameterException.h"

#include <math.h>

namespace rttb
{
	namespace models
	{
		rttb::models::BioModelValueType calcLQ(DoseTypeGy dose, DoseCalcType alpha,
            DoseCalcType beta, unsigned int nFractions)
		{
			if (dose < 0 || alpha < 0 || beta < 0)
			{
				throw core::InvalidParameterException("Parameter invalid: dose, alpha, beta must be >=0!");
			}
            return exp(-((alpha * dose) + (beta * dose * dose / DoseCalcType(nFractions))));
		}

	}
}