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


#include <boost/shared_ptr.hpp>

#include "rttbITKImageMaskAccessorConverter.h"
#include "rttbInvalidDoseException.h"
#include "rttbGeometricInfo.h"
#include "itkImageRegionIterator.h"
#include "rttbITKImageMaskAccessor.h"

namespace rttb
{
	namespace io
	{
		namespace itk
		{
			ITKImageMaskAccessorConverter::ITKImageMaskAccessorConverter(MaskAccessorPointer accessor)
			{
				setMaskAccessor(accessor);
			}

			bool ITKImageMaskAccessorConverter::process()
			{
				//Transfer GeometricInfo to ITK Properties
				core::GeometricInfo geoInfo = _maskAccessor->getGeometricInfo();

				ITKImageMaskAccessor::ITKMaskImageType::RegionType region;

				ITKImageMaskAccessor::ITKMaskImageType::IndexType start = {{0, 0, 0}};

				ITKImageMaskAccessor::ITKMaskImageType::SizeType size = {{geoInfo.getNumColumns(), geoInfo.getNumRows(), geoInfo.getNumSlices()}};

				ITKImageMaskAccessor::ITKMaskImageType::SpacingType spacing;

				for (unsigned int i = 0; i < 3; ++i)
				{
					spacing[i] = geoInfo.getSpacing()[i];
				}

				ITKImageMaskAccessor::ITKMaskImageType::PointType origin;

				for (unsigned int i = 0; i < 3; ++i)
				{
					origin[i] = geoInfo.getImagePositionPatient()[i];
				}

				ITKImageMaskAccessor::ITKMaskImageType::DirectionType direction;
				OrientationMatrix OM = geoInfo.getOrientationMatrix();

				for (int col = 0; col < 3; ++col)
				{
					for (int row = 0; row < 3; ++row)
					{
						direction(col, row) = OM(col, row);
					}
				}

				//Create image, assign properties
				region.SetSize(size);
				region.SetIndex(start);

				_itkImage = ITKImageMaskAccessor::ITKMaskImageType::New();
				_itkImage->SetRegions(region);
				_itkImage->SetSpacing(spacing);
				_itkImage->SetDirection(direction);
				_itkImage->SetOrigin(origin);
				_itkImage->Allocate();

				::itk::ImageRegionIterator<ITKImageMaskAccessor::ITKMaskImageType> imageIterator(_itkImage, region);
				VoxelGridID id = 0;

				//Transfer Mask values to itk image
				//Large advantage: rttbVoxelGridId ordering is the same as itk ordering
				while (!imageIterator.IsAtEnd())
				{
					VoxelGridIndex3D aIndex;

					if (_maskAccessor->getGeometricInfo().validID(id))
					{
						rttb::core::MaskVoxel maskVoxel = core::MaskVoxel(id);;
						_maskAccessor->getMaskAt(id, maskVoxel);
						// Set the current pixel
						imageIterator.Set(maskVoxel.getRelevantVolumeFraction());
					}
					else
					{
						if (failsOnInvalidIDs())
						{
							throw core::InvalidDoseException("invalid Mask id!");
							return false;
						}
						else
						{
							imageIterator.Set(_invalidDoseValue);
						}
					}

					++imageIterator;
					++id;
				}

				return true;
			}


		}//end namespace mask
	}//end namespace io
}//end namespace rttb

