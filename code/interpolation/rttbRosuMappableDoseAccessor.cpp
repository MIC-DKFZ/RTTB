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

#include "rttbRosuMappableDoseAccessor.h"
#include "rttbNullPointerException.h"
#include "rttbMappingOutsideOfImageException.h"
#include "rttbLinearInterpolation.h"

namespace rttb
{
	namespace interpolation
	{
		RosuMappableDoseAccessor::RosuMappableDoseAccessor(const core::GeometricInfo& geoInfoTargetImage,
		        const DoseAccessorPointer doseMovingImage,
		        const TransformationInterface::Pointer aTransformation,
		        bool acceptPadding,
		        DoseTypeGy defaultOutsideValue): MappableDoseAccessorBase(geoInfoTargetImage, doseMovingImage,
			                aTransformation, acceptPadding, defaultOutsideValue)
		{
			//define linear interpolation
			InterpolationBase::Pointer interpolationLinear = InterpolationBase::Pointer(
			            new LinearInterpolation());
			_spInterpolation = interpolationLinear;
			_spInterpolation->setDoseAccessorPointer(_spOriginalDoseDataMovingImage);
		}

		DoseTypeGy RosuMappableDoseAccessor::getDoseAt(const VoxelGridIndex3D& aIndex) const
		{
			//Transform requested voxel coordinates of original image into world coordinates with RTTB
			WorldCoordinate3D worldCoordinateTarget;

			if (_geoInfoTargetImage.indexToWorldCoordinate(aIndex, worldCoordinateTarget))
			{
				std::vector<WorldCoordinate3D> octants = getOctants(worldCoordinateTarget);

				if (octants.size() > 2)
				{
					DoseTypeGy interpolatedDoseValue = 0.0;

					//get trilinear interpolation value of every octant point
					for (std::vector<WorldCoordinate3D>::const_iterator octantIterator = std::begin(octants);
					     octantIterator != std::end(octants);
					     ++octantIterator)
					{
						//transform coordinates
						WorldCoordinate3D worldCoordinateMoving;
						WorldCoordinate3D worldCoordinateTargetOctant = *octantIterator;
						_spTransformation->transformInverse(worldCoordinateTargetOctant, worldCoordinateMoving);

						try
						{
							interpolatedDoseValue += _spInterpolation->getValue(worldCoordinateMoving);
						}
						//Mapped outside of image? Check if padding is allowed
						catch (core::MappingOutsideOfImageException& /*e*/)
						{
							if (_acceptPadding)
							{
								interpolatedDoseValue += _defaultOutsideValue;
							}
							else
							{
								throw core::MappingOutsideOfImageException("Mapping outside of image");
							}
						}
						catch (core::Exception& e)
						{
							std::cout << e.what() << std::endl;
							return -1;
						}
					}

					return interpolatedDoseValue / (DoseTypeGy)octants.size();
				}
				else
				{
					if (_acceptPadding)
					{
						return _defaultOutsideValue;
					}
					else
					{
						throw core::MappingOutsideOfImageException("Too many samples are mapped outside the image!");
						return -1;
					}
				}
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

		std::vector<WorldCoordinate3D> RosuMappableDoseAccessor::getOctants(
		    const WorldCoordinate3D& aCoordinate) const
		{
			std::vector<WorldCoordinate3D> octants;
			SpacingVectorType3D spacingTargetImage = _geoInfoTargetImage.getSpacing();

			core::GeometricInfo geometricInfoDoseData = _spOriginalDoseDataMovingImage->getGeometricInfo();

			//as the corner point is the coordinate of the voxel (grid), 0.25 and 0.75 are the center of the subvoxels
			for (double xOct = 0.25; xOct <= 0.75; xOct += 0.5)
			{
				for (double yOct = 0.25; yOct <= 0.75; yOct += 0.5)
				{
					for (double zOct = 0.25; zOct <= 0.75; zOct += 0.5)
					{
						WorldCoordinate3D aWorldCoordinate(aCoordinate.x() + (xOct * spacingTargetImage.x()),
						                                   aCoordinate.y() + (yOct * spacingTargetImage.y()),
						                                   aCoordinate.z() + (zOct * spacingTargetImage.z()));

						if (geometricInfoDoseData.isInside(aWorldCoordinate))
						{
							octants.push_back(aWorldCoordinate);
						}
					}
				}
			}

			return octants;
		}


	}//end namespace interpolation
}//end namespace rttb
