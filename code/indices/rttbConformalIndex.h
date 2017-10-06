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

#ifndef __CONFORMAL_INDEX_H
#define __CONFORMAL_INDEX_H


#include "rttbDvhBasedDoseIndex.h"
#include "rttbBaseType.h"

#include "RTTBIndicesExports.h"

namespace rttb
{

	namespace indices
	{

		/*! @class ConformalIndex
			@brief This class representing a ConformalIndex Object. 
            @details Conformal Index \f$(COIN)= Conformation Number(CN)* (1-Vref,0/Vnt,0)*(1-Vref,1/Vnt,1)... i\f$, i: i-th critiacal organ
			Conformation Number \f$(CN)= \frac{TVref}{TV} * \frac{TVref}{Vref}\f$
			@ingroup indices
		*/
        class RTTBIndices_EXPORT ConformalIndex : public DvhBasedDoseIndex
		{
		protected:
			/*! @brief Calculate conformal index
			  @exception InvalidParameterException Thrown if dvhSet or aDoseReference invalid
			*/
			bool calcIndex() override;


		public:
			/*! @brief Constructor
			*/
			ConformalIndex(DVHSetPtr dvhSet, DoseTypeGy aDoseReference);

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
