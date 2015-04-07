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
// @version $Revision $ (last changed revision)
// @date $Date $ (last change date)
// @author $Author $ (last changed by)
*/
#include <sstream>

#include "rttbMaskVoxelListTester.h"

namespace rttb{

	namespace testing{

		MaskVoxelListTester::MaskVoxelListTester(MaskVoxelListPointer aReferenceList, MaskVoxelListPointer aCompareList){
			_referenceList = aReferenceList;
			_compareList = aCompareList;

			_sameSize = false;
			_masVoxelsDiffer = false;
			_numDifference = 0;
		}

		void MaskVoxelListTester::setReferenceList(const MaskVoxelListPointer aReferenceList){
			_referenceList = aReferenceList;
		}
		void MaskVoxelListTester::setCompareList(const MaskVoxelListPointer aCompareList){
			_compareList = aCompareList;
		}

		lit::StringType MaskVoxelListTester::getTestDescription(void) const {
			return "Compare two MaskVoxelLists and determine if the contained content is equal.";
		};

		bool MaskVoxelListTester::doCheck(void) const{
			_pResults->onTestStart(getCurrentTestLabel());

			_masVoxelsDiffer = false;

			if (_referenceList->size()==_compareList->size()){
				_sameSize = true;
			}
			else{
				_sameSize = false;
				return false;
			}
			_numDifference = 0;
			_maxDifference = 0;

			MaskVoxelList::iterator iterR,iterC;
			iterC = _compareList->begin();
			VoxelGridID index = 0;			
			for(iterR = _referenceList->begin(); iterR != _referenceList->end(); ++iterR,++iterC,++index){
				if(iterR->getVoxelGridID()==iterC->getVoxelGridID()){
					if (iterR->getRelevantVolumeFraction() == iterC->getRelevantVolumeFraction()){
						continue;
					}
					else{
						float diff = iterR->getRelevantVolumeFraction()-iterC->getRelevantVolumeFraction();
						if (diff > _maxDifference) {
							_maxDifference = diff;
						}
						/*if(diff > 0.001){
							std::cout <<iterR->getVoxelGridID()<< ": ("<< iterR->getRelevantVolumeFraction() << ","<<iterC->getRelevantVolumeFraction()<<"); ";
						}*/
						_numDifference++;
					}
				}
				else{
					_failedListIndex = index;
					_masVoxelsDiffer = true;
					return false;
				}
			}//end for(VoxelGridID id = 0; id < _referenceList->getGridSi...
			if (_numDifference > 0){
				return false;
			}
			return true;
		}

		void MaskVoxelListTester::handleSuccess(void) const{
			std::ostringstream stream;
			stream << "Both Lists are equal"<<std::endl;

			_pResults->onTestSuccess(getCurrentTestLabel(), stream.str());
		}

		void MaskVoxelListTester::handleFailure(void) const{
			std::ostringstream stream;
			stream << "Lists were different"<< std::endl;

			if(_sameSize){
				stream << std::endl << "Error of volume fraction voxel count:   "<< _numDifference << std::endl;
				stream << std::endl << "Maximum difference in volume fraction:  "<< _maxDifference << std::endl;
				if (_masVoxelsDiffer){
					stream << std::endl << "Mask points to different grid position in:  "<< _failedListIndex << std::endl;
				}
			}
			else{
				stream << "Lists have different size"<< std::endl;
				stream << "Reference List is "<<_referenceList->size()<<" voxels long and comparison List "<<
					_compareList->size()<< std::endl;
			}
			_pResults->onTestFailure(getCurrentTestLabel(), stream.str());
		}

	}
}