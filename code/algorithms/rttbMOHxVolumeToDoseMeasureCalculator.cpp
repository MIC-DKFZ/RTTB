#include "rttbMOHxVolumeToDoseMeasureCalculator.h"
namespace rttb
{

	namespace algorithms
	{
		MOHxVolumeToDoseMeasureCalculator::MOHxVolumeToDoseMeasureCalculator(const std::vector<double>& precomputeVolumeValues,
			const VolumeType volume, const std::vector<DoseTypeGy>& doseVector, const std::vector<double>& voxelProportionVector,
			const DoseVoxelVolumeType currentVoxelVolume, VolumeToDoseMeasure::complexStatistics name) : VolumeToDoseMeasureCalculator(precomputeVolumeValues, volume,
				doseVector, voxelProportionVector, currentVoxelVolume, name) {}

		DoseTypeGy MOHxVolumeToDoseMeasureCalculator::computeSpecificValue(double xAbsolute) const
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

				for (auto i = _doseVector.size() - 1; i != -1; i--)
				{
					double voxelProportion = _voxelProportionVector.at(i);
					countVoxels += voxelProportion;
					sum += _doseVector.at(i) * voxelProportion;

					if (countVoxels >= noOfVoxel)
					{
						break;
					}
				}
				return (DoseTypeGy)(sum / noOfVoxel);
			}
		}
	}
}