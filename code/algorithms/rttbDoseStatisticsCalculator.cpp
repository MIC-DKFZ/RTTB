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
// @version $Revision$ (last changed revision)
// @date    $Date$ (last change date)
// @author  $Author$ (last changed by)
*/

#include "rttbDoseStatisticsCalculator.h"

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/thread/locks.hpp> 

#include "rttbNullPointerException.h"
#include "rttbInvalidDoseException.h"
#include "rttbInvalidParameterException.h"

#include <boost/thread/thread.hpp>

namespace rttb
{

	namespace algorithms
	{

		DoseStatisticsCalculator::DoseStatisticsCalculator(DoseIteratorPointer aDoseIterator)
		{
			if (aDoseIterator == nullptr)
			{
				throw core::NullPointerException("DoseIterator must not be nullptr");
			}
			else
			{
				_doseIterator = aDoseIterator;
			}

			_simpleDoseStatisticsCalculated = false;
			_complexDoseStatisticsCalculated = false;

			_multiThreading = false;
			_mutex = ::boost::make_shared<::boost::shared_mutex>();
		}


		DoseStatisticsCalculator::~DoseStatisticsCalculator()
		{

		}

		DoseStatisticsCalculator::DoseIteratorPointer DoseStatisticsCalculator::getDoseIterator() const
		{
			return _doseIterator;
		}


		DoseStatisticsCalculator::DoseStatisticsPointer DoseStatisticsCalculator::calculateDoseStatistics(
			bool computeComplexMeasures, unsigned int maxNumberMinimaPositions,
			unsigned int maxNumberMaximaPositions)
		{
			if (_doseIterator == nullptr)
			{
				throw core::NullPointerException("_doseIterator must not be nullptr!");
			}

			//"simple" dose statistics are mandatory
			calculateSimpleDoseStatistics(maxNumberMinimaPositions, maxNumberMaximaPositions);

			if (computeComplexMeasures)
			{
				//more complex dose statistics are optional with default maximum dose and default relative x values
				calculateComplexDoseStatistics(_statistics->getMaximum(), std::vector<double>(),
					std::vector<double>());
			}
			return _statistics;
		}


		DoseStatisticsCalculator::DoseStatisticsPointer DoseStatisticsCalculator::calculateDoseStatistics(
			DoseTypeGy referenceDose, unsigned int maxNumberMinimaPositions,
			unsigned int maxNumberMaximaPositions)
		{
			if (_doseIterator == nullptr)
			{
				throw core::NullPointerException("_doseIterator must not be nullptr!");
			}

			if (referenceDose <= 0)
			{
				throw rttb::core::InvalidParameterException("Reference dose must be > 0 !");
			}

			//simple dose statistics
			calculateSimpleDoseStatistics(maxNumberMinimaPositions, maxNumberMaximaPositions);


			//more complex dose statistics with given reference dose and default x values
			calculateComplexDoseStatistics(referenceDose, std::vector<double>(), std::vector<double>());

			return _statistics;
		}

		DoseStatisticsCalculator::DoseStatisticsPointer DoseStatisticsCalculator::calculateDoseStatistics(
			const std::vector<double>& precomputeDoseValues,
			const std::vector<double>& precomputeVolumeValues, DoseTypeGy referenceDose,
			unsigned int maxNumberMinimaPositions,
			unsigned int maxNumberMaximaPositions)
		{

			if (_doseIterator == nullptr)
			{
				throw core::NullPointerException("_doseIterator must not be nullptr!");
			}

			//"simple" dose statistics
			calculateSimpleDoseStatistics(maxNumberMinimaPositions, maxNumberMaximaPositions);
			if (referenceDose <= 0)
			{
				//more complex dose statistics with default maximum dose and relative x values
				calculateComplexDoseStatistics(_statistics->getMaximum(), precomputeDoseValues,
					precomputeVolumeValues);
			}
			else
			{
				//more complex dose statistics with given reference dose and relative x values
				calculateComplexDoseStatistics(referenceDose, precomputeDoseValues, precomputeVolumeValues);
			}

			return _statistics;

		}

