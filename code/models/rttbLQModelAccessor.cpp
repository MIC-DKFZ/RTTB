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

#include "rttbLQModelAccessor.h"
#include "rttbDoseBasedModels.h"
#include "rttbInvalidDoseException.h"
#include "rttbInvalidParameterException.h"
#include "rttbMappingOutsideOfImageException.h"

namespace rttb
{
	namespace models
	{
		LQModelAccessor::~LQModelAccessor()
		= default;

		LQModelAccessor::LQModelAccessor(DoseAccessorPointer dose, BioModelParamType alpha,
            BioModelParamType beta, unsigned int nFractions,
		                                 double doseScaling) :
                                         _dose(dose), _alpha(alpha), _beta(beta), _nFractions(nFractions), _alphaMap(nullptr), _betaMap(nullptr), _doseScaling(doseScaling), _withAlphaBetaMaps(false)
		{
			if (_dose == nullptr)
			{
				throw core::InvalidDoseException("Dose is nullptr");
			}

			if (_doseScaling < 0)
			{
				throw core::InvalidParameterException("Dose Scaling must be >0");
			}

			assembleGeometricInfo();
		}

        LQModelAccessor::LQModelAccessor(DoseAccessorPointer dose, DoseAccessorPointer alphaMap, DoseAccessorPointer betaMap, unsigned int nFractions,
            double doseScaling) :_dose(dose), _alpha(-1.), _beta(-1.), _nFractions(nFractions), _alphaMap(betaMap), _betaMap(alphaMap), _doseScaling(doseScaling), _withAlphaBetaMaps(true)
        {
            if (_dose == nullptr || _alphaMap == nullptr || _betaMap == nullptr)
            {
                throw core::InvalidDoseException("Dose or alphaMap or betaMap is nullptr");
            }

            if (_doseScaling < 0)
            {
                throw core::InvalidParameterException("Dose Scaling must be >0");
            }

            assembleGeometricInfo();
        }

		GenericValueType LQModelAccessor::getValueAt(const VoxelGridID aID) const
		{
            VoxelGridIndex3D aVoxelGridIndex3D;

            if (_geoInfo.convert(aID, aVoxelGridIndex3D))
            {
                return getValueAt(aVoxelGridIndex3D);
            }
            else 
            {
                throw core::MappingOutsideOfImageException("Error in conversion from index to world coordinates");
            }
		}

		GenericValueType LQModelAccessor::getValueAt(const VoxelGridIndex3D& aIndex) const
		{
            if (_withAlphaBetaMaps){
                return calcLQ(_dose->getValueAt(aIndex) * _doseScaling, _alphaMap->getValueAt(aIndex), _betaMap->getValueAt(aIndex), _nFractions);
            }
            else {
                return calcLQ(_dose->getValueAt(aIndex) * _doseScaling, _alpha, _beta, _nFractions);
            }
			
		}

		void LQModelAccessor::assembleGeometricInfo()
		{
			_geoInfo = _dose->getGeometricInfo();
		}
	}
}

