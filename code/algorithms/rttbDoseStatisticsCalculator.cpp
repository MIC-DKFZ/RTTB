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
// @version $Revision: 877 $ (last changed revision)
// @date    $Date: 2015-01-09 10:51:10 +0100 (Fr, 09 Jan 2015) $ (last change date)
// @author  $Author: hentsch $ (last changed by)
*/

#include "rttbDoseStatisticsCalculator.h"

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/assign/list_of.hpp>

#include "rttbNullPointerException.h"
#include "rttbInvalidDoseException.h"
#include "rttbInvalidParameterException.h"

namespace rttb
{

	namespace algorithms
	{
		DoseStatisticsCalculator::DoseStatisticsCalculator()
		{
			simpleDoseStatisticsCalculated = false;
			_doseIterator = NULL;
		}

		DoseStatisticsCalculator::DoseStatisticsCalculator(DoseIteratorPointer aDoseIterator)
		{
			setDoseIterator(aDoseIterator);
			simpleDoseStatisticsCalculated = false;
		}


		DoseStatisticsCalculator::~DoseStatisticsCalculator()
		{

		}


		void DoseStatisticsCalculator::setDoseIterator(DoseIteratorPointer aDoseIterator)
		{
			if (aDoseIterator == NULL)
			{
				throw core::NullPointerException("DoseIterator must not be NULL");
			}
			else
			{
				_doseIterator = aDoseIterator;
			}
		}

		DoseStatisticsCalculator::DoseIteratorPointer DoseStatisticsCalculator::getDoseIterator() const
		{
			return _doseIterator;
		}


		DoseStatisticsCalculator::DoseStatisticsPointer DoseStatisticsCalculator::calculateDoseStatistics(
		    bool computeComplexMeasures, const std::vector<double>& precomputeDoseValues,
		    const std::vector<double>& precomputeVolumeValues)
		{

			if (!_doseIterator)
			{
				throw core::NullPointerException("_doseIterator must not be NULL!");
			}

			//"simple" dose statistics are obligatory
			calculateSimpleDoseStatistics();

			if (computeComplexMeasures)
			{
				//more complex dose statistics are optional
				calculateComplexDoseStatistics(precomputeDoseValues, precomputeVolumeValues);
			}

			return _statistics;

		}


		void DoseStatisticsCalculator::calculateSimpleDoseStatistics()
		{
			_doseVector.clear();
			_voxelProportionVector.clear();

			std::multimap<double, int> doseValueVSIndexMap;
			std::vector<double> voxelProportionVectorTemp;


			DoseStatisticType maximumDose = 0;
			DoseStatisticType minimumDose = 0;
			DoseStatisticType meanDose = 0;
			DoseStatisticType stdDeviationDose = 0;


			float sum = 0;
			rttb::DoseStatisticType numVoxels = 0.0;
			float squareSum = 0;
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
			std::multimap<double, int>::iterator it;

			for (it = doseValueVSIndexMap.begin(); it != doseValueVSIndexMap.end(); ++it)
			{
				_doseVector.push_back((float)(*it).first);
				_voxelProportionVector.push_back(voxelProportionVectorTemp.at((*it).second));
			}

			volume *= numVoxels;

			_statistics = boost::make_shared<DoseStatistics>(minimumDose, maximumDose, meanDose, stdDeviationDose, numVoxels,
			              volume);

			simpleDoseStatisticsCalculated = true;

			ResultListPointer minimumVoxelPositions = computeMinimumPositions(100);
			ResultListPointer maximumVoxelPositions = computeMaximumPositions(100);

			_statistics->setMinimumVoxelPositions(minimumVoxelPositions);
			_statistics->setMaximumVoxelPositions(maximumVoxelPositions);


		}


