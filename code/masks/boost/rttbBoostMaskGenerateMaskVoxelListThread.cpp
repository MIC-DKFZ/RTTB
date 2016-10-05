
#include "rttbBoostMaskGenerateMaskVoxelListThread.h"


#include "rttbInvalidParameterException.h"
#include <boost/make_shared.hpp>


namespace rttb
{
	namespace masks
	{
		namespace boost
		{
			BoostMaskGenerateMaskVoxelListThread::BoostMaskGenerateMaskVoxelListThread(
			    const VoxelIndexVector& aGlobalBoundingBox,
			    GeometricInfoPointer aGeometricInfo,
			    const BoostArrayMap& aVoxelizationMap,
			    double aVoxelizationThickness,
			    unsigned int aBeginSlice,
			    unsigned int aEndSlice,
			    MaskVoxelQueuePointer aResultMaskVoxelQueue) :
				_globalBoundingBox(aGlobalBoundingBox), _geometricInfo(aGeometricInfo),
				_voxelizationMap(aVoxelizationMap), _voxelizationThickness(aVoxelizationThickness),
				_beginSlice(aBeginSlice), _endSlice(aEndSlice),
				_resultMaskVoxelQueue(aResultMaskVoxelQueue)
			{}

			void BoostMaskGenerateMaskVoxelListThread::operator()()
			{
				rttb::VoxelGridIndex3D minIndex = _globalBoundingBox.at(0);
				rttb::VoxelGridIndex3D maxIndex = _globalBoundingBox.at(1);
				int globalBoundingBoxSize0 = maxIndex[0] - minIndex[0] + 1;
				int globalBoundingBoxSize1 = maxIndex[1] - minIndex[1] + 1;

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

							std::map<double, double>::iterator it;
							BoostArrayMap::iterator itMap;

							for (it = weightVectorForZ.begin(), itMap = _voxelizationMap.begin(); it != weightVectorForZ.end()
							     && itMap != _voxelizationMap.end(); ++it, ++itMap)
							{
								double weight = (*it).second;

								BoostArray2D voxelizationArray = (*itMap).second;
								//calc sum of all voxelization plane, use weight
								volumeFraction += voxelizationArray[x][y] * weight;
							}


							if (volumeFraction > 1 && (volumeFraction - 1) <= errorConstant)
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
								core::MaskVoxel* maskVoxelPtr = new core::MaskVoxel(gridID, volumeFraction);
								_resultMaskVoxelQueue->push(maskVoxelPtr);//push back the mask voxel in structure
							}
						}

					}
				}

			}

			void BoostMaskGenerateMaskVoxelListThread::calcWeightVector(const rttb::VoxelGridID& aIndexZ,
			        std::map<double, double>& weightVector) const
			{
				BoostArrayMap::const_iterator it;

				double indexZMin = aIndexZ - 0.5;
				double indexZMax = aIndexZ + 0.5;

				for (it = _voxelizationMap.begin(); it != _voxelizationMap.end(); ++it)
				{
					double voxelizationPlaneIndexMin = (*it).first - 0.5 * _voxelizationThickness;
					double voxelizationPlaneIndexMax = (*it).first + 0.5 * _voxelizationThickness;
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

					weightVector.insert(std::pair<double, double>((*it).first, weight));
				}
			}
		}
	}
}