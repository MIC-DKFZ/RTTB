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
// @version $Revision: 793 $ (last changed revision)
// @date    $Date: 2014-10-10 10:24:45 +0200 (Fr, 10 Okt 2014) $ (last change date)
// @author  $Author: hentsch $ (last changed by)
*/

#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>

#include "rttbITKImageMaskAccessor.h"
#include "rttbInvalidDoseException.h"

namespace rttb
{
	namespace io
	{
		namespace mask
		{
			ITKImageMaskAccessor::ITKImageMaskAccessor(ITKMaskImageType::ConstPointer aMaskImage)
				: _mask(aMaskImage)
			{
				if (_mask.IsNull())
				{
					throw core::InvalidDoseException("Mask image = 0!") ;
				}

				assembleGeometricInfo();
			}


			ITKImageMaskAccessor::~ITKImageMaskAccessor()
			{
			};

			bool ITKImageMaskAccessor::assembleGeometricInfo()
			{
				_geoInfo =  boost::make_shared<core::GeometricInfo>();

				_geoInfo->setSpacing(SpacingVectorType3D(_mask->GetSpacing()[0], _mask->GetSpacing()[1],
				                                        _mask->GetSpacing()[2]));

				if (_geoInfo->getSpacing()[0] == 0 || _geoInfo->getSpacing()[1] == 0 || _geoInfo->getSpacing()[2] == 0)
				{
					throw core::InvalidDoseException("Pixel spacing = 0!");
				}

				_geoInfo->setImagePositionPatient(WorldCoordinate3D(_mask->GetOrigin()[0], _mask->GetOrigin()[1],
				                                 _mask->GetOrigin()[2]));
				OrientationMatrix OM(0);

				for (int col = 0; col < 3; ++col)
				{
					for (int row = 0; row < 3; ++row)
					{
						OM(col, row) = _mask->GetDirection()(col, row);
					}
				}

				_geoInfo->setOrientationMatrix(OM);
				_geoInfo->setNumColumns(_mask->GetLargestPossibleRegion().GetSize()[0]);
				_geoInfo->setNumRows(_mask->GetLargestPossibleRegion().GetSize()[1]);
				_geoInfo->setNumSlices(_mask->GetLargestPossibleRegion().GetSize()[2]);

				if (_geoInfo->getNumColumns() == 0 || _geoInfo->getNumRows() == 0 || _geoInfo->getNumSlices() == 0)
				{
					throw core::InvalidDoseException("Empty mask!") ;
				}

				return true;

			}


			void ITKImageMaskAccessor::updateMask()
			{
				return;
			}

			ITKImageMaskAccessor::MaskVoxelListPointer ITKImageMaskAccessor::getRelevantVoxelVector()
			{
				updateMask();
				_relevantVoxelVector = getRelevantVoxelVector(0);
				return _relevantVoxelVector;
			}

			ITKImageMaskAccessor::MaskVoxelListPointer ITKImageMaskAccessor::getRelevantVoxelVector(float lowerThreshold)
			{
				MaskVoxelListPointer filteredVoxelVectorPointer(new MaskVoxelList);
				updateMask();
				int size =  _geoInfo->getNumColumns()*_geoInfo->getNumRows()*_geoInfo->getNumSlices();
				filteredVoxelVectorPointer->reserve(size);
				for(int gridIndex =0 ; gridIndex < size; gridIndex++){
					core::MaskVoxel currentVoxel = core::MaskVoxel(gridIndex);
					if(getMaskAt(gridIndex, currentVoxel)){
						if(currentVoxel.getRelevantVolumeFraction() > lowerThreshold){
							filteredVoxelVectorPointer->push_back(currentVoxel);
						}
					}
				}

				return filteredVoxelVectorPointer;
			}

			bool ITKImageMaskAccessor::getMaskAt(VoxelGridID aID, core::MaskVoxel& voxel) const
			{
				VoxelGridIndex3D aVoxelGridIndex;

				if (_geoInfo->convert(aID, aVoxelGridIndex))
				{
					return getMaskAt(aVoxelGridIndex, voxel);
				}
				else
				{
					return false;
				}

			}

			bool ITKImageMaskAccessor::getMaskAt(const VoxelGridIndex3D& aIndex, core::MaskVoxel& voxel) const
			{
				voxel.setRelevantVolumeFraction(0);

				if (_geoInfo->validIndex(aIndex))
				{
					const ITKMaskImageType::IndexType pixelIndex = {{aIndex[0], aIndex[1], aIndex[2]}};
					double value = _mask->GetPixel(pixelIndex);
					VoxelGridID gridId;
					_geoInfo->convert(aIndex, gridId);
					if(value >= 0 && value <=1 ){
						voxel.setRelevantVolumeFraction(value);
					}
					else{
						std::cerr << "The pixel value of the mask should be >=0 and <=1!"<<std::endl;
						return false;
					}
				}
				else
				{
					return false;
				}
			}

			const core::GeometricInfo& ITKImageMaskAccessor::getGeometricInfo() const
			{
				return *_geoInfo;
			};
		}
	}
}

