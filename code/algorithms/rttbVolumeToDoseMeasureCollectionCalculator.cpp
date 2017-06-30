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

#include "rttbVolumeToDoseMeasureCollectionCalculator.h"
#include <boost/thread/thread.hpp>
#include "rttbInvalidParameterException.h"

#include <boost/make_shared.hpp>
//#include <boost/thread/locks.hpp> 

namespace rttb
{

	namespace algorithms
	{
		VolumeToDoseMeasureCollectionCalculator::VolumeToDoseMeasureCollectionCalculator(const std::vector<double>& precomputeVolumeValues, const VolumeType volume,
			const std::vector<DoseTypeGy>& doseVector, const std::vector<double>& voxelProportionVector, const DoseVoxelVolumeType currentVoxelVolume,
			VolumeToDoseMeasureCollection::complexStatistics name, bool multiThreading) : _measureCollection(::boost::make_shared<VolumeToDoseMeasureCollection>(name)), _precomputeVolumeValues(precomputeVolumeValues),
			_volume(volume), _doseVector(doseVector), _voxelProportionVector(voxelProportionVector), _currentVoxelVolume(currentVoxelVolume), _multiThreading(multiThreading) {}

		void VolumeToDoseMeasureCollectionCalculator::compute()
		{
			std::vector<boost::thread> threads;

			for (size_t i = 0; i < _precomputeVolumeValues.size(); ++i)
			{
				double xAbsolute = _precomputeVolumeValues.at(i) * _volume;
				if (!_measureCollection->isInCollection(xAbsolute)) {
					if (_multiThreading)
					{
						throw rttb::core::InvalidParameterException("MultiThreading is not implemented yet.");
						//threads.push_back(boost::thread(&VolumeToDoseMeasureCollectionCalculator::insertIntoMeasureCollection, this, xAbsolute, computeSpecificValue(xAbsolute)));
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
		void VolumeToDoseMeasureCollectionCalculator::addPrecomputeVolumeValues(const std::vector<double>& values)
		{
			for (size_t i = 0; i < values.size(); ++i) {
				if (values.at(i) > 1 || values.at(i) < 0) {
					throw rttb::core::InvalidParameterException("Values must be between 1 and 0!");
				}
				if (!isInVector(values.at(i))) {
					_precomputeVolumeValues.push_back(values.at(i));
				}				
			}
		}

		bool VolumeToDoseMeasureCollectionCalculator::isInVector(double value)
		{
			double doubleImprecision = 1e-5;
			for (size_t i = 0; i < _precomputeVolumeValues.size(); ++i) {
				if (_precomputeVolumeValues.at(i) - doubleImprecision <= value && value <= _precomputeVolumeValues.at(i) + doubleImprecision)
				{
					return true;
				}
			}
			return false;
		}

		DoseStatistics::VolumeToDoseMeasureCollectionPointer VolumeToDoseMeasureCollectionCalculator::getMeasureCollection()
		{
			return _measureCollection;
		}

		void VolumeToDoseMeasureCollectionCalculator::insertIntoMeasureCollection(VolumeType  xAbsolute, DoseTypeGy resultDose)
		{
			_measureCollection->insertValue(xAbsolute, resultDose);
		}
	}
}
