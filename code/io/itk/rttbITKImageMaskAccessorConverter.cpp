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
#include "itkMaskAccessorImageSource.h"

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

				for (unsigned int col = 0; col < 3; ++col)
				{
					for (unsigned int row = 0; row < 3; ++row)
					{
						direction(col, row) = OM(col, row);
					}
				}

        ::itk::MaskAccessorImageSource::Pointer imagesource = ::itk::MaskAccessorImageSource::New();

        imagesource->SetSize(size);
        imagesource->SetSpacing(spacing);
        imagesource->SetDirection(direction);
        imagesource->SetOrigin(origin);
        imagesource->SetAccessor(_maskAccessor);
        imagesource->SetFailsOnInvalidIDs(failsOnInvalidIDs());
        imagesource->SetInvalidMaskValue(_invalidDoseValue);
        imagesource->Update();
        _itkImage = imagesource->GetOutput();

				return true;
			}


		}//end namespace mask
	}//end namespace io
}//end namespace rttb

