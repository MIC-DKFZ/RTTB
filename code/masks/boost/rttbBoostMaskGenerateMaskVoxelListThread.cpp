
#include "rttbBoostMaskGenerateMaskVoxelListThread.h"


#include "rttbInvalidParameterException.h"
#include <boost/make_shared.hpp>


namespace rttb
{
	namespace masks
	{
		namespace boostRedesign
		{
			BoostMaskGenerateMaskVoxelListThread::BoostMaskGenerateMaskVoxelListThread(
			    int aGlobalBoundingBoxSize0,
			    int aGlobalBoundingBoxSize1,
			    const rttb::VoxelGridIndex3D& aMinIndex,
			    const rttb::VoxelGridIndex3D& aMaxIndex,
			    GeometricInfoPointer aGeometricInfo,
			    const BoostArrayMap& aVoxelizationMap,
			    double aVoxelizationThickness,
			    unsigned int aBeginSlice,
			    unsigned int aEndSlice,
			    MaskVoxelQueuePointer aResultMaskVoxelQueue) :
				_globalBoundingBoxSize0(aGlobalBoundingBoxSize0),
				_globalBoundingBoxSize1(aGlobalBoundingBoxSize1),
				_minIndex(aMinIndex), _maxIndex(aMaxIndex), _geometricInfo(aGeometricInfo),
				_voxelizationMap(aVoxelizationMap), _voxelizationThickness(aVoxelizationThickness),
				_beginSlice(aBeginSlice), _endSlice(aEndSlice),
				_resultMaskVoxelQueue(aResultMaskVoxelQueue)
			{}

			void BoostMaskGenerateMaskVoxelListThread::operator()()
			{
				std::cout << "BoostMaskGenerateMaskVoxelListThread " << _beginSlice << " - " << _endSlice <<
				          ": running" << std::endl;

				for (unsigned int indexZ = _beginSlice; indexZ < _endSlice; ++indexZ)
				{

					//calculate weight vector
					std::map<double, double> weightVectorForZ;
					calcWeightVector(indexZ, weightVectorForZ);

					//For each x,y, calc sum of all voxelization plane, use weight vector
					for (unsigned int x = 0; x < _globalBoundingBoxSize0; ++x)
					{
						for (unsigned int y = 0; y < _globalBoundingBoxSize1; ++y)
						{
							rttb::VoxelGridIndex3D currentIndex;
							currentIndex[0] = x + _minIndex[0];
							currentIndex[1] = y + _minIndex[1];
							currentIndex[2] = indexZ;
							rttb::VoxelGridID gridID;
							_geometricInfo->convert(currentIndex, gridID);
							double volumeFraction = 0;

							std::map<double, double>::iterator it;

							for (it = weightVectorForZ.begin(); it != weightVectorForZ.end(); ++it)
							{
								BoostArrayMap::iterator findVoxelizationIt = _voxelizationMap.find((*it).first);
								double weight = (*it).second;

								if (findVoxelizationIt == _voxelizationMap.end())
								{
									throw rttb::core::InvalidParameterException("Error: The contour plane should be homogeneus!");
								}

								BoostArray2D voxelizationArray = (*findVoxelizationIt).second;
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

				std::cout << "BoostMaskGenerateMaskVoxelListThread" << _beginSlice << " - " << _endSlice <<
				          ": finished" << std::endl;
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