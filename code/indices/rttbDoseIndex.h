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
#ifndef __DOSE_INDEX_H
#define __DOSE_INDEX_H


#include "rttbBaseType.h"
#include "rttbDVHSet.h"

namespace rttb{


  namespace indices{
    /*! @class DoseIndex
		@brief This is the interface for dose/plan comparison indices.
		@ingroup indices
    */
    class DoseIndex
    {
    protected: 

	  /*! @todo Use Shared Pointers for _dvhSet*/
      core::DVHSet* _dvhSet;

      IndexValueType _value;

      DoseTypeGy _doseReference;

      /*! @brief If init() successful*/
      bool initSuccess;

      /*! @brief Dose index calculation */
      virtual bool calcIndex()=0;


    public:
      /*! @brief Initialize the calculation
		  @return Return true if successfully
      */
      virtual bool init()=0;

      /*! @brief Set the reference dose
      */
      void setDoseReference(DoseTypeGy aDoseReference);

      /*! @brief Get the reference dose
      */
      DoseTypeGy getDoseReference() const;

      /*! @brief Get the value of dose/plan comparison index 
		  @return Return the value of this index 
      @exception Exception Thrown if the class was not initialized previously.
      */
      IndexValueType getValue() const;

      /*! @brief Dose/plan comparison index calculation for tvIndex-th treated volume
	      (tv = target volume; th = healthy tissue)
          @param tvIndex index in the DVH in the current set of tv-DVHs
		  @todo is this name good? getIndexAt() instead?
      */
      virtual IndexValueType getDoseIndexAt(const GridIndexType tvIndex)=0;
    };
  }
}


#endif
