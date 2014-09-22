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

#include "rttbBinaryFunctorDoseAccessor.h"

#ifndef __BINARY_FUNCTOR_DOSE_ACCESSOR_TPP
#define __BINARY_FUNCTOR_DOSE_ACCESSOR_TPP

namespace rttb
{
	namespace algorithms
	{
		template <class TDoseOperation>
		BinaryFunctorDoseAccessor<TDoseOperation>::BinaryFunctorDoseAccessor(const DoseAccessorPointer
		        dose1, const DoseAccessorPointer dose2,
		        const TDoseOperation& functor)
		{
			if (dose1 == NULL || dose2 == NULL)
			{
				throw core::NullPointerException("Pointers to input accessors cannot be NULL.");
			}

			if (!(dose1->getGeometricInfo() == dose2->getGeometricInfo()))
			{
				throw core::InvalidParameterException("The geometricInfo of all given accessors needs to be equal.");
			}

			_spDose1 = dose1;
			_spDose2 = dose2;
			_functor = functor;
		}

		template <class TDoseOperation> DoseTypeGy BinaryFunctorDoseAccessor<TDoseOperation>::getDoseAt(
		    const VoxelGridID aID) const
		{
			if (getGeometricInfo().validID(aID))
			{
				DoseTypeGy value = _functor.calc(_spDose1->getDoseAt(aID), _spDose2->getDoseAt(aID));
				return value;
			}
			else
			{
				return -1;
			}
		}

		template <class TDoseOperation> DoseTypeGy BinaryFunctorDoseAccessor<TDoseOperation>::getDoseAt(
		    const VoxelGridIndex3D& aIndex) const
		{
			VoxelGridID aVoxelGridID;

			if (_spDose1->getGeometricInfo().convert(aIndex, aVoxelGridID))
			{
				return getDoseAt(aVoxelGridID);
			}
			else
			{
				return -1;
			}
		}

	}
}
#endif