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

#include "rttbITKImageAccessor.h"
#include "rttbException.h"
#include "rttbInvalidDoseException.h"

namespace rttb
{
	namespace io
	{
		namespace itk
		{
			ITKImageAccessor::~ITKImageAccessor()
			= default;

			ITKImageAccessor::ITKImageAccessor(ITKImageType::ConstPointer image)
			{
				_data = image;

				if (_data.IsNull())
				{
					throw core::InvalidDoseException("Dose image = 0!") ;
				}

				assembleGeometricInfo();
			}

			GenericValueType ITKImageAccessor::getValueAt(const VoxelGridID aID) const
			{
				VoxelGridIndex3D aVoxelGridIndex;

				if (_geoInfo.convert(aID, aVoxelGridIndex))
				{
					return getValueAt(aVoxelGridIndex);
				}
				else
				{
					return -1;
				}
			}

			GenericValueType ITKImageAccessor::getValueAt(const VoxelGridIndex3D& aIndex) const
			{
				if (_geoInfo.validIndex(aIndex))
				{
					const ITKImageType::IndexType pixelIndex = {{aIndex[0], aIndex[1], aIndex[2]}};
					return _data->GetPixel(pixelIndex);
				}
				else
				{
					return -1;
				}

			}

			bool ITKImageAccessor::assembleGeometricInfo()
			{
				_geoInfo.setSpacing(SpacingVectorType3D(_data->GetSpacing()[0], _data->GetSpacing()[1],
				                                        _data->GetSpacing()[2]));

				if (_geoInfo.getSpacing()[0] == 0 || _geoInfo.getSpacing()[1] == 0 || _geoInfo.getSpacing()[2] == 0)
				{
					throw core::InvalidDoseException("Pixel spacing = 0!");
				}

				_geoInfo.setImagePositionPatient(WorldCoordinate3D(_data->GetOrigin()[0], _data->GetOrigin()[1],
				                                 _data->GetOrigin()[2]));
				OrientationMatrix OM(0);

				for (unsigned int col = 0; col < 3; ++col)
				{
					for (unsigned int row = 0; row < 3; ++row)
					{
						OM(col, row) = _data->GetDirection()(col, row);
					}
				}

				_geoInfo.setOrientationMatrix(OM);
				_geoInfo.setNumColumns(_data->GetLargestPossibleRegion().GetSize()[0]);
				_geoInfo.setNumRows(_data->GetLargestPossibleRegion().GetSize()[1]);
				_geoInfo.setNumSlices(_data->GetLargestPossibleRegion().GetSize()[2]);

				if (_geoInfo.getNumColumns() == 0 || _geoInfo.getNumRows() == 0 || _geoInfo.getNumSlices() == 0)
				{
					throw core::InvalidDoseException("Empty dicom dose!") ;
				}

				return true;

			}

		}
	}
}

