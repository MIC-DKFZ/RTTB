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

				assembleGeometricInfo();
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

			bool ITKImageDoseAccessor::assembleGeometricInfo()
			{
				_geoInfo.setSpacing(SpacingVectorType3D(_dose->GetSpacing()[0], _dose->GetSpacing()[1],
				                                        _dose->GetSpacing()[2]));

				if (_geoInfo.getSpacing()[0] == 0 || _geoInfo.getSpacing()[1] == 0 || _geoInfo.getSpacing()[2] == 0)
				{
					throw core::InvalidDoseException("Pixel spacing = 0!");
				}

				_geoInfo.setImagePositionPatient(WorldCoordinate3D(_dose->GetOrigin()[0], _dose->GetOrigin()[1],
				                                 _dose->GetOrigin()[2]));
				OrientationMatrix OM(0);

				for (int col = 0; col < 3; ++col)
				{
					for (int row = 0; row < 3; ++row)
					{
						OM(col, row) = _dose->GetDirection()(col, row);
					}
				}

				_geoInfo.setOrientationMatrix(OM);
				_geoInfo.setNumColumns(_dose->GetLargestPossibleRegion().GetSize()[0]);
				_geoInfo.setNumRows(_dose->GetLargestPossibleRegion().GetSize()[1]);
				_geoInfo.setNumSlices(_dose->GetLargestPossibleRegion().GetSize()[2]);

				if (_geoInfo.getNumColumns() == 0 || _geoInfo.getNumRows() == 0 || _geoInfo.getNumSlices() == 0)
				{
					throw core::InvalidDoseException("Empty dicom dose!") ;
				}

				return true;

			}

		}
	}
}