		void DoseStatisticsCalculator::calculateComplexDoseStatistics(const std::vector<double>& precomputeDoseValues,
		        const std::vector<double>& precomputeVolumeValues)
		{
			std::vector<double> precomputeDoseValuesNonConst = precomputeDoseValues;
			std::vector<double> precomputeVolumeValuesNonConst = precomputeVolumeValues;

			//set default values
			if (precomputeDoseValues.empty())
			{
				DoseTypeGy maxDose = _statistics->getMaximum();
				std::vector<double> defaultPrecomputeDoseValues = boost::assign::list_of(0.02 * maxDose)(0.05 * maxDose)(0.1 * maxDose)(
				            0.9 * maxDose)(
				            0.95 * maxDose)(0.98 * maxDose);
				precomputeDoseValuesNonConst = defaultPrecomputeDoseValues;
			}

			if (precomputeVolumeValues.empty())
			{
				VolumeType volume = _statistics->getVolume();
				std::vector<double> defaultPrecomputeVolumeValues = boost::assign::list_of(0.02 * volume)(
				            0.05 * volume)(0.1 * volume)(0.9 * volume)
				        (0.95 * volume)(0.98 * volume);
				precomputeVolumeValuesNonConst = defaultPrecomputeVolumeValues;
			}

			DoseToVolumeFunctionType Vx = computeDoseToVolumeMulti(precomputeDoseValuesNonConst, DoseStatistics::Vx);
			VolumeToDoseFunctionType Dx = computeVolumeToDoseFunctionMulti(precomputeVolumeValuesNonConst, DoseStatistics::Dx);
			VolumeToDoseFunctionType MOHx = computeVolumeToDoseFunctionMulti(precomputeVolumeValuesNonConst, DoseStatistics::MOHx);
			VolumeToDoseFunctionType MOCx = computeVolumeToDoseFunctionMulti(precomputeVolumeValuesNonConst, DoseStatistics::MOCx);
			VolumeToDoseFunctionType MaxOHx = computeVolumeToDoseFunctionMulti(precomputeVolumeValuesNonConst,
			                                  DoseStatistics::MaxOHx);
			VolumeToDoseFunctionType MinOCx = computeVolumeToDoseFunctionMulti(precomputeVolumeValuesNonConst,
			                                  DoseStatistics::MinOCx);

			_statistics->setVx(Vx);
			_statistics->setDx(Dx);
			_statistics->setMOHx(MOHx);
			_statistics->setMOCx(MOCx);
			_statistics->setMaxOHx(MaxOHx);
			_statistics->setMinOCx(MinOCx);
		}



