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
// @version $Revision: 707 $ (last changed revision)
// @date    $Date: 2014-09-04 16:37:24 +0200 (Do, 04 Sep 2014) $ (last change date)
// @author  $Author: floca $ (last changed by)
*/
#ifndef __DVH_BASED_DOSE_INDEX_H
#define __DVH_BASED_DOSE_INDEX_H


#include "rttbBaseType.h"
#include "rttbDVHSet.h"
#include "rttbDoseIndex.h"

namespace rttb{


	namespace indices{
		/*! @class DoseIndex
		@brief This is the interface for dose/plan comparison indices.
		@ingroup indices
		*/
		class DvhBasedDoseIndex: public DoseIndex
		{
		public:

		protected: 

			/*! @todo Use Shared Pointers for _dvhSet*/
			core::DVHSet* _dvhSet;

			/*! @brief Dose index calculation */
			virtual bool calcIndex()=0;

			virtual bool checkInputs()=0;

		public:

			
			/*! @brief Dose/plan comparison index calculation for tvIndex-th treated volume
			(tv = target volume; th = healthy tissue)
			@param tvIndex index in the DVH in the current set of tv-DVHs
			@todo is this name good? getIndexAt() instead?
			*/
			virtual IndexValueType getDoseIndexAt(const core::DVHSet::IndexType tvIndex)=0;
		};
	}
}


#endif
