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

#include "rttbDoseToVolumeMeasureCollectionCalculator.h"
#include <boost/thread/thread.hpp>
#include "rttbInvalidParameterException.h"
#include "rttbUtils.h"

#include <boost/make_shared.hpp>

//#include <boost/thread/locks.hpp> 

namespace rttb
{

	namespace algorithms
	{
		DoseToVolumeMeasureCollectionCalculator::DoseToVolumeMeasureCollectionCalculator(const std::vector<double>& precomputeDoseValues,
			const DoseTypeGy referenceDose, const core::DoseIteratorInterface::DoseIteratorPointer doseIterator,
			DoseToVolumeMeasureCollection::complexStatistics name, bool multiThreading) : _doseIterator(doseIterator), _referenceDose(referenceDose), _measureCollection(::boost::make_shared<DoseToVolumeMeasureCollection>(name)),
			  _multiThreading(multiThreading)
        {
            addPrecomputeDoseValues(precomputeDoseValues);
        }

		void DoseToVolumeMeasureCollectionCalculator::compute()
		{
			std::vector<boost::thread> threads;

			for (size_t i = 0; i < _precomputeDoseValues.size(); ++i)
			{
				double xAbsolute = _precomputeDoseValues.at(i) * _referenceDose;
				if (!rttb::core::isKey(_measureCollection->getAllValues(), xAbsolute)) {
					if (_multiThreading)
					{
						throw rttb::core::InvalidParameterException("MultiThreading is not implemented yet.");
						//threads.push_back(boost::thread(&DoseToVolumeMeasureCollectionCalculator::insertIntoMeasureCollection, this , xAbsolute, computeSpecificValue(xAbsolute)));
					}
					else
					{
						insertIntoMeasureCollection(xAbsolute, this->computeSpecificValue(xAbsolute));
					}
				}
			}

			for (unsigned int i = 0; i<threads.size(); i++)
			{
				threads.at(i).join();
			}
		}
		
		void DoseToVolumeMeasureCollectionCalculator::addPrecomputeDoseValues(const std::vector<double>& values)
		{
			for (size_t i = 0; i < values.size(); ++i) {
				if (values.at(i) > 1 || values.at(i) < 0) {
					throw rttb::core::InvalidParameterException("Values must be between 1 and 0!");
				}
				if (!rttb::core::isKey(_precomputeDoseValues, values.at(i))) {
					_precomputeDoseValues.push_back(values.at(i));
				}
			}
		}

		DoseStatistics::DoseToVolumeMeasureCollectionPointer DoseToVolumeMeasureCollectionCalculator::getMeasureCollection()
		{
			return _measureCollection;
		}

		void DoseToVolumeMeasureCollectionCalculator::insertIntoMeasureCollection(DoseTypeGy xAbsolute, VolumeType resultVolume)
		{
			_measureCollection->insertValue(xAbsolute, resultVolume);
		}
	}
}
