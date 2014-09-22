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

#include "rttbITKImageDoseAccessor.h"
#include "rttbException.h"
#include "rttbInvalidDoseException.h"

namespace rttb
{
	namespace io
	{
		namespace itk
		{
			ITKImageDoseAccessor::~ITKImageDoseAccessor()
			{

			}

			ITKImageDoseAccessor::ITKImageDoseAccessor(ITKDoseImageType::ConstPointer doseImage):
				_doseGridScaling(1.0)
			{
				_dose = doseImage;

				if (_dose.IsNull())
				{
					throw core::InvalidDoseException("Dose image = 0!") ;
				}

				_geoInfo = convertToGeometricInfo(_dose);
			}

			DoseTypeGy ITKImageDoseAccessor::getDoseAt(const VoxelGridID aID) const
			{
				VoxelGridIndex3D aVoxelGridIndex;

				if (_geoInfo.convert(aID, aVoxelGridIndex))
				{
					return getDoseAt(aVoxelGridIndex);
				}
				else
				{
					return -1;
				}
			}

			DoseTypeGy ITKImageDoseAccessor::getDoseAt(const VoxelGridIndex3D& aIndex) const
			{
				if (_geoInfo.validIndex(aIndex))
				{
					const ITKDoseImageType::IndexType pixelIndex = {{aIndex[0], aIndex[1], aIndex[2]}};
					return _dose->GetPixel(pixelIndex) * _doseGridScaling;
				}
				else
				{
					return -1;
				}

			}

			bool isValid(const core::GeometricInfo& geoInfo)
			{
				if (geoInfo.getSpacing()[0] == 0 || geoInfo.getSpacing()[1] == 0 || geoInfo.getSpacing()[2] == 0)
				{
					throw core::InvalidDoseException("Dose spacing = 0!") ;
					return false;
				}

				if (geoInfo.getNumRows() == 0 || geoInfo.getNumColumns() == 0 || geoInfo.getNumSlices() == 0)
				{
					throw core::InvalidDoseException("Dose row/col/slices = 0!") ;
					return false;
				}

				return true;
			}

			core::GeometricInfo convertToGeometricInfo(const ITKImageBaseType* image)
			{
				core::GeometricInfo geoInfo;
				geoInfo.setSpacing(SpacingVectorType3D(image->GetSpacing()[0], image->GetSpacing()[1],
				                                       image->GetSpacing()[2]));
				geoInfo.setImagePositionPatient(WorldCoordinate3D(image->GetOrigin()[0], image->GetOrigin()[1],
				                                image->GetOrigin()[2]));
				OrientationMatrix OM(0);

				for (int col = 0; col < 3; ++col)
				{
					for (int row = 0; row < 3; ++row)
					{
						OM(col, row) = image->GetDirection()(col, row);
					}
				}

				geoInfo.setOrientationMatrix(OM);
				geoInfo.setNumColumns(image->GetLargestPossibleRegion().GetSize()[0]);
				geoInfo.setNumRows(image->GetLargestPossibleRegion().GetSize()[1]);
				geoInfo.setNumSlices(image->GetLargestPossibleRegion().GetSize()[2]);

				if (isValid(geoInfo))
				{
					return geoInfo;
				}
				else
				{
					throw core::InvalidDoseException("no valid GeometricInfo after conversion from itk");
					return core::GeometricInfo();
				}
			}

		}
	}
}

