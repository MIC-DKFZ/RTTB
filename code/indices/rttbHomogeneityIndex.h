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

#include "rttbDvhBasedDoseIndex.h"
#include "rttbBaseType.h"

#include "RTTBIndicesExports.h"

namespace rttb
{

	namespace indices
	{
		/*! @class HomogeneityIndex
			@brief This class representing a HomogeneityIndex Object. 
            @details Homogeneity Index \f$(HI) = \frac{D_{max}(PTV)-D_{min}(PTV)}{D_{ref}}\f$
			@ingroup indices
		*/
        class RTTBIndices_EXPORT HomogeneityIndex : public DvhBasedDoseIndex
		{
		protected:
			/*! @brief Calculate Conformity index
			  @exception InvalidParameterException Thrown if aDoseReference invalid
			*/
			bool calcIndex() override;

		public:
			/*! @brief Constructor
			*/
			HomogeneityIndex(core::DVHSet::Pointer dvhSet, DoseTypeGy aDoseReference);

			/*! @brief Dose index calculation for tvIndex-th treated volume
			    @param tvIndex index in the DVH in the current set of tv-DVHs
			    @return Return index value
			    @exception InvalidParameterException Thrown if tvIndex or aDoseReference invalid
			*/
			IndexValueType getValueAt(const core::DVHSet::IndexType tvIndex) override;

		};

	}
}


#endif
