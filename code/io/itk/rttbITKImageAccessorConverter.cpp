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

#include <assert.h>

#include <boost/shared_ptr.hpp>

#include "rttbITKImageAccessorConverter.h"
#include "rttbException.h"
#include "rttbInvalidDoseException.h"
#include "rttbGeometricInfo.h"
#include "itkImageRegionIterator.h"

namespace rttb
{
	namespace io
	{
		namespace itk
		{

			ITKImageAccessorConverter::ITKImageAccessorConverter(DoseAccessorPointer accessor)
			{
				setDoseAccessor(accessor);
			}

			bool ITKImageAccessorConverter::process()
			{
				//Transfer GeometricInfo to ITK Properties
				core::GeometricInfo geoInfo = _doseAccessor->getGeometricInfo();

				ITKImageType::RegionType region;
				ITKImageType::IndexType start;

				for (unsigned int i = 0; i < 3; ++i)
				{
					start[i] = 0;
				}

				ITKImageType::SizeType size;
				size[0] = geoInfo.getNumColumns();
				size[1] = geoInfo.getNumRows();
				size[2] = geoInfo.getNumSlices();

				ITKImageType::SpacingType spacing;

				for (unsigned int i = 0; i < 3; ++i)
				{
					spacing[i] = geoInfo.getSpacing()[i];
				}

				ITKImageType::PointType origin;

				for (unsigned int i = 0; i < 3; ++i)
				{
					origin[i] = geoInfo.getImagePositionPatient()[i];
				}

				ITKImageType::DirectionType direction;
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

				_itkImage = ITKImageType::New();
				_itkImage->SetRegions(region);
				_itkImage->SetSpacing(spacing);
				_itkImage->SetDirection(direction);
				_itkImage->SetOrigin(origin);
				_itkImage->Allocate();

				::itk::ImageRegionIterator<ITKImageType> imageIterator(_itkImage, region);
				VoxelGridID id = 0;

				//Transfer dose values to itk image
				//Large advantage: rttbVoxelGridId ordering is the same as itk ordering
				while (!imageIterator.IsAtEnd())
				{
					VoxelGridIndex3D aIndex;

					if (_doseAccessor->getGeometricInfo().validID(id))
					{
						// Set the current pixel
						imageIterator.Set(_doseAccessor->getValueAt(id));
					}
					else
					{
						if (failsOnInvalidIDs())
						{
							throw core::InvalidDoseException("invalid dose id!");
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

		}//end namespace itk
	}//end namespace io
}//end namespace rttb

