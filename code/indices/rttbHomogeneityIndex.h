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
#ifndef __HOMOGENEITY_INDEX_H
#define __HOMOGENEITY_INDEX_H

#include <string>
#include <vector>

#include "rttbDvhBasedDoseIndex.h"
#include "rttbBaseType.h"

namespace rttb
{

	namespace indices
	{
		/*! @class HomogeneityIndex
			@brief This class representing a HomogeneityIndex Object. Homogeneity Index (HI) = (Dmax(PTV)-Dmin(PTV))/Dref
			@ingroup indices
		*/
		class HomogeneityIndex: public DvhBasedDoseIndex
		{
		protected:
			/*! @brief Calculate Conformity index
			  @exception InvalidParameterException Thrown if aDoseReference invalid
			*/
			bool calcIndex();

		public:
			/*! @brief Constructor
			*/
			HomogeneityIndex(DVHSetPtr dvhSet, DoseTypeGy aDoseReference);

			/*! @brief Dose index calculation for tvIndex-th treated volume
			    @param tvIndex index in the DVH in the current set of tv-DVHs
			    @return Return index value
			    @exception InvalidParameterException Thrown if tvIndex or aDoseReference invalid
			*/
			IndexValueType getValueAt(const core::DVHSet::IndexType tvIndex);

		};

	}
}


#endif
