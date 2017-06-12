#include "rttbMaxOHxVolumeToDoseMeasureCalculator.h"
namespace rttb
{

	namespace algorithms
	{
		MaxOHxVolumeToDoseMeasureCalculator::MaxOHxVolumeToDoseMeasureCalculator(const std::vector<double>& precomputeVolumeValues,
			const VolumeType volume, const std::vector<DoseTypeGy>& doseVector, const std::vector<double>& voxelProportionVector,
			const DoseVoxelVolumeType currentVoxelVolume, VolumeToDoseMeasure::complexStatistics name) : VolumeToDoseMeasureCalculator(precomputeVolumeValues, volume,
				doseVector, voxelProportionVector, currentVoxelVolume, name) {}

		DoseTypeGy MaxOHxVolumeToDoseMeasureCalculator::computeSpecificValue(double xAbsolute) const
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