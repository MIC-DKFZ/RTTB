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

#include "rttbBinaryFunctorAccessor.h"

#ifndef __BINARY_FUNCTOR_ACCESSOR_TPP
#define __BINARY_FUNCTOR_ACCESSOR_TPP

namespace rttb
{
	namespace algorithms
	{
		template <class TDoseOperation>
		BinaryFunctorAccessor<TDoseOperation>::BinaryFunctorAccessor(const AccessorPointer
		        data1, const AccessorPointer data2,
		        const TDoseOperation& functor)
		{
			if (data1 == nullptr || data2 == nullptr)
			{
				throw core::NullPointerException("Pointers to input accessors cannot be nullptr.");
			}

			if (!(data1->getGeometricInfo() == data2->getGeometricInfo()))
			{
				throw core::InvalidParameterException("The geometricInfo of all given accessors needs to be equal.");
			}

			_spData1 = data1;
			_spData2 = data2;
			_functor = functor;
		}

		template <class TDoseOperation> GenericValueType BinaryFunctorAccessor<TDoseOperation>::getValueAt(
		    const VoxelGridID aID) const
		{
			if (getGeometricInfo().validID(aID))
			{
				GenericValueType value = _functor.calc(_spData1->getValueAt(aID), _spData2->getValueAt(aID));
				return value;
			}
			else
			{
				return -1;
			}
		}

		template <class TDoseOperation> DoseTypeGy BinaryFunctorAccessor<TDoseOperation>::getValueAt(
		    const VoxelGridIndex3D& aIndex) const
		{
			VoxelGridID aVoxelGridID;

			if (_spData1->getGeometricInfo().convert(aIndex, aVoxelGridID))
			{
				return getValueAt(aVoxelGridID);
			}
			else
			{
				return -1;
			}
		}

	}
}
#endif