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

#include "rttbMOCxVolumeToDoseMeasureCollectionCalculator.h"

namespace rttb
{

	namespace algorithms
	{
		MOCxVolumeToDoseMeasureCollectionCalculator::MOCxVolumeToDoseMeasureCollectionCalculator(const std::vector<double>& precomputeVolumeValues,
			const VolumeType volume, const std::vector<DoseTypeGy>& doseVector, const std::vector<double>& voxelProportionVector,
			const DoseVoxelVolumeType currentVoxelVolume, bool multiThreading) : VolumeToDoseMeasureCollectionCalculator(precomputeVolumeValues, volume,
				doseVector, voxelProportionVector, currentVoxelVolume, VolumeToDoseMeasureCollection::MOCx, multiThreading) {}

		DoseTypeGy MOCxVolumeToDoseMeasureCollectionCalculator::computeSpecificValue(double xAbsolute) const
		{
			double noOfVoxel = xAbsolute / _currentVoxelVolume;
			if (noOfVoxel == 0)
			{
				return 0;
			}
			else
			{
				double countVoxels = 0;
				double sum = 0;
				auto it = _doseVector.begin();
				auto itD = _voxelProportionVector.begin();

				for (; it != _doseVector.end(); ++it, ++itD)
				{
					double voxelProportion = *itD;
					countVoxels += voxelProportion;
					sum += (*it) * voxelProportion;

					if (countVoxels >= noOfVoxel)
					{
						break;
					}
				}
				return static_cast<DoseTypeGy>(sum / noOfVoxel);
			}
		}
	}
}