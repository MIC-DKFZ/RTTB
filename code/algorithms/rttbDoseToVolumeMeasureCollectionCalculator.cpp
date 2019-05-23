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

#include "rttbDoseToVolumeMeasureCollectionCalculator.h"

#include <thread>

#include "rttbInvalidParameterException.h"
#include "rttbUtils.h"

#include <boost/make_shared.hpp>

namespace rttb
{

	namespace algorithms
	{
		DoseToVolumeMeasureCollectionCalculator::DoseToVolumeMeasureCollectionCalculator(const std::vector<double>& precomputeDoseValues,
			const DoseTypeGy referenceDose, const core::DoseIteratorInterface::Pointer doseIterator,
			DoseToVolumeMeasureCollection::complexStatistics name, bool multiThreading) : _doseIterator(doseIterator), _referenceDose(referenceDose), _measureCollection(::boost::make_shared<DoseToVolumeMeasureCollection>(name)),
			  _multiThreading(multiThreading)
        {
            addPrecomputeDoseValues(precomputeDoseValues);
        }

		void DoseToVolumeMeasureCollectionCalculator::compute()
		{
			std::vector<std::thread> threads;

			for (double _precomputeDoseValue : _precomputeDoseValues)
			{
				double xAbsolute = _precomputeDoseValue * _referenceDose;
				if (!rttb::core::isKey(_measureCollection->getAllValues(), xAbsolute)) {
					if (_multiThreading)
					{
						throw rttb::core::InvalidParameterException("MultiThreading is not implemented yet.");
					}
					else
					{
						insertIntoMeasureCollection(xAbsolute, this->computeSpecificValue(xAbsolute));
					}
				}
			}

			for (auto & thread : threads)
			{
				thread.join();
			}
		}
		
		void DoseToVolumeMeasureCollectionCalculator::addPrecomputeDoseValues(const std::vector<double>& values)
		{
			for (double value : values) {
				if (value > 1 || value < 0) {
					throw rttb::core::InvalidParameterException("Values must be between 1 and 0!");
				}
				if (!rttb::core::isKey(_precomputeDoseValues, value)) {
					_precomputeDoseValues.push_back(value);
				}
			}
		}

		DoseToVolumeMeasureCollection::Pointer DoseToVolumeMeasureCollectionCalculator::getMeasureCollection()
		{
			return _measureCollection;
		}

		void DoseToVolumeMeasureCollectionCalculator::insertIntoMeasureCollection(DoseTypeGy xAbsolute, VolumeType resultVolume)
		{
			_measureCollection->insertValue(xAbsolute, resultVolume);
		}
	}
}
