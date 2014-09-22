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

		bool MappableDoseAccessorBase::validID(const VoxelGridID aID) const
		{
			return (aID >= 0 && aID < getGridSize());
		}

		bool MappableDoseAccessorBase::validIndex(const VoxelGridIndex3D& aIndex) const
		{
			VoxelGridID aID;

			if (!convert(aIndex, aID))
			{
				return false;
			}
			else
			{
				return validID(aID);
			}
		}

		DoseTypeGy MappableDoseAccessorBase::getDoseAt(const VoxelGridID aID) const
		{
			VoxelGridIndex3D aVoxelGridIndex3D;

			if (convert(aID, aVoxelGridIndex3D))
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

		bool MappableDoseAccessorBase::convert(const VoxelGridID& gridID, VoxelGridIndex3D& gridIndex) const
		{
			if (validID(gridID))
			{
				gridIndex(0) = gridID % _geoInfoTargetImage.getNumColumns();
				VoxelGridID tempID = (gridID - gridIndex.x()) / _geoInfoTargetImage.getNumColumns();
				gridIndex(1) = tempID % _geoInfoTargetImage.getNumRows();
				gridIndex(2) = (tempID - gridIndex.y()) / _geoInfoTargetImage.getNumRows();
				return true;
			}

			return false;
		}

		bool MappableDoseAccessorBase::convert(const VoxelGridIndex3D& gridIndex, VoxelGridID& gridID) const
		{
			if ((gridIndex.x() >= (unsigned int)_geoInfoTargetImage.getNumColumns())
			    || (gridIndex.y() >= (unsigned int)_geoInfoTargetImage.getNumRows())
			    || (gridIndex.z() >= (unsigned int)_geoInfoTargetImage.getNumSlices()))
			{
				return false;
			}
			else
			{
				gridID = gridIndex.z() * _geoInfoTargetImage.getNumColumns() * _geoInfoTargetImage.getNumRows() +
				         gridIndex.y() * _geoInfoTargetImage.getNumColumns()
				         + gridIndex.x();
				return validID(gridID);
			}
		}

	}
}
