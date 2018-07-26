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

#include "rttbBoostMaskGenerateMaskVoxelListThread.h"

#include "rttbInvalidParameterException.h"
#include <boost/thread.hpp>

namespace rttb
{
	namespace masks
	{
		namespace boost
		{
			BoostMaskGenerateMaskVoxelListThread::BoostMaskGenerateMaskVoxelListThread(
			    const VoxelIndexVector& aGlobalBoundingBox,
        core::GeometricInfo::Pointer aGeometricInfo,
                BoostArrayMapPointer aVoxelizationMap,
			    double aVoxelizationThickness,
			    unsigned int aBeginSlice,
			    unsigned int aEndSlice,
                MaskVoxelListPointer aMaskVoxelList,
        bool strictVoxelization,
                ::boost::shared_ptr<::boost::shared_mutex> aMutex) :
				_globalBoundingBox(aGlobalBoundingBox), _geometricInfo(aGeometricInfo),
				_voxelizationMap(aVoxelizationMap), _voxelizationThickness(aVoxelizationThickness),
				_beginSlice(aBeginSlice), _endSlice(aEndSlice),
                _resultMaskVoxelList(aMaskVoxelList), _strictVoxelization(strictVoxelization), _mutex(aMutex)
			{}

			void BoostMaskGenerateMaskVoxelListThread::operator()()
			{
				rttb::VoxelGridIndex3D minIndex = _globalBoundingBox.at(0);
				rttb::VoxelGridIndex3D maxIndex = _globalBoundingBox.at(1);
				unsigned int globalBoundingBoxSize0 = maxIndex[0] - minIndex[0] + 1;
				unsigned int globalBoundingBoxSize1 = maxIndex[1] - minIndex[1] + 1;

                std::vector<core::MaskVoxel> maskVoxelsInThread;

				for (unsigned int indexZ = _beginSlice; indexZ < _endSlice; ++indexZ)
				{
					//calculate weight vector
					std::map<double, double> weightVectorForZ;
					calcWeightVector(indexZ, weightVectorForZ);

					//For each x,y, calc sum of all voxelization plane, use weight vector
					for (unsigned int x = 0; x < globalBoundingBoxSize0; ++x)
					{
						for (unsigned int y = 0; y < globalBoundingBoxSize1; ++y)
						{
							rttb::VoxelGridIndex3D currentIndex;
							currentIndex[0] = x + minIndex[0];
							currentIndex[1] = y + minIndex[1];
							currentIndex[2] = indexZ;
							rttb::VoxelGridID gridID;
							_geometricInfo->convert(currentIndex, gridID);
							double volumeFraction = 0;

                            auto it = weightVectorForZ.cbegin();
                            auto itMap = _voxelizationMap->cbegin();

							for (; it != weightVectorForZ.cend()
							     && itMap != _voxelizationMap->cend(); ++it, ++itMap)
							{
								double weight = it->second;
                                if (weight > 0){
                                    BoostArray2DPointer voxelizationArray = itMap->second;
				
                                    //calc sum of all voxelization plane, use weight
                                    volumeFraction += (*voxelizationArray)[x][y] * weight;
                                }
							}

							if (volumeFraction > 1 && ((volumeFraction - 1) <= errorConstant || !_strictVoxelization))
							{
								volumeFraction = 1;
							}
							else if (volumeFraction < 0 || (volumeFraction - 1) > errorConstant)
							{
								throw rttb::core::InvalidParameterException("Mask calculation failed! The volume fraction should >= 0 and <= 1!");
							}

							//insert mask voxel if volumeFraction > 0
							if (volumeFraction > 0)
							{
								core::MaskVoxel maskVoxelPtr = core::MaskVoxel(gridID, volumeFraction);
                                maskVoxelsInThread.push_back(maskVoxelPtr);
							}
						}

					}
				}

                ::boost::unique_lock<::boost::shared_mutex> lock(*_mutex);
                _resultMaskVoxelList->insert(_resultMaskVoxelList->end(), maskVoxelsInThread.begin(), maskVoxelsInThread.end());
			}

			void BoostMaskGenerateMaskVoxelListThread::calcWeightVector(const rttb::VoxelGridID& aIndexZ,
			        std::map<double, double>& weightVector) const
			{
				double indexZMin = aIndexZ - 0.5;
				double indexZMax = aIndexZ + 0.5;

                for (auto & it : *_voxelizationMap)
				{
					double voxelizationPlaneIndexMin = it.first - 0.5 * _voxelizationThickness;
					double voxelizationPlaneIndexMax = it.first + 0.5 * _voxelizationThickness;
					double weight = 0;
					
					if ((voxelizationPlaneIndexMin < indexZMin) && (voxelizationPlaneIndexMax > indexZMin))
					{
						if (voxelizationPlaneIndexMax < indexZMax)
						{
							weight = voxelizationPlaneIndexMax - indexZMin;
						}
						else
						{
							weight = 1;
						}
					}
					else if ((voxelizationPlaneIndexMin >= indexZMin) && (voxelizationPlaneIndexMin < indexZMax))
					{
						if (voxelizationPlaneIndexMax < indexZMax)
						{
							weight = _voxelizationThickness;
						}
						else
						{
							weight = indexZMax - voxelizationPlaneIndexMin;
						}
					}

					weightVector.insert(std::pair<double, double>(it.first, weight));
				}
			}
		}
	}
}
