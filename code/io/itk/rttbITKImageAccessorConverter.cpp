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

#include <cassert>

#include <boost/shared_ptr.hpp>

#include "rttbITKImageAccessorConverter.h"
#include "rttbException.h"
#include "rttbInvalidDoseException.h"
#include "rttbGeometricInfo.h"
#include "itkDoseAccessorImageFilter.h"

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

				for (unsigned int col = 0; col < 3; ++col)
				{
					for (unsigned int row = 0; row < 3; ++row)
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

				::itk::DoseAccessorImageFilter::Pointer accessorFilter = ::itk::DoseAccessorImageFilter::New();
				accessorFilter->SetInput(_itkImage);
				accessorFilter->SetAccessor(_doseAccessor);
				accessorFilter->Update();

				_itkImage = accessorFilter->GetOutput();

				return true;
			}

		}//end namespace itk
	}//end namespace io
}//end namespace rttb

