#include "rttbMOCxVolumeToDoseMeasureCalculator.h"
namespace rttb
{

	namespace algorithms
	{
		MOCxVolumeToDoseMeasureCalculator::MOCxVolumeToDoseMeasureCalculator(const std::vector<double>& precomputeVolumeValues,
			const VolumeType volume, const std::vector<DoseTypeGy>& doseVector, const std::vector<double>& voxelProportionVector,
			const DoseVoxelVolumeType currentVoxelVolume, VolumeToDoseMeasure::complexStatistics name) : VolumeToDoseMeasureCalculator(precomputeVolumeValues, volume,
				doseVector, voxelProportionVector, currentVoxelVolume, name) {}

		DoseTypeGy MOCxVolumeToDoseMeasureCalculator::computeSpecificValue(double xAbsolute) const
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
				std::vector<DoseTypeGy>::const_iterator it = _doseVector.begin();
				std::vector<double>::const_iterator itD = _voxelProportionVector.begin();

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
				return (DoseTypeGy)(sum / noOfVoxel);
			}
		}
	}
}