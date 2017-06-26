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

#include "rttbMinOCxVolumeToDoseMeasureCollectionCalculator.h"

namespace rttb
{

	namespace algorithms
	{
		MinOCxVolumeToDoseMeasureCollectionCalculator::MinOCxVolumeToDoseMeasureCollectionCalculator(const std::vector<double>& precomputeVolumeValues,
			const VolumeType volume, const std::vector<DoseTypeGy>& doseVector, const std::vector<double>& voxelProportionVector,
			const DoseVoxelVolumeType currentVoxelVolume, const DoseStatisticType minimum, const DoseStatisticType maximum, 
			bool multiThreading) : VolumeToDoseMeasureCollectionCalculator(precomputeVolumeValues, volume,
				doseVector, voxelProportionVector, currentVoxelVolume, VolumeToDoseMeasureCollection::MinOCx, 
				multiThreading), _minimum(minimum), _maximum(maximum) {}

		DoseTypeGy MinOCxVolumeToDoseMeasureCollectionCalculator::computeSpecificValue(double xAbsolute) const
		{
			double noOfVoxel = xAbsolute / _currentVoxelVolume;
			DoseTypeGy resultDose = 0;

			double countVoxels = 0;
			std::vector<DoseTypeGy>::const_iterator it = _doseVector.begin();
			std::vector<double>::const_iterator itD = _voxelProportionVector.begin();

			for (; itD != _voxelProportionVector.end(); ++itD, ++it)
			{
				countVoxels += *itD;

				if (countVoxels >= noOfVoxel)
				{
					break;
				}
			}

			if (it != _doseVector.end())
			{
				++it;

				if (it != _doseVector.end())
				{
					resultDose = *it;
				}
				else
				{
					resultDose = (DoseTypeGy)_maximum;
				}
			}
			else
			{
				resultDose = (DoseTypeGy)_minimum;
			}
			return resultDose;
		}
	}
}