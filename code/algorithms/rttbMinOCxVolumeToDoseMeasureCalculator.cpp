#include "rttbMinOCxVolumeToDoseMeasureCalculator.h"
namespace rttb
{

	namespace algorithms
	{
		MinOCxVolumeToDoseMeasureCalculator::MinOCxVolumeToDoseMeasureCalculator(const std::vector<double>& precomputeVolumeValues,
			const VolumeType volume, const std::vector<DoseTypeGy>& doseVector, const std::vector<double>& voxelProportionVector,
			const DoseVoxelVolumeType currentVoxelVolume, const DoseStatisticType minimum, const DoseStatisticType maximum, VolumeToDoseMeasure::complexStatistics name) : VolumeToDoseMeasureCalculator(precomputeVolumeValues, volume,
				doseVector, voxelProportionVector, currentVoxelVolume, name), _minimum(minimum), _maximum(maximum) {}

		DoseTypeGy MinOCxVolumeToDoseMeasureCalculator::computeSpecificValue(double xAbsolute) const
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