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

#include "rttbNullPointerException.h"
#include "rttbInvalidParameterException.h"

#ifndef __ARITHMETIC_TPP
#define __ARITHMETIC_TPP

namespace rttb
{
	namespace algorithms
	{
		namespace arithmetic
		{

			template <class TDoseOperation>
			void arithmetic(const DoseAccessorPointer dose1, const DoseAccessorPointer dose2,
			                MutableDoseAccessorPointer result, TDoseOperation op)
			{
				//handle null pointers
				if (dose1 == nullptr || dose2 == nullptr || result == nullptr)
				{
					throw core::NullPointerException("Pointers to input accessors cannot be nullptr.");
				}

				//handle differences in geometricInfo
				if (!(dose1->getGeometricInfo().equalsAlmost(dose2->getGeometricInfo())
				      && dose1->getGeometricInfo().equalsAlmost(result->getGeometricInfo())))
				{
					throw core::InvalidParameterException("The geometricInfo of all given accessors needs to be equal.");
				}

				//apply operation op to doses with equal geometricInfo (same grid)
				GridSizeType numVoxels = dose1->getGridSize();

				for (VoxelGridID id = 0; id < numVoxels; ++id)
				{
					DoseTypeGy opVal = op.calc(dose1->getValueAt(id), dose2->getValueAt(id));
					result->setDoseAt(id, opVal);
				}
			}

			template <class TDoseMaskOperation>
			void arithmetic(const DoseAccessorPointer dose, const MaskAccessorPointer mask,
			                MutableDoseAccessorPointer result, TDoseMaskOperation op)
			{
				//handle null pointers
				if (dose == nullptr || mask == nullptr || result == nullptr)
				{
					throw core::NullPointerException("Pointers to input accessors cannot be nullptr.");
				}

				//handle differences in geometricInfo
				if (!(dose->getGeometricInfo().equalsAlmost(mask->getGeometricInfo())
				      && dose->getGeometricInfo().equalsAlmost(result->getGeometricInfo())))
				{
					throw core::InvalidParameterException("The geometricInfo of all given accessors needs to be equal.");
				}

				//apply operation op to accessors with equal geometricInfo (same grid)
				core::MaskVoxel mVoxel(0);
				GridSizeType numVoxels = dose->getGridSize();

				for (VoxelGridID id = 0; id < numVoxels; ++id)
				{
					mask->getMaskAt(id, mVoxel);
					DoseTypeGy opVal = op.calc(dose->getValueAt(id), mVoxel.getRelevantVolumeFraction());
					result->setDoseAt(id, opVal);
				}
			}

			template <class TMaskOperation>
			void arithmetic(const MaskAccessorPointer mask1, const MaskAccessorPointer mask2,
			                MutableMaskAccessorPointer result, TMaskOperation op)
			{
				//handle null pointers
				if (mask1 == nullptr || mask2 == nullptr || result == nullptr)
				{
					throw core::NullPointerException("Pointers to input accessors cannot be nullptr.");
				}

				//handle differences in geometricInfo
				if (!(mask1->getGeometricInfo().equalsAlmost(mask2->getGeometricInfo())
				      && mask1->getGeometricInfo().equalsAlmost(result->getGeometricInfo())))
				{
					throw core::InvalidParameterException("The geometricInfo of all given accessors needs to be equal.");
				}

				//apply operation op to accessors with equal geometricInfo (same grid)
				core::MaskVoxel m1Voxel(0);
				//initialize results list with mask1 values
				result->setRelevantVoxelVector(mask1->getRelevantVoxelVector());

				MaskVoxelListPointer voxelListMask2 = mask2->getRelevantVoxelVector();
				auto it = voxelListMask2->begin();

				while (it != voxelListMask2->end())
				{
					mask1->getMaskAt(it->getVoxelGridID(), m1Voxel);
					FractionType opVal = op.calc(m1Voxel.getRelevantVolumeFraction(), it->getRelevantVolumeFraction());
					result->setMaskAt(it->getVoxelGridID(), core::MaskVoxel(it->getVoxelGridID(), opVal));
					++it;
				}

			}

		}
	}
}
#endif