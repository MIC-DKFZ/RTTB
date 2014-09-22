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
#include "rttbDvhBasedModels.h"
#include "rttbInvalidParameterException.h"

namespace rttb{
  namespace models{
    DoseStatisticType getEUD(const DVHPointer dvh, const DoseCalcType aA) {
      if(aA==0)
      {
        throw core::InvalidParameterException("Parameter invalid: aA should not be zero!");
      }
      DataDifferentialType dataDifferential = dvh->getDataDifferential();
      if(dataDifferential.empty())
      {
        throw core::InvalidParameterException("Parameter invalid: DVH data differential should not be empty!");
      }

      double eud=0;

      DoseTypeGy deltaD = dvh->getDeltaD();
      DVHVoxelNumber numberOfVoxels = dvh->getNumberOfVoxels();
      for(GridIndexType i=0; i<dataDifferential.size(); i++)
      {
        double doseGyi=(i+0.5)*deltaD;
        double relativeVolumei=dataDifferential[i]/numberOfVoxels;
        eud+=pow((double)doseGyi,(double)aA)*relativeVolumei;
      }
      eud=pow((double)eud,(double)(1/aA));			
      return eud;

    }

    std::map<DoseTypeGy,DoseCalcType> calcBEDDVH(const DVHPointer dvh, const int numberOfFractions, const DoseCalcType alpha_beta, const bool relativeVolume){
      std::map<DoseTypeGy,DoseCalcType> dataBED;
      std::map<DoseTypeGy,DoseCalcType> dataBEDRelative;

      DataDifferentialType dataDifferential = dvh->getDataDifferential();
      if(dataDifferential.empty())
      {
        throw core::InvalidParameterException("Parameter invalid: DVH data differential should not be empty!");
      }
      if(alpha_beta<=0)
      {
        throw core::InvalidParameterException("Parameter invalid: alpha_beta must be >0!");
      }
      if(numberOfFractions<=0)
      {
        throw core::InvalidParameterException("Parameter invalid: numberOfFractions must be >0!");
      }

      DoseTypeGy deltaD = dvh->getDeltaD();
      DVHVoxelNumber numberOfVoxels = dvh->getNumberOfVoxels();

      std::deque<rttb::DoseCalcType>::iterator it;
      int i=0;

      for(it=dataDifferential.begin();it!=dataDifferential.end();it++ )
      {
        DoseTypeGy doseGyi=((i+0.5)*deltaD);
        DoseTypeGy bedi=0;
        bedi=(doseGyi*(1+doseGyi/(numberOfFractions*alpha_beta)));
        if(!relativeVolume)
        {
          dataBED.insert(std::pair<rttb::DoseTypeGy,rttb::DoseCalcType>(bedi,(*it)));
        }
        else
        {
          dataBEDRelative.insert(std::pair<rttb::DoseTypeGy,rttb::DoseCalcType>(bedi,(*it)/numberOfVoxels));
        }
        i++;
      }

      if(!relativeVolume)
      {
        return dataBED;
      }
      else
      {
        return dataBEDRelative;
      }
    }

    std::map<rttb::DoseTypeGy,rttb::DoseCalcType> calcLQED2DVH(const DVHPointer dvh, const int numberOfFractions, const DoseCalcType alpha_beta, const bool relativeVolume) {
      std::map<rttb::DoseTypeGy,rttb::DoseCalcType> dataLQED2;
      std::map<rttb::DoseTypeGy,rttb::DoseCalcType> dataLQED2Relative;

      DataDifferentialType dataDifferential = dvh->getDataDifferential();
      if(dataDifferential.empty())
      {
        throw core::InvalidParameterException("Parameter invalid: DVH data differential should not be empty!");
      }
      if(alpha_beta<=0)
      {
        throw core::InvalidParameterException("Parameter invalid: alpha_beta must be >0!");
      }
      if(numberOfFractions<=0)
      {
        throw core::InvalidParameterException("Parameter invalid: numberOfFractions must be >0!");
      }

      DoseTypeGy deltaD = dvh->getDeltaD();
      DVHVoxelNumber numberOfVoxels = dvh->getNumberOfVoxels();

      std::deque<rttb::DoseCalcType>::iterator it;
      int i=0;

      for(it=dataDifferential.begin();it!=dataDifferential.end();it++ )
      {
        DoseTypeGy doseGyi=((i+0.5)*deltaD);
        DoseTypeGy lqed2i=0;
        lqed2i=(doseGyi*((alpha_beta+doseGyi/numberOfFractions)/(alpha_beta+2)));

        if(!relativeVolume)
        {
          dataLQED2.insert(std::pair<rttb::DoseTypeGy,rttb::DoseCalcType>(lqed2i,*it));
        }
        else
        {
          dataLQED2Relative.insert(std::pair<rttb::DoseTypeGy,rttb::DoseCalcType>(lqed2i,(*it)/numberOfVoxels));
        }
        i++;
      }

      if(!relativeVolume)
      {
        return dataLQED2;
      }
      else
      {
        return dataLQED2Relative;
      }
    }
  }
}