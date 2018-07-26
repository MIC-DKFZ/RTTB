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

#include "rttbVolumeToDoseMeasureCollectionCalculator.h"
#include <boost/thread/thread.hpp>
#include "rttbInvalidParameterException.h"
#include "rttbUtils.h"

#include <boost/make_shared.hpp>
//#include <boost/thread/locks.hpp> 

namespace rttb
{

	namespace algorithms
	{
		VolumeToDoseMeasureCollectionCalculator::VolumeToDoseMeasureCollectionCalculator(const std::vector<double>& precomputeVolumeValues, const VolumeType volume,
			const std::vector<DoseTypeGy>& doseVector, const std::vector<double>& voxelProportionVector, const DoseVoxelVolumeType currentVoxelVolume,
			VolumeToDoseMeasureCollection::complexStatistics name, bool multiThreading) : _doseVector(doseVector), _currentVoxelVolume(currentVoxelVolume), _voxelProportionVector(voxelProportionVector),
			_volume(volume), _measureCollection(::boost::make_shared<VolumeToDoseMeasureCollection>(name)),  _multiThreading(multiThreading)
        {
            addPrecomputeVolumeValues(precomputeVolumeValues);
        }

		void VolumeToDoseMeasureCollectionCalculator::compute()
		{
			std::vector<boost::thread> threads;

			for (double _precomputeVolumeValue : _precomputeVolumeValues)
			{
				double xAbsolute = _precomputeVolumeValue * _volume;
				if (!rttb::core::isKey(_measureCollection->getAllValues(), xAbsolute)) {
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

			for (auto & thread : threads)
			{
				thread.join();
			}
		}
		void VolumeToDoseMeasureCollectionCalculator::addPrecomputeVolumeValues(const std::vector<double>& values)
		{
			for (double value : values) {
				if (value > 1 || value < 0) {
					throw rttb::core::InvalidParameterException("Values must be between 1 and 0!");
				}
				if (!rttb::core::isKey(_precomputeVolumeValues, value)) {
					_precomputeVolumeValues.push_back(value);
				}				
			}
		}

		VolumeToDoseMeasureCollection::Pointer VolumeToDoseMeasureCollectionCalculator::getMeasureCollection()
		{
			return _measureCollection;
		}

		void VolumeToDoseMeasureCollectionCalculator::insertIntoMeasureCollection(VolumeType  xAbsolute, DoseTypeGy resultDose)
		{
			_measureCollection->insertValue(xAbsolute, resultDose);
		}
	}
}
