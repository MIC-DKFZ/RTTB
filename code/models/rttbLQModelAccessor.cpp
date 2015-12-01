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
// @version $Revision$ (last changed revision)
// @date    $Date$ (last change date)
// @author  $Author$ (last changed by)
*/

#include "rttbLQModelAccessor.h"
#include "rttbDoseBasedModels.h"
#include "rttbInvalidDoseException.h"
#include "rttbInvalidParameterException.h"

namespace rttb
{
	namespace models
	{
		LQModelAccessor::~LQModelAccessor()
		{

		}

		LQModelAccessor::LQModelAccessor(DoseAccessorPointer dose, BioModelParamType alpha,
		                                 BioModelParamType beta,
		                                 double doseScaling) :
			_dose(dose), _alpha(alpha), _beta(beta), _doseScaling(doseScaling)
		{
			if (_dose == NULL)
			{
				throw core::InvalidDoseException("Dose is NULL");
			}

			if (_doseScaling < 0)
			{
				throw core::InvalidParameterException("Dose Scaling must be >0");
			}

			assembleGeometricInfo();
		}

		GenericValueType LQModelAccessor::getValueAt(const VoxelGridID aID) const
		{
			return calcLQ(_dose->getValueAt(aID) * _doseScaling, _alpha, _beta);
		}

		GenericValueType LQModelAccessor::getValueAt(const VoxelGridIndex3D& aIndex) const
		{
			return calcLQ(_dose->getValueAt(aIndex) * _doseScaling, _alpha, _beta);
		}

		bool LQModelAccessor::assembleGeometricInfo()
		{
			_geoInfo = _dose->getGeometricInfo();

			return true;

		}
	}
}

