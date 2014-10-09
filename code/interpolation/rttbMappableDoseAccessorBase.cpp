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

#include "rttbMappableDoseAccessorBase.h"
#include "rttbNullPointerException.h"
#include "rttbMappingOutsideOfImageException.h"

namespace rttb
{
	namespace interpolation
	{
		MappableDoseAccessorBase::MappableDoseAccessorBase(const core::GeometricInfo& geoInfoTargetImage,
		        const DoseAccessorPointer doseMovingImage,
		        const TransformationInterface::Pointer aTransformation, bool acceptPadding,
		        double defaultOutsideValue): _spOriginalDoseDataMovingImage(doseMovingImage),
			_spTransformation(aTransformation), _geoInfoTargetImage(geoInfoTargetImage),
			_acceptPadding(acceptPadding), _defaultOutsideValue(defaultOutsideValue)
		{
			//handle null pointers
			if (doseMovingImage == NULL || aTransformation == NULL)
			{
				throw core::NullPointerException("Pointers to input accessors/transformation cannot be NULL.");
			}
		}

		DoseTypeGy MappableDoseAccessorBase::getDoseAt(const VoxelGridID aID) const
		{
			VoxelGridIndex3D aVoxelGridIndex3D;

			if (_geoInfoTargetImage.convert(aID, aVoxelGridIndex3D))
			{
				return getDoseAt(aVoxelGridIndex3D);
			}
			else
			{
				if (_acceptPadding)
				{
					return _defaultOutsideValue;
				}
				else
				{
					throw core::MappingOutsideOfImageException("Error in conversion from index to world coordinates");
					return -1;
				}
			}
		}

	}
}
