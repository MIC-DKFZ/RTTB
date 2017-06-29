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
//#include <boost/thread/locks.hpp> 

namespace rttb
{

	namespace algorithms
	{
		DoseToVolumeMeasureCollectionCalculator::DoseToVolumeMeasureCollectionCalculator(const std::vector<double>& precomputeDoseValues,
			const DoseTypeGy referenceDose, const core::DoseIteratorInterface::DoseIteratorPointer doseIterator,
			DoseToVolumeMeasureCollection::complexStatistics name, bool multiThreading) : _measureCollection(DoseToVolumeMeasureCollection(name)),
			_precomputeDoseValues(precomputeDoseValues), _referenceDose(referenceDose), _doseIterator(doseIterator), _multiThreading(multiThreading) {}

		void DoseToVolumeMeasureCollectionCalculator::compute()
		{
			computeAdditionalValues(_precomputeDoseValues);
		}
		
		void DoseToVolumeMeasureCollectionCalculator::computeAdditionalValues(const std::vector<double>& value)
		{
			std::vector<boost::thread> threads;

			for (size_t i = 0; i < value.size(); ++i)
			{
				if (value.at(i) > 1 || value.at(i) < 0) {
					throw rttb::core::InvalidParameterException("Values must be between 1 and 0!");
				}
				double xAbsolute = value.at(i) * _referenceDose;
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

			for (unsigned int i = 0; i<threads.size(); i++)
			{
				threads.at(i).join();
			}
		}

		DoseToVolumeMeasureCollection DoseToVolumeMeasureCollectionCalculator::getMeasureCollection()
		{
			return _measureCollection;
		}

		void DoseToVolumeMeasureCollectionCalculator::insertIntoMeasureCollection(DoseTypeGy xAbsolute, VolumeType resultVolume)
		{
			_measureCollection.insertValue(xAbsolute, resultVolume);
		}
	}
}
