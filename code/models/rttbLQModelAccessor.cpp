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
// @version $Revision: 793 $ (last changed revision)
// @date    $Date: 2014-10-10 10:24:45 +0200 (Fr, 10 Okt 2014) $ (last change date)
// @author  $Author: hentsch $ (last changed by)
*/

#include "rttbLQModelAccessor.h"
#include "rttbDoseBasedModels.h"
#include "rttbInvalidDoseException.h"

namespace rttb
{
	namespace models
	{
		LQModelAccessor::~LQModelAccessor()
		{

		}

		LQModelAccessor::LQModelAccessor(DoseAccessorPointer dose, BioModelParamType alpha, BioModelParamType beta) :
			_dose(dose), _alpha(alpha), _beta(beta)
		{
			if (_dose == NULL)
			{
				throw core::InvalidDoseException("Dose is NULL");
			}

			assembleGeometricInfo();
		}

		BioModelValueType LQModelAccessor::getValueAt(const VoxelGridID aID) const
		{
			return calcLQ(_dose->getValueAt(aID), _alpha, _beta);
		}

		BioModelValueType LQModelAccessor::getValueAt(const VoxelGridIndex3D& aIndex) const
		{
			return calcLQ(_dose->getValueAt(aIndex), _alpha, _beta);
		}

		bool LQModelAccessor::assembleGeometricInfo()
		{
			_geoInfo = _dose->getGeometricInfo();

			return true;

		}
	}
}

