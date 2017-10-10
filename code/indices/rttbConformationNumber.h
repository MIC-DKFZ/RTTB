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

#include "rttbDvhBasedDoseIndex.h"
#include "rttbBaseType.h"

#include "RTTBIndicesExports.h"

namespace rttb
{

	namespace indices
	{

		/*! @class ConformationNumber
			@brief This class representing a ConformationNumber Object. 
            @details Conformation Number \f$(CN)= (TVref/TV) * (TVref/Vref)\f$
			@ingroup indices
		*/
        class RTTBIndices_EXPORT ConformationNumber : public DvhBasedDoseIndex
		{
		protected:
			/*! @brief Calculate conformation number
			    @exception InvalidParameterException Thrown if dvhSet or aDoseReference invalid
			*/
			bool calcIndex() override;


		public:

			/*! @brief Constructor
			*/
			ConformationNumber(DVHSetPtr dvhSet, DoseTypeGy aDoseReference);


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
