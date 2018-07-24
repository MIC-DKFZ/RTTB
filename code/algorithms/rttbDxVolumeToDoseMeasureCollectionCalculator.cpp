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

#include "rttbDxVolumeToDoseMeasureCollectionCalculator.h"

namespace rttb
{

	namespace algorithms
	{
		DxVolumeToDoseMeasureCollectionCalculator::DxVolumeToDoseMeasureCollectionCalculator(const std::vector<double>& precomputeVolumeValues,
			const VolumeType volume, const std::vector<DoseTypeGy>& doseVector, const std::vector<double>& voxelProportionVector,
			const DoseVoxelVolumeType currentVoxelVolume, const DoseStatisticType minimum, bool multiThreading) :
			VolumeToDoseMeasureCollectionCalculator(precomputeVolumeValues, volume, doseVector, voxelProportionVector, currentVoxelVolume,
				VolumeToDoseMeasureCollection::Dx, multiThreading), _minimum(minimum) {}

		DoseTypeGy DxVolumeToDoseMeasureCollectionCalculator::computeSpecificValue(double xAbsolute) const
		{
			double noOfVoxel = xAbsolute / _currentVoxelVolume;
			DoseTypeGy resultDose = 0;

			double countVoxels = 0;
			bool voxelOverflow = false;
			for (auto i = _doseVector.size() - 1; i != static_cast<size_t>(-1); i--)
			{
				countVoxels += _voxelProportionVector.at(i);

				if (countVoxels >= noOfVoxel)
				{
					voxelOverflow = true;
					resultDose = _doseVector.at(i);
					break;
				}
			}

			if (!voxelOverflow)
			{
				resultDose = _minimum;
			}
			return resultDose;
		}
	}
}