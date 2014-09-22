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

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

#include "rttbDVHCalculator.h"
#include "rttbNullPointerException.h"
#include "rttbInvalidParameterException.h"
#include "rttbMaskedDoseIteratorInterface.h"


namespace rttb{
	namespace core{

		DVHCalculator::DVHCalculator(DoseIteratorPointer aDoseIterator,const IDType aStructureID,const IDType aDoseID, 
			DoseTypeGy aDeltaD, const int aNumberOfBins){
				if(!aDoseIterator)
				{
					throw NullPointerException("aDoseIterator must not be NULL! ");
				}
				_doseIteratorPtr=aDoseIterator;
				_structureID=aStructureID;
				_doseID=aDoseID;

				if(aNumberOfBins<=0 || aDeltaD<0 )
				{
					throw InvalidParameterException("aNumberOfBins/aDeltaD must be >0! ");
				}

				_numberOfBins=aNumberOfBins;
				_deltaD=aDeltaD;

				if(_deltaD==0)
				{
					aDoseIterator->reset();
					DoseTypeGy max=0;
					DoseTypeGy currentVal = 0;
					while(aDoseIterator->isPositionValid())
					{
						currentVal = aDoseIterator->getCurrentDoseValue();
						if(currentVal>max)
						{
							max=currentVal;
						}
						aDoseIterator->next();
					}
					_deltaD=(max*1.5/_numberOfBins);

					if(_deltaD==0)
					{
						_deltaD=0.1;
					}
				}

		}

		DVHCalculator::~DVHCalculator(){}

		DVHCalculator::DVHPointer DVHCalculator::generateDVH(){

			std::deque<DoseCalcType> dataDifferential(_numberOfBins,0);

			// calculate DVH
			_doseIteratorPtr->reset();
			DoseTypeGy currentVal = 0;
			while(_doseIteratorPtr->isPositionValid())
			{
				FractionType voxelProportion=_doseIteratorPtr->getCurrentRelevantVolumeFraction();
				currentVal = _doseIteratorPtr->getCurrentDoseValue();

				int dose_bin = static_cast<int>(currentVal/_deltaD);

				if(dose_bin < _numberOfBins)
				{
					dataDifferential[dose_bin]+=voxelProportion;
				}
				else{
					throw InvalidParameterException("_numberOfBins is too small: dose bin out of bounds! ");
				}
				_doseIteratorPtr->next();
			}
			if (boost::dynamic_pointer_cast<MaskedDoseIteratorPointer>(_doseIteratorPtr))
			{
				_dvh=boost::make_shared<DVH>(dataDifferential,_deltaD,_doseIteratorPtr->getCurrentVoxelVolume(),_structureID,_doseID, _doseIteratorPtr->getVoxelizationID());
			}
			else{
				_dvh=boost::make_shared<DVH>(dataDifferential,_deltaD,_doseIteratorPtr->getCurrentVoxelVolume(),_structureID,_doseID);
			}

			return _dvh;
		}

	}//end namespace core
}//end namespace rttb