		void DoseStatisticsCalculator::calculateSimpleDoseStatistics(unsigned int maxNumberMinimaPositions,
			unsigned int maxNumberMaximaPositions)
		{
			_doseVector.clear();
			_voxelProportionVector.clear();

			std::multimap<double, int> doseValueVSIndexMap;
			std::vector<double> voxelProportionVectorTemp;


			DoseStatisticType maximumDose = 0;
			DoseStatisticType minimumDose = std::numeric_limits<DoseStatisticType>::max();
			DoseStatisticType meanDose;
			DoseStatisticType stdDeviationDose;


			DoseTypeGy sum = 0;
			VolumeType numVoxels = 0.0;
			DoseTypeGy squareSum = 0;
			VolumeType volume = 0;

			_doseIterator->reset();
			int i = 0;
			DoseTypeGy doseValue = 0;

			while (_doseIterator->isPositionValid())
			{
				doseValue = _doseIterator->getCurrentDoseValue();

				if (i == 0)
				{
					minimumDose = doseValue;
					volume = _doseIterator->getCurrentVoxelVolume();
				}

				rttb::FractionType voxelProportion = _doseIterator->getCurrentRelevantVolumeFraction();
				sum += doseValue * voxelProportion;

				numVoxels += voxelProportion;
				squareSum += doseValue * doseValue * voxelProportion;

				if (doseValue > maximumDose)
				{
					maximumDose = doseValue;
				}
				else if (doseValue < minimumDose)
				{
					minimumDose = doseValue;
				}

				voxelProportionVectorTemp.push_back(voxelProportion);
				doseValueVSIndexMap.insert(std::pair<double, int>(doseValue, i));

				i++;
				_doseIterator->next();
			}

			if (numVoxels != 0)
			{
				meanDose = sum / numVoxels;

				//standard deviation is defined only for n>=2
				if (numVoxels >= 2)
				{
					//uncorrected variance is calculated
					DoseStatisticType varianceDose = (squareSum / numVoxels - meanDose * meanDose);

					if (varianceDose < errorConstant)
					{
						stdDeviationDose = 0;
					}
					else
					{
						stdDeviationDose = pow(varianceDose, 0.5);
					}
				}
				else
				{
					stdDeviationDose = 0;
				}

			}

			//sort dose values and corresponding volume fractions in member variables
			for (auto & it : doseValueVSIndexMap)
			{
				_doseVector.push_back((float)it.first);
				_voxelProportionVector.push_back(voxelProportionVectorTemp.at(it.second));
			}

			volume *= numVoxels;

			_statistics = boost::make_shared<DoseStatistics>(minimumDose, maximumDose, meanDose,
				stdDeviationDose, numVoxels,
				volume);

			_simpleDoseStatisticsCalculated = true;

			ResultListPointer minimumVoxelPositions = computeMinimumPositions(maxNumberMinimaPositions);
			ResultListPointer maximumVoxelPositions = computeMaximumPositions(maxNumberMaximaPositions);

			_statistics->setMinimumVoxelPositions(minimumVoxelPositions);
			_statistics->setMaximumVoxelPositions(maximumVoxelPositions);
		}


		void DoseStatisticsCalculator::calculateComplexDoseStatistics(DoseTypeGy referenceDose,
			const std::vector<double>& precomputeDoseValues,
			const std::vector<double>& precomputeVolumeValues)
		{
			if (!_simpleDoseStatisticsCalculated)
			{
				throw core::InvalidDoseException("simple DoseStatistics have to be computed in order to call calculateComplexDoseStatistics()");
			}

			std::vector<double> precomputeDoseValuesNonConst = precomputeDoseValues;
			std::vector<double> precomputeVolumeValuesNonConst = precomputeVolumeValues;

			//set default values
			if (precomputeDoseValues.empty())
			{
				std::vector<double> defaultPrecomputeDoseValues = boost::assign::list_of(0.02)(0.05)(0.1)(0.9)(
					0.95)(0.98);
				precomputeDoseValuesNonConst = defaultPrecomputeDoseValues;
			}

			if (precomputeVolumeValues.empty())
			{
				std::vector<double> defaultPrecomputeVolumeValues = boost::assign::list_of(0.02)(0.05)(0.1)(0.9)(
					0.95)(0.98);
				precomputeVolumeValuesNonConst = defaultPrecomputeVolumeValues;
			}

			_Vx = ::boost::make_shared<VxDoseToVolumeMeasureCollectionCalculator>(precomputeDoseValuesNonConst, referenceDose, _doseIterator);
			_Vx->compute();

			_Dx = ::boost::make_shared<DxVolumeToDoseMeasureCollectionCalculator>(precomputeVolumeValuesNonConst, _statistics->getVolume(),
				this->_doseVector, this->_voxelProportionVector, this->_doseIterator->getCurrentVoxelVolume(), _statistics->getMinimum());
			_Dx->compute();

			_MOHx = ::boost::make_shared<MOHxVolumeToDoseMeasureCollectionCalculator>(precomputeVolumeValuesNonConst, _statistics->getVolume(),
				this->_doseVector, this->_voxelProportionVector, this->_doseIterator->getCurrentVoxelVolume());
			_MOHx->compute();

			_MOCx = ::boost::make_shared<MOCxVolumeToDoseMeasureCollectionCalculator>(precomputeVolumeValuesNonConst, _statistics->getVolume(),
				this->_doseVector, this->_voxelProportionVector, this->_doseIterator->getCurrentVoxelVolume());
			_MOCx->compute();

			_MaxOHx = ::boost::make_shared<MaxOHxVolumeToDoseMeasureCollectionCalculator>(precomputeVolumeValuesNonConst, _statistics->getVolume(),
				this->_doseVector, this->_voxelProportionVector, this->_doseIterator->getCurrentVoxelVolume());
			_MaxOHx->compute();

			_MinOCx = ::boost::make_shared<MinOCxVolumeToDoseMeasureCollectionCalculator>(precomputeVolumeValuesNonConst, _statistics->getVolume(),
				this->_doseVector, this->_voxelProportionVector, this->_doseIterator->getCurrentVoxelVolume(), _statistics->getMinimum(), _statistics->getMaximum());
			_MinOCx->compute();

			_statistics->setVx(_Vx->getMeasureCollection());
			_statistics->setDx(_Dx->getMeasureCollection());
			_statistics->setMOHx(_MOHx->getMeasureCollection());
			_statistics->setMOCx(_MOCx->getMeasureCollection());
			_statistics->setMaxOHx(_MaxOHx->getMeasureCollection());
			_statistics->setMinOCx(_MinOCx->getMeasureCollection());
			_statistics->setReferenceDose(referenceDose);
			_complexDoseStatisticsCalculated = true;
		}