		DoseStatisticsCalculator::ResultListPointer DoseStatisticsCalculator::computeMaximumPositions(
		    unsigned int maxNumberMaxima)
		{
			if (!simpleDoseStatisticsCalculated)
			{
				throw core::InvalidDoseException("simple DoseStatistics have to be computed in order to call computeMaximumAndPosition()");

			}

			ResultListPointer maxVoxelVector = boost::make_shared<std::vector<std::pair<DoseTypeGy, VoxelGridID> > >();

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
		    unsigned int maxNumberMinima)
		{
			if (!simpleDoseStatisticsCalculated)
			{
				throw core::InvalidDoseException("simple DoseStatistics have to be computed in order to call computeMinimumAndPosition()");

			}

			ResultListPointer minVoxelVector = boost::make_shared<std::vector<std::pair<DoseTypeGy, VoxelGridID> > >();

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

		VolumeType DoseStatisticsCalculator::computeVx(DoseTypeGy xDoseAbsolute) const
		{
			rttb::FractionType count = 0;
			_doseIterator->reset();

			DoseTypeGy currentDose = 0;

			while (_doseIterator->isPositionValid())
			{
				currentDose = _doseIterator->getCurrentDoseValue();

				if (currentDose >= xDoseAbsolute)
				{
					count += _doseIterator->getCurrentRelevantVolumeFraction();
				}

				_doseIterator->next();
			}

			return count * this->_doseIterator->getCurrentVoxelVolume();
		}

		DoseTypeGy DoseStatisticsCalculator::computeDx(VolumeType xVolumeAbsolute) const
		{
			double noOfVoxel = xVolumeAbsolute / _doseIterator->getCurrentVoxelVolume();
			DoseTypeGy resultDose = 0;

			double countVoxels = 0;
			int i = _doseVector.size() - 1;

			for (; i >= 0; i--)
			{
				countVoxels += _voxelProportionVector.at(i);

				if (countVoxels >= noOfVoxel)
				{
					break;
				}
			}

			if (i >= 0)
			{
				resultDose = _doseVector.at(i);
			}
			else
			{
				resultDose = _statistics->getMinimum();
			}

			return resultDose;
		}

		DoseTypeGy DoseStatisticsCalculator::computeMOHx(VolumeType xVolumeAbsolute) const
		{
			double noOfVoxel = xVolumeAbsolute / _doseIterator->getCurrentVoxelVolume();


			if (noOfVoxel == 0)
			{
				return 0;
			}
			else
			{
				double countVoxels = 0;
				double sum = 0;

				for (int i = _doseVector.size() - 1; i >= 0; i--)
				{
					double voxelProportion = _voxelProportionVector.at(i);
					countVoxels += voxelProportion;
					sum += _doseVector.at(i) * voxelProportion;

					if (countVoxels >= noOfVoxel)
					{
						break;
					}
				}

				return (DoseTypeGy)(sum / noOfVoxel);
			}
		}

		DoseTypeGy DoseStatisticsCalculator::computeMOCx(DoseTypeGy xVolumeAbsolute) const
		{
			double noOfVoxel = xVolumeAbsolute / _doseIterator->getCurrentVoxelVolume();


			if (noOfVoxel == 0)
			{
				return 0;
			}
			else
			{
				double countVoxels = 0;
				double sum = 0;
				std::vector<DoseTypeGy>::const_iterator it = _doseVector.begin();
				std::vector<double>::const_iterator itD = _voxelProportionVector.begin();

				for (; it != _doseVector.end(); ++it, ++itD)
				{
					double voxelProportion = *itD;
					countVoxels += voxelProportion;
					sum += (*it) * voxelProportion;

					if (countVoxels >= noOfVoxel)
					{
						break;
					}
				}

				return (DoseTypeGy)(sum / noOfVoxel);
			}
		}

		DoseTypeGy DoseStatisticsCalculator::computeMaxOHx(DoseTypeGy xVolumeAbsolute) const
		{
			double noOfVoxel = xVolumeAbsolute / _doseIterator->getCurrentVoxelVolume();
			DoseTypeGy resultDose = 0;

			double countVoxels = 0;
			int i = _doseVector.size() - 1;

			for (; i >= 0; i--)
			{
				countVoxels += _voxelProportionVector.at(i);

				if (countVoxels >= noOfVoxel)
				{
					break;
				}
			}

			if (i - 1 >= 0)
			{
				resultDose = _doseVector.at(i - 1);
			}

			return resultDose;
		}

		DoseTypeGy DoseStatisticsCalculator::computeMinOCx(DoseTypeGy xVolumeAbsolute) const
		{
			double noOfVoxel = xVolumeAbsolute / _doseIterator->getCurrentVoxelVolume();
			DoseTypeGy resultDose = 0;

			double countVoxels = 0;
			std::vector<DoseTypeGy>::const_iterator it = _doseVector.begin();
			std::vector<double>::const_iterator itD = _voxelProportionVector.begin();

			for (; itD != _voxelProportionVector.end(); ++itD, ++it)
			{
				countVoxels += *itD;

				if (countVoxels >= noOfVoxel)
				{
					break;
				}
			}

			if (it != _doseVector.end())
			{
				++it;

				if (it != _doseVector.end())
				{
					resultDose = *it;
				}
				else
				{
					resultDose = (DoseTypeGy)_statistics->getMaximum();
				}
			}
			else
			{
				resultDose = (DoseTypeGy)_statistics->getMinimum();
			}

			return resultDose;
		}

		DoseStatisticsCalculator::DoseToVolumeFunctionType DoseStatisticsCalculator::computeDoseToVolumeMulti(
		    const std::vector<double>& precomputeDoseValues, DoseStatistics::complexStatistics name) const
		{
			DoseToVolumeFunctionType VxMulti;

			for (int i = 0; i < precomputeDoseValues.size(); ++i)
			{
				if (name == DoseStatistics::Vx)
				{
					VxMulti.insert(std::pair<DoseTypeGy, VolumeType>(precomputeDoseValues.at(i),
					               computeVx(precomputeDoseValues.at(i))));
				}
			}

			return VxMulti;
		}

		DoseStatisticsCalculator::VolumeToDoseFunctionType DoseStatisticsCalculator::computeVolumeToDoseFunctionMulti(
		    const std::vector<double>& precomputeVolumeValues, DoseStatistics::complexStatistics name) const
		{
			VolumeToDoseFunctionType multiValues;

			for (int i = 0; i < precomputeVolumeValues.size(); ++i)
			{
				switch (name)
				{
					case DoseStatistics::Dx:
						multiValues.insert(std::pair<VolumeType, DoseTypeGy>(precomputeVolumeValues.at(i),
						                   computeDx(precomputeVolumeValues.at(i))));
						break;

					case DoseStatistics::MOHx:
						multiValues.insert(std::pair<VolumeType, DoseTypeGy>(precomputeVolumeValues.at(i),
						                   computeMOHx(precomputeVolumeValues.at(i))));
						break;

					case DoseStatistics::MOCx:
						multiValues.insert(std::pair<VolumeType, DoseTypeGy>(precomputeVolumeValues.at(i),
						                   computeMOCx(precomputeVolumeValues.at(i))));
						break;

					case DoseStatistics::MaxOHx:
						multiValues.insert(std::pair<VolumeType, DoseTypeGy>(precomputeVolumeValues.at(i),
						                   computeMaxOHx(precomputeVolumeValues.at(i))));
						break;

					case DoseStatistics::MinOCx:
						multiValues.insert(std::pair<VolumeType, DoseTypeGy>(precomputeVolumeValues.at(i),
						                   computeMinOCx(precomputeVolumeValues.at(i))));
						break;

					default:
						throw core::InvalidParameterException("unknown DoseStatistics name!");
				}
			}

			return multiValues;
		}

	}//end namespace algorithms
}//end namespace rttb

