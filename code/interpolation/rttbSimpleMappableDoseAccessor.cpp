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

#include "rttbSimpleMappableDoseAccessor.h"
#include "rttbNullPointerException.h"
#include "rttbMappingOutsideOfImageException.h"

namespace rttb
{
	namespace interpolation
	{
		SimpleMappableDoseAccessor::SimpleMappableDoseAccessor(const core::GeometricInfo&
		        geoInfoTargetImage,
            core::DoseAccessorInterface::ConstPointer doseMovingImage, const TransformationInterface::Pointer aTransformation,
		        const InterpolationBase::Pointer aInterpolation, bool acceptPadding,
		        double defaultOutsideValue): MappableDoseAccessorInterface(geoInfoTargetImage, doseMovingImage,
			                aTransformation, acceptPadding, defaultOutsideValue),
			_spInterpolation(aInterpolation)
		{
			//handle null pointers
			if (aInterpolation == nullptr)
			{
				throw core::NullPointerException("Pointer to interpolation cannot be nullptr.");
			}
			else
			{
				_spInterpolation->setAccessorPointer(_spOriginalDoseDataMovingImage);
			}
		}

		GenericValueType SimpleMappableDoseAccessor::getValueAt(const VoxelGridID aID) const
		{
			VoxelGridIndex3D aVoxelGridIndex3D;

			if (_geoInfoTargetImage.convert(aID, aVoxelGridIndex3D))
			{
				return getValueAt(aVoxelGridIndex3D);
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
				}
			}
		}

		GenericValueType SimpleMappableDoseAccessor::getValueAt(const VoxelGridIndex3D& aIndex) const
		{
			//Transform requested voxel coordinates of original image into world coordinates with RTTB
			WorldCoordinate3D worldCoordinateTarget;

			if (_geoInfoTargetImage.indexToWorldCoordinate(aIndex, worldCoordinateTarget))
			{
				//transform coordinates
				WorldCoordinate3D worldCoordinateMoving;
				_spTransformation->transformInverse(worldCoordinateTarget, worldCoordinateMoving);

				//Use Interpolation to compute dose at mappedImage
				try
				{
					return _spInterpolation->getValue(worldCoordinateMoving);
				}
				//Mapped outside of image? Check if padding is allowed
				catch (core::MappingOutsideOfImageException& /*e*/)
				{
					if (_acceptPadding)
					{
						return _defaultOutsideValue;
					}
					else
					{
						throw core::MappingOutsideOfImageException("Error in conversion from index to world coordinates");
					}
				}
				catch (core::Exception& e)
				{
					std::cout << e.what() << std::endl;
					return -1;
				}
			}
			//ok, if that fails, throw exception. Makes no sense to go further
			else
			{
				if (_acceptPadding)
				{
					return _defaultOutsideValue;
				}
				else
				{
					throw core::MappingOutsideOfImageException("Error in conversion from index to world coordinates");
				}
			}
		}

	}//end namespace interpolation
}//end namespace rttb
