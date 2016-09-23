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
                                         _dose(dose), _alpha(alpha), _beta(beta), _betaMap(nullptr), _alphaMap(nullptr), _doseScaling(doseScaling), _withAlphaBetaMaps(false)
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

        LQModelAccessor::LQModelAccessor(DoseAccessorPointer dose, DoseAccessorPointer alphaMap, DoseAccessorPointer betaMap,
            double doseScaling) :_dose(dose), _alpha(-1.), _beta(-1.), _betaMap(alphaMap), _alphaMap(betaMap), _doseScaling(doseScaling), _withAlphaBetaMaps(true)
        {
            if (_dose == NULL || _alphaMap == nullptr || _betaMap == nullptr)
            {
                throw core::InvalidDoseException("Dose or alphaMap or betaMap is NULL");
            }

            if (_doseScaling < 0)
            {
                throw core::InvalidParameterException("Dose Scaling must be >0");
            }

            assembleGeometricInfo();
        }

		GenericValueType LQModelAccessor::getValueAt(const VoxelGridID aID) const
		{
            if (_withAlphaBetaMaps){
                return calcLQ(_dose->getValueAt(aID) * _doseScaling, _alphaMap->getValueAt(aID), _betaMap->getValueAt(aID));
            }
            else {
                return calcLQ(_dose->getValueAt(aID) * _doseScaling, _alpha, _beta);
            }
		}

		GenericValueType LQModelAccessor::getValueAt(const VoxelGridIndex3D& aIndex) const
		{
            if (_withAlphaBetaMaps){
                return calcLQ(_dose->getValueAt(aIndex) * _doseScaling, _alphaMap->getValueAt(aIndex), _betaMap->getValueAt(aIndex));
            }
            else {
                return calcLQ(_dose->getValueAt(aIndex) * _doseScaling, _alpha, _beta);
            }
			
		}

		bool LQModelAccessor::assembleGeometricInfo()
		{
			_geoInfo = _dose->getGeometricInfo();

			return true;

		}
	}
}

