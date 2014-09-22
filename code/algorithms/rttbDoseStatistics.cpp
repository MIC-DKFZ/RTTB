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

#include <string>
#include <vector>
#include <exception>
#include <math.h>
#include <deque>
#include <algorithm>
#include <iostream>
#include <map>

#include "rttbDoseStatistics.h"
#include "rttbNullPointerException.h"
#include "rttbInvalidDoseException.h"

namespace rttb{

	namespace algorithms{
		DoseStatistics::DoseStatistics()
			{
			initSuccess=false;
			}

		DoseStatistics::DoseStatistics(DoseIteratorPointer aDoseIterator)
			{
			_doseIterator=aDoseIterator;
			initSuccess=false;
			this->init();
			}

		void DoseStatistics::setDoseIterator(DoseIteratorPointer aDoseIterator)
			{
			_doseIterator=aDoseIterator;
			initSuccess=false;
			this->init();
			}

		DoseStatistics::DoseIteratorPointer DoseStatistics::getDoseIterator() const
			{
			return _doseIterator;
			}


		bool DoseStatistics::init() {

			if(!_doseIterator)
				{
				throw core::NullPointerException("_doseIterator must not be NULL!");
				}

			doseVector.clear();
			voxelProportionVector.clear();

			std::multimap<double,int> doseValueVSIndexMap;
			std::vector<double> voxelProportionVectorTemp;


			this->_maximum=0;
			this->_mean=0;
			this->_stdDeviation=0;
			this->_variance=0;

			float sum=0;
			_numVoxels=0;
			float squareSum=0;

			_doseIterator->reset();
			int i=0;
			DoseTypeGy doseValue = 0;
			while(_doseIterator->isPositionValid())
				{ 
				doseValue = _doseIterator->getCurrentDoseValue();

				if(i==0)
					{
					_minimum=doseValue;
					}

				rttb::FractionType voxelProportion=_doseIterator->getCurrentRelevantVolumeFraction();
				sum+=doseValue*voxelProportion;
				_numVoxels+=voxelProportion;
				squareSum+=doseValue*doseValue*voxelProportion;
				if(doseValue>this->_maximum)
					{
					_maximum=doseValue;
					}
				else if(doseValue<this->_minimum)
					{
					_minimum=doseValue;
					}

				voxelProportionVectorTemp.push_back(voxelProportion);
				doseValueVSIndexMap.insert(std::pair<double,int>(doseValue,i));

				i++;
				_doseIterator->next();
				}
			if(_numVoxels!=0)
				{
				_mean=sum/_numVoxels;
				_variance=(squareSum/_numVoxels-_mean*_mean);
				if (_variance < errorConstant)
					{
					_stdDeviation = 0;
					}
				else
					{
					_stdDeviation=pow(_variance,0.5);
					}
				}

			//sort dose values and corresponding volume fractions in member variables
			std::multimap<double,int>::iterator it;
			for(it=doseValueVSIndexMap.begin();it!=doseValueVSIndexMap.end();++it)
				{
				doseVector.push_back((float)(*it).first);
				voxelProportionVector.push_back(voxelProportionVectorTemp.at((*it).second));
				}

			initSuccess=true;

			return true;
			}

		double DoseStatistics::getNumberOfVoxels(){
			if(!initSuccess)
				{
          throw core::InvalidDoseException("DoseStatistics is not initialized: set dose using setDoseIterator()! ");
				}
			return _numVoxels;
			}

		DoseStatisticType DoseStatistics::getMaximum(ResultListPointer maxVoxelVector) const{
			if(!initSuccess){
        throw core::InvalidDoseException("DoseStatistics is not initialized: set dose using setDoseIterator()! ");
				
				}
			if (maxVoxelVector==NULL){
				throw core::NullPointerException("resultsVector must not be NULL! ");
				}
			if(maxVoxelVector->size()==0){
				this->_doseIterator->reset();
				DoseTypeGy doseValue=0;
				while(_doseIterator->isPositionValid())
					{
					doseValue = _doseIterator->getCurrentDoseValue();
					if(doseValue==_maximum)
						{
						VoxelGridID currentID = _doseIterator->getCurrentVoxelGridID();
						std::pair<DoseTypeGy,VoxelGridID> voxel (doseValue,currentID);
						maxVoxelVector->push_back(voxel);
						}
					_doseIterator->next();
					}
				}
			return _maximum;
			}

		DoseStatisticType DoseStatistics::getMinimum(ResultListPointer minVoxelVector, int number) const{
			if(!initSuccess)
				{
				throw core::InvalidDoseException("DoseStatistics is not initialized: set dose using setDoseIterator()! ");

				}
			if (minVoxelVector==NULL)
				{
				throw core::NullPointerException("resultsVector must not be NULL! ");
				}
			/*! @todo: Architecture Annotation:
				Finding the positions for the minimum only once reduces computation time,
				but will require sensible use by the programmers. To be save the output vector
				minVoxelVector will be always cleared here to garantee that no false values are 
				presented. This change may be revoced to increase computation speed later on 
				(only compute if(minVoxelVector->size()==0)).
			*/
			minVoxelVector->clear();
			int count=0;
			this->_doseIterator->reset();
			DoseTypeGy doseValue = 0;
			while(_doseIterator->isPositionValid() && count<number){
				doseValue = _doseIterator->getCurrentDoseValue();
				if(doseValue==_minimum){
					VoxelGridID currentID = _doseIterator->getCurrentVoxelGridID();
					std::pair<DoseTypeGy,VoxelGridID> voxel (doseValue,currentID);
					minVoxelVector->push_back(voxel);
					count++;
					}
				_doseIterator->next();
				}
			return _minimum;
			}

