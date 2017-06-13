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

#include "rttbMaxOHxVolumeToDoseMeasureCollectionCalculator.h"

namespace rttb
{

	namespace algorithms
	{
		MaxOHxVolumeToDoseMeasureCollectionCalculator::MaxOHxVolumeToDoseMeasureCollectionCalculator(const std::vector<double>& precomputeVolumeValues,
			const VolumeType volume, const std::vector<DoseTypeGy>& doseVector, const std::vector<double>& voxelProportionVector,
			const DoseVoxelVolumeType currentVoxelVolume, VolumeToDoseMeasureCollection::complexStatistics name) : VolumeToDoseMeasureCollectionCalculator(precomputeVolumeValues, volume,
				doseVector, voxelProportionVector, currentVoxelVolume, name) {}

		DoseTypeGy MaxOHxVolumeToDoseMeasureCollectionCalculator::computeSpecificValue(double xAbsolute) const
		{
			double noOfVoxel = xAbsolute / _currentVoxelVolume;
			DoseTypeGy resultDose = 0;

			double countVoxels = 0;
			for (auto i = _doseVector.size() - 1; i != -1; i--)
			{
				countVoxels += _voxelProportionVector.at(i);

				if (countVoxels >= noOfVoxel)
				{
					if (i > 0)
					{
						resultDose = _doseVector.at(i - 1);
					}
					break;
				}
			}

			return resultDose;
		}
	}
}