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

#include "rttbHomogeneityIndex.h"
#include "rttbNullPointerException.h"
#include "rttbInvalidParameterException.h"
#include "rttbExceptionMacros.h"

namespace rttb{
	namespace indices{

		HomogeneityIndex::HomogeneityIndex(core::DVHSet* dvhSet, DoseTypeGy aDoseReference)
		{
			_dvhSet=dvhSet;
			_doseReference=aDoseReference;
			initSuccess=false;
		}

		bool HomogeneityIndex::init()
		{
			if(!_dvhSet){
				throw core::NullPointerException("DVHSet must not be NULL! ");
			}
			if(this->calcIndex()){
				initSuccess=true;
				return true;
			}
			return false;
		}

		bool HomogeneityIndex::calcIndex()
		{
			double max=0;
			double min;
			std::vector<core::DVH> dvhTVSet=this->_dvhSet->getDVHTVSet();
			std::vector<core::DVH>::iterator it;
				
			for(it=dvhTVSet.begin(); it!=dvhTVSet.end();it++)		
			{
				core::DVH dvh=*(it);
				if(it==dvhTVSet.begin())
					min=dvh.getMinimum();
				if(dvh.getMaximum()>max)
					max=dvh.getMaximum();
				if(dvh.getMinimum()<min)
					min=dvh.getMinimum();
			}

			if(this->getDoseReference()!=0){
				_value=(max-min)/this->getDoseReference();
			}
			else{
				rttbExceptionMacro(core::InvalidParameterException, << "Reference dose "<<this->getDoseReference()<<" invalid: Volume of reference dose should not be 0!");
			}
			return true;
			
		}

		IndexValueType HomogeneityIndex::getDoseIndexAt(GridIndexType tvIndex){
			std::vector<core::DVH> dvhTVSet=this->_dvhSet->getDVHTVSet();
			if(tvIndex>=dvhTVSet.size()){
				rttbExceptionMacro(core::InvalidParameterException, <<"tvIndex invalid: it should be <"<<dvhTVSet.size()<<"!");
			}
			core::DVH dvh=dvhTVSet.at(tvIndex);
			if(this->getDoseReference()<=0){
				rttbExceptionMacro(core::InvalidParameterException, << "Reference dose "<<this->getDoseReference()<<" invalid: Volume of reference dose should not be 0!");
			}
			return (dvh.getMaximum()-dvh.getMinimum())/this->getDoseReference();
		}

	}//end namespace indices
}//end namespace rttb