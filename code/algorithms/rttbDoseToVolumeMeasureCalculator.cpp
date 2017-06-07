#include "rttbDoseToVolumeMeasureCalculator.h"
#include <boost/thread/thread.hpp>
//#include <boost/thread/locks.hpp> 

namespace rttb
{

	namespace algorithms
	{
		DoseToVolumeMeasureCalculator::DoseToVolumeMeasureCalculator(const std::vector<double>& precomputeVolumeValues, const DoseTypeGy& referenceDose, 
			const core::DoseIteratorInterface::DoseIteratorPointer doseIterator, DoseToVolumeMeasure::complexStatistics name) : 
			measure(DoseToVolumeMeasure(name)), _precomputeVolumeValues(precomputeVolumeValues),
			_referenceDose(referenceDose), _doseIterator(doseIterator) {}

		void DoseToVolumeMeasureCalculator::compute()
		{
			std::vector<boost::thread> threads;

			for (size_t i = 0; i < _precomputeVolumeValues.size(); ++i)
			{
				if (false)//_multiThreading)
				{
					threads.push_back(boost::thread(&DoseToVolumeMeasureCalculator::computeSpecificValue, this, _precomputeVolumeValues.at(i) * _referenceDose));
				}
				else
				{
					this->computeSpecificValue(_precomputeVolumeValues.at(i) * _referenceDose);
				}
			}

			for (unsigned int i = 0; i<threads.size(); i++)
			{
				threads.at(i).join();
			}
		}

		DoseToVolumeMeasure DoseToVolumeMeasureCalculator::getMeasure()
		{
			return measure;
		}

		void DoseToVolumeMeasureCalculator::insertIntoMeasure(DoseTypeGy xAbsolute, VolumeType resultVolume)
		{
			measure.insertValue(std::pair<DoseTypeGy, VolumeType>(xAbsolute, resultVolume));
		}
	}
}
