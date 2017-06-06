#include "rttbVolumeToDoseMeasureCalculator.h"
#include <boost/thread/thread.hpp>
//#include <boost/thread/locks.hpp> 

namespace rttb
{

	namespace algorithms
	{
		VolumeToDoseMeasureCalculator::VolumeToDoseMeasureCalculator(const std::vector<double>& precomputeVolumeValues, const VolumeType& volume,
			const std::vector<DoseTypeGy>& doseVector, const std::vector<double>& voxelProportionVector, const DoseVoxelVolumeType& currentVoxelVolume, 
			VolumeToDoseMeasure::complexStatistics name) : measure(VolumeToDoseMeasure(name)), _precomputeVolumeValues(precomputeVolumeValues),
			_volume(volume), _doseVector(doseVector), _voxelProportionVector(voxelProportionVector), _currentVoxelVolume(currentVoxelVolume) {}

		void VolumeToDoseMeasureCalculator::compute()
		{
			std::vector<boost::thread> threads;

			for (size_t i = 0; i < _precomputeVolumeValues.size(); ++i)
			{
				if (false)//_multiThreading)
				{
					threads.push_back(boost::thread(&VolumeToDoseMeasureCalculator::computeSpecificValue, this, _precomputeVolumeValues.at(i) * _volume));
				}
				else
				{
					this->computeSpecificValue(_precomputeVolumeValues.at(i) * _volume);
				}
			}

			for (unsigned int i = 0; i<threads.size(); i++)
			{
				threads.at(i).join();
			}
		}

		VolumeToDoseMeasure VolumeToDoseMeasureCalculator::getMeasure()
		{
			return measure;
		}
		void VolumeToDoseMeasureCalculator::insertIntoMeasure(VolumeType  xAbsolute, DoseTypeGy resultDose)
		{
			measure.insertValue(std::pair<VolumeType, DoseTypeGy>(xAbsolute, resultDose));
		}
	}
}
