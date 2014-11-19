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

#include "rttbConformalIndex.h"
#include "rttbNullPointerException.h"
#include "rttbInvalidParameterException.h"
#include "rttbExceptionMacros.h"

namespace rttb{

	namespace indices{

		ConformalIndex::ConformalIndex(ConformalIndex::DVHSetPtr dvhSet, DoseTypeGy aDoseReference)
			:DvhBasedDoseIndex(dvhSet, aDoseReference)
		{
			init();
		}

		bool ConformalIndex::calcIndex()
		{
			VolumeType TV=_dvhSet->getTVVolume(0);
			VolumeType Vref=_dvhSet->getWholeVolume(_doseReference);
			if(TV!=0 && Vref!=0){
				_value=(_dvhSet->getTVVolume(_doseReference)/TV)*
					(_dvhSet->getTVVolume(_doseReference)/Vref);

				std::vector<core::DVH> dvhHTSet=this->_dvhSet->getDVHHTSet();
				std::vector<core::DVH>::iterator it;

				for(it=dvhHTSet.begin(); it!=dvhHTSet.end();++it)		
				{
					core::DVH dvh=*(it);
					VolumeType HT=dvh.getVx(0);
					if(HT!=0)
						_value*=(1-dvh.getVx(this->_doseReference)/HT);
				}
			}
			else if(TV==0){
				throw core::InvalidParameterException("DVH Set invalid: Target volume should not be 0!");
			}
			else{
				rttbExceptionMacro(core::InvalidParameterException, << "Reference dose "<<this->getDoseReference()<<" invalid: Volume of reference dose should not be 0!");
			}
			return true;
		}

		IndexValueType ConformalIndex::getValueAt(core::DVHSet::IndexType tvIndex){
			std::vector<core::DVH> dvhTVSet=this->_dvhSet->getDVHTVSet();
			VolumeType Vref=_dvhSet->getWholeVolume(_doseReference);
			if(tvIndex>=dvhTVSet.size()){
				rttbExceptionMacro(core::InvalidParameterException, <<"tvIndex invalid: it should be <"<<dvhTVSet.size()<<"!");
			}
			else{
				core::DVH dvh=dvhTVSet.at(tvIndex);

				VolumeType TV=dvh.getVx(0);
				if(TV==0){
					throw core::InvalidParameterException("DVH invalid: Volume of tvIndex should not be 0!");
				}
				else if(Vref==0){
					rttbExceptionMacro(core::InvalidParameterException, << "Reference dose "<<this->getDoseReference()<<" invalid: Volume of reference dose should not be 0!");
				}

				double value=dvh.getVx(_doseReference)/TV;//the irradiation factor of i-th target volume
				value=value*dvh.getVx(_doseReference)/Vref;//conformation number

				std::vector<core::DVH> dvhHTSet=this->_dvhSet->getDVHHTSet();
				std::vector<core::DVH>::iterator it;

				for(it=dvhHTSet.begin(); it!=dvhHTSet.end();++it)		
				{
					dvh=*(it);
					VolumeType HT=dvh.getVx(0);
					if(HT!=0){
						value*=(1-dvh.getVx(this->_doseReference)/HT);
					}
				}
				return value;
			}
		}

	}//end namespace indices
}//end namespace rttb
