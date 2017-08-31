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
// @version $Revision: 1674 $ (last changed revision)
// @date    $Date: 2017-01-27 10:34:46 +0100 (Fr, 27 Jan 2017) $ (last change date)
// @author  $Author: hentsch $ (last changed by)
*/

#include "rttbMOHxVolumeToDoseMeasureCollectionCalculator.h"

namespace rttb
{

	namespace algorithms
	{
		MOHxVolumeToDoseMeasureCollectionCalculator::MOHxVolumeToDoseMeasureCollectionCalculator(const std::vector<double>& precomputeVolumeValues,
			const VolumeType volume, const std::vector<DoseTypeGy>& doseVector, const std::vector<double>& voxelProportionVector,
			const DoseVoxelVolumeType currentVoxelVolume, bool multiThreading) :
			VolumeToDoseMeasureCollectionCalculator(precomputeVolumeValues, volume, doseVector, voxelProportionVector, currentVoxelVolume,
				VolumeToDoseMeasureCollection::MOHx, multiThreading) {}

		DoseTypeGy MOHxVolumeToDoseMeasureCollectionCalculator::computeSpecificValue(double xAbsolute) const
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

				for (auto i = _doseVector.size() - 1; i!=static_cast<size_t>(-1); i--)
				{
					double voxelProportion = _voxelProportionVector.at(i);
					countVoxels += voxelProportion;
					sum += _doseVector.at(i) * voxelProportion;

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