		void DoseStatisticsCalculator::addPrecomputeValues(const std::vector<double>& values)
		{
			if (!_complexDoseStatisticsCalculated)
			{
				throw core::InvalidDoseException("Complex DoseStatistics have to be computed in order to call addPrecomputeDoseValues()");
			}
			_Vx->addPrecomputeDoseValues(values);
			_Dx->addPrecomputeVolumeValues(values);
			_MOHx->addPrecomputeVolumeValues(values);
			_MOCx->addPrecomputeVolumeValues(values);
			_MaxOHx->addPrecomputeVolumeValues(values);
			_MinOCx->addPrecomputeVolumeValues(values);
		}
		void DoseStatisticsCalculator::recalculateDoseStatistics()
		{
			if (!_complexDoseStatisticsCalculated)
			{
				throw core::InvalidDoseException("Complex DoseStatistics have to be computed in order to call recalculateDoseStatistics()");
			}
			_Vx->compute();			
			_Dx->compute();
			_MOHx->compute();
			_MOCx->compute();
			_MaxOHx->compute();
			_MinOCx->compute();
		}

		DoseStatisticsCalculator::ResultListPointer DoseStatisticsCalculator::computeMaximumPositions(
			unsigned int maxNumberMaxima) const
		{
			if (!_simpleDoseStatisticsCalculated)
			{
				throw core::InvalidDoseException("simple DoseStatistics have to be computed in order to call computeMaximumPositions()");
			}

			ResultListPointer maxVoxelVector =
				boost::make_shared<std::vector<std::pair<DoseTypeGy, VoxelGridID> > >();

			unsigned int count = 0;
			this->_doseIterator->reset();
			DoseTypeGy doseValue = 0;

			while (_doseIterator->isPositionValid() && count < maxNumberMaxima)
			{
				doseValue = _doseIterator->getCurrentDoseValue();

				if (doseValue == _statistics->getMaximum())
				{
					VoxelGridID currentID = _doseIterator->getCurrentVoxelGridID();
					std::pair<DoseTypeGy, VoxelGridID> voxel(doseValue, currentID);
					maxVoxelVector->push_back(voxel);
					count++;
				}

				_doseIterator->next();
			}

			return maxVoxelVector;
		}

		DoseStatisticsCalculator::ResultListPointer DoseStatisticsCalculator::computeMinimumPositions(
			unsigned int maxNumberMinima) const
		{
			if (!_simpleDoseStatisticsCalculated)
			{
				throw core::InvalidDoseException("simple DoseStatistics have to be computed in order to call computeMinimumPositions()");

			}

			ResultListPointer minVoxelVector =
				boost::make_shared<std::vector<std::pair<DoseTypeGy, VoxelGridID> > >();

			/*! @todo: Architecture Annotation:
				Finding the positions for the minimum only once reduces computation time,
				but will require sensible use by the programmers. To be save the output vector
				minVoxelVector will be always cleared here to garantee that no false values are
				presented. This change may be revoced to increase computation speed later on
				(only compute if(minVoxelVector->size()==0)).
				*/
			unsigned int count = 0;
			this->_doseIterator->reset();
			DoseTypeGy doseValue = 0;

			while (_doseIterator->isPositionValid() && count < maxNumberMinima)
			{
				doseValue = _doseIterator->getCurrentDoseValue();

				if (doseValue == _statistics->getMinimum())
				{
					VoxelGridID currentID = _doseIterator->getCurrentVoxelGridID();
					std::pair<DoseTypeGy, VoxelGridID> voxel(doseValue, currentID);
					minVoxelVector->push_back(voxel);
					count++;
				}

				_doseIterator->next();
			}

			return minVoxelVector;
		}

		void DoseStatisticsCalculator::setMultiThreading(const bool choice) 
		{
			_multiThreading = choice;
		}

	}//end namespace algorithms
}//end namespace rttb

