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
#ifndef __CONFORMITY_INDEX_H
#define __CONFORMITY_INDEX_H


#include "rttbDoseIndex.h"
#include "rttbBaseType.h"

namespace rttb{

  namespace indices{

    /*! @class ConformityIndex
		@brief This class representing a ConformityIndex Object. Conformity Index (CI): CI(D)=IFtv(D)*(1-IFht(D)), D:reference dose, 
		IFtv(D): the irradiation factor of the PTV, defined as the fraction of the PTV receiving a dose higher than D
		IFht(D): the irradiation factor of healthy tissue, defined as the radio of the volume of tissue outside the PTV receiving a dose greater than D to the volume of isodose D
		@ingroup indices
	*/
    class ConformityIndex: public DoseIndex
    {
    protected: 
      /*! @brief Calculate Conformity index
		  @exception InvalidParameterException Thrown if dvhSet or aDoseReference invalid
      */
      bool calcIndex();


    public: 
      /*! @brief Constructor
      */
      ConformityIndex(core::DVHSet* dvhSet, DoseTypeGy aDoseReference);

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
