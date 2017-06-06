#include "rttbDxVolumeToDoseMeasureCalculator.h"
namespace rttb
{

	namespace algorithms
	{
		DxVolumeToDoseMeasureCalculator::DxVolumeToDoseMeasureCalculator(const std::vector<double>& precomputeVolumeValues, 
			const VolumeType& volume, const std::vector<DoseTypeGy>& doseVector, const std::vector<double>& voxelProportionVector, 
			const DoseVoxelVolumeType& currentVoxelVolume, const DoseStatisticType& minimum, VolumeToDoseMeasure::complexStatistics name) : VolumeToDoseMeasureCalculator(precomputeVolumeValues, volume,
				doseVector, voxelProportionVector, currentVoxelVolume, name), _minimum(minimum) {}

	void rttb::algorithms::DxVolumeToDoseMeasureCalculator::computeSpecificValue(double xAbsolute)
	{
		double noOfVoxel = xAbsolute / _currentVoxelVolume;
		DoseTypeGy resultDose = 0;

		double countVoxels = 0;
		bool voxelOverflow = false;
		for (auto i = _doseVector.size() - 1; i != -1; i--)
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
		insertIntoMeasure(xAbsolute, resultDose);
	}
}
}