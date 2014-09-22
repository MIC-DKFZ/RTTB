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

#ifndef __CONFORMATION_NUMBER_H
#define __CONFORMATION_NUMBER_H



#include "rttbDoseIndex.h"
#include "rttbBaseType.h"

namespace rttb{

  namespace indices{

    /*! @class ConformationNumber
		@brief This class representing a ConformationNumber Object. Conformation Number (CN)= (TVref/TV) * (TVref/Vref)
		@ingroup indices
    */
    class ConformationNumber: public DoseIndex
    {
    protected: 
      /*! @brief Calculate conformation number
          @exception InvalidParameterException Thrown if dvhSet or aDoseReference invalid
      */
      bool calcIndex();

      
    public: 

      /*! @brief Constructor
      */
      ConformationNumber(core::DVHSet* dvhSet, DoseTypeGy aDoseReference);

      /*! @return Return true if calcIndex() finished sucessfully
          @exception NullPointerException thrown if dvhSet is NULL
      */
      bool init();

      /*! @brief Dose index calculation for tvIndex-th treated volume
          @param tvIndex index in the DVH in the current set of tv-DVHs
          @return Return index value 
          @exception InvalidParameterException Thrown if tvIndex or aDoseReference invalid
      */
	  IndexValueType getDoseIndexAt(const GridIndexType tvIndex);

    };
  }
}

#endif