		DoseStatisticType DoseStatistics::getMean() const{
			if(!initSuccess)
				{
				throw core::InvalidDoseException("DoseStatistics is not initialized: set dose using setDoseIterator()! ");
				}
			return _mean;
			}

		DoseStatisticType DoseStatistics::getStdDeviation() const{
			if(!initSuccess)
				{
				throw core::InvalidDoseException("DoseStatistics is not initialized: set dose using setDoseIterator()! ");
				}
			return _stdDeviation;
			}

		DoseStatisticType DoseStatistics::getVariance() const{
			if(!initSuccess)
				{
				throw core::InvalidDoseException("DoseStatistics is not initialized: set dose using setDoseIterator()! ");
				}
			return _variance;
			}

		VolumeType DoseStatistics::getVx(DoseTypeGy xDoseAbsolute) const{
			rttb::FractionType count=0;
			_doseIterator->reset();

			DoseTypeGy currentDose = 0;
			while(_doseIterator->isPositionValid())
				{
				currentDose = _doseIterator->getCurrentDoseValue();
				if(currentDose>=xDoseAbsolute)
					{
					count+=_doseIterator->getCurrentRelevantVolumeFraction();
					}
				_doseIterator->next();
				}
			return count*this->_doseIterator->getCurrentVoxelVolume();
			}

		DoseTypeGy DoseStatistics::getDx(DoseTypeGy xVolumeAbsolute) const{
			double noOfVoxel=xVolumeAbsolute/_doseIterator->getCurrentVoxelVolume();
			DoseTypeGy resultDose=0;

			double countVoxels=0;
			int i=doseVector.size()-1;
			for(;i>=0;i--){
				countVoxels+=voxelProportionVector.at(i);
				if(countVoxels>=noOfVoxel)
					{
					break;
					}
				}
			if(i>=0)
				{
				resultDose=doseVector.at(i);
				}
			else
				{
				resultDose=_minimum;
				}

			return resultDose;
			}

		DoseTypeGy DoseStatistics::getMOHx(DoseTypeGy xVolumeAbsolute) const{
			double noOfVoxel=xVolumeAbsolute/_doseIterator->getCurrentVoxelVolume();
			double countVoxels=0;
			double sum=0;

			if(noOfVoxel==0)
				{
				return 0;
				}
			else{
				for(int i=doseVector.size()-1;i>=0;i--)
					{
					double voxelProportion=voxelProportionVector.at(i);
					countVoxels+=voxelProportion;
					sum+=doseVector.at(i)*voxelProportion;
					if(countVoxels>=noOfVoxel)
						{
						break;
						}
					}
				return (DoseTypeGy)(sum/noOfVoxel);
				}
			}

		DoseTypeGy DoseStatistics::getMOCx(DoseTypeGy xVolumeAbsolute) const{
			double noOfVoxel=xVolumeAbsolute/_doseIterator->getCurrentVoxelVolume();
			double countVoxels=0;
			double sum=0;

			if(noOfVoxel==0)
				{
				return 0;
				}
			else
				{
				std::vector<DoseTypeGy>::const_iterator it=doseVector.begin();
				std::vector<double>::const_iterator itD=voxelProportionVector.begin();

				for(;it!=doseVector.end();it++,itD++)
					{
					double voxelProportion=*itD;
					countVoxels+=voxelProportion;
					sum+=(*it)*voxelProportion;
					if(countVoxels>=noOfVoxel)
						{
						break;
						}
					}
				return (DoseTypeGy)(sum/noOfVoxel);
				}
			}

		DoseTypeGy DoseStatistics::getMaxOHx(DoseTypeGy xVolumeAbsolute) const{
			double noOfVoxel=xVolumeAbsolute/_doseIterator->getCurrentVoxelVolume();
			DoseTypeGy resultDose=0;

			double countVoxels=0;
			int i=doseVector.size()-1;
			for(;i>=0;i--)
				{
				countVoxels+=voxelProportionVector.at(i);
				if(countVoxels>=noOfVoxel)
					{
					break;
					}
				}
			if(i-1>=0)
				{
				resultDose=doseVector.at(i-1);
				}
			return resultDose;
			}

		DoseTypeGy DoseStatistics::getMinOCx(DoseTypeGy xVolumeAbsolute) const{
			double noOfVoxel=xVolumeAbsolute/_doseIterator->getCurrentVoxelVolume();
			DoseTypeGy resultDose=0;

			double countVoxels=0;
			std::vector<DoseTypeGy>::const_iterator it=doseVector.begin();
			std::vector<double>::const_iterator itD=voxelProportionVector.begin();
			for(;itD!=voxelProportionVector.end();itD++,it++)
				{
				countVoxels+=*itD;
				if(countVoxels>=noOfVoxel)
					{
					break;
					}
				}
			if(it!=doseVector.end())
				{
				it++;
				if(it!=doseVector.end())
					{
					resultDose=*it;
					}
				else
					{
					resultDose=(DoseTypeGy)_maximum;
					}
				}
			else
				{
				resultDose=(DoseTypeGy)_maximum;
				}
			return resultDose;
			}

		}//end namespace algorithms
	}//end namespace rttb

