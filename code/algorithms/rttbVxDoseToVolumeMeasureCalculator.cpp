#include "rttbVxDoseToVolumeMeasureCalculator.h"
namespace rttb
{

	namespace algorithms
	{
		VxDoseToVolumeMeasureCalculator::VxDoseToVolumeMeasureCalculator(const std::vector<double>& precomputeDoseValues,
			const DoseTypeGy& referenceDose, const core::DoseIteratorInterface::DoseIteratorPointer doseIterator, DoseToVolumeMeasure::complexStatistics name) : 
			DoseToVolumeMeasureCalculator(precomputeDoseValues, referenceDose, doseIterator, name) {}

		void VxDoseToVolumeMeasureCalculator::computeSpecificValue(double xAbsolute)
		{

			rttb::FractionType count = 0;
			_doseIterator->reset();

			DoseTypeGy currentDose = 0;

			while (_doseIterator->isPositionValid())
			{
				currentDose = _doseIterator->getCurrentDoseValue();

				if (currentDose >= xAbsolute)
				{
					count += _doseIterator->getCurrentRelevantVolumeFraction();
				}

				_doseIterator->next();
			}
			insertIntoMeasure(xAbsolute, count * this->_doseIterator->getCurrentVoxelVolume());
		}
	}
